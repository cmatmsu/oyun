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

#ifndef ONESHOTFINISHPAGE_H__
#define ONESHOTFINISHPAGE_H__

#include <wx/wizard.h>
#include "finishpage.h"

class OneShotPage;


/**
    \class OneShotFinishPage
    \ingroup ui
    
    \brief Wizard page on which the user saves the results of a \c Tournament
    
    This page offers the user the choice to save the data from a
    one-shot tournament (including scores and match details)
    before quitting Logos.
*/
class OneShotFinishPage : public FinishPage
{
	DECLARE_CLASS(OneShotFinishPage)
	
public:
	/**
	    \brief Constructor
	    
	    Constructs the wizard page and all its controls
	    
	    \param parent The parent of this wizard page
	    \param prev The \c OneShotPage that comes before this
	*/
	OneShotFinishPage(LogosWizard *parent, OneShotPage *prev = NULL);
	
private:
	DECLARE_EVENT_TABLE()
	
	/**
	    \brief Previous \c OneShotPage, which holds our data
	*/
	OneShotPage *previous;
	
	/**
	    \brief Show this wizard page's help documentation
	    \param event The event generated
	*/
	void OnHelp(wxWizardEvent &event);
	
	/**
	    \brief Called when the "Save CSV" button is clicked
	    \param event The event generated
	*/
	void OnSaveCSV(wxCommandEvent &event);
	
	/**
	    \brief Called when the "Save Text" button is clicked
	    \param event The event generated
	*/
	void OnSaveText(wxCommandEvent &event);
};

#endif

// Local Variables:
// mode: c++
// End:
