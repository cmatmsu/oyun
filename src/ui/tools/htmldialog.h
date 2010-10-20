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

#ifndef HTMLDIALOG_H__
#define HTMLDIALOG_H__

#include <wx/html/htmlwin.h>


/**
    \class HtmlDialog
    \ingroup ui
    
    \brief Dialog box which displays an HTML page
    
    This is a straightforward dialog box which has a title and displays
    a page of HTML.  First construct one, and then set its contents
    with the SetHtml method.
*/
class HtmlDialog : public wxDialog
{
	DECLARE_CLASS(HtmlDialog)
	
public:
	/**
	    \brief Constructor
	    
	    Create a dialog box.  All parameters are as per the \c wxDialog constructor, with the
	    exception of the \p title parameter, which will also be displayed as a large title
	    text above the HTML display.
	    
	    \param parent Can be NULL, a frame, or another dialog box
	    \param id An identifier for the dialog.  A value of \c -1 is taken to mean a default
	    \param title The title of the dialog
	    \param pos The dialog position.  The value \c wxDefaultPosition indicates a default
	               position, chosen by either the windowing system or wxWidgets, depending on
	               platform
	    \param size The dialog size.  The value \c wxDefaultSize indicates a default size,
	                chosen by either the windowing system or wxWidgets, depending on platform
	    \param style The window style
	    \param name Used to associate a name with the window, allowing the application to set
	                user Motif resource values for individual dialog boxes
	*/
	HtmlDialog (wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos = wxDefaultPosition,
	            const wxSize &size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, 
	            const wxString &name = wxT("dialogBox"));
	
	/**
	    \brief Set the displayed HTML page
	    
	    \param html A full HTML document to be displayed in the dialog box
	*/
	void SetHtml (const wxString &html)
	{ htmlWindow->SetPage(html); }

private:
	/**
	    \brief The window's sizer
	*/
	wxBoxSizer *sizer;
	
	
	/**
	    \brief The displayed title text
	*/
	wxStaticText *text;
	
	/**
	    \brief The window which will display the HTML page
	*/
	wxHtmlWindow *htmlWindow;
	
	/**
	    \brief The OK button in the dialog
	*/
	wxButton *ok;
};

#endif

// Local Variables:
// mode: c++
// End:
