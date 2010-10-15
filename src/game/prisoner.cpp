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

#include "prisoner.h"
#include "player.h"


void PrisonerDilemma::GetGamePayoff(const Player *playerOne, int &playerOneScore,
                                    const Player *playerTwo, int &playerTwoScore)
{
	// Straightforwardly implement the payoff matrix.
	if (playerOne->nextMove == 'C' && playerTwo->nextMove == 'C')
		playerOneScore = playerTwoScore = 3;
	else if (playerOne->nextMove == 'D' && playerTwo->nextMove == 'D')
		playerOneScore = playerTwoScore = 1;
	else if (playerOne->nextMove == 'D')
	{
		playerOneScore = 5;
		playerTwoScore = 0;
	}
	else
	{
		playerOneScore = 0;
		playerTwoScore = 5;
	}
}

/** \cond TEST */

TEST(PrisonerDilemma, GameMoves)
{
	PrisonerDilemma game;
	
	// Make sure the prisoner's dilemma reports the right set of moves
	CHECK_EQUAL(wxT("CD"), game.GetGameMoves());
}

TEST(PrisonerDilemma, Payouts)
{
	PrisonerDilemma game;
	MockPlayer p1, p2;
	
	// Make sure the prisoner's dilemma gives us the right payouts
	// {5,0,1,3}
	p1.nextMove = wxT('C');
	p2.nextMove = wxT('C');
	CHECK(game.Play(&p1, &p2));
	
	CHECK_EQUAL(3, p1.GetScore());
	CHECK_EQUAL(3, p2.GetScore());
	
	p1.Reset();
	p2.Reset();
	
	p1.nextMove = wxT('D');
	p2.nextMove = wxT('C');
	CHECK(game.Play(&p1, &p2));
	
	CHECK_EQUAL(5, p1.GetScore());
	CHECK_EQUAL(0, p2.GetScore());
	
	p1.Reset();
	p2.Reset();
	
	p1.nextMove = wxT('C');
	p2.nextMove = wxT('D');
	CHECK(game.Play(&p1, &p2));
	
	CHECK_EQUAL(0, p1.GetScore());
	CHECK_EQUAL(5, p2.GetScore());
	
	p1.Reset();
	p2.Reset();
	
	p1.nextMove = wxT('D');
	p2.nextMove = wxT('D');
	CHECK(game.Play(&p1, &p2));
	
	CHECK_EQUAL(1, p1.GetScore());
	CHECK_EQUAL(1, p2.GetScore());
}

/** \endcond */

