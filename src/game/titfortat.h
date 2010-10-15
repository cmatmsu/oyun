/*
    Copyright (C) 2004-2009 by Charles Pence
    [cpence at gmail dot com]

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

#ifndef TITFORTAT_H__
#define TITFORTAT_H__

#include "player.h"
class Game;


/**
    \class TitForTatPlayer
    \ingroup game
    
    \brief A player which implements the tit-for-tat strategy
    
    A built-in version of the common (and, by all accounts, maximal)
    tit-for-tat strategy.  This player leads with the cooperate move,
    and thereafter simply duplicates the last move which its opponent
    chose.
*/
class TitForTatPlayer : public Player
{
public:
	TitForTatPlayer() :
	    Player()
	{ }
	
	/**
	    \brief Copy constructor
	    \param p Player to be copied
	*/
	TitForTatPlayer(const TitForTatPlayer &p) :
	    Player(p)
	{ }
	
	virtual Player *Clone() const
	{ return new TitForTatPlayer(*this); }
	virtual bool Think(const Game *gamePlayed, const Player *nextOpponent);

	virtual const wxString &GetPlayerName() const
	{
		static const wxString name(_("Built-In Tit-For-Tat"));
		return name;
	}
};

#endif

