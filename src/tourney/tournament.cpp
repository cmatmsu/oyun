/*
    Copyright (C) 2004-2011 by Charles Pence
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

#include <wx/wxprec.h>
#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

#include <wx/progdlg.h>
#include <TestHarness.h>

#include "../ui/logosapp.h"
#include "../game/game.h"
#include "tournament.h"
#include "match.h"


Tournament::Tournament(Game *newGame) : played(false), game(newGame)
{ }

Tournament::~Tournament()
{
	// Free player lists
	for (size_t i = 0 ; i < playerOneList.GetCount() ; i++)
		delete playerOneList[i];
	playerOneList.Clear();

	for (size_t i = 0 ; i < playerTwoList.GetCount() ; i++)
		delete playerTwoList[i];
	playerTwoList.Clear();

	// Free matches list
	for (size_t i = 0 ; i < matches.GetCount() ; i++)
		delete matches[i];
	matches.Clear();

	// Destroy the old tournament scores
	scores.clear();
}

void Tournament::Reset()
{
	// Clear scores
	scores.clear();

	// Free matches list
	for (size_t i = 0 ; i < matches.GetCount() ; i++)
		delete matches[i];
	matches.Clear();

	// Leave the players intact, recalculate matches
	RecalculateMatchList();

	// We haven't played now
	played = false;
}

void Tournament::AddPlayer(const Player *player)
{
	// Add to lists
	playerOneList.push_back(player->Clone());
	playerTwoList.push_back(player->Clone());
	
	// Reset if we've played already
	Reset();
}

void Tournament::RemovePlayer(const Player *player)
{
	for (size_t i = 0 ; i < playerOneList.GetCount() ; i++)
	{
		Player *t = playerOneList[i];
		
		if (t->GetID() == player->GetID())
		{
			playerOneList.RemoveAt(i);
			break;
		}
	}

	for (size_t i = 0 ; i < playerTwoList.GetCount() ; i++)
	{
		Player *t = playerTwoList[i];
		
		if (t->GetID() == player->GetID())
		{
			playerTwoList.RemoveAt(i);
			break;
		}
	}
}

void Tournament::RecalculateMatchList()
{
	// Free matches list
	for (size_t i = 0 ; i < matches.GetCount() ; i++)
		delete matches[i];
	matches.Clear();

	// Create the list again from scratch
	unsigned int i, j;
	for (i = 0 ; i < playerOneList.size() ; i++)
	{
		for (j = i ; j < playerTwoList.size() ; j++)
		{
			Match *newMatch = new Match(playerOneList[i], playerTwoList[j]);
			matches.Add(newMatch);
		}
	}
}

bool Tournament::Run()
{
	// See if we need to reset the matches and such
	if (played)
		Reset();

	// Make sure we're really ready to go
	if (!playerOneList.size() || !playerTwoList.size() || !matches.GetCount())
		return false;

	wxBeginBusyCursor();

	// Run the tournament itself
	for (size_t i = 0 ; i < matches.GetCount() ; i++)
	{
		// Error already set in Match::Play()
		if (!matches[i]->Play(game, false))
			return false;
	}

	wxEndBusyCursor();

	// Accumulate the scores for each player
	for (size_t i = 0 ; i < matches.GetCount() ; i++)
	{
		scores[matches[i]->playerOne->GetID()] += matches[i]->playerOneScore;
		scores[matches[i]->playerTwo->GetID()] += matches[i]->playerTwoScore;
	}

	// Set the played flag
	played = true;
	return true;
}

/** \cond TEST */

TEST(Tournament, AddPlayer)
{
	// Ensure that adding a player works
	MockGame game;
	MockPlayer player;
	Tournament tourney(&game);
	
	tourney.AddPlayer(&player);
	
	CHECK_EQUAL(1, tourney.GetNumPlayers());
}

TEST(Tournament, PlayerLists)
{
	// Ensure that you get one player in each player list
	MockGame game;
	MockPlayer player;
	Tournament tourney(&game);
	
	tourney.AddPlayer(&player);
	
	CHECK_EQUAL(1, tourney.playerOneList.GetCount());
	CHECK_EQUAL(1, tourney.playerTwoList.GetCount());
	
	CHECK_TYPES_EQUAL(player, *(tourney.playerOneList[0]));
	CHECK_TYPES_EQUAL(player, *(tourney.playerTwoList[0]));
}

TEST(Tournament, RemovePlayer)
{
	// Add a player
	MockGame game;
	MockPlayer player;
	Tournament tourney(&game);
	
	tourney.AddPlayer(&player);
	
	// Make sure we've got it
	CHECK_EQUAL(1, tourney.GetNumPlayers());
	
	// Now remove it
	tourney.RemovePlayer(&player);
	
	// Make sure it completely goes away
	CHECK_EQUAL(0, tourney.GetNumPlayers());
	CHECK_EQUAL(0, tourney.playerOneList.GetCount());
	CHECK_EQUAL(0, tourney.playerTwoList.GetCount());
}

TEST(Tournament, Matches)
{
	// Add a player
	MockGame game;
	MockPlayer p1, p2;
	Tournament tourney(&game);
	
	tourney.AddPlayer(&p1);
	
	// Make sure we have the right number of matches
	CHECK_EQUAL(1, tourney.GetNumMatches());
	
	// Check that the match was filled out right
	CHECK_EQUAL(p1.GetID(), tourney.GetMatch(0)->playerOne->GetID());
	CHECK_EQUAL(p1.GetID(), tourney.GetMatch(0)->playerTwo->GetID());
	
	// Add another player
	tourney.AddPlayer(&p2);
	
	// Make sure we have the right number of matches
	CHECK_EQUAL(3, tourney.GetNumMatches());
	
	// Check that each was filled out right, without requiring anything about
	// the order of the matches
	for (int i = 0 ; i < 3 ; i++)
	{
		Match *m = tourney.GetMatch(i);
		
		CHECK(m->playerOne->GetID() == p1.GetID() ||
		      m->playerOne->GetID() == p2.GetID());
		CHECK(m->playerTwo->GetID() == p1.GetID() ||
		      m->playerTwo->GetID() == p2.GetID());
	}
}

TEST(Tournament, Run)
{
	// Add two players
	MockGame game;
	MockPlayer p1, p2;
	Tournament tourney(&game);
	
	p1.nextMove = p2.nextMove = wxT('C');
	
	tourney.AddPlayer(&p1);
	tourney.AddPlayer(&p2);
	
	CHECK(tourney.Run());
	
	// Make sure we have scores for both players
	CHECK_EQUAL(2, tourney.scores.size());
	
	// Again, the match lengths are indeterminate, but both players should
	// gain some score here
	CHECK(tourney.scores[p1.GetID()] > 0);
	CHECK(tourney.scores[p2.GetID()] > 0);
}

TEST(Tournament, Reset)
{
	// Add two players
	MockGame game;
	MockPlayer p1, p2;
	Tournament tourney(&game);
	
	p1.nextMove = p2.nextMove = wxT('C');
	
	tourney.AddPlayer(&p1);
	tourney.AddPlayer(&p2);
	
	CHECK(tourney.Run());
	
	// Now, reset
	tourney.Reset();
	
	// Check that we *do* have players and matches
	CHECK_EQUAL(2, tourney.GetNumPlayers());
	CHECK_EQUAL(3, tourney.GetNumMatches());
	
	// Check that we *do not* have scores
	CHECK_EQUAL(0, tourney.scores.size());
}

/** \endcond */



