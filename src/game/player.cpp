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

#include <wx/wxprec.h>
#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

#include <TestHarness.h>

#include "player.h"
#include "game.h"

/** \cond TEST */

TEST(Player, Initial)
{
	MockPlayer player;

	// Make sure we start with no score and an invalid move	
	CHECK_EQUAL('\0', player.nextMove);
	CHECK_EQUAL(0, player.GetScore());
}

TEST(Player, UniqueIDs)
{
	MockPlayer p1, p2;
	
	// Make sure we're generating unique identifiers
	CHECK_NOT_EQUAL(p1.GetID(), p2.GetID());
}

TEST(Player, Clone)
{
	MockPlayer playerOne;
	Player *playerTwo;
	
	// Clone the player and check its data
	playerOne.nextMove = 'C';
	playerTwo = playerOne.Clone();

	CHECK_TYPES_EQUAL(playerOne, *playerTwo);
	CHECK_EQUAL(playerOne.nextMove, playerTwo->nextMove);
	CHECK_EQUAL(playerOne.GetScore(), playerTwo->GetScore());
	CHECK_EQUAL(playerOne.GetID(), playerTwo->GetID());

	delete playerTwo;
}

TEST(Player, AccumulateScore)
{
	MockPlayer playerOne;
	MockPlayer playerTwo;
	MockGame game;

	playerOne.nextMove = 'C';
	playerTwo.nextMove = 'D';

	CHECK(game.Play(&playerOne, &playerTwo));

	CHECK_EQUAL(1, playerOne.GetScore());
	CHECK_EQUAL(0, playerTwo.GetScore());
}

TEST(Player, Reset)
{
	MockPlayer playerOne;
	MockPlayer playerTwo;
	MockGame game;

	playerOne.nextMove = 'C';
	playerTwo.nextMove = 'D';

	CHECK(game.Play(&playerOne, &playerTwo));

	// Now, reset the player
	playerOne.Reset();

	// Make sure that everything has been reset
	CHECK_EQUAL(0, playerOne.GetScore());
}

/** \endcond */


