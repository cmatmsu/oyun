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

/**
    \defgroup ui UI Module
    
    This module contains the user interface code.  Oyun is a wxWidgets
    application.  Its entry-point is handled by the \c OyunApp class, 
    which creates a \c wxWizard containing all of our UI.  All pages in
    the wizard are derived from the \c OyunWizardPage class.
    
    The general flow of the Oyun wizard is as follows:
    
    \li \c OyunWizard - the overall wizard object
    \li \c PlayersPage - the user selects a list of players
    \li \c TypePage - the user chooses whether they will run a
        \c Tournament or an \c EvoTournament
    \li \c OneShotPage - if the user has chosen a one-shot round-robin
        tournament (the \c Tournament class), run it
    \li \c OneShotFinishPage - save the data from a one-shot tournament
    \li \c EvoPage - if the user has chosen an evolutionary tournament,
        (the \c EvoTournament class), run it
    \li \c EvoFinishPage - save the data from an evolutionary tournament
*/

#ifndef OYUNAPP_H__
#define OYUNAPP_H__

/**
    \class OyunApp
    \ingroup ui
    
    \brief The main Oyun application class
    
    In usual wxWidgets style, this class constitutes the program entry
    point.  It creates the \c wxWizard and the various pages, links them
    together, and starts the wizard.  When the wizard is closed, execution
    terminates.
*/
class OyunApp : public wxApp
{
	DECLARE_DYNAMIC_CLASS(OyunApp)

public:
	/**
	    \brief Constructor
	    
	    We override the constructor to set a few internal wxWidgets
	    settings, such as configuration paths.
	*/
	OyunApp();
	
	
	/**
	    \brief Called at the beginning of program execution
	    
	    This function does a little initialization, and then creates a wizard
	    which takes over program execution.
	    
	    \returns True if execution is to continue, false otherwise
	*/
	virtual bool OnInit();
	
	
	/**
	    \brief Create the Oyun wizard
	    
	    This function creates the \c OyunWizard class.  It may be called more
	    than once, for example on Mac OS X.
	*/
	void CreateWizard();
	
	/**
	    \brief Show a help page by filename
	    
	    This function opens the online help to a specific page.  By default,
	    \p filename is an empty string, which will show the help's contents.
	    Note that the \p filename expected here is an internal \c .html file,
	    specified as a relative path from the root of the documentation tree.
	    
	    \param parent The window to be the parent of the help dialog
	    \param filename The filename within the documentation tree to open.  If
	                    empty, show the help contents.
	*/
	void ShowHelp(wxWindow *parent = NULL, const wxString &filename = wxT(""));
	
	
	//
	// Mac-specific code
	//	
#ifdef __WXMAC__
	/**
	    \brief Mac override for drag-and-drop
	    
	    This is a special function used for drag-and-drop on OS X.
	    
	    \param filename File dropped on the window
	*/
	virtual void MacOpenFile(const wxString &filename);
	
	
private:
	DECLARE_EVENT_TABLE()
	
	/**
	    \brief Create the common Mac OS X menu bar
	*/
	void CreateMacMenuBar();
	
	/**
	    \brief Called when new is clicked in the menu
	    
	    The menu bar is shown only on Mac OS X.
	    
	    \param event The event generated
	*/
	void OnMenuNew(wxCommandEvent &event);
	
	/**
	    \brief Called when exit is clicked in the menu
	    
	    The menu bar is shown only on Mac OS X.
	    
	    \param event The event generated
	*/
	void OnMenuExit(wxCommandEvent &event);

	/**
	    \brief Called when about is clicked in the menu
	    
	    The menu bar is shown only on Mac OS X.
	    
	    \param event The event generated
	*/
	void OnMenuAbout(wxCommandEvent &event);

	/**
	    \brief Called when help is clicked in the menu
	    
	    The menu bar is shown only on Mac OS X.
	    
	    \param event The event generated
	*/
	void OnMenuHelp(wxCommandEvent &event);
#endif
};

DECLARE_APP(OyunApp);


#endif

// Local Variables:
// mode: c++
// End:
