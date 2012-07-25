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

#include <wx/wxprec.h>
#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

#ifdef BUILD_TESTS
#  include <TestHarness.h>
#endif

#include "../common/rng.h"
#include "random.h"
#include "game.h"


bool RandomPlayer::Think(const Game *gamePlayed, const Player * WXUNUSED(nextOpponent))
{
	float randomNumber = Random::GenerateFloatHigh() * 
		(float)gamePlayed->GetGameMoves().Length();
	int moveToChoose = (int)floor(randomNumber);
	
	nextMove = gamePlayed->GetGameMoves()[moveToChoose];
	return true;
}


/** \cond TEST */
#ifdef BUILD_TESTS

TEST(RandomPlayer, Clone)
{
	RandomPlayer playerOne;
	Player *playerTwo;

	// Make sure that RandomPlayer::Clone works
	playerTwo = playerOne.Clone();
	CHECK_TYPES_EQUAL(playerOne, *playerTwo);

	delete playerTwo;
}

TEST(RandomPlayer, Strategy)
{
	RandomPlayer playerOne;
	RandomPlayer playerTwo;
	MockGame game;
	
	// There's nothing we can do to check that the random moves are
	// actually random, so just check to see that it sets the move
	// to *something*
	playerOne.nextMove = 0;
	CHECK(playerOne.Think(&game, &playerTwo));

	CHECK_NOT_EQUAL(0, playerOne.nextMove);
}

#endif
/** \endcond */

