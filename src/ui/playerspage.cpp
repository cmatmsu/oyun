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

#include "../common/error.h"
#include "../game/fsaplayer.h"
#include "../game/random.h"
#include "../game/titfortat.h"

#include "tools/htmldialog.h"
#include "logosapp.h"
#include "logoswizard.h"
#include "playerspage.h"


IMPLEMENT_CLASS(PlayersPage, LogosWizardPage)

enum
{
	ID_BUTTON_BUILTIN = wxID_HIGHEST + 1,
	ID_BUTTON_FILE,
	ID_BUTTON_REMOVE,

	ID_MENU_REMOVE,
	ID_MENU_VIEW
};


BEGIN_EVENT_TABLE(PlayersPage, LogosWizardPage)
	EVT_WIZARD_PAGE_CHANGING(wxID_ANY, PlayersPage::OnNext)
	EVT_WIZARD_HELP(wxID_ANY, PlayersPage::OnHelp)
	EVT_CONTEXT_MENU(PlayersPage::OnContextMenu)
	
	EVT_BUTTON(ID_BUTTON_BUILTIN, PlayersPage::OnAddBuiltinButton)
	EVT_BUTTON(ID_BUTTON_FILE, PlayersPage::OnAddFileButton)
	EVT_BUTTON(ID_BUTTON_REMOVE, PlayersPage::OnRemoveButton)
	
	EVT_MENU(ID_MENU_REMOVE, PlayersPage::OnRemoveMenu)
	EVT_MENU(ID_MENU_VIEW, PlayersPage::OnViewMenu)
	
	EVT_NOTIFY(wxEVT_DATA_UPDATE, wxID_ANY, PlayersPage::OnDataUpdate)
END_EVENT_TABLE()



PlayersPage::PlayersPage(LogosWizard *parent, wxWizardPage *next) : 
                         LogosWizardPage(_("Select Players"),
                                         _("You can choose which players will be used in this tournament."),
                                         parent, NULL, next)
{
	wxString introText(_("Welcome to Logos!  The following wizard will allow you "
	                     "to run an iterated prisoner's dilemma tournament.\n\n"
	                     "To begin, please construct a list of players."));
	
	// Make all of our controls
	text = new wxStaticText(this, wxID_ANY, introText);
	text->Wrap(GetClientSize().GetWidth() - 24);
	
	builtin = new wxButton(this, ID_BUTTON_BUILTIN, _("Add &Built-In Player..."));
	file = new wxButton(this, ID_BUTTON_FILE, _("Add &Finite State Machine..."));
	remove = new wxButton(this, ID_BUTTON_REMOVE, _("&Remove Player"));
	
	list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL,
	                     wxLB_SINGLE | wxLB_ALWAYS_SB | wxLB_SORT | wxSUNKEN_BORDER);
	
	
	// Add the controls to the sizer	
	sizer->Add(text, 0, wxEXPAND | wxALL, 12);

	buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(builtin, 1, wxEXPAND | wxRIGHT, 6);
	buttonSizer->Add(file, 1, wxEXPAND | wxLEFT | wxRIGHT, 6);
	buttonSizer->Add(remove, 1, wxEXPAND | wxLEFT, 6);
	sizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 12);
	
	sizer->Add(list, 1, wxEXPAND | wxLEFT | wxRIGHT, 12);
}

PlayersPage::~PlayersPage()
{
	for (size_t i = 0 ; i < players.size() ; i++)
		delete players[i];
	players.Clear();
}


void PlayersPage::OnHelp(wxWizardEvent & WXUNUSED(event))
{
	wxGetApp().ShowHelp(this, wxT("Player-Selection-Page.html"));
}

void PlayersPage::OnDataUpdate(wxNotifyEvent & WXUNUSED(event))
{
	// Update the list of players
	list->Clear();
	for (size_t i = 0 ; i < players.size() ; i++)
	{
		Player *player = players[i];

		wxString displayString;
		displayString = player->GetPlayerName();
		displayString += wxT(" (");
		displayString += player->GetPlayerAuthor();
		displayString += wxT(")");

		list->Append(displayString, (void *)i);
	}
	
	// Update the control status if we're on screen
	if (IsShownOnScreen())
	{
		if (players.GetCount() == 0)
		{
			// Default button should be the built-in button if no players
			// have been added
			builtin->SetDefault();
			builtin->SetFocus();
		}
		else
		{
			// Otherwise, it should be the Next button
			wxWindow *win = wxWindow::FindWindowById(wxID_FORWARD, GetParent());
			wxButton *nextButton = wxDynamicCast(win, wxButton);
			
			if (nextButton)
			{
				nextButton->SetDefault();
				nextButton->SetFocus();
			}
		}
	}
}

void PlayersPage::AddPlayer(Player *player)
{
	// Add the player
	players.push_back(player);
	parent->AddPlayer(player);
	parent->Update();
}

void PlayersPage::RemovePlayer(size_t playerIndex)
{
	if (playerIndex > players.size())
		return;
	
	parent->RemovePlayer(players[playerIndex]);
	parent->Update();

	delete players[playerIndex];
	players.RemoveAt(playerIndex);
}



void PlayersPage::OnAddBuiltinButton(wxCommandEvent & WXUNUSED(event))
{
	wxString choices[] = {wxString(_("Random player")), wxString(_("Tit-for-tat player"))};

	wxSingleChoiceDialog *dialog = new wxSingleChoiceDialog(this, 
		_("Which built-in player do you wish to add?"),
		_("Add Built-In Players"), 2, choices);
	if (dialog->ShowModal() == wxID_CANCEL)
		return;

	int sel = dialog->GetSelection();
	dialog->Destroy();

	// Make sure the selection is legitimate
	if (sel < 0 || sel >= 2)
		return;

	// Add whatever player we need to
	if (sel == 0)
		AddPlayer(new RandomPlayer);
	else
		AddPlayer(new TitForTatPlayer);
}

void PlayersPage::OnAddFileButton(wxCommandEvent & WXUNUSED(event))
{
	wxString wildCardFilter(_("Text files (*.txt)|*.txt"));
	wxFileDialog *fileDialog;

	fileDialog = new wxFileDialog(this, _("Select finite script automata..."), 
								  wxT(""), wxT(""), wildCardFilter, 
								  wxFD_OPEN|wxFD_MULTIPLE|wxFD_FILE_MUST_EXIST);
	
	if (fileDialog->ShowModal() == wxID_OK)
	{
		wxArrayString filenames;

		fileDialog->GetPaths(filenames);
		size_t numFilenames = filenames.GetCount();

		if (numFilenames)
		{
			for (size_t i = 0 ; i < numFilenames ; i++)
			{
				FSAPlayer *player = new FSAPlayer;

				if (player->Load(parent->game, filenames[i]))
					AddPlayer(player);
				else
				{
					wxString errStr(wxString::Format(_("Could not load player %s.  Error reported:\n%s"), 
					                filenames[i].c_str(), Error::Get().c_str()));
					wxMessageBox(errStr, _("Logos: Error"), wxOK | wxICON_ERROR, this);

					delete player;
				}
			}
		}
	}

	fileDialog->Destroy();
}

bool PlayersPage::OnFileDrop(const wxArrayString &files)
{
	bool ret = true;
	
	for (size_t i = 0 ; i < files.GetCount() ; i++)
	{
		FSAPlayer *player = new FSAPlayer;
		
		if (player->Load(parent->game, files[i]))
			AddPlayer(player);
		else
		{
			wxString errStr(wxString::Format(_("Could not load player %s.  Error reported:\n%s"), 
			                files[i].c_str(), Error::Get().c_str()));
			wxMessageBox(errStr, _("Logos: Error"), wxOK | wxICON_ERROR, this);
			
			delete player;
			ret = false;
		}
	}
	
	return ret;
}

void PlayersPage::OnRemoveButton(wxCommandEvent & WXUNUSED(event))
{
	int selectedIndex = list->GetSelection();
	if (selectedIndex == wxNOT_FOUND)
		return;
	
	void *clientData = list->GetClientData(selectedIndex);
	intptr_t playerArrayIndex = (intptr_t)clientData;
	
	RemovePlayer (playerArrayIndex);
}

void PlayersPage::OnNext(wxWizardEvent &event)
{
	// If they haven't added any players, veto.
	if (!players.GetCount())
	{
		wxMessageBox(_("You must add at least one player to continue."), 
		             _("Logos: Error"), wxOK | wxICON_ERROR, this);
		event.Veto();
		return;
	}
	
	// We don't need to hold this event up
	event.Skip();
}

void PlayersPage::OnContextMenu(wxContextMenuEvent &event)
{
	wxPoint point = event.GetPosition();
	wxPoint menuPos;
	int item;
	
	// If this message was generated by the mouse, then hit-test,
	// select, and pop up
	if (point != wxDefaultPosition)
	{
		// Get the coordinate where we'll open the menu
		menuPos = ScreenToClient(point);
		
		// Translate to the list's client coordinates
		list->ScreenToClient(&point.x, &point.y);
		
		item = list->HitTest(point);
		if (item == wxNOT_FOUND)
			return;
		
		// Select the menu item
		list->SetSelection(item);
	}
	else
	{
		// Otherwise, then we only pop up a menu if an item
		// is already selected
		item = list->GetSelection();
		if (item == wxNOT_FOUND)
			return;
		
		// Find a location for the pop-up menu.  Following the behavior of
		// apps like Firefox, pop it up at the 0,0 position of the list
		// box.  (Translate 0,0 in list-client position to screen coords,
		// then convert that to wizard-page coords for PopupMenu.)
		menuPos.x = menuPos.y = 0;
		menuPos = list->ClientToScreen(menuPos);
		menuPos = ScreenToClient(menuPos);
	}
	
	// Abort if this is a bad player
	void *clientData = list->GetClientData(item);
	size_t playerArrayIndex = (size_t)clientData;
	Player *player = players[playerArrayIndex];
	
	if (playerArrayIndex > players.size() || !player)
		return;
	
	// Make the player
	wxMenu *menu = new wxMenu;
	menu->Append(ID_MENU_REMOVE, _("&Remove Player"));
	menu->Append(ID_MENU_VIEW, _("&View Player Source..."));

	// See if we need to disable the "view" button
	if (player->GetPlayerAuthor() == wxString(_("Built-in")))
		menu->Enable(ID_MENU_VIEW, false);
	
	// Set the menu identifier values
	menuPlayer = player;
	menuPlayerIndex = playerArrayIndex;
	
	// Show the menu
	PopupMenu(menu, menuPos);

	delete menu;
}

void PlayersPage::OnRemoveMenu(wxCommandEvent & WXUNUSED(event))
{
	RemovePlayer(menuPlayerIndex);
}

void PlayersPage::OnViewMenu(wxCommandEvent & WXUNUSED(event))
{
	FSAPlayer *player = dynamic_cast<FSAPlayer *>(menuPlayer);
	if (player == NULL)
		return;

	// Make the HTML to display
	wxString html = wxT("<html><body><pre><font size=-1>") + player->GetSource() + 
	                wxT("</font></pre></body></html>");

	// Show the dialog box
	HtmlDialog *dialog = new HtmlDialog(this, -1, _("Player Source"), wxDefaultPosition, wxSize(450, 450));
	dialog->SetHtml(html);
	dialog->ShowModal();
	dialog->Destroy();
}

