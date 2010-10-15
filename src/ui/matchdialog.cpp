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

#include <wx/statline.h>

#include "matchdialog.h"
#include "../tourney/match.h"

IMPLEMENT_CLASS(MatchDialog, wxDialog)


MatchDialog::MatchDialog(wxWindow *parent, Match *match) :
	wxDialog(parent, wxID_ANY, wxString(_("Match Details")))
{
	// Make the header controls
	header = new wxStaticText(this, wxID_ANY, _("Match Details"));
	header->SetFont(wxFont(14, wxSWISS, wxNORMAL, wxBOLD));
	
	// Get a font for this
	int pointSize = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT).GetPointSize();
	wxFont fixed(pointSize, wxMODERN, wxNORMAL, wxNORMAL);
	
	// Get the font metrics and figure out what the dimensions of our text areas should
	// be
	wxCoord width, height;
	GetTextExtent(wxT("CD"), &width, &height, NULL, NULL, &fixed);
	
	// Assume 3*height + a little bit is good for the scrollbar and two lines
	height *= 3;
	height += 12;
	width *= 30;
	
	for (size_t i = 0 ; i < 5 ; i++)
	{
		wxString str;
		str.Printf(_("Match game %d:"), i + 1);
		labels[i] = new wxStaticText(this, wxID_ANY, str);
		
		str.Clear();
		wxArrayString &history = match->matchHistory[i];
		
		str += _("Player 1: ");
		for (size_t j = 0 ; j < history.size() ; j++)
			str += history[j][0];
		str += wxT("\n");
		
		str += _("Player 2: ");
		for (size_t j = 0 ; j < history.size() ; j++)
			str += history[j][1];
		
		games[i] = new wxTextCtrl(this, wxID_ANY, str, wxDefaultPosition, wxSize(width, height),
		                          wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL | wxTE_DONTWRAP);
		games[i]->SetFont(fixed);
		games[i]->ShowPosition(0);
	}
	
	ok = new wxButton(this, wxID_OK);
	ok->SetDefault();
	
	// Add the controls to a sizer
	sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(header, 0, wxEXPAND | wxALL, 6);
	sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 6);
	
	for (size_t i = 0 ; i < 5 ; i++)
	{
		sizer->Add(labels[i], 0, wxEXPAND | wxALL, 6);
		sizer->Add(games[i], 0, wxEXPAND | wxALL, 12);
	}
	
	sizer->Add(ok, 0, wxALIGN_RIGHT | wxALL, 12);
	
	SetSizerAndFit(sizer);
}


