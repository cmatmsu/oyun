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

#ifndef PRISONER_H__
#define PRISONER_H__

#include "game.h"
#include "player.h"

/**
    \class PrisonerDilemma
    \ingroup game
    
    \brief The prisoner's dilemma game
    
    This is an implementation of the Prisoner's dilemma with payoff matrix:
    \code
        C  D
     C [3, 0]
     D [5, 1]
    \endcode
*/
class PrisonerDilemma : public Game
{
public:
	PrisonerDilemma() 
	{ gameMoves = wxT("CD"); }
	virtual ~PrisonerDilemma() { }

protected:
	virtual void GetGamePayoff(const Player *playerOne, int &playerOneScore,
	                           const Player *playerTwo, int &playerTwoScore);
};


#endif

// Local Variables:
// mode: c++
// End:
