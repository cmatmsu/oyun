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

#include <TestHarness.h>

#include "game.h"
#include "player.h"

/** \cond TEST */

TEST(Game, GetMoves)
{
	MockGame game;
	
	CHECK_EQUAL(wxT("CD"), game.GetGameMoves());
}

TEST(Game, ValidateMoves)
{
	MockPlayer playerOne;
	MockPlayer playerTwo;
	MockGame game;

	// Invalid moves should fail
	playerOne.nextMove = '7';
	playerTwo.nextMove = 'C';
		
	CHECK(!game.Play(&playerOne, &playerTwo));

	playerOne.nextMove = 'C';
	playerTwo.nextMove = '7';

	CHECK(!game.Play(&playerOne, &playerTwo));
	
	playerOne.nextMove = 'C';
	playerTwo.nextMove = 'D';
	
	CHECK(game.Play(&playerOne, &playerTwo));
}

TEST(Game, CorrectScoring)
{
	MockPlayer playerOne;
	MockPlayer playerTwo;
	MockGame game;

	// Check to make sure that the scoring works (i.e. that the
	// payouts are as expected)
	playerOne.nextMove = 'C';
	playerTwo.nextMove = 'C';
	CHECK(game.Play(&playerOne, &playerTwo));

	CHECK_EQUAL(1, playerOne.GetScore());
	CHECK_EQUAL(0, playerTwo.GetScore());
}

TEST(Game, SaveHistory)
{
	MockPlayer playerOne;
	MockPlayer playerTwo;
	MockGame game;
	
	playerOne.nextMove = wxT('C');
	playerTwo.nextMove = wxT('D');
	CHECK(game.Play(&playerOne, &playerTwo));
	
	// Make sure that the game accurately saves moves into the history
	CHECK_EQUAL(wxT('C'), game.GetGameHistory()[0][0]);
	CHECK_EQUAL(wxT('D'), game.GetGameHistory()[0][1]);
	
	// Make sure that we've only saved one game
	CHECK_EQUAL(1, game.GetGameHistory().GetCount());
}

TEST(Game, Reset)
{
	MockPlayer playerOne;
	MockPlayer playerTwo;
	MockGame game;
	
	playerOne.nextMove = wxT('C');
	playerTwo.nextMove = wxT('D');
	CHECK(game.Play(&playerOne, &playerTwo));
	
	// Make sure that we've only saved one game
	CHECK_EQUAL(1, game.GetGameHistory().GetCount());
	
	// Now, reset the game
	game.Reset();
	
	// And make sure there's now no history
	CHECK_EQUAL(0, game.GetGameHistory().GetCount());
}

/** \endcond */

