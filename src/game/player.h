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

#ifndef PLAYER_H__
#define PLAYER_H__

class Game;

/**
    \class Player
    \ingroup game

    \brief Base class for all game players

    This is the basic encapsulation of a game playing entity--it is capable
    of coming up with a move, having that move acted upon by \c Game::Play,
    and then dealing with the results of that interaction.  Anything useful is
    done by derived classes of this base.
*/
class Player
{
public:
	/**
	    \brief Constructor
	    
	    This constructor generates an ID for this player, and resets its
	    score.
	*/
	Player() :
	    nextMove(wxT('\0')),
	    id(Player::GenerateID()),
	    score(0)
	{ }
	
	/**
	    \brief Copy constructor
	    
	    Initialize a player as an exact copy of a previous player, including
	    the unique ID.
	    
	    \param p Player to be copied
	*/
	Player(const Player &p) :
	    nextMove(p.nextMove),
	    id(p.id),
	    score(p.score),
	    opponentMoves(p.opponentMoves)
	{ }
	
	virtual ~Player() {}
	
	/**
	    \brief Generate an unique player ID
	    
	    To permit adding multiple players of the same type to tournaments,
	    players need to be referred to by unique identification numbers
	    in tournament code.  This function generates a unique player ID by
	    incrementing a static counter.
	    
	    \returns A unique player identifier
	*/
	static int GenerateID()
	{
		static int idCounter = 111;
		return (idCounter++);
	}
	
	/**
	    \brief Create a newly allocated copy of the current player
	    
	    To properly implement an "add player" event (<tt>wxEVT_ADD_PLAYER</tt>),
	    we need to be able to create an arbitrary copy of a given player,
	    without knowing its type at runtime.  This method returns an
	    exact duplicate of the current object, and is simply implemented
	    as <tt>return new [Type](*this);</tt>.

	    \returns A copy of the current player
	*/
	virtual Player *Clone() const = 0;

	/**
	    \brief Determine the next game move
	    
	    This function sets nextMove based upon some function of 
	    <tt>pastGames[&nextOpponent]</tt>.
	    
	    \note The behavior of the Think function should \em not depend on
	    anything but the identity (i.e. address) of \p nextOpponent -- in
	    particular, cheating (reading the value of \c nextOpponent->nextMove, 
	    that is) will do you \em no good, because you have no guarantee 
	    whether or not \c nextOpponent has had its \c Think function 
	    called already.
	    
	    \param gamePlayed The game currently being played
	    \param nextOpponent The opponent which is currently being played

	    \returns True if move was made successfully, false otherwise
	*/
	virtual bool Think(const Game * WXUNUSED(gamePlayed), const Player * WXUNUSED(nextOpponent)) = 0;

	/**
	    \brief The move the player will take in the next game turn
	    
	    This value is set by \c Think and should not be relied upon before
	    \c Think is called.
	*/
	wxChar nextMove;

	/**
	    \brief Reset the player state
	    
	    This function is called before the \c Match class starts a new match,
	    giving the \c Player a chance to reset all its internal data to 
	    prepare for a new game.  After this function is called, the \c Player 
	    must forget all previous interaction it has had with any other players, 
	    and must also have a score of zero.
	*/
	virtual void Reset()
	{
		score = 0;
		opponentMoves.clear();
	}

	/**
	    \brief Register the result of a game interaction
	    
	    This function accumulates the \c pastGames buffer as well as handling
	    keeping score.  Once again, only the address of the \p opponent 
	    parameter should be used.
	    
	    \param opponent The opponent against which the score was obtained
	    \param opponentsMove The move the opponent took this game
	    \param payoff The score this player received this game
	*/
	void AddPayoff(const Player *opponent, char opponentsMove, int payoff)
	{
		// Add in the payoff to the score
		score += payoff;

		// Save this interaction in our history
		wxString &oldMoves = opponentMoves[opponent];
		oldMoves.Append(opponentsMove);
	}

	/**
	    \brief Get the player's name
	    \returns Player's name
	*/
	virtual const wxString &GetPlayerName() const
	{
		static const wxString name(_("Default Player"));
		return name;
	}

	/**
	    \brief Get the player's author
	    \returns Player's author
	*/
	virtual const wxString &GetPlayerAuthor() const
	{
		static const wxString author(_("Built-in"));
		return author;
	}
	
	/**
	    \brief Get the player's ID
	    \returns Player's ID
	*/
	int GetID() const
	{
		return id;
	}
	
	/**
	    \brief Get the player's score
	    \returns Player's score
	*/
	int GetScore() const
	{
		return score;
	}

protected:
	/**
	    \brief This player's unique identifier
	    
	    This value is an identifier unique to this execution of
	    Oyun, used to tell the difference between players in a
	    tournament.
	*/
	int id;
	
	/**
	    \brief This player's score for this game
	    
	    This value accumulates over the course of a \c Match (which
	    consists of many repeated games).
	*/
	int score;

	/**
	    \brief A map which associates \c Player pointers with strings	
	    
	    This map associates an individual competing player object (determined
	    by address) with a list of moves that player has taken in the past,
	    stored as a \c wxString.
	*/
	WX_DECLARE_HASH_MAP(Player *, wxString, wxPointerHash,
	    wxPointerEqual, MoveMap);

	/**
	    \brief Storage for past player interactions.
	    
	    A collection of all opponents this player has faced and the moves that
	    those players have taken.  Serves as "memory" for more complicated
	    players (like tit-for-tat), which respond to the actions of others.
	*/
	MoveMap opponentMoves;
};

/**
    \typedef PlayerPtrArray
    \ingroup game

    \brief A variable-length array of players
*/
WX_DEFINE_ARRAY_PTR(Player *, PlayerPtrArray);


/** \cond TEST */

class MockPlayer : public Player
{
public:
	MockPlayer() : Player() {}
	MockPlayer(const MockPlayer &p) : Player(p) {}
	virtual Player *Clone() const
	{ return new MockPlayer(*this); }
	bool Think(const Game * WXUNUSED(gamePlayed), const Player * WXUNUSED(nextOpponent))
	{ return true; }
};

/** \endcond */

#endif

// Local Variables:
// mode: c++
// End:
