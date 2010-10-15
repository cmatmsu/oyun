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

#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <TestHarness.h>

#include "../common/error.h"
#include "fsaplayer.h"
#include "game.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(FSAStateArray);


bool FSAPlayer::Load(const Game *game, const wxString &fsaScript)
{
	// Open the file
	wxTextFile file;
	wxString str;
	
	if (!file.Open(fsaScript))
	{
		Error::Set(wxString::Format(_("Could not open file %s"), fsaScript.c_str()));
		return false;
	}

	playerAuthor = file.GetFirstLine();
	if (file.Eof())
	{
		Error::Set(wxString::Format(_("File %s ended while reading player author"), fsaScript.c_str()));
		return false;
	}

	playerName = file.GetNextLine();
	if (file.Eof())
	{
		Error::Set(wxString::Format(_("File %s ended while reading player name"), fsaScript.c_str()));
		return false;
	}

	// Strip newlines from those
	playerAuthor = playerAuthor.BeforeFirst(wxT('\r')).BeforeFirst(wxT('\n'));
	playerName = playerName.BeforeFirst(wxT('\r')).BeforeFirst(wxT('\n'));

	// Get the number of actions
	long numActions;
	str = file.GetNextLine();
	if (file.Eof())
	{
		Error::Set(wxString::Format(_("File %s ended while reading number of lines"), fsaScript.c_str()));
		return false;
	}

	if (!str.ToLong(&numActions))
	{
		Error::Set(wxString::Format(_("File %s had a number of actions that's not a number"), fsaScript.c_str()));
		return false;
	}

	// Load the finite states
	for (int i = 0 ; i < numActions ; i++)
	{
		if (file.Eof())
		{
			Error::Set(wxString::Format(_("File %s ended before the advertised number of actions could be read"), fsaScript.c_str()));
			return false;
		}

		wxString sourceLine = file.GetNextLine();

		// Add to the player source code
		playerSource += sourceLine;
		playerSource += wxT("\n");
		
		// Tokenize the string
		wxStringTokenizer tokens(sourceLine, wxT(","));
		if (tokens.CountTokens() != 3)
		{
			Error::Set(wxString::Format(_("File %s, action %i: doesn't have the correct syntax (not enough tokens?)"), fsaScript.c_str(), i));
			return false;
		}
		
		wxString strAction, strTrans0, strTrans1;
		strAction = tokens.GetNextToken();
		strTrans0 = tokens.GetNextToken();
		strTrans1 = tokens.GetNextToken();
		
		wxChar action = strAction[0];

		long transitions[2];
		if (!strTrans0.ToLong(&transitions[0]))
		{
			Error::Set(wxString::Format(_("File %s, action %i: first transition value is not a number"), fsaScript.c_str(), i));
			return false;
		}
		if (!strTrans1.ToLong(&transitions[1]))
		{
			Error::Set(wxString::Format(_("File %s, action %i: second transition value is not a number"), fsaScript.c_str(), i));
			return false;
		}
		
		if (transitions[0] >= numActions)
		{
			Error::Set(wxString::Format(_("File %s, action %i: first transition is out of bounds"), fsaScript.c_str(), i));
			return false;
		}
		if (transitions[1] >= numActions)
		{
			Error::Set(wxString::Format(_("File %s, action %i: second transition is out of bounds"), fsaScript.c_str(), i));
			return false;
		}

		// Check the validity of the move
		int moveidx = game->GetGameMoves().Find(action);
		if (moveidx == -1)
		{
			Error::Set(wxString::Format(_("File %s, action %i: requested move is not valid for this game"), fsaScript.c_str(), i));
			return false;
		}

		machine.Add(FSAState(action, transitions));
	}
	
	return true;
}

bool FSAPlayer::Think(const Game *gamePlayed, const Player *nextOpponent)
{
	// Recall our history with this player
	wxString &oldMoves = opponentMoves[nextOpponent];

	// Do we have a history?
	if (!oldMoves.Length())
	{
		// This must be the first move of the game, so just do it--start on state 0
		eip = 0;
	}
	else
	{
		// What did they do to us last time?
		wxChar lastMove = oldMoves[oldMoves.Length() - 1];
		int state = gamePlayed->GetGameMoves().Find(lastMove);
		
		if (state != 0 && state != 1)
		{
			Error::Set(wxString::Format(_("Player %s faced an opponent who made an invalid move last turn"), 
			           playerAuthor.c_str()));
			return false;
		}

		eip = machine[eip].transitions[state];
	}

	if (eip >= machine.GetCount())
	{
		Error::Set(wxString::Format(_("Player %s has an index which stepped out of bounds (%d >= %d)"), 
		           playerAuthor.c_str(), eip, machine.GetCount()));
		return false;
	}
	
	nextMove = machine[eip].action;
	
	return true;
}


/** \cond TEST */

// FSA player tests
TEST(FSAPlayer, Loading)
{
	MockGame game;
	FSAPlayer *fsa;
	
	// Load the all-C player
	fsa = new FSAPlayer;
	CHECK(fsa->Load(&game, wxT("data/test/testc.txt")));
	delete fsa;
	
	// Load the all-D player, check some stats
	fsa = new FSAPlayer;
	CHECK(fsa->Load(&game, wxT("data/test/testd.txt")));
	CHECK_EQUAL(1, fsa->GetNumLines());
	delete fsa;
}

TEST(FSAPlayer, BadData)
{
	MockGame game;
	FSAPlayer fsa;

	// Various kinds of bad user data: an invalid move	
	CHECK(!fsa.Load(&game, wxT("data/test/badmove.txt")));
	
	// An invalid transition (step out of bounds)
	CHECK(!fsa.Load(&game, wxT("data/test/badtrans.txt")));
	
	// Not enough states specified
	CHECK(!fsa.Load(&game, wxT("data/test/notenough.txt")));
	
	// A non-numerical transition value
	CHECK(!fsa.Load(&game, wxT("data/test/nan.txt")));
}

TEST(FSAPlayer, SimplePlayers)
{
	FSAPlayer allc;
	FSAPlayer alld;
	MockGame game;
	
	// Load the two simple testing players
	CHECK(allc.Load(&game, wxT("data/test/testc.txt")));
	CHECK(alld.Load(&game, wxT("data/test/testd.txt")));
	
	// Have both of them set moves
	CHECK(allc.Think(&game, &alld));
	CHECK(alld.Think(&game, &allc));
	
	CHECK_EQUAL(wxT('C'), allc.nextMove);
	CHECK_EQUAL(wxT('D'), alld.nextMove);
}

TEST(FSAPlayer, Strategy)
{
	FSAPlayer tft;
	MockPlayer opp;
	MockGame game;
	
	// Load a TFT player, then run the TFT Strategy test
	CHECK(tft.Load(&game, wxT("data/test/tft.txt")));
	
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

TEST(FSAPlayer, Clone)
{
	FSAPlayer playerOne;
	Player *playerTwo;
	MockGame game;

	CHECK(playerOne.Load(&game, wxT("data/test/testc.txt")));
	
	// Make sure FSAPlayer::Clone works
	playerTwo = playerOne.Clone();
	CHECK_TYPES_EQUAL(playerOne, *playerTwo);

	delete playerTwo;
}

/** \endcond */


