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

/**
    \defgroup game Game Module
    
    This group contains, roughly, games and players.  Players may either
    be written in C++ and hard-coded (called "built-in" in the user 
    interface), or specified by external finite state machines.
    Currently there is only one game, \c PrisonerDilemma, and it is written
    in C++.  A format for specifying normal-form games is forthcoming.
    
    All games derive from the \c Game class, and all players derive from
    the \c Player class.
*/

#ifndef GAME_H__
#define GAME_H__

#include "../common/error.h"
#include "player.h"

/**
    \class Game
    \ingroup game

    \brief Abstract base class for all games

    This base class implements the basic methods required for an iterated game
    between players.  It handles validation of basic game moves and storing of
    game history over time.
*/
class Game
{
public:
	virtual ~Game() { }

	/**
	    \brief Play one round of this game between two players

	    This function will validate the players' next moves, compute the score,
	    and add the turn to the game history.
	    
	    \param playerOne First player in game
	    \param playerTwo Second player in game
	    \returns True if the game was successfully played, false if one of the
	             players attempted an invalid move
	*/
	bool Play(Player *playerOne, Player *playerTwo)
	{
		// Check the incoming values to make sure we're legit
		if (gameMoves.Find(playerOne->nextMove) == -1)
		{
			Error::Set(wxString::Format(_("Player %s made an invalid move (move not in {%s})"),
			           playerOne->GetPlayerName().c_str(), gameMoves.c_str()));
			return false;
		}
		
		if (gameMoves.Find(playerTwo->nextMove) == -1)
		{
			Error::Set(wxString::Format(_("Player %s made an invalid move (move not in {%s})"),
			           playerTwo->GetPlayerName().c_str(), gameMoves.c_str()));
			return false;
		}
		
		// Compute the score for this round
		int playerOneScore, playerTwoScore;
		GetGamePayoff(playerOne, playerOneScore, playerTwo, playerTwoScore);
		
		// Add this move to the game history
		wxString thisTurn;
		thisTurn.Append(playerOne->nextMove);
		thisTurn.Append(playerTwo->nextMove);
		gameHistory.Add(thisTurn);
		
		// Tell the players what just happened
		playerOne->AddPayoff(playerTwo, playerTwo->nextMove, playerOneScore);
		playerTwo->AddPayoff(playerOne, playerOne->nextMove, playerTwoScore);
		
		return true;
	}

	/**
	    \brief Reset the game state
	    
	    This function clears the stored game history.
	*/
	virtual void Reset()
	{ gameHistory.clear(); }

protected:
	/**
	    \brief Permissible game moves
	   
	    This string is a list of the permissible moves each player can make.
	    As an example, the prisoner's dilemma gameMoves string would be "CD",
	    as players may either cooperate or defect on each turn.
	    
	    \note If the game is one, like the prisoner's dilemma, for which the 
	    \c TitForTat player has a "first move" that it \em should always take, 
	    that move must be \c gameMoves[0].  \c TitForTatPlayer will open with 
	    \c gameMoves[0].  In other words, if there is a "cooperate" action,
	    it must be listed first.
	*/
	wxString gameMoves;

	/**
	    \brief The history of all turns that have been taken in this game
	    
	    Each member of this array is the result of a particular turn.  It is
	    a \c wxString of length two -- the first character is the first player's
	    move, the second character the second.
	    
	    Note that this game history does not notice if the players participating
	    in a game change while the game is in progress.  If you wish to use
	    this history, you shouldn't change the players, or you should make sure
	    to call \c Game::Reset before doing so.
	*/
	wxArrayString gameHistory;

	/**
	    \brief Function determining game payoff
	    
	    This function will read \p playerOne.nextMove and \p playerTwo.nextMove
	    to determine each player's score.  The score earned is set as output in
	    \p playerOneScore and \p playerTwoScore.
	    
	    \param playerOne First player in game
	    \param[out] playerOneScore Score the first player will receive this round
	    \param playerTwo Second player in game
	    \param[out] playerTwoScore Score the second player will receive this round
	*/
	virtual void GetGamePayoff(const Player *playerOne, int &playerOneScore,
	                           const Player *playerTwo, int &playerTwoScore) = 0;

public:
	/**
	    \brief Get the acceptable moves string for this game
	    \returns Acceptable moves string
	*/
	const wxString &GetGameMoves() const
	{ return gameMoves; }

	/**
	    \brief Get the move history for this game
	    \returns Game move history
	*/
	const wxArrayString &GetGameHistory() const
	{ return gameHistory; }
};


/** \cond TEST */

// A mock game object for unit testing.  The allowable moves are C and D,
// and the first player always wins, receiving a payoff of 1, regardless of
// the moves taken.
class MockGame : public Game
{
public:
	MockGame()
	{ gameMoves = wxT("CD"); }
	virtual ~MockGame() { }

protected:
	void GetGamePayoff(const Player * WXUNUSED(playerOne), int &playerOneScore,
	                   const Player * WXUNUSED(playerTwo), int &playerTwoScore)
	{
		// In MockGame, playerOne always wins, and gets one point.
		playerOneScore = 1;
		playerTwoScore = 0;
	}
};

/** \endcond */


#endif

// Local Variables:
// mode: c++
// End:
