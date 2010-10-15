/*
    Copyright (C) 2004-2009 by Charles Pence
    [cpence at gmail dot com]

    This file is part of Logos.

    Logos is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Logos is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Logos.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <wx/wxprec.h>
#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

#include <wx/filename.h>

#include <stdlib.h>
#include <set>

#include "../ui/logosapp.h"
#include "filesystem.h"

namespace FS
{

#ifdef __WXMSW__

//
// On Win32, use the W32 CRT '_fullpath' function
//

bool GetRealPath(const wxString &fileName, wxString &realPath)
{
	const char *fileNameMB = fileName.mb_str(*wxConvCurrent);
	char *realPathMB = _fullpath(NULL, fileNameMB, 0);
	if (!realPathMB)
		return false;
	
	realPath = wxString(realPathMB, *wxConvCurrent);
	free(realPathMB);
	
	return true;
}

#else

//
// Thanks to the Insane Coding blog for this!
//

// A buffer-safe version of getcwd()
bool getcwd(wxString& path)
{
	typedef std::pair<dev_t, ino_t> file_id;
	
	bool success = false;
	int start_fd = open(".", O_RDONLY);
	if (start_fd != -1)
	{
		struct stat sb;
		if (!fstat(start_fd, &sb))
		{
			file_id current_id(sb.st_dev, sb.st_ino);
			if (!stat("/", &sb)) //Get info for root directory, so we can determine when we hit it
			{
				wxArrayString path_components;
				file_id root_id(sb.st_dev, sb.st_ino);
				
				while (current_id != root_id) //If they're equal, we've obtained enough info to build the path
				{
					bool pushed = false;
					
					if (!chdir("..")) //Keep recursing towards root each iteration
					{
						DIR *dir = opendir(".");
						if (dir)
						{
							dirent *entry;
							while ((entry = readdir(dir))) //We loop through each entry trying to find where we came from
							{
								if ((strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") && !lstat(entry->d_name, &sb)))
								{
									file_id child_id(sb.st_dev, sb.st_ino);
									if (child_id == current_id) //We found where we came from, add its name to the list
									{
										path_components.push_back(wxString(entry->d_name, *wxConvCurrent));
										pushed = true;
										break;
									}
								}
							}
							closedir(dir);
							
							if (pushed && !stat(".", &sb)) //If we have a reason to contiue, we update the current dir id
							{
								current_id = file_id(sb.st_dev, sb.st_ino);
							}
						}//Else, Uh oh, can't read information at this level
					}
					if (!pushed) { break; } //If we didn't obtain any info this pass, no reason to continue
				}
				
				if (current_id == root_id) //Unless they're equal, we failed above
				{
					//Built the path, will always end with a slash
					path = wxT("/");
					for (wxArrayString::reverse_iterator i = path_components.rbegin(); i != path_components.rend(); ++i)
					{
						path += *i + wxT("/");
					}
					success = true;
				}
				if (fchdir(start_fd) == -1)
					success = false;
			}
		}
		close(start_fd);
	}

	return(success);
}

// Split a path into file and directory parts
void relative_dir_base_split(const wxString& path, wxString& dir, wxString& base)
{
	size_t slash_pos = path.rfind(wxT("/"));
	if (slash_pos != wxString::npos)
	{
		slash_pos++;
		dir = path.substr(0, slash_pos);
		base = path.substr(slash_pos);
	}
	else
	{
		dir.clear(); 
		base = path;
	}
}

bool chdir_getcwd(const wxString& dir, wxString& path)
{
	bool success = false;
	int start_fd = open(".", O_RDONLY);
	if (start_fd != -1)
	{
		if (!chdir(dir.mb_str(*wxConvCurrent)))
		{
			success = getcwd(path);
			if (fchdir(start_fd) == -1)
				success = false;
		}
		close(start_fd);
	}
	return(success);
}


static inline bool realpath_file(const wxString& path, wxString& resolved_path)
{
	bool success = false;
	wxString dir;
	wxString base;
	relative_dir_base_split(path, dir, base);
	
	//If there is a directory, get the realpath() for it, otherwise the current directory
	if (dir.size() ? chdir_getcwd(dir, resolved_path) : getcwd(resolved_path))
	{
		resolved_path += base;
		success = true;
	}
	return(success);
}

bool readlink_internal(const wxString& path, wxString& buffer, ssize_t length)
{
	bool success = false;
	if (length > 0)
	{
		char *buf = (char *)malloc(length + 1);
		if (buf)
		{
			ssize_t amount = ::readlink(path.mb_str(*wxConvCurrent), buf, length+1); //Give room for failure
			if ((amount > 0) && (amount <= length)) //If > length, it was modified mid check
			{
				buf[amount] = 0;
				buffer = wxString(buf, *wxConvCurrent);
				success = true;
			}
			free(buf);
		}
	}
	return(success);
}

void build_path_base_swap(wxString &path, const wxString& newbase)
{
	wxString dir;
	wxString base;
	relative_dir_base_split(path, dir, base);
	
	if (dir.size())
	{
		path = dir + newbase;
	}
	else
	{
		path = newbase;
	}
}

bool symlink_resolve(const wxString& start, wxString& end)
{
	typedef std::pair<dev_t, ino_t> file_id;
	
	bool success = false;
	if (start.size())
	{
		wxString path = start; //Need a modifyable copy
		struct stat sb;
		std::set<file_id> seen_links;
		
		bool resolved_link;
		do //The symlink resolve loop
		{
			resolved_link = false;
			if (!lstat(path.mb_str(*wxConvCurrent), &sb))
			{
				file_id current_id(sb.st_dev, sb.st_ino);
				if (seen_links.find(current_id) == seen_links.end()) //Not a link we've seen
				{
					seen_links.insert(current_id); //Add to our set
					
					if (S_ISLNK(sb.st_mode)) //Another link
					{
						wxString newpath;
						if (readlink_internal(path, newpath, sb.st_size))
						{
							if (newpath[0] == '/') //Absolute
							{
								path = newpath;
							}
							else //We need to calculate the relative path in relation to the current
							{
								build_path_base_swap(path, newpath);
							}
							resolved_link = true;
						} //Else, Link can't be read, time to quit
					}
					else //Yay, it's not a link! got to the last part finally!
					{
						success = realpath_file(path, end);
					}
				} //Else, Nice try, someone linked a link back into a previous link during the scan to try to trick us into an infinite loop
			} //Else, Dangling link, can't resolve
		} while (resolved_link);
	}
	return(success);
}

bool GetRealPath(const wxString &fileName, wxString &realPath)
{
	bool success = false;
	if (fileName.size())
	{
		struct stat sb;
		if (!stat(fileName.mb_str(*wxConvCurrent), &sb))
		{
			success = S_ISDIR(sb.st_mode) ? chdir_getcwd(fileName, realPath) : symlink_resolve(fileName, realPath);
		}
	}
	return(success);
}

#endif


#if defined(__WXMAC__)

wxString GetBundleResourcesPath()
{
	// If we fail, return current-dir
	const wxString fail(wxT("./"));
	
	// Get the real path to the executable
	wxString executablePath;
	if (!GetRealPath(wxGetApp().argv[0], executablePath))
		return fail;
	
	// Check that we're in a bundle -- the last two directories in the
	// directories list should be Contents/MacOS.
	wxFileName executableFileName(executablePath);
	size_t numDirs = executableFileName.GetDirCount();
	
	if ((executableFileName.GetDirs()[numDirs - 1] != wxT("MacOS")) ||
	    (executableFileName.GetDirs()[numDirs - 2] != wxT("Contents")))
		return fail;
	
	// Okay, clip off the filename and last dir ('/MacOS/')
	executableFileName.SetFullName(wxT(""));
	executableFileName.RemoveLastDir();
	
	// Add the 'Resources' directory
	executableFileName.AppendDir(wxT("Resources"));
	
	// Return the path in native format
	return executableFileName.GetFullPath(wxPATH_NATIVE);
}

#endif


wxString GetDocPath()
{
#if defined(__WXMSW__)

	// On Win32, we just copy the HTB file to the installation
	// directory.
	return wxT("./logos.htb");

#elif defined(__WXMAC__)

	// On Mac, we put the help file inside the application bundle
	return GetBundleResourcesPath() + wxT("logos.htb");

#else

	// Everywhere else, assume we've used autoconf to build, and the
	// user has installed Logos into some prefix or other
	wxString docDir(STRINGIZE(ACDATADIR), *wxConvCurrent);
	if (docDir[docDir.length() - 1] != wxT('/'))
		docDir.Append(wxT('/'));
	docDir.Append(wxT("logos/"));
	
	wxString docFile(docDir);
	docFile.Append(wxT("logos.htb"));
	
	// If the file doesn't exist, try to get a 'DOCDIR' environment variable
	if (!wxFileName::FileExists(docFile))
	{
		wxString docDirEnv;
		if (!wxGetEnv(wxT("DOCDIR"), &docDirEnv))
			return docFile;
		
		if (docDirEnv[docDirEnv.length() - 1] != wxT('/'))
			docDirEnv.Append(wxT('/'));
		docDirEnv.Append(wxT("logos.htb"));
		
		if (wxFileName::FileExists(docDirEnv))
			return docDirEnv;
	}
	
	return docFile;

#endif
}


};


