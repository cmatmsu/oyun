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

#ifndef PLAYERSPAGE_H__
#define PLAYERSPAGE_H__

#include <wx/wizard.h>
#include <wx/listbox.h>
#include "tools/logoswizardpage.h"
#include "../game/player.h"


/**
    \class PlayersPage
    \ingroup ui
    
    \brief Wizard page on which user selects a list of players
    
    This wizard page includes a large list box showing the current
    players loaded for the tournament, as well as buttons allowing
    the user to add and remove different types of players.  Right-click
    on a player shows a pop-up menu.
*/
class PlayersPage : public LogosWizardPage
{
	DECLARE_CLASS(PlayersPage)
	
public:
	/**
	    \brief Constructor
	    
	    Constructs the wizard page and all its controls.
	    
	    \param parent The parent of this wizard page
	    \param next The next page in the wizard
	*/
	PlayersPage(LogosWizard *parent, wxWizardPage *next = NULL);

	virtual ~PlayersPage();
	
	/**
	    \brief Add a dropped list of FSA players
	    
	    Called from the drag-and-drop code, adds a list of players
	    to the tournament.
	    
	    \param files The list of filenames
	    \returns True if the add was entirely successful, false if not
	*/
	bool OnFileDrop(const wxArrayString &files);
	
private:
	DECLARE_EVENT_TABLE()
	
	/**
	    \brief Show this wizard page's help documentation
	    \param event The event generated
	*/
	void OnHelp(wxWizardEvent &event);
	
	/**
	    \brief Called when the "Add Built-In Player" button is pressed
	    \param event The event generated
	*/
	void OnAddBuiltinButton(wxCommandEvent &event);
	
	/**
	    \brief Called when the "Add Finite State Machine" button is pressed
	    \param event The event generated
	*/
	void OnAddFileButton(wxCommandEvent &event);
	
	/**
	    \brief Called when the "Remove Player" button is pressed
	    \param event The event generated
	*/
	void OnRemoveButton(wxCommandEvent &event);
	
	
	/**
	    \brief Called when "Remove Player" is clicked in a pop-up menu
	    \param event The event generated
	*/
	void OnRemoveMenu(wxCommandEvent &event);
	
	/**
	    \brief Called when "View Player Source" is clicked in a pop-up menu
	    \param event The event generated
	*/
	void OnViewMenu(wxCommandEvent &event);
	
	/**
	    \brief Called when the user wants a context menu for the player list
	    
	    This function displays our pop-up menu.
	    
	    \param event The event generated
	*/
	void OnContextMenu(wxContextMenuEvent &event);
	
	
	/**
	    \brief Called when the wizard moves to the next page
	    
	    This function makes sure that at least one player has been added
	    to the tournament before allowing the user to advance to the next
	    page.
	    
	    \param event The event generated
	*/
	void OnNext(wxWizardEvent &event);
	
	/**
	    \brief Called when the page receives a 'data update' message
	    
	    This function updates the view after the data has been updated.
	    Notably, when a player is added from this page, we add the player,
	    broadcast the \c wxEVT_ADD_PLAYER message, and then call
	    \c LogosWizard::Update().  Then, the update event invokes this
	    event handler, which triggers a UI update.
	    
	    \param event The event generated
	*/
	void OnDataUpdate(wxNotifyEvent &event);
	
	
	/**
	    \brief Add a player to the internal list and fire an add-player
	           event
	    
	    This function encapsulates the portion of the player-adding logic
	    shared between the two add-player buttons -- adding the player to
	    the internal \c players array, and sending the
	    \c wxEVT_ADD_PLAYER event.
	    
	    \param player The player to be added
	*/
	void AddPlayer (Player *player);
	
	/**
	    \brief Remove a player from the internal list and fire a remove-player
	           event
	    
	    This function encapsulates the portion of the player-removing
	    logic shared between the remove player button and the right-click
	    menu -- removing the player from our internal \c players array, and
	    sending the \c wxEVT_REMOVE_PLAYER event.
	    
	    \param playerIndex The internal array index of the player to be removed
	*/
	void RemovePlayer (size_t playerIndex);
	
	/**
	    \brief The internal list of players maintained by this page
	*/
	PlayerPtrArray players;
	
	
	/**
	    \brief The introductory message text
	*/
	wxStaticText *text;
	
	
	/**
	    \brief A horizontal sizer containing our UI buttons
	*/
	wxBoxSizer *buttonSizer;
	
	/**
	    \brief The "Add Built-In Player" button
	*/
	wxButton *builtin;
	
	/**
	    \brief The "Add Finite State Machine" button
	*/
	wxButton *file;
	
	/**
	    \brief The "Remove Player" button
	*/
	wxButton *remove;
	
	
	/**
	    \brief The list box which displays the players
	*/
	wxListBox *list;
	
	
	/**
	    \brief The player right-clicked while a pop-up menu is active
	    
	    This member is set to the player that has been right-clicked 
	    when a pop-up menu is opened.  It is used by the various
	    pop-up menu event handlers.
	*/
	Player *menuPlayer;
	
	/**
	    \brief The index of the player right-clicked while a pop-up menu
	           is active
	    
	    This member is set to the array index of the player that has
	    been right-clicked when a pop-up menu is opened.  It is used by
	    the various pop-up menu event handlers.
	*/
	int menuPlayerIndex;
};

#endif

