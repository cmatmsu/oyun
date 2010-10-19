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

#include <wx/wizard.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>

#include "../common/error.h"
#include "../game/player.h"
#include "../tourney/tournament.h"
#include "../tourney/match.h"

#include "logosapp.h"
#include "logoswizard.h"
#include "oneshotpage.h"
#include "matchdialog.h"

IMPLEMENT_CLASS(OneShotPage, LogosWizardPage)

enum
{
	ID_RUN_TOURNAMENT = wxID_HIGHEST + 1,
	ID_SHOW_DETAILS,
	ID_MATCH_LIST
};


// Implement the event table for TournamentWindow
BEGIN_EVENT_TABLE(OneShotPage, LogosWizardPage)
	EVT_WIZARD_PAGE_CHANGING(wxID_ANY, OneShotPage::OnPageChanging)
	EVT_WIZARD_HELP(wxID_ANY, OneShotPage::OnHelp)
	
	EVT_BUTTON(ID_RUN_TOURNAMENT, OneShotPage::OnRunTournament)
	EVT_BUTTON(ID_SHOW_DETAILS, OneShotPage::OnMatchDetails)
	
	EVT_LIST_ITEM_SELECTED(ID_MATCH_LIST, OneShotPage::OnMatchSelected)
	EVT_LIST_ITEM_DESELECTED(ID_MATCH_LIST, OneShotPage::OnMatchSelected)
	
	EVT_NOTIFY(wxEVT_DATA_UPDATE, wxID_ANY, OneShotPage::OnDataUpdate)
	EVT_NOTIFY(wxEVT_ADD_PLAYER, wxID_ANY, OneShotPage::OnAddPlayer)
	EVT_NOTIFY(wxEVT_REMOVE_PLAYER, wxID_ANY, OneShotPage::OnRemovePlayer)
END_EVENT_TABLE()


OneShotPage::OneShotPage(LogosWizard *parent, wxWizardPage *prev, wxWizardPage *next) :
                         LogosWizardPage(_("One-Shot Tournament"),
                                         _("This tournament runs one round of the prisoner's dilemma between a set of players."),
                                         parent, prev, next)
{
	// Make the tournament
	tourney = new Tournament(parent->game);


	// Create the splitter
	splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
	                                wxSP_NOBORDER | wxSP_3DSASH | wxTAB_TRAVERSAL);

	// Create the left and right windows
	rightWindow = new wxPanel(splitter);
	leftWindow = new wxPanel(splitter);


	// Create the controls in the desired tab-order
	runTournament = new wxButton(rightWindow, ID_RUN_TOURNAMENT, _("&Run Tournament"));
	details = new wxButton(rightWindow, ID_SHOW_DETAILS, _("&Show Details..."));
	details->Disable();
	
	playerListLabel = new wxStaticText(leftWindow, wxID_ANY, _("Tournament players:"));
	
	playerList = new wxListCtrl(leftWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize,
	                            wxLC_REPORT | wxLC_HRULES | wxLC_SINGLE_SEL | wxSUNKEN_BORDER);
	playerList->SetMinSize(wxSize(100, 300));
	playerList->InsertColumn(0, _("Player"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
	playerList->InsertColumn(1, _("Score"), wxLIST_FORMAT_RIGHT, 50);

	matchListLabel = new wxStaticText(rightWindow, wxID_ANY, _("Matches in tournament:"));

	matchList = new wxListCtrl(rightWindow, ID_MATCH_LIST, wxDefaultPosition, wxDefaultSize, 
	                           wxLC_REPORT | wxLC_HRULES | wxLC_SINGLE_SEL | wxSUNKEN_BORDER);
	matchList->SetMinSize(wxSize(400, 200));
 	matchList->InsertColumn(0, _("Competitors"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
	matchList->InsertColumn(1, _("Victor"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
	matchList->InsertColumn(2, _("Score 1"), wxLIST_FORMAT_RIGHT, wxLIST_AUTOSIZE);
	matchList->InsertColumn(3, _("Score 2"), wxLIST_FORMAT_RIGHT, wxLIST_AUTOSIZE);

	winner = new wxStaticText(rightWindow, wxID_ANY, wxT(" "), wxDefaultPosition, 
	                          wxDefaultSize, wxST_NO_AUTORESIZE);
	wxFont winnerFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	winnerFont.SetPointSize(16);
	winnerFont.SetWeight(wxFONTWEIGHT_BOLD);
	winner->SetFont(winnerFont);


	// Make a sizer for the left side
	leftSizer = new wxBoxSizer(wxVERTICAL);

	leftSizer->Add(playerListLabel, 0, wxEXPAND | wxALL, 6);
	leftSizer->Add(playerList, 1, wxEXPAND | wxALL, 6);

	leftSizer->Layout();
	leftWindow->SetSizerAndFit(leftSizer);

	// Make a sizer for the right side
	rightSizer = new wxBoxSizer(wxVERTICAL);
	
	buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(runTournament, 0, wxEXPAND | wxALL, 6);
	buttonSizer->Add(details, 0, wxEXPAND | wxALL, 6);
	buttonSizer->AddStretchSpacer();
	
	rightSizer->Add(buttonSizer, 0, wxEXPAND);
	rightSizer->Add(matchListLabel, 0, wxEXPAND | wxALL, 6);
	rightSizer->Add(matchList, 1, wxEXPAND | wxALL, 6);
	
	// We need the wxRESERVE_SPACE_EVEN_IF_HIDDEN flag, but it's
	// a fairly new addition to WX
	int winnerFlags = wxEXPAND | wxALL;
#if wxCHECK_VERSION(2,8,8)
	winnerFlags |= wxRESERVE_SPACE_EVEN_IF_HIDDEN;
#endif
	rightSizer->Add(winner, 0, winnerFlags, 12);

	rightSizer->Layout();
	rightWindow->SetSizerAndFit(rightSizer);


	// Set up the splitter
	splitter->SplitVertically(leftWindow, rightWindow, 200);
	splitter->SetMinimumPaneSize(100);


	// Throw it all in the page sizer
	sizer->Add(splitter, 1, wxEXPAND | wxALL, 6);


	// And hide the winner display, or things go wrong
	winner->Show(false);
}

OneShotPage::~OneShotPage()
{
	delete tourney;
}


int wxCALLBACK OneShotPage::PlayerAlphaSortColumn0(long item1, long item2, long sortData)
{
	OneShotPage *op = (OneShotPage *)sortData;
	Player *one = op->tourney->playerOneList[item1];
	Player *two = op->tourney->playerOneList[item2];

	return (one->GetPlayerName() < two->GetPlayerName() ? -1 :
		(one->GetPlayerName() == two->GetPlayerName() ? 0 : 1));
}

int wxCALLBACK OneShotPage::PlayerNumSortColumn1(long item1, long item2, long sortData)
{
	OneShotPage *op = (OneShotPage *)sortData;
	Player *one = op->tourney->playerOneList[item1];
	Player *two = op->tourney->playerOneList[item2];

	int oneScore = op->tourney->scores[one->GetID()];
	int twoScore = op->tourney->scores[two->GetID()];

	// N.B. this is intentionally backwards, sort descending
	if (oneScore < twoScore) return 1;
	if (oneScore > twoScore) return -1;
	return 0;
}

int wxCALLBACK OneShotPage::MatchAlphaSortColumn0(long item1, long item2, long sortData)
{
	OneShotPage *op = (OneShotPage *)sortData;
	Match *one = op->tourney->GetMatch(item1);
	Match *two = op->tourney->GetMatch(item2);

	return (one->playerOne->GetPlayerName() < two->playerOne->GetPlayerName() ? -1 :
		(one->playerOne->GetPlayerName() == two->playerOne->GetPlayerName() ? 0 : 1));
}

void OneShotPage::OnDataUpdate(wxNotifyEvent & WXUNUSED(event))
{
	// Update all of the UI, we've gotten a generic "data update"
	// message.
	UpdatePlayerList();
	UpdateMatchList();

	// Update the game winner if the tournament has been played
	if (tourney->IsPlayed())
	{
		int max = 0, winnerID = 0;
		wxString winnerName(_("Error"));

		// Get the winner's ID from the map
		Tournament::ScoreMap::iterator iter;
		for (iter = tourney->scores.begin() ; iter != tourney->scores.end() ; ++iter)
		{
			if (iter->second > max)
			{
				winnerID = iter->first;
				max = iter->second;
			}
		}
		
		// Get the winner's name from the player list
		for (size_t i = 0 ; i < tourney->playerOneList.GetCount() ; i++)
		{
			if (tourney->playerOneList[i]->GetID() == winnerID)
				winnerName = tourney->playerOneList[i]->GetPlayerName();
		}

		// Show the big winner banner
		winnerName.Prepend(_("Winner: "));
		winner->SetLabel(winnerName);
		
		rightSizer->Show(winner, true);
		rightSizer->Layout();
	}
	
	// Update the control states if we're visible
	if (IsShownOnScreen())
	{
		if (tourney->IsPlayed())
		{
			// Default button should be the 'Next' button after we've played
			wxWindow *win = wxWindow::FindWindowById(wxID_FORWARD, GetParent());
			wxButton *nextButton = wxDynamicCast(win, wxButton);
			
			if (nextButton)
			{
				nextButton->SetDefault();
				nextButton->SetFocus();
			}
			
			if (matchList->GetSelectedItemCount())
				details->Enable();
			else
				details->Disable();
		}
		else
		{
			// Default button should be the 'Run' button before we've played
			runTournament->SetDefault();
			runTournament->SetFocus();
			
			details->Disable();
		}
	}
}


void OneShotPage::CalcColumnWidth(wxListCtrl *list, int column)
{
	list->SetColumnWidth(column, wxLIST_AUTOSIZE);
	
	int size = list->GetColumnWidth(column) + 18;
	if (size < 60)
		size = 60;
	
	list->SetColumnWidth(column, size);
}


void OneShotPage::UpdatePlayerList()
{
	// Clear the list
	playerList->ClearAll();

	// Reset the columns
	playerList->InsertColumn(0, _("Player"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
	playerList->InsertColumn(1, _("Score"), wxLIST_FORMAT_RIGHT, 50);

	// See if we have any work to do
	if (!tourney->playerOneList.size())
		return;

	// Loop through and add the players to the list
	// This is weird, because the players are added two-at-a-time
	for (size_t i = 0 ; i < tourney->playerOneList.GetCount() ; i++)
	{
		// Add to the list
		Player *player = tourney->playerOneList[i];
		wxListItem item;

		item.m_itemId = i;
		item.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_DATA;
		item.m_text = player->GetPlayerName();
		item.m_col = 0;
		item.m_data = i;
		playerList->InsertItem(item);

		// See if we've played a match
		item.m_col = 1;
		if (tourney->IsPlayed())
			item.m_text.Printf(wxT("%d"), tourney->scores[player->GetID()]);
		else
			item.m_text = wxT("");
		playerList->SetItem(item);
	}


	// Update the width of the columns 
	CalcColumnWidth(playerList, 0);
	CalcColumnWidth(playerList, 1);

	// Sort--if we've played, by col. 1 numerically descending, else by col. 2
	if (tourney->IsPlayed())
		playerList->SortItems((wxListCtrlCompare)PlayerNumSortColumn1, (long)this);
	else
		playerList->SortItems((wxListCtrlCompare)PlayerAlphaSortColumn0, (long)this);
}

void OneShotPage::UpdateMatchList()
{
	// Clear the report control
	matchList->ClearAll();

	// Reset the columns
	matchList->InsertColumn(0, _("Competitors"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
	matchList->InsertColumn(1, _("Victor"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
	matchList->InsertColumn(2, _("Score 1"), wxLIST_FORMAT_RIGHT, wxLIST_AUTOSIZE);
	matchList->InsertColumn(3, _("Score 2"), wxLIST_FORMAT_RIGHT, wxLIST_AUTOSIZE);
	
	// See if we have nothing to do
	if (!tourney->GetNumMatches())
		return;

	// Run through the match list and add them all
	int i;
	for (i = 0 ; i < tourney->GetNumMatches() ; i++)
	{
		wxListItem item;
		Match *match = tourney->GetMatch(i);

		item.m_itemId = 0;
		item.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_DATA;
		item.m_text.Printf(_("%s vs %s"), match->playerOne->GetPlayerName().c_str(),
			match->playerTwo->GetPlayerName().c_str());
		item.m_col = 0;
		item.m_data = i;
		matchList->InsertItem(item);

		// Add results if we've played the tournament
		if (tourney->IsPlayed())
		{
			if (match->playerOneScore > match->playerTwoScore)
				item.m_text = _("Player One");
			else if (match->playerTwoScore > match->playerOneScore)
				item.m_text = _("Player Two");
			else
				item.m_text = _("Tie");
			item.m_col = 1;
			matchList->SetItem(item);

			item.m_text.Printf(wxT("%d"), match->playerOneScore);
			item.m_col = 2;
			matchList->SetItem(item);

			item.m_text.Printf(wxT("%d"), match->playerTwoScore);
			item.m_col = 3;
			matchList->SetItem(item);
		}
		else
		{
			// Add blank results
			item.m_text = wxT("");
			item.m_col = 1;
			matchList->SetItem(item);
			item.m_col = 2;
			matchList->SetItem(item);
			item.m_col = 3;
			matchList->SetItem(item);
		}
	}

	// Update the width of the columns
	CalcColumnWidth(matchList, 0);
	CalcColumnWidth(matchList, 1);
	CalcColumnWidth(matchList, 2);
	CalcColumnWidth(matchList, 3);

	// Sort
	matchList->SortItems((wxListCtrlCompare)MatchAlphaSortColumn0, (long)this);
}



void OneShotPage::OnPageChanging(wxWizardEvent &event)
{
	// We only care if we're going forward
	if (!event.GetDirection())
	{
		event.Skip();
		return;
	}
	
	// Don't allow them to go forward if we haven't run the tournament
	if (!tourney->IsPlayed())
	{
		wxMessageBox(_("You must run the tournament to continue."),
		             _("Logos: Error"), wxOK | wxICON_ERROR, this);
		event.Veto();
		return;
        }
	
	// We don't need to hold this event up
	event.Skip();
}

void OneShotPage::OnHelp(wxWizardEvent & WXUNUSED(event))
{
	wxGetApp().ShowHelp(this, wxT("oneshotpage.html"));
}

void OneShotPage::OnRunTournament(wxCommandEvent & WXUNUSED(event))
{
	wxBusyCursor busy;
	
	// Run the tournament
	if (!tourney->Run())
	{
		wxString errStr(wxString::Format(_("Could not run tournament.  Error reported:\n\n%s"), Error::Get().c_str()));
		wxMessageBox(errStr, _("Logos: Error"), wxOK | wxICON_ERROR, this);
		return;
	}
	
	// This qualifies as a data update
	parent->Update();
}

void OneShotPage::OnAddPlayer(wxNotifyEvent &event)
{
	// Send this to the tournament
	tourney->AddPlayer((Player *)event.GetClientData());
}

void OneShotPage::OnRemovePlayer(wxNotifyEvent &event)
{
	// Send this to the tournament
	tourney->RemovePlayer((Player *)event.GetClientData());
}


void OneShotPage::OnMatchSelected(wxListEvent & WXUNUSED(event))
{
	int selectedItemCount = matchList->GetSelectedItemCount();
	
	if (selectedItemCount && tourney->IsPlayed())
		details->Enable();
	else
		details->Disable();
}

void OneShotPage::OnMatchDetails(wxCommandEvent & WXUNUSED(event))
{
	int itemIndex;
	itemIndex = matchList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	
	if (itemIndex == -1)
		return;
	
	long matchIndex = matchList->GetItemData(itemIndex);	
	if (matchIndex < 0 || matchIndex > tourney->GetNumMatches())
		return;
	
	Match *match = tourney->GetMatch(matchIndex);
	
	MatchDialog dialog(this, match);
	dialog.ShowModal();
}

