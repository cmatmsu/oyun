/*
    Copyright (C) 2004-2011 by Charles Pence
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

#ifndef ONESHOTPAGE_H__
#define ONESHOTPAGE_H__

#include "tools/logoswizardpage.h"

class Tournament;
class Match;

#include <wx/wizard.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>


/**
    \class OneShotPage
    \ingroup ui
    
    \brief Wizard page on which a round-robin tournament is run
    
    This page runs and presents the results of a round-robin tournament
    between a set of players.
*/
class OneShotPage : public LogosWizardPage
{
	DECLARE_CLASS(OneShotPage)
	
public:
	/**
	    \brief Constructor
	    
	    Constructs the wizard page and all its controls.
	    
	    \param parent The parent of this wizard page
	    \param prev The previous page in the wizard
	    \param next The next page in the wizard
	*/
	OneShotPage(LogosWizard *parent, wxWizardPage *prev = NULL, wxWizardPage *next = NULL);
	
	virtual ~OneShotPage();

private:
	DECLARE_EVENT_TABLE()
	
	friend class OneShotFinishPage;
	
	/**
	    \brief The tournament to be run
	*/
	Tournament *tourney;
	
	
	/**
	    \brief Update the player list control
	    
	    This is a non-destructive UI update, called when we receive
	    a \c wxEVT_DATA_UPDATE message.
	*/
	void UpdatePlayerList();
	
	/**
	    \brief Update the match list control
	    
	    This is a non-destructive UI update, called when we receive
	    a \c wxEVT_DATA_UPDATE message.
	*/
	void UpdateMatchList();
	
	/**
	    \brief Utility function to calculate column width
	    
	    The \c wxLIST_AUTOSIZE constant sizes the columns of the list
	    views a little too small.  This function sets the size of a
	    column initially using \c wxLIST_AUTOSIZE, then adds some padding.
	    
	    \param ctrl The list control to work with
	    \param column The column to resize
	*/
	void CalcColumnWidth(wxListCtrl *ctrl, int column);
	
	
	wxSplitterWindow *splitter;	/**< \brief The splitter between the player list and the match list */
	wxPanel *leftWindow;		/**< \brief The panel on the left containing the player list */
	wxPanel *rightWindow;		/**< \brief The panel on the right containing the match list */
	
	wxSizer *leftSizer;		/**< \brief The sizer for the \c leftWindow */
	wxSizer *rightSizer;		/**< \brief The sizer for the \c rightWindow */

	wxSizer *buttonSizer;		/**< \brief The sizer which contains the buttons */	
	wxButton *runTournament;	/**< \brief The run tournament button */
	wxButton *details;		/**< \brief The show details button */

	wxStaticText *playerListLabel;	/**< \brief The label for the player list */
	wxListCtrl *playerList;		/**< \brief The player list on the left side of the page */
	
	wxStaticText *matchListLabel;	/**< \brief The label for the match list */
	wxListCtrl *matchList;		/**< \brief The match list on the right side of the page */
	wxStaticText *winner;		/**< \brief The text display of the tournament winner */
	
	
	/**
	    \brief Called when the wizard page is changing
	    \param event The event generated
	*/
	void OnPageChanging(wxWizardEvent &event);
	
	/**
	    \brief Show this wizard page's help documentation
	    \param event The event generated
	*/
	void OnHelp(wxWizardEvent &event);
	
	/**
	    \brief Called when the "Run Tournament" button is pressed
	    \param event The event generated
	*/
	void OnRunTournament(wxCommandEvent &event);
	
	/**
	    \brief Called when a match-list item is clicked
	    \param event The event generated
	*/
	void OnMatchSelected(wxListEvent &event);
	
	/**
	    \brief Called when the "Match Details" button is pressed
	    \param event The event generated
	*/
	void OnMatchDetails(wxCommandEvent &event);
	
	
	/**
	    \brief Called when the page receives a 'data update' message
	    
	    This function updates all of the UI controls when a player is
	    added, removed, or the tournament is run.
	    
	    \param event The event generated
	*/
	void OnDataUpdate(wxNotifyEvent &event);
	
	/**
	    \brief Add a player to the tournament
	    \param event The event generated
	*/
	void OnAddPlayer(wxNotifyEvent &event);
	
	/**
	    \brief Remove a player from the tournament
	    \param event The event generated
	*/
	void OnRemovePlayer(wxNotifyEvent &event);
	
	
	/**
	    \brief Sort the \c playerList by the first column, alphabetically
	    
	    \param item1 Index of the first item to be compared
	    \param item2 Index of the second item to be compared
	    \param sortData Pointer to the \c OneShotPage class
	    \returns Results of the comparison, similar to \c strcmp()
	*/
	static int wxCALLBACK PlayerAlphaSortColumn0(long item1, long item2, long sortData);
	
	/**
	    \brief Sort the \c playerList by the second column, numerically

	    \param item1 Index of the first item to be compared
	    \param item2 Index of the second item to be compared
	    \param sortData Pointer to the \c OneShotPage class
	    \returns Results of the comparison, similar to \c strcmp()
	*/
	static int wxCALLBACK PlayerNumSortColumn1(long item1, long item2, long sortData);
	
	/**
	    \brief Sort the \c matchList by the first column, alphabetically

	    \param item1 Index of the first item to be compared
	    \param item2 Index of the second item to be compared
	    \param sortData Pointer to the \c OneShotPage class
	    \returns Results of the comparison, similar to \c strcmp()
	*/
	static int wxCALLBACK MatchAlphaSortColumn0(long item1, long item2, long sortData);
};


#endif

// Local Variables:
// mode: c++
// End:
