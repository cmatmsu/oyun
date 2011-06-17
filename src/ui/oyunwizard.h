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

#ifndef OYUNWIZARD_H__
#define OYUNWIZARD_H__

#include <wx/wizard.h>
#include <wx/dnd.h>

class Game;
class Player;

class PlayersPage;
class TypePage;
class OneShotPage;
class OneShotFinishPage;
class EvoPage;
class EvoFinishPage;


/**
    \class OyunWizard
    \ingroup ui
    
    \brief The Oyun wizard class
    
    This wizard creates the various wizard pages and adds them to the
    main window.  For the moment, the wizard has the \c Game object as
    well.
*/
class OyunWizard : public wxWizard
{
	DECLARE_DYNAMIC_CLASS(OyunWizard);
	
public:
	/**
	    \brief Default constructor
	    
	    The default constructor is to be used for two-step creation
	    with the Create() function.
	*/
	OyunWizard();
	
	/**
	    \brief Constructor
	    
	    \param parent The parent window, may be \c NULL
	*/
	OyunWizard(wxWindow *parent);
	
	virtual ~OyunWizard();
	
	/**
	    \brief Create the dialog
	    
	    Only call this function if the wizard was constructed with its default
	    constructor.
	    
	    \param parent The parent window, may be \c NULL

	    \returns True if successfully created, false otherwise
	*/
	bool Create(wxWindow *parent);
	
	
	/**
	    \brief Send a data-update message to all pages
	    
	    This function constructs a \c wxEVT_DATA_UPDATE event and sends it 
	    to all of the wizard pages.
	*/
	void Update();
	
	/**
	    \brief Send a new-player message to all pages
	    
	    This function constructs a \c wxEVT_ADD_PLAYER event and sends it to
	    all of the wizard pages.  As soon as the function returns, the player
	    passed to this function can be deleted.
	    
	    \param player Player of the type to be added to all pages
	*/
	void AddPlayer(Player *player);

	/**
	    \brief Send a remove-player message to all pages
	    
	    This function constructs a \c wxEVT_REMOVE_PLAYER event and sends it
	    to all of the wizard pages.  As soon as the function returns, the player
	    passed to this function can be deleted.  The pages will delete any
	    players which are of the same type, name, and author as the \p player
	    parameter.
	    
	    \param player Player like that to be removed
	*/
	void RemovePlayer(Player *player);


	
	/**
	    \brief Handle drag-and-dropped files
	    
	    This function will add an FSA player to the player list, if the players
	    page is currently visible.
	    
	    \param files Array of filenames dropped
	    \returns True if the drop was successful, false otherwise
	*/
	bool OnFileDrop(const wxArrayString &files);

	
	/**
	    \brief The game being played
	*/
	Game *game;

	
private:
	DECLARE_EVENT_TABLE()
	
	/**
	    \brief Called when the wizard finishes
	    \param event The event generated
	*/
	void OnFinished(wxWizardEvent &event);
	
	/**
	    \brief The first page: select players
	*/
	PlayersPage *pagePlayers;
	
	/**
	    \brief The page on which the tournament type is chosen
	*/
	TypePage *pageType;
	
	/**
	    \brief The page where a \c Tournament class is run
	*/
	OneShotPage *pageOneShot;
	
	/**
	    \brief The page where data from a \c Tournament class is saved
	*/
	OneShotFinishPage *pageOneShotFinish;
	
	/**
	    \brief The page where an \c EvoTournament class is run
	*/
	EvoPage *pageEvo;
	
	/**
	    \brief The page where data from an \c EvoTournament is saved
	*/
	EvoFinishPage *pageEvoFinish;
};


BEGIN_DECLARE_EVENT_TYPES()
	/**
	    \var wxEVT_DATA_UPDATE
	    \ingroup ui

	    \brief Event fired to signal generic "data" update
	    
	    This event is intended to trigger a general UI update by all
	    wizard pages.  Which data in particular has been updated is 
	    unspecified.
	    
	    The event itself is a \c wxNotifyEvent, and can be captured
	    by a class with:
	    \code
	    EVT_NOTIFY(wxEVT_DATA_UPDATE, wxID_ANY, Handler)
	    \endcode
	    where Handler has the prototype:
	    \code
	    void Handler(wxNotifyEvent &event);
	    \endcode
	    
	    \see OyunWizard::Update
	*/
	DECLARE_EVENT_TYPE(wxEVT_DATA_UPDATE, -1)
	
	/**
	    \var wxEVT_ADD_PLAYER
	    \ingroup ui
	    
	    \brief Event fired when a player is added to the list
	    
	    The event itself is a \c wxNotifyEvent, and can be captured
	    by a class with:
	    \code
	    EVT_NOTIFY(wxEVT_ADD_PLAYER, wxID_ANY, Handler)
	    \endcode
	    where Handler has the prototype:
	    \code
	    void Handler(wxNotifyEvent &event);
	    \endcode
	    The player pointer to be added may be recovered with:
	    \code
	    Player *player = (Player *)event.GetClientData();
	    \endcode
	    
	    \see OyunWizard::AddPlayer
	*/
	DECLARE_EVENT_TYPE(wxEVT_ADD_PLAYER, -1)
	
	/**
	    \var wxEVT_REMOVE_PLAYER
	    \ingroup ui
	    
	    \brief Event fired when a player is removed from the list
	    
	    The event itself is a \c wxNotifyEvent, and can be captured
	    by a class with:
	    \code
	    EVT_NOTIFY(wxEVT_REMOVE_PLAYER, wxID_ANY, Handler)
	    \endcode
	    where Handler has the prototype:
	    \code
	    void Handler(wxNotifyEvent &event);
	    \endcode
	    The player pointer to be added may be recovered with:
	    \code
	    Player *player = (Player *)event.GetClientData();
	    \endcode
	    
	    \see OyunWizard::RemovePlayer
	*/
	DECLARE_EVENT_TYPE(wxEVT_REMOVE_PLAYER, -1)
END_DECLARE_EVENT_TYPES()


/**
    \class OyunWizardDropTarget
    \ingroup ui
    
    \brief This drop target just passes dropped files to the \c OyunWizard.
*/
class OyunWizardDropTarget : public wxFileDropTarget
{
	// N.B.: This class should *not* have a DECLARE_DYNAMIC_CLASS, as the
	// wxFileDropTarget (and wxDropTarget) are not derived from wxObject.
	
public:
	/**
	    \brief Constructor
	    \param wizard The wizard to send file drops to
	*/
	OyunWizardDropTarget(OyunWizard *wizard) : wxFileDropTarget(), parent(wizard) { }
	virtual ~OyunWizardDropTarget() { }
	
	/**
	    \brief Pass dropped files to \c OyunWizard
	    \param x The x-coordinate of the mouse
	    \param y The y-coordinate of the mouse
	    \param filenames An array of filenames
	    \returns True if the drop was successful, false otherwise
	*/
	virtual bool OnDropFiles(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y), const wxArrayString &filenames)
	{ return parent->OnFileDrop(filenames);  }

private:
	/**
	    \brief The wizard to send file drop messages to
	*/
	OyunWizard *parent;
};



#endif

// Local Variables:
// mode: c++
// End:
