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

#include <wx/wxprec.h>
#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

#include "typepage.h"
#include "logosapp.h"

IMPLEMENT_CLASS(TypePage, LogosWizardPage)

enum
{
	ID_RB_ONESHOT = wxID_HIGHEST + 1,
	ID_RB_EVOLUTIONARY,
	ID_RB_ONESHOT_LABEL,
	ID_RB_EVOLUTIONARY_LABEL
};


BEGIN_EVENT_TABLE(TypePage, LogosWizardPage)
	EVT_WIZARD_HELP(wxID_ANY, TypePage::OnHelp)
	EVT_RADIOBUTTON(ID_RB_ONESHOT, TypePage::OnRadioButton)
	EVT_RADIOBUTTON(ID_RB_EVOLUTIONARY, TypePage::OnRadioButton)
END_EVENT_TABLE()


TypePage::TypePage(LogosWizard *parent, wxWizardPage *prev, wxWizardPage *oneshot, 
                   wxWizardPage *evolutionary) :
                   LogosWizardPage(_("Choose Tournament Type"),
                                   _("Two different types of game-theoretic tournaments are available."),
                                   parent, prev)
{
	// Save member pointers
	nextOneShot = oneshot;
	nextEvolutionary = evolutionary;
	
	// For starters, we default to one-shot.  Update it now, just to be
	// safe.
	SetNext(oneshot);
	
	// Make some controls
	rbOneShotSizer = new wxBoxSizer(wxHORIZONTAL);
	rbEvolutionarySizer = new wxBoxSizer(wxHORIZONTAL);

	rbOneShot = new wxRadioButton(this, ID_RB_ONESHOT, "", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	rbOneShot->SetValue(true);
	rbEvolutionary = new wxRadioButton(this, ID_RB_EVOLUTIONARY, "");

	rbOneShotLabel = new wxStaticText(this, ID_RB_ONESHOT_LABEL,
		_("&One-Shot Tournament:\n\n"
		"\u2022 Run one iterated prisoner's dilemma\n"
		"tournament between a group of competitors.\n"
		"\u2022 Results are presented as a score and\n"
		"individual match-ups can be viewed in detail."));
	rbEvolutionaryLabel = new wxStaticText(this, ID_RB_EVOLUTIONARY_LABEL,
		_("&Evolutionary Tournament:\n\n"
		"\u2022 Run an evolutionary tournament between\n"
		"a group of competitors.\n"
		"\u2022 Score at each generation is used to\n"
		"determine each player's proportion in the\n"
		"population in the next generation.\n"
		"\u2022 Results are presented as a graph of the\n"
		"population over time."));

	rbOneShotSizer->Add(rbOneShot, 0, wxALL, 5);
	rbOneShotSizer->Add(rbOneShotLabel, 0, wxALL, 5);

	rbEvolutionarySizer->Add(rbEvolutionary, 0, wxALL, 5);
	rbEvolutionarySizer->Add(rbEvolutionaryLabel, 0, wxALL, 5);
	
	sizer->AddStretchSpacer();
	sizer->Add(rbOneShotSizer, 0, wxLEFT | wxRIGHT, 100);
	sizer->AddStretchSpacer();
	sizer->Add(rbEvolutionarySizer, 0, wxLEFT | wxRIGHT, 100);
	sizer->AddStretchSpacer();
}


void TypePage::OnHelp(wxWizardEvent & WXUNUSED(event))
{
	wxGetApp().ShowHelp(this, wxT("Tournament-Type-Page.html"));
}

void TypePage::OnRadioButton(wxCommandEvent & WXUNUSED(event))
{
	if (rbOneShot->GetValue())
	{
		// Set the next page to one-shot
		SetNext(nextOneShot);
	}
	else
	{
		// Set the next page to evolutionary
		SetNext(nextEvolutionary);
	}
}

