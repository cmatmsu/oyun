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

/**
    \defgroup tourney Tournament Module
    
    This module contains the match and tournament code.  A \c Match is a
    controlled set of five games (or occasionally one) between two
    given players.  The two tournament types (<tt>Tournament</tt> and
    <tt>EvoTournament</tt>) are collections of matches between a given
    set of players.  The \c Tournament class is a round-robin tournament
    with one winner using five-game matches, while the \c EvoTournament is
    an "evolutionary" tournament in which the score of each player after
    each match determines each player's fraction in the next "generation"
    of players.
*/

#ifndef TOURNEY_TOURNAMENT_H__
#define TOURNEY_TOURNAMENT_H__

#include "../game/player.h"
#include "../tourney/match.h"
class Game;


/**
    \class Tournament
    \ingroup tourney
    
    \brief Runs a round-robin tournament between a set of players
    
    This class is responsible for running a round-robin tournament between
    a set of players it receives using the \c wxEVT_ADD_PLAYER event.
    
    Every time a player is added (see the \c AddPlayer method), two new
    copies of it are created (see <tt>Player::Clone</tt>) and added to
    the \c playerOneList and \c playerTwoList.  The tournament is then set
    up by creating a match for each combination of the elements in this list.
    
    The individual matches are then run, and the scores are accumulated in
    the \c scores member, which can be queried to determine the results.
*/
class Tournament
{
public:
	/**
	    \brief Constructor
	    
	    Sets the \c game value.
	    
	    \param newGame Initial value of the \c game member
	*/
	Tournament(Game *newGame);

	~Tournament();
	
	
	/**
	    \brief Add a player to the internal list
	    
	    This function is intended to be called in response to the 
	    \c wxEVT_ADD_PLAYER event.  The player passed will be copied
	    twice, and the copies stored on the two player lists.
	    
	    This function will not store the pointer passed to it.
	    
	    \param player Player to be cloned and added to the tournament
	*/
	void AddPlayer(const Player *player);

	/**
	    \brief Remove a player like this from the internal list
	    
	    This function will search the player lists for a player of the
	    same type, name, and author as the one passed, and remove any
	    players which match.  Notably, this means that all sorts of
	    unspecified bugs will occur if two players of the same type,
	    name, and author are added to the tournament.
	    
	    This function will not store the pointer passed to it.
	    
	    \param player Player of the same type, name, and author as those
	                  to be removed
	*/
	void RemovePlayer(const Player *player);
	
	
	/**
	    \brief Run the tournament
	    
	    Run the actual matches in the tournament, accumulating the
	    player scores into the \c scores member.
	    
	    \returns True if the tournament ran successfully, false otherwise
	*/
	bool Run();

	/**
	    \brief Reset all internal data
	    
	    This clears all scores and matches, preparing to run another
	    tournament using the same player lists.
	*/
	void Reset();
	
	
	/**
	    \brief Has the tournament been played?
	    \returns True if tournament played, false otherwise
	*/
	bool IsPlayed() const {return played;} 

	/**
	    \brief Get the number of players in the tournament
	    \returns Number of players
	*/
	int GetNumPlayers() const {return playerOneList.size();}
	
	/**
	    \brief Get the number of matches played
	    \returns Number of matches played
	*/
	int GetNumMatches() const {return matches.size();}
	
	/**
	    \brief Get a given match from the matches array
	    
	    \note This function is not bounds-checked, make sure that
	    \p idx is less than <tt>GetNumMatches()</tt>.
	    
	    \param idx The index of the match to be returned

	    \returns Match at the given index
	*/
	Match *GetMatch(int idx) {return matches[idx];}
	
	
	/**
	    \brief List of all "player one" players in this tournament
	*/
	PlayerPtrArray playerOneList;
	
	/**
	    \brief List of all "player two" players in this tournament
	*/
	PlayerPtrArray playerTwoList;
	
	
	/**
	    \brief A map which associates player IDs (integers) with 
	           integer player scores
	*/
	WX_DECLARE_HASH_MAP(int, int, wxIntegerHash, wxIntegerEqual, ScoreMap);
	
	/**
	    \brief The net score of every player in the tournament
	*/
	ScoreMap scores;

private:
	/**
	    \brief Recompute the list of matches
	    
	    Calculates the round-robin match list, with one match between
	    each pair of players.  See the \c matches member.
	*/
	void RecalculateMatchList();
	

	/**
	    \brief True when the tournament is played
	*/
	bool played;
	
	
	/**
	    \brief List of all matches to be played
	*/
	MatchPtrArray matches;
	
	/**
	    \brief Game to be played
	*/
	Game *game;
};


#endif

// Local Variables:
// mode: c++
// End:
