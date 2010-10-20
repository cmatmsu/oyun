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

#ifndef RANDOM_H__
#define RANDOM_H__

#include "player.h"
class Game;

/**
    \class RandomPlayer
    \ingroup game
    
    \brief A player which moves randomly from the available choices
    
    A built-in, "worst-case" player, the random player simply selects randomly
    from one of the available game moves.
*/
class RandomPlayer : public Player
{
public:
	RandomPlayer() : 
	    Player()
	{ }
	
	/**
	    \brief Copy constructor
	    \param p Player to be copied
	*/
	RandomPlayer(const RandomPlayer &p) :
	    Player(p)
	{ }
	
	virtual Player *Clone() const
	{ return new RandomPlayer(*this); }
	virtual bool Think(const Game *gamePlayed, const Player *nextOpponent);

	virtual const wxString &GetPlayerName() const
	{
		static const wxString name(_("Built-In Random"));
		return name;
	}
};


#endif

// Local Variables:
// mode: c++
// End:
