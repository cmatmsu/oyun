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

#include <wx/textfile.h>
#include <wx/filename.h>

#include "../tourney/tournament.h"

#include "logosapp.h"
#include "oneshotfinishpage.h"
#include "oneshotpage.h"

enum
{
	ID_SAVE_CSV = wxID_HIGHEST,
	ID_SAVE_TEXT
};

IMPLEMENT_CLASS(OneShotFinishPage, FinishPage)


BEGIN_EVENT_TABLE(OneShotFinishPage, FinishPage)
	EVT_WIZARD_HELP(wxID_ANY, OneShotFinishPage::OnHelp)
	EVT_BUTTON(ID_SAVE_CSV, OneShotFinishPage::OnSaveCSV)
	EVT_BUTTON(ID_SAVE_TEXT, OneShotFinishPage::OnSaveText)
END_EVENT_TABLE()



OneShotFinishPage::OneShotFinishPage(LogosWizard *parent, OneShotPage *prev) :
                                     FinishPage(parent, (wxWizardPage *)prev)
{
	previous = prev;
	
	AddButton(ID_SAVE_CSV, _("Save CS&V..."), _("Save a spreadsheet containing the summary data.\n\n"
	                                            "This is a spreadsheet with the overview data of each\n"
	                                            "player's net score and each played match."));
	AddButton(ID_SAVE_TEXT, _("Save &Details..."), _("Save a text file with detailed information.\n\n"
	                                                 "This is a document containing not only the information\n"
	                                                 "about player net scores and match summaries, but the\n"
	                                                 "detailed list of moves played in each match."));
}

void OneShotFinishPage::OnHelp(wxWizardEvent & WXUNUSED(event))
{
	wxGetApp().ShowHelp(this, wxT("oneshotfinishpage.html"));
}

void OneShotFinishPage::OnSaveCSV(wxCommandEvent & WXUNUSED(event))
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
	
	// This is a seven-column CSV:
	//
	// Logos Tournament Summary
	//
	// Player, Author, Net Score
	// ...
	//
	// Player 1, Player 1 Author, Player 2, Player 2 Author, Winner, Player 1 Score, Player 2 Score
	// ...
	
	// Write the header
	wxString header = _("Logos Tournament Summary") + wxString(wxT(",,,,,,"));
	file.AddLine(header);
	
	// Write a blank line
	wxString blankLine = wxT(",,,,,,");
	file.AddLine(blankLine);
	
	// Write the header for the first block
	wxString firstHeader = _("Player Name") + wxString(wxT(",")) +
	                       _("Player Author") + wxString(wxT(",")) +
	                       _("Net Score") + wxString(wxT(",,,,"));
	file.AddLine(firstHeader);
	
	// Add the data for the first table
	size_t numPlayers = previous->tourney->playerOneList.GetCount();
	for (size_t p = 0 ; p < numPlayers ; p++)
	{
		Player *player = previous->tourney->playerOneList[p];
		int score = previous->tourney->scores[player->GetID()];
		
		wxString line = player->GetPlayerName() + wxT(",") +
		                player->GetPlayerAuthor() + wxT(",") +
		                wxString::Format(wxT("%d"), score) + wxT(",,,,");
		file.AddLine(line);
	}
	
	// Add another blank line
	file.AddLine(blankLine);
	
	// Write the header for the second block
	wxString secondHeader = _("Player 1") + wxString(wxT(",")) +
	                        _("Player 1 Author") + wxString(wxT(",")) +
	                        _("Player 2") + wxString(wxT(",")) +
	                        _("Player 2 Author") + wxString(wxT(",")) +
	                        _("Winner") + wxString(wxT(",")) +
	                        _("Player 1 Score") + wxString(wxT(",")) +
	                        _("Player 2 Score");
	file.AddLine(secondHeader);
	
	size_t numMatches = previous->tourney->GetNumMatches();
	for (size_t m = 0 ; m < numMatches ; m++)
	{
		Match *match = previous->tourney->GetMatch(m);
		
		wxString result;
		if (match->playerOneScore > match->playerTwoScore)
			result = _("Player One");
		else if (match->playerTwoScore > match->playerOneScore)
			result = _("Player Two");
		else
			result = _("Tie");
		
		wxString line = match->playerOne->GetPlayerName() + wxT(",") +
		                match->playerOne->GetPlayerAuthor() + wxT(",") +
		                match->playerTwo->GetPlayerName() + wxT(",") +
		                match->playerTwo->GetPlayerAuthor() + wxT(",") +
		                result + wxT(",") +
		                wxString::Format(wxT("%d"), match->playerOneScore) + wxT(",") +
		                wxString::Format(wxT("%d"), match->playerTwoScore);
		file.AddLine(line);
	}
	
	file.Write();
	file.Close();
	
	dataSaved = true;
}

void OneShotFinishPage::OnSaveText(wxCommandEvent & WXUNUSED(event))
{
	// Get a filename from the user
	static const wxString filter(_("RTF document (*.rtf)|*.rtf"));
	wxString str;
	
	str = wxFileSelector(_("Select where to save the RTF file"), wxEmptyString, _("tournament.rtf"),
	                     wxT(".rtf"), filter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT, this);
	if (str.IsEmpty())
		return;
	
	// Figure out what the type of the file is
	wxFileName filename(str);
	wxString extension = filename.GetExt();
	
	if (extension != wxT("rtf"))
		filename.SetExt(wxT("rtf"));

	// Save the RTF file
	wxTextFile file(filename.GetFullPath());

	if (file.Exists())
	{
		file.Open();
		file.Clear();
	}
	else
		file.Create();	
	
	// Write a standard RTF file header
	file.AddLine(wxT("{\\rtf1\\ansi\\ansicpg1252\\deff0{\\fonttbl{\\f0\\fswiss Arial;}{\\f1\\fmodern Courier New;}}\\fs24\n"));
	
	// we want to write lines like:
	// {\par\pard\plain Text!}
	//
	// To enable/disable font attributes we do:
	// \fs24 = 12-pt font
	// \fs36 = 18-pt font
	// \b = bold font
	// \b0 = disable bold font
	// \i = italic font
	// \i0 = disable italic font
	
	// We'll need this often later
	wxString blankLine = wxString(wxT("{\\par\\pard\\plain  }"));
	
	// Write a heading
	wxString header = wxString(wxT("{\\par\\pard\\plain \\fs36\\b ")) +
	                  _("Logos: Detailed Tournament Report") + 
	                  wxString(wxT(" \\b0\\fs24}"));
	file.AddLine(header);
	file.AddLine(blankLine);
	file.AddLine(blankLine);
	
	// Write a header for the player summary data
	wxString playerSumm = wxString(wxT("{\\par\\pard\\plain \\fs28\\i ")) +
	                      _("Player Summary Data") +
	                      wxString(wxT(" \\i0}"));
	file.AddLine(playerSumm);
	file.AddLine(blankLine);
	
	// Write the player summary data
	size_t numPlayers = previous->tourney->playerOneList.GetCount();
	for (size_t p = 0 ; p < numPlayers ; p++)
	{
		Player *player = previous->tourney->playerOneList[p];
		int score = previous->tourney->scores[player->GetID()];
		wxString scoreStr = wxString::Format(wxT("%i"), score);
		
		wxString line;
		line = wxT("{\\par\\pard\\plain ") +
		       player->GetPlayerName() + wxT(" [") +
		       player->GetPlayerAuthor() + wxT("]: ") +
		       scoreStr + wxT(" ") +
		       _("points") + wxT(" }");
		
		file.AddLine(line);
	}
	file.AddLine(blankLine);
	file.AddLine(blankLine);
	
	// Write a header for the match summary data
	wxString matchSumm = wxString(wxT("{\\par\\pard\\plain \\fs28\\i ")) +
	                     _("Match Summary Data") +
	                     wxString(wxT(" \\i0}"));
	file.AddLine(matchSumm);
	file.AddLine(blankLine);
	
	// Write the match summary info
	size_t numMatches = previous->tourney->GetNumMatches();
	for (size_t m = 0 ; m < numMatches ; m++)
	{
		Match *match = previous->tourney->GetMatch(m);

		wxString playerOne = wxT("\\b ") +
		                     match->playerOne->GetPlayerName() + wxT(" [") +
		                     match->playerOne->GetPlayerAuthor() + wxT("]") +
		                     wxT("\\b0 ");
		wxString playerTwo = wxT("\\b ") +
		                     match->playerTwo->GetPlayerName() + wxT(" [") +
		                     match->playerTwo->GetPlayerAuthor() + wxT("]") +
		                     wxT("\\b0 ");
		wxString players = wxString::Format(_("%s played %s:"), playerOne.c_str(), playerTwo.c_str());
		
		wxString result;
		if (match->playerOneScore == match->playerTwoScore)
		{
			result = wxString::Format(_("the match was a tie, %d to %d"), 
			                          match->playerOneScore, match->playerTwoScore);
		}
		else
		{
			wxString winner;
			int winnerScore, loserScore;
			
			if (match->playerOneScore > match->playerTwoScore)
			{
				winner = wxString(wxT("\\b ")) + _("Player One") + wxString(wxT("\\b0 "));
				winnerScore = match->playerOneScore;
				loserScore = match->playerTwoScore;
			}
			else
			{
				winner = wxString(wxT("\\b ")) + _("Player Two") + wxString(wxT("\\b0 "));
				winnerScore = match->playerTwoScore;
				loserScore = match->playerOneScore;
			}
			
			result = wxString::Format(_("%s won, by a score of %d to %d"), winner.c_str(),
			                          winnerScore, loserScore);
		}
		
		wxString line = wxT("{\\par\\pard\\plain \\~\\~\\~\\bullet\\~\\~\\~ ") + 
		                players + wxT(" ") + result + wxT(" }");
		file.AddLine(line);
	}
	
	// Now, write detailed data for each match
	for (size_t m = 0 ; m < numMatches ; m++)
	{
		Match *match = previous->tourney->GetMatch(m);
		
		// Emit a page break
		file.AddLine(wxT("\\page"));
		
		// Once again, get the player strings
		wxString playerOne = match->playerOne->GetPlayerName() + wxT(" [") +
		                     match->playerOne->GetPlayerAuthor() + wxT("]");
		wxString playerTwo = match->playerTwo->GetPlayerName() + wxT(" [") +
		                     match->playerTwo->GetPlayerAuthor() + wxT("]");
		
		// Make a header
		wxString secHeader = wxString(wxT("{\\par\\pard\\plain \\fs32\\b ")) +
		                     wxString::Format(_("Match %d: %s vs. %s"), m + 1, playerOne.c_str(), playerTwo.c_str()) + 
	        	             wxString(wxT(" \\b0\\fs24}"));
		file.AddLine(secHeader);
		file.AddLine(blankLine);
		file.AddLine(blankLine);
		
		// These pages deserve some explanation
		wxString descrip = wxString(wxT("{\\par\\pard\\plain ")) +
		                   _("The moves for this match are listed in order, in groups of forty.  Moves for player one "
		                     "are on the first line of each group, player two on the second line.") +
		                   wxString(wxT(" }"));
		file.AddLine(descrip);
		file.AddLine(blankLine);
		
		// Output information for each of the five games
		for (size_t gm = 0 ; gm < 5 ; gm++)
		{
			wxArrayString &history = match->matchHistory[gm];
			size_t numMoves = history.size();
			
			// Output info on which game this is
			wxString gameHeader = wxString(wxT("{\\par\\pard\\plain ")) +
			                      wxString::Format(_("Game %d/5 (%d moves):"), gm + 1, numMoves) +
			                      wxString(wxT(" }"));
			file.AddLine(gameHeader);
			file.AddLine(blankLine);
			
			// Output a header for the match moves list
			file.AddLine(wxT("{\\par\\pard\\plain\\f1{1   .    10   .    20   .    30   .    40}\\f0}"));
			file.AddLine(blankLine);
					
			// Now, we have a problem here, because we want to keep the vertical
			// alignment between the strings.  So, we should just output 30 matches
			// per line.
			size_t numLines = (size_t)ceil(numMoves / 40.0);
			for (size_t l = 0 ; l < numLines ; l++)
			{
				// Figure out which moves are on this line
				size_t start = l * 40;
				size_t end = l * 40 + 40;
				if (end > numMoves)
					end = numMoves;
				
				// Output the player one moves
				wxString p1line = wxT("{\\par\\pard\\plain\\f1{");
				for (size_t mv = start ; mv < end ; mv++)
					p1line += history[mv][0];
				p1line += wxT("}\\f0}");
				file.AddLine(p1line);

				// Output the player two moves
				wxString p2line = wxT("{\\par\\pard\\plain\\f1{");
				for (size_t mv = start ; mv < end ; mv++)
					p2line += history[mv][1];
				p2line += wxT("}\\f0}");
				file.AddLine(p2line);
				
				// Add a blank line for spacing
				file.AddLine(blankLine);
			}
		}
	}
	
	// Write a standard RTF file footer
	file.AddLine(wxT("}"));
	
	file.Write();
	file.Close();
	
	dataSaved = true;
}

