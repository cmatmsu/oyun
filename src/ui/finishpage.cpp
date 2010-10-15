/*
    Copyright (C) 2004-2009 by Charles Pence
    [cpence at gmail dot com]

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

#include "finishpage.h"

IMPLEMENT_CLASS(FinishPage, LogosWizardPage)


BEGIN_EVENT_TABLE(FinishPage, LogosWizardPage)
	EVT_WIZARD_PAGE_CHANGING(wxID_ANY, FinishPage::OnChanging)
END_EVENT_TABLE()



FinishPage::FinishPage(LogosWizard *parent, wxWizardPage *prev) :
                       LogosWizardPage(_("Save Tournament Results"),
                                       _("You can save the data from this tournament in a variety of ways."),
                                       parent, prev, NULL)
{
	dataSaved = false;
	
	
	wxString instructions = _("You have finished running the tournament, and you can now save\n"
	                          "the data you collected in several different formats.\n\n"
	                          "Once you press Finish, Logos will quit.");
	
	// Make all of our controls
	text = new wxStaticText(this, wxID_ANY, instructions);
	text->Wrap(GetClientSize().GetWidth() - 24);
	
	
	// Add the controls to the sizer
	sizer->Add(text, 0, wxEXPAND | wxALL, 12);
	
	// Make the button sizer (for start, with no rows or columns)
	buttonSizer = new wxFlexGridSizer(2, 6, 6);
	buttonSizer->AddGrowableCol(1);
	buttonSizer->SetFlexibleDirection(wxHORIZONTAL);
	buttonSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_ALL);
	
	sizer->Add(buttonSizer, 1, wxEXPAND | wxALL, 12);
}

void FinishPage::AddButton(int id, const wxString &label, const wxString &description)
{
	wxButton *button = new wxButton(this, id, label);
	wxStaticText *text = new wxStaticText(this, wxID_ANY, description);
	
	buttonSizer->Add(button, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 24);
	buttonSizer->Add(text, 0, wxALIGN_CENTER_VERTICAL);
}


void FinishPage::OnChanging(wxWizardEvent &event)
{
	if (event.GetDirection() != true)
	{
		event.Skip();
		return;
	}
	
	// Don't complain if they've already saved some data
	if (dataSaved)
	{
		event.Skip();
		return;
	}
	
	if (wxMessageBox(_("Finishing the wizard will quit Logos, and you haven't saved the data from your tournament yet.\n\n"
	                   "Are you sure you want to quit?"),
	                 _("Quit"), wxICON_QUESTION | wxYES_NO, this) != wxYES)
	{
		// User decided to cancel
		event.Veto();
		return;
	}
	
	// Okay, do whatever
	event.Skip();
}


