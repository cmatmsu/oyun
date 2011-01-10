/*
    Copyright (C) 2004-2011 by Charles Pence
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

#ifndef TYPEPAGE_H__
#define TYPEPAGE_H__

#include <wx/wizard.h>
#include "tools/logoswizardpage.h"


/**
    \class TypePage
    \ingroup ui
    
    \brief Wizard page on which user selects tournament type
    
    This is a very simple wizard page containing only two radio buttons,
    which determines whether the tournament will be an evolutionary or
    round-robin tournament.
*/
class TypePage : public LogosWizardPage
{
	DECLARE_CLASS(TypePage)
	
public:
	/**
	    \brief Constructor
	    
	    At this point in the wizard, there is a branch -- the user will
	    continue either to a \c OneShotPage or to an \c EvoPage, depending
	    on the radio button selected.  This constructor thus accepts two
	    'next'-like parameters.
	    
	    \param parent The parent of this wizard page
	    \param prev The previous page in the wizard
	    \param oneshot The page to go to if a one-shot tournament is selected
	    \param evolutionary The page to go to if an evolutionary tournament
	                        is selected
	*/
	TypePage(LogosWizard *parent, wxWizardPage *prev = NULL, 
			 wxWizardPage *oneshot = NULL, 
			 wxWizardPage *evolutionary = NULL);
	
	
	/**
	    \brief Implement a method like \c wxWizardPageSimple::Chain but with
	           two 'next' pages
	    
	    \param type The \c TypePage to be chained
	    \param oneshot The page to go to if a one-shot tournament is selected
	    \param evolutionary The page to go to if an evolutionary tournament
	                        is selected
	*/
	static void Chain(TypePage *type, wxWizardPageSimple *oneshot, wxWizardPageSimple *evolutionary)
	{
		// Default to oneshot when chain is called
		type->SetOneShotPage(oneshot);
		type->SetEvolutionaryPage(evolutionary);
		type->SetNext(oneshot);

		oneshot->SetPrev(type);
		evolutionary->SetPrev(type);
	}
	
	/**
	    \brief Accessor used by the Chain method
	    \param oneshot New value for the next one-shot page
	*/
	void SetOneShotPage(wxWizardPage *oneshot)
	{ nextOneShot = oneshot; }
	
	/**
	    \brief Accessor used by the Chain method
	    \param evolutionary New value for the next evolutionary page
	*/
	void SetEvolutionaryPage(wxWizardPage *evolutionary)
	{ nextEvolutionary = evolutionary; }

private:
	DECLARE_EVENT_TABLE()
	
	/**
	    \brief Show this wizard page's help documentation
	    \param event The event generated
	*/
	void OnHelp(wxWizardEvent &event);
	
	/**
	    \brief Called when a radio button is selected
	    \param event The event generated
	*/
	void OnRadioButton(wxCommandEvent &event);
	
	/**
	    \brief Called when the page receives a 'data update' message	    
	    \param event The event generated
	*/
	void OnDataUpdate(wxNotifyEvent &event);
	
	
	/**
	    \brief The page to travel to if a one-shot tournament is chosen
	*/
	wxWizardPage *nextOneShot;
	
	/**
	    \brief The page to travel to if an evolutionary tournament is chosen
	*/
	wxWizardPage *nextEvolutionary;
	
	
	/**
	    \brief The instruction text printed over the radio buttons
	*/
	wxStaticText *text;
	
	/**
	    \brief The radio button for choosing a one-shot tournament
	*/
	wxRadioButton *rbOneShot;
	
	/**
	    \brief The radio button for choosing an evolutionary tournament
	*/
	wxRadioButton *rbEvolutionary;

	/**
		\brief The text label for the first radio button
	*/
	wxStaticText *rbOneShotLabel;

	/**
		\brief The text label for the second radio button
	*/
	wxStaticText *rbEvolutionaryLabel;
};

#endif

// Local Variables:
// mode: c++
// End:
