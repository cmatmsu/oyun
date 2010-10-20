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

#ifndef LOGOSWIZARDPAGE_H__
#define LOGOSWIZARDPAGE_H__

#include <wx/wizard.h>
class LogosWizard;


/**
    \class LogosWizardPage
    \ingroup ui
    
    \brief Base class for all Logos wizard pages
    
    This class gives us a standardized format for the titles of our
    wizard pages, and standard/default behavior when the user clicks
    the "Cancel" and "Help" buttons within the wizard.
*/
class LogosWizardPage : public wxWizardPageSimple
{
	DECLARE_CLASS(LogosWizardPage)
	
public:
	/**
	    \brief Constructor
	    
	    Creates the title and subtitle (set off against a differently
	    colored background), as well as the basic sizer that the
	    window uses.
	    
	    \param titleStr The title of this wizard page
	    \param subtitleStr The subtitle of this wizard page, a short
	                    sentence describing its use
	    \param parent The \c wxWizard this page will be used with
	    \param prev The previous wizard page
	    \param next The next wizard page
	*/
	LogosWizardPage(const wxString &titleStr, const wxString &subtitleStr, LogosWizard *parent,
	                wxWizardPage *prev = NULL, wxWizardPage *next = NULL);

protected:
	/**
	    \brief Parent of this wizard page
	*/
	LogosWizard *parent;
	
	/**
	    \brief Sizer used for this window
	    
	    This sizer will contain the title and subtitle after the window
	    is constructed.  All derived classes of \c LogosWizardPage should
	    add their controls to this sizer.
	*/
	wxBoxSizer *sizer;

private:
	DECLARE_EVENT_TABLE()

	/**
	    \brief Ensure the user wants to quit
	    
	    Since the "Cancel" button on the wizard will in fact quit Logos,
	    present a message box confirming that this is the user's intent.
	    
	    \param event Event passed from the \c wxWizard
	*/
	void OnWizardCancel(wxWizardEvent &event);
	
	/**
	    \brief Show the help contents
	    
	    If derived classes do not choose to intercept the \c wxWizard help
	    message, this handler will be invoked, which simply shows the
	    help's contents.
	    
	    \param event Event passed from the \c wxWizard
	*/
	void OnHelp(wxWizardEvent &event);

	/**
	    \brief Send a \c wxEVT_DATA_UPDATE when the page changes
	    \param event Event pass from the \c wxWizard
	*/
	void OnPageChange(wxWizardEvent &event);	
	
	/**
	    \brief Container window for title and subtitle
	    
	    This container paints a differently-colored background underneath
	    the title and subtitle.
	*/
	wxWindow *container;
	
	/**
	    \brief Sizer for the contents of the container
	*/
	wxBoxSizer *containerSizer;
	
	
	/**
	    \brief Title text
	*/
	wxStaticText *title;
	
	/**
	    \brief Subtitle text
	*/
	wxStaticText *subtitle;
};

#endif

// Local Variables:
// mode: c++
// End:
