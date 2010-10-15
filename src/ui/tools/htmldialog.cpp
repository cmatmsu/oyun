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

#include "htmldialog.h"

IMPLEMENT_CLASS(HtmlDialog, wxDialog)

HtmlDialog::HtmlDialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos,
                       const wxSize &size, long style, const wxString &name) : 
                       wxDialog(parent, id, title, pos, size, style, name)
{
        text = new wxStaticText(this, wxID_ANY, title);
        text->SetFont(wxFont(14, wxSWISS, wxNORMAL, wxBOLD));

	htmlWindow = new wxHtmlWindow(this, wxID_ANY, wxDefaultPosition, wxSize(350, 200),
                                      wxHW_SCROLLBAR_AUTO | wxBORDER_SUNKEN);
	
	ok = new wxButton(this, wxID_OK);
	ok->SetDefault();

	sizer = new wxBoxSizer(wxVERTICAL);	
        sizer->Add(text, 0, wxEXPAND | wxALL, 6);
        sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 6);
	sizer->Add(htmlWindow, 1, wxEXPAND | wxALL, 12);
	sizer->Add(ok, 0, wxALIGN_RIGHT | wxALL, 12);

        SetSizerAndFit(sizer);
}


