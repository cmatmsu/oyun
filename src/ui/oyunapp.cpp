/*
    Copyright (C) 2004-2011 by Charles Pence
    charles@charlespence.net

    This file is part of Oyun.

    Oyun is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Oyun is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Oyun.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <wx/wxprec.h>
#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

#include <wx/html/helpctrl.h>
#include <wx/filesys.h>
#include <wx/fs_arc.h>
#include <wx/listctrl.h>
#include <wx/aboutdlg.h>

#include <TestHarness.h>

#include "../common/filesystem.h"
#include "../common/rng.h"

#include "oyunapp.h"
#include "oyunwizard.h"

// Implement the wxApp
IMPLEMENT_DYNAMIC_CLASS(OyunApp, wxApp)
IMPLEMENT_APP(OyunApp)

OyunApp::OyunApp()
{
	// Some assorted wxWidgets initialization calls
	wxInitAllImageHandlers();
	wxFileSystem::AddHandler(new wxArchiveFSHandler);

	// We need a true-color visual for drawing the plots
	// later on
	SetUseBestVisual(true, true);

	// Tell the wxApp who we are
	SetAppName(wxT("Oyun"));
	SetClassName(wxT("Oyun"));
	SetVendorName(wxT("OyunTeam"));
}

bool OyunApp::OnInit()
{
	// Play like a nice Linux application
	for (int i = 1 ; i < argc ; i++)
	{
		if (!wxStrcmp(argv[i], wxT("--version")))
		{
			const wchar_t *version = wxT(STRINGIZE( OYUN_VERSION ));
			const wxString verstring =
				_("Oyun %ls\n"
				  "Copyright (C) 2004-2011 Charles Pence\n"
				  "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
				  "This is free software: you are free to change and redistribute it.\n"
				  "There is NO WARRANTY, to the extent permitted by law.\n");
			wxPrintf(verstring, version);
			
			return false;
		}
		else if (!wxStrcmp(argv[i], wxT("--help")))
		{
			const wxString helpstring =
				_("Usage: oyun [OPTION]...\n"
				  "Run an evolutionary game theory tournament.\n"
				  "\n"
				  "  --test       run the Oyun testing suite\n"
				  "  --help       display this help and exit\n"
				  "  --version    output version information and exit\n"
				  "\n"
				  "Report bugs to: <charles@charlespence.net>.\n"
				  "Oyun home page: <http://charlespence.net/oyun/>.\n");
			wxPrintf(wxT("%s"), helpstring);
			
			return false;
		}
	}
	
	// See if we are to run unit tests
	for (int i = 1 ; i < argc ; i++)
	{
		if (!wxStrcmp(argv[i], wxT("--test")))
		{
			TestResult result;

      // Return the success status in the exit code
      if (TestRegistry::runAllTests(result))
        return EXIT_SUCCESS;
      else
        return EXIT_FAILURE;
		}
		else
		{
			// Invalid command-line parameter
			wxPrintf(_("oyun: unrecognized option `%ls'\n"
			           "Try `oyun --help' for more information.\n"), argv[i]);
			
			return false;
		}
	}
	
	// Seed the RNG
	Random::Seed(time(NULL));
	
#ifdef __WXMAC__
	// Create the common OS X menu bar if we need it
	CreateMacMenuBar();
#endif
	
	// Make the first wizard
	CreateWizard();
	
	return true;
}

void OyunApp::CreateWizard()
{
	// Create a wizard object, and set it as the top-level window
	// if we don't have one already
	OyunWizard *wizard = new OyunWizard(NULL);
	wizard->Show(true);
	
	if (!GetTopWindow())
		SetTopWindow(wizard);
}

void OyunApp::ShowHelp(wxWindow *parent, const wxString &fileName)
{
	if (parent == NULL)
		parent = GetTopWindow();
	
	wxHtmlModalHelp help(parent, FS::GetDocPath(), fileName,
	                     wxHF_DEFAULT_STYLE | wxHF_DIALOG | wxHF_MODAL | wxHF_MERGE_BOOKS);
}


//
// Mac-specific code
//
#ifdef __WXMAC__

// Handle files dropped on the application icon
void OyunApp::MacOpenFile(const wxString &filename)
{
	// See if we have a top-level window
	wxWindow *win = GetTopWindow();
	if (!win)
		return;
	
	OyunWizard *wizard = wxDynamicCast(win, OyunWizard);
	if (!wizard)
		return;
	
	// Pass this to our file-drop handler
	wxArrayString arr;
	arr.Add(filename);
	
	wizard->OnFileDrop(arr);
}


// Declare menu events
BEGIN_EVENT_TABLE(OyunApp, wxApp)
	EVT_MENU(wxID_NEW, OyunApp::OnMenuNew)
	EVT_MENU(wxID_EXIT, OyunApp::OnMenuExit)
	EVT_MENU(wxID_INDEX, OyunApp::OnMenuHelp)
	EVT_MENU(wxID_ABOUT, OyunApp::OnMenuAbout)
END_EVENT_TABLE()

void OyunApp::CreateMacMenuBar()
{
	// Make a top-menu bar for Mac OS X
	wxMenuBar *menuBar = new wxMenuBar();
	wxMenu *fileMenu = new wxMenu;
	fileMenu->Append(wxID_NEW);
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT);
	
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(wxID_INDEX);
	helpMenu->Append(wxID_ABOUT);
	
	menuBar->Append(fileMenu, _("&File"));
	menuBar->Append(helpMenu, _("&Help"));

	wxMenuBar::MacSetCommonMenuBar(menuBar);
}

void OyunApp::OnMenuNew(wxCommandEvent & WXUNUSED(event))
{
	// Create a new wizard
	CreateWizard();
}

void OyunApp::OnMenuExit(wxCommandEvent & WXUNUSED(event))
{
	// Quit, forcibly
	ExitMainLoop();
}

void OyunApp::OnMenuAbout(wxCommandEvent & WXUNUSED(event))
{
	wxString appName = wxT("Oyun");
	wxString appVersion = wxT( STRINGIZE( OYUN_VERSION ) );
	wxString appInfo = _("An evolutionary game theory simulator");
	wxString appCopyright = _("(C) 2004-2011 Charles Pence");

	wxAboutDialogInfo info;
	
	info.SetName(appName);
	info.SetVersion(appVersion);
	info.SetDescription(appInfo);

	info.AddDeveloper(wxT("Charles Pence"));

	info.SetCopyright(appCopyright);
	
	wxAboutBox(info);
}

void OyunApp::OnMenuHelp(wxCommandEvent & WXUNUSED(event))
{
	ShowHelp();
}

#endif


