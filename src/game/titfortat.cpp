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

#include "titfortat.h"
#include "game.h"


bool TitForTatPlayer::Think(const Game *gamePlayed, const Player *nextOpponent)
{
	// Recall our history with this player
	wxString &oldMoves = opponentMoves[nextOpponent];
	
	// Do we have a history?
	if (!oldMoves.size())
	{
		nextMove = gamePlayed->GetGameMoves()[0];
		return true;
	}
	
	// What did they do to us last time?
	nextMove = oldMoves[oldMoves.Length() - 1];
	return true;
}


/** \cond TEST */
#ifdef BUILD_TESTS

TEST(TitForTatPlayer, Strategy)
{
	TitForTatPlayer tft;	
	MockPlayer opp;
	MockGame game;
	
	// Set next moves
	CHECK(tft.Think(&game, &opp));
	opp.nextMove = wxT('C');
	
	// TFT's initial move should be C
	CHECK_EQUAL(wxT('C'), tft.nextMove);
	
	// Play game
	CHECK(game.Play(&tft, &opp));
	
	// Set next moves
	CHECK(tft.Think(&game, &opp));
	opp.nextMove = wxT('D');
	
	// TFT should have played C
	CHECK_EQUAL(wxT('C'), tft.nextMove);
	
	// Play game
	CHECK(game.Play(&tft, &opp));
	
	// Set next moves
	CHECK(tft.Think(&game, &opp));
	opp.nextMove = wxT('C');
	
	// TFT should have played D
	CHECK_EQUAL(wxT('D'), tft.nextMove);
}

TEST(TitForTatPlayer, Clone)
{
	TitForTatPlayer playerOne;
	Player *playerTwo;
	
	// Make sure TitForTatPlayer::Clone works
	playerTwo = playerOne.Clone();
	CHECK_TYPES_EQUAL(playerOne, *playerTwo);

	delete playerTwo;
}

#endif
/** \endcond */

