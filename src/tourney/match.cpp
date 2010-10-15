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

#include "../game/game.h"
#include "match.h"


bool Match::Play(Game *game, bool quick)
{
	// Clear score buffers
	playerOneScore = playerTwoScore = 0;
	
	// Run five games
	int max = (quick ? 1 : 5);
	for (int i = 0 ; i < max ; i++)
	{
		// These were pre-computed using Axelrod's game-end factor (0.00346)
		int matchLengths[5] = {168, 359, 306, 622, 319};
		if (quick) matchLengths[0] = 200;
		
		// Prepare the players and the game for a new match
		game->Reset();
		playerOne->Reset();
		playerTwo->Reset();
		
		// Run the match
		for (int j = 0 ; j < matchLengths[i] ; j++)
		{
			// Error already set in Player::Think()
			if (!playerOne->Think(game, playerTwo) ||
			    !playerTwo->Think(game, playerOne))
				return false;
			
			// Error already set in Game::Play()
			if (!game->Play(playerOne, playerTwo))
				return false;
		}
		
		// Save off the history and clear the old one
		matchHistory[i] = game->GetGameHistory();
		// Save off the scores
		playerOneScore += playerOne->GetScore();
		playerTwoScore += playerTwo->GetScore();
	}
	
	return true;
}


/** \cond TEST */

TEST(Match, AssignsPlayers)
{
	MockPlayer p1, p2;
	Match match(&p1, &p2);

	POINTERS_EQUAL(&p1, match.playerOne);
	POINTERS_EQUAL(&p2, match.playerTwo);
}

TEST(Match, Quick)
{
	MockPlayer p1, p2;
	MockGame game;
	Match match(&p1, &p2);

	p1.nextMove = 'C';
	p2.nextMove = 'D';

	CHECK(match.Play(&game, true));
	
	// This match should have a simple score of 200-0, since
	// the first player always wins and receives 1 point.
	CHECK_EQUAL(200, match.playerOneScore);
	CHECK_EQUAL(0, match.playerTwoScore);
}

TEST(Match, LongMatch)
{
	MockPlayer p1, p2;
	MockGame game;
	Match match(&p1, &p2);

	p1.nextMove = 'C';
	p2.nextMove = 'D';

	CHECK(match.Play(&game));
	
	// We want it to be at least possible for the game lengths
	// to be indeterminate, so all we can check is that the first
	// player has received *some* score.
	CHECK(match.playerOneScore > 0);
	CHECK_EQUAL(0, match.playerTwoScore);
}

TEST(Match, History)
{
	MockPlayer p1, p2;
	MockGame game;
	Match match(&p1, &p2);

	p1.nextMove = 'C';
	p2.nextMove = 'D';

	CHECK(match.Play(&game, true));
	
	// Get the history and make sure it's stored properly
	wxArrayString &hist = match.matchHistory[0];

	CHECK_EQUAL(200, (int)hist.size());
	for (int i = 0 ; i < 200 ; i++)
	{
		CHECK_EQUAL(wxT('C'), hist[i][0]);
		CHECK_EQUAL(wxT('D'), hist[i][1]);
	}
}

/** \endcond */


