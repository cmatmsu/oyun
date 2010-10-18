/*
    Copyright (C) 2004-2010 by Charles Pence
    charles@charlespence.net

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

#ifndef FILESYSTEM_H__
#define FILESYSTEM_H__

#define XSTRINGIZE(x) #x
#define STRINGIZE(x) XSTRINGIZE(x)

/**
    \namespace FS
    \brief Namespace containing file system utility code
*/
namespace FS
{

/**
    \brief Get the full path of a relative file
    \ingroup common
    
    This is a platform-agnostic version of realpath(), without any silly
    buffer size restrictions.
    
    \param fileName Relative path to file (in)
    \param realPath Fully expanded file path (out)
    \returns True if successful, false otherwise
*/
bool GetRealPath(const wxString &fileName, wxString &realPath);


/**
    \brief Get the installed path to the documentation
    \ingroup common
    
    This is a platform-specific function (possibly set at compile-time)
    that tells us where we've installed our platform documentation.
    
    \returns File path to the \c logos.htb file
*/
wxString GetDocPath();

};

#endif

