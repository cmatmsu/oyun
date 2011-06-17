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

#include <wx/statline.h>

#include "oyunwizardpage.h"

#include "../oyunapp.h"
#include "../oyunwizard.h"

IMPLEMENT_CLASS(OyunWizardPage, wxWizardPageSimple)

BEGIN_EVENT_TABLE(OyunWizardPage, wxWizardPageSimple)
	EVT_WIZARD_CANCEL(wxID_ANY, OyunWizardPage::OnWizardCancel)
	EVT_WIZARD_HELP(wxID_ANY, OyunWizardPage::OnHelp)
	EVT_WIZARD_PAGE_CHANGED(wxID_ANY, OyunWizardPage::OnPageChange)
END_EVENT_TABLE()


OyunWizardPage::OyunWizardPage(const wxString &titleStr, const wxString &subtitleStr,
    OyunWizard *wizard, wxWizardPage *prev, wxWizardPage *next) : 
    wxWizardPageSimple(wizard, prev, next), parent(wizard)
{
	// Make the container and its sizer
	container = new wxWindow(this, wxID_ANY);
	container->SetBackgroundStyle(wxBG_STYLE_COLOUR);
	container->SetOwnBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	
	// Query the fonts for the controls	
	wxFont defaultFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	wxFont boldFont = defaultFont;
	boldFont.SetWeight(wxFONTWEIGHT_BOLD);
	
	// Make the text controls
	title = new wxStaticText(container, wxID_ANY, titleStr);
	title->SetFont(boldFont);
	
	subtitle = new wxStaticText(container, wxID_ANY, subtitleStr);
	subtitle->SetFont(defaultFont);
	
	// Add everything to the sizer
	containerSizer = new wxBoxSizer(wxVERTICAL);
	container->SetSizer(containerSizer);

	containerSizer->AddSpacer(12);
	containerSizer->Add(title, 0, wxLEFT | wxRIGHT, 6);
	containerSizer->AddSpacer(6);
	containerSizer->Add(subtitle, 0, wxLEFT | wxRIGHT, 12);
	containerSizer->AddSpacer(12);
	containerSizer->Fit(container);

	// Make the sizer for the main window and fill it	
	sizer = new wxBoxSizer(wxVERTICAL);
	
	SetSizer(sizer);
	sizer->Add(container, 0, wxEXPAND);
	sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND);
	sizer->AddSpacer(12);

	containerSizer->Layout();
}

void OyunWizardPage::OnWizardCancel(wxWizardEvent &event)
{
	if (wxMessageBox(_("Closing this wizard will quit Oyun.\n\n"
	                   "Are you sure you want to quit?"),
	                 _("Quit"), wxICON_QUESTION | wxYES_NO, this) != wxYES)
	{
		// User decided to cancel
		event.Veto();
		return;
	}
	
	// Otherwise, let this go
	event.Skip();
}

void OyunWizardPage::OnHelp(wxWizardEvent & WXUNUSED(event))
{
	wxGetApp().ShowHelp(this);
}

void OyunWizardPage::OnPageChange(wxWizardEvent & WXUNUSED(event))
{
	parent->Update();
}

