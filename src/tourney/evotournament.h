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

#ifndef TOURNEY_EVOTOURNAMENT_H__
#define TOURNEY_EVOTOURNAMENT_H__

class Game;
#include "../game/player.h"

/**
    \typedef GenerationWeights
    \ingroup tourney
    \brief A map which associates player names (<tt>wxString</tt> values)
           with population fraction
    
    These population-fraction values are always between \c 0.0 and \c 1.0,
    and the contents of any \c GenerationWeights map always sum to \c 1.0
    (100% of the population).
*/
WX_DECLARE_HASH_MAP(int, float, wxIntegerHash, wxIntegerEqual, GenerationWeights);

/**
    \typedef GenerationWeightArray
    \ingroup tourney
    \brief A variable-length array of GenerationWeights
*/
WX_DECLARE_OBJARRAY(GenerationWeights, GenerationWeightArray);



/**
    \class EvoTournament
    \ingroup tourney
    
    \brief Runs an evolutionary tournament between a set of players
    
    This class is responsible for running an "evolutionary" tournament
    between a set of players.  A list of players is maintained (see the
    \c AddPlayer and \c RemovePlayer methods), and the tournament is
    run by computing the relative score of each player to determine the
    fraction of each in the next generation of the population.
    
    The players' fractions at each generation are stored in the \c data
    member for later use or graphing.
*/
class EvoTournament
{
public:
	/**
	    \brief Constructor
	    
	    Sets the \c game value.
	    
	    \param gm Initial value of the \c game member.
	*/
	EvoTournament(Game *gm);

	virtual ~EvoTournament();
	
	
	/**
	    \brief Add a player to the internal list
	    
	    This function is intended to be called in respone to the
	    \c wxEVT_ADD_PLAYER event.  The player passed will be
	    cloned and placed on the \c players list.
	    
	    This function will not store the pointer passed to it.
	    
	    \param player Player to be cloned and added to the tournament
	*/
	void AddPlayer(const Player *player);
	
	/**
	    \brief Remove this player from the internal list
	    
	    This function will search the internal list for a player of
	    the same ID as the one passed, and remove it if found.
	    
	    This function will not store the pointer passed to it.
	    
	    \param player Player of the same ID as that to be removed
	*/
	void RemovePlayer(const Player *player);


	/**
	    \brief Run an evolutionary tournament
	    
	    Computes the population fractions, beginning at equal fractions
	    and evolving proportional to player scores over time.  The results
	    are accumulated in the \c data member.

	    \param numGenerations Number of evolutionary generations to compute	    
	    \returns True if the tournament ran successfully, false otherwise
	*/
	bool Run(int numGenerations);
	
	
	/**
	    \brief Has the tournament been played?
	*/
	bool IsPlayed() { return played; }

	/**
	    \brief Reset all internal data
	    
	    This function clears the tournament data and resets the
	    \c played value to false.
	*/
	void Reset();
	
	
	/**
	    \brief List of all players in this tournament
	*/
	PlayerPtrArray players;
	
	/**
	    \brief The player fractions at every generation of the tournament
	    
	    This array has as many elements as generations in the tournament,
	    and each element specifies the fractional weight of the players
	    in the tournament at that generation.
	*/
	GenerationWeightArray data;

private:
	/**
	    \brief True when the tournament has been played
	*/
	bool played;
	
	/**
	    \brief Game to be played
	*/
	Game *game;
};

#endif

