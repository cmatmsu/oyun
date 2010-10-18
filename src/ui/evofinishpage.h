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

#ifndef EVOFINISHPAGE_H__
#define EVOFINISHPAGE_H__

#include <wx/wizard.h>
#include "finishpage.h"

class EvoPage;


/**
    \class EvoFinishPage
    \ingroup ui
    
    \brief Wizard page on which the user saves the results of an \c EvoTournament
    
    This page offers the user the choice to save the data from an
    evolutionary tournament -- either the data or the graph itself -- 
    before quitting Logos.
*/
class EvoFinishPage : public FinishPage
{
	DECLARE_CLASS(EvoFinishPage)
	
public:
	/**
	    \brief Constructor
	    
	    Constructs the wizard page and all its controls
	    
	    \param parent The parent of this wizard page
	    \param prev The \c EvoPage that comes before this
	*/
	EvoFinishPage(LogosWizard *parent, EvoPage *prev = NULL);
	
private:
	DECLARE_EVENT_TABLE()
	
	/**
	    \brief Previous \c EvoPage, which holds our data
	*/
	EvoPage *previous;
	
	/**
	    \brief Show this wizard page's help documentation
	    \param event The event generated
	*/
	void OnHelp(wxWizardEvent &event);
	
	/**
	    \brief Called when the "Save Image" button is clicked
	    \param event The event generated
	*/
	void OnSaveImage(wxCommandEvent &event);

	/**
	    \brief Called when the "Save SVG" button is clicked
	    \param event The event generated
	*/
	void OnSaveSVG(wxCommandEvent &event);

	/**
	    \brief Called when the "Save CSV" button is clicked
	    \param event The event generated
	*/
	void OnSaveCSV(wxCommandEvent &event);
};

#endif


