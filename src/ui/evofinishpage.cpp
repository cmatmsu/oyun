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

#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/filename.h>
#include <wx/wfstream.h>

#include "../tourney/evotournament.h"

#include "logosapp.h"
#include "evofinishpage.h"
#include "evopage.h"

enum
{
	ID_SAVE_IMAGE = wxID_HIGHEST,
	ID_SAVE_SVG,
	ID_SAVE_CSV
};

IMPLEMENT_CLASS(EvoFinishPage, FinishPage)


BEGIN_EVENT_TABLE(EvoFinishPage, FinishPage)
	EVT_WIZARD_HELP(wxID_ANY, EvoFinishPage::OnHelp)
	EVT_BUTTON(ID_SAVE_IMAGE, EvoFinishPage::OnSaveImage)
	EVT_BUTTON(ID_SAVE_SVG, EvoFinishPage::OnSaveSVG)
	EVT_BUTTON(ID_SAVE_CSV, EvoFinishPage::OnSaveCSV)
END_EVENT_TABLE()



EvoFinishPage::EvoFinishPage(LogosWizard *parent, EvoPage *prev) :
                             FinishPage(parent, (wxWizardPage *)prev)
{
	previous = prev;
	
	AddButton(ID_SAVE_IMAGE, _("Save &Image..."), _("Save an image of the graph of the tournament results.\n\n"
	                                                "You can save the graph displayed on the previous page in\n"
	                                                "any one of several image formats (like JPG, PNG, BMP, and\n"
	                                                "so on)."));
	AddButton(ID_SAVE_SVG, _("Save &SVG..."), _("Save an SVG image of the graph of the tournament results.\n\n"
	                                            "This is a scalable image, with better quality than the above,\n"
	                                            "but can only be opened by some programs (such as Adobe Illustrator\n"
	                                            "or Inkscape)."));
	AddButton(ID_SAVE_CSV, _("Save CS&V..."), _("Save a spreadsheet of the detailed tournament data.\n\n"
	                                            "This is a spreadsheet containing the frequency of each player\n"
	                                            "at each generation in the evolutionary tournament."));
}

void EvoFinishPage::OnHelp(wxWizardEvent & WXUNUSED(event))
{
	wxGetApp().ShowHelp(this, wxT("evofinishpage.html"));
}

void EvoFinishPage::OnSaveImage(wxCommandEvent & WXUNUSED(event))
{
	// Get a filename from the user
	static const wxString filter(_("JPEG image (*.jpg)|*.jpg|PNG image (*.png)|*.png|BMP image (*.bmp)|*.bmp"));
	wxString str;
	
	str = wxFileSelector(_("Select where to save the image file"), wxEmptyString, _("graph.jpg"),
	                     wxT(".jpg"), filter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT, this);
	if (str.IsEmpty())
		return;
	
	// Figure out what the type of the file is
	wxFileName filename(str);
	wxString extension = filename.GetExt();
	wxString mimeType;
	
	if (extension == wxT("jpg"))
		mimeType = "image/jpeg";
	else if (extension == wxT("png"))
		mimeType = "image/png";
	else if (extension == wxT("bmp"))
		mimeType = "image/x-bmp";
	else
	{
		filename.SetExt(wxT("jpg"));
		mimeType = "image/jpeg";
	}
	
	// Save the image file
	wxFileOutputStream fileStream(filename.GetFullPath());
	if (fileStream.IsOk())
		previous->imageGraph.SaveFile(fileStream, mimeType);
	else
		; // FIXME: show error box
	
	dataSaved = true;
}

void EvoFinishPage::OnSaveSVG(wxCommandEvent & WXUNUSED(event))
{
	// Get a filename from the user
	static const wxString filter(_("SVG image (*.svg)|*.svg"));
	wxString str;
	
	str = wxFileSelector(_("Select where to save the SVG file"), wxEmptyString, _("graph.svg"),
	                     wxT(".svg"), filter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT, this);
	if (str.IsEmpty())
		return;
	
	// Figure out what the type of the file is
	wxFileName filename(str);
	wxString extension = filename.GetExt();
	
	if (extension != wxT("svg"))
		filename.SetExt(wxT("svg"));
	
	// Save the SVG file
	wxFile file(filename.GetFullPath(), wxFile::write);
	if (!file.IsOpened())
		return;
	
	file.Write(previous->svgGraph);
	file.Flush();
	file.Close();

	dataSaved = true;
}

void EvoFinishPage::OnSaveCSV(wxCommandEvent & WXUNUSED(event))
{
	// Get a filename from the user
	static const wxString filter(_("CSV spreadsheet (*.csv)|*.csv"));
	wxString str;
	
	str = wxFileSelector(_("Select where to save the CSV file"), wxEmptyString, _("tournament.csv"),
	                     wxT(".csv"), filter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT, this);
	if (str.IsEmpty())
		return;
	
	// Figure out what the type of the file is
	wxFileName filename(str);
	wxString extension = filename.GetExt();
	
	if (extension != wxT("csv"))
		filename.SetExt(wxT("csv"));

	// Save the CSV file
	wxTextFile file(filename.GetFullPath());

	if (file.Exists())
	{
		file.Open();
		file.Clear();
	}
	else
		file.Create();

	// Generate the lines of the CSV file	
	size_t numPlayers = previous->evoTourney->players.GetCount();
	size_t numGenerations = previous->evoTourney->data.GetCount();
	wxArrayString lines;
	lines.SetCount(numPlayers);
	
	for (size_t p = 0 ; p < numPlayers ; p++)
	{
		Player *player = previous->evoTourney->players[p];
		int id = player->GetID();
		
		// Start by writing out the player name and author
		wxString playerData;
		playerData = player->GetPlayerName() + wxT(",") +
		             player->GetPlayerAuthor() + wxT(",");
		
		lines[p] = playerData;
		
		for (size_t gen = 0 ; gen < numGenerations ; gen++)
		{
			// Get this generation's worth of data
			GenerationWeights &w = previous->evoTourney->data[gen];
			
			// Get the value for this player at this generation
			wxString val;
			val.Printf(wxT("%f"), w[id]);
			
			// Append a comma if we need to
			if (gen != numGenerations - 1)
				val += wxT(",");
			
			// Add the value to the line
			lines[p] += val;
		}
	}

	// Write the file header
	wxString header = _("Player Name") + wxString(wxT(",")) +
	                  _("Player Author") + wxString(wxT(",")) +
	                  _("Initial Fraction") + wxString(wxT(","));
	for (size_t gen = 1 ; gen < numGenerations ; gen++)
	{
		header += wxString::Format(_("Generation %d"), gen);
		if (gen != numGenerations - 1)
			header += wxT(",");
	}
	file.AddLine(header);
	
	// Write the data
	for (size_t p = 0 ; p < numPlayers ; p++)
		file.AddLine(lines[p]);

	file.Write();
	file.Close();

	dataSaved = true;
}

