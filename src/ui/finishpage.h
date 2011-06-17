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

#ifndef FINISHPAGE_H__
#define FINISHPAGE_H__

#include <wx/wizard.h>
#include "tools/oyunwizardpage.h"


/**
    \class FinishPage
    \ingroup ui
    
    \brief Base class for all final (finishing) wizard pages
*/
class FinishPage : public OyunWizardPage
{
	DECLARE_CLASS(FinishPage)
	
public:
	/**
	    \brief Constructor
	    
	    Constructs the wizard page and all its controls
	    
	    \param parent The parent of this wizard page
	    \param prev The page that comes before this one
	*/
	FinishPage(OyunWizard *parent, wxWizardPage *prev = NULL);
	
protected:	
	/**
	    \brief True if we've saved some data, false otherwise
	*/
	bool dataSaved;
	
	/**
	    \brief Sizer for the buttons
	*/
	wxFlexGridSizer *buttonSizer;
	
	/**
	    \brief Adds a button used to save a particular kind of data
	    
	    This creates a button with a description beside it.
	    
	    \param id The ID of the button
	    \param label The button label
	    \param description The text description next to the button
	*/
	void AddButton(int id, const wxString &label, const wxString &description);
	
	
private:
	DECLARE_EVENT_TABLE()

	/**
	    \brief Called when the wizard page is changing
	
	    We pop up a message box if the user has yet to save
	    any data.
	
	    \param event The event generated
	*/
	void OnChanging(wxWizardEvent &event);

	/**
	    \brief Instructions to the user
	*/
	wxStaticText *text;
};

#endif

// Local Variables:
// mode: c++
// End:
