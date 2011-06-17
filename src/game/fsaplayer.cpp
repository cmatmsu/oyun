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

#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <TestHarness.h>

#include "../common/error.h"
#include "fsaplayer.h"
#include "game.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(FSAStateArray);


bool FSAPlayer::Load(const Game *game, const wxString &fileName)
{
	// Open the file and read it into an array
	wxTextFile file;
		
	if (!file.Open(fileName))
	{
		Error::Set(wxString::Format(_("Could not open file %s"), fileName.c_str()));
		return false;
	}

	wxArrayString fileLines;
	wxString str;
	
	for (str = file.GetFirstLine() ; !file.Eof() ; str = file.GetNextLine())
		fileLines.Add(str);

	file.Close();
	
	return DoLoad(game, fileLines);
}

bool FSAPlayer::LoadFromString(const Game *game, const wxString &fsaScript)
{
	wxStringTokenizer tokenizer(fsaScript, "\n", wxTOKEN_RET_EMPTY_ALL);
	wxArrayString lines;
	
	while (tokenizer.HasMoreTokens())
		lines.Add(tokenizer.GetNextToken ());

	return DoLoad(game, lines);
}


bool FSAPlayer::DoLoad (const Game *game, const wxArrayString &fsaScript)
{
	// We must have at least four lines, or something's wrong
	if (fsaScript.Count() < 4)
	{
		Error::Set(_("FSM script ended while reading initial information (check your FSA script syntax)"));
		return false;
	}

	// Get the player author and name
	playerAuthor = fsaScript[0];
	playerName = fsaScript[1];
	
	// Strip newlines from those
	playerAuthor = playerAuthor.BeforeFirst(wxT('\r')).BeforeFirst(wxT('\n'));
	playerName = playerName.BeforeFirst(wxT('\r')).BeforeFirst(wxT('\n'));

	// Get the number of actions
	long numActions;
	if (!fsaScript[2].ToLong(&numActions))
	{
		Error::Set(_("FSA script had a number of actions that's not a number"));
		return false;
	}

	// Make sure there's enough lines
	if (fsaScript.Count() < numActions + 3)
	{
		Error::Set(_("FSA script ended before the advertised number of actions"));
		return false;
	}
	
	// Load the finite states
	for (int i = 0 ; i < numActions ; i++)
	{
		wxString sourceLine = fsaScript[i + 3];

		// Add to the player source code
		playerSource += sourceLine;
		playerSource += wxT("\n");
		
		// Tokenize the string
		wxStringTokenizer tokens(sourceLine, wxT(","));
		if (tokens.CountTokens() != 3)
		{
			Error::Set(wxString::Format(_("FSM script, action %i: doesn't have the correct syntax (not enough tokens?)"), i));
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
			Error::Set(wxString::Format(_("FSM script, action %i: first transition value is not a number"), i));
			return false;
		}
		if (!strTrans1.ToLong(&transitions[1]))
		{
			Error::Set(wxString::Format(_("FSM script, action %i: second transition value is not a number"), i));
			return false;
		}
		
		if (transitions[0] >= numActions)
		{
			Error::Set(wxString::Format(_("FSM script, action %i: first transition is out of bounds"), i));
			return false;
		}
		if (transitions[1] >= numActions)
		{
			Error::Set(wxString::Format(_("FSM script, action %i: second transition is out of bounds"), i));
			return false;
		}

		// Check the validity of the move
		int moveidx = game->GetGameMoves().Find(action);
		if (moveidx == -1)
		{
			Error::Set(wxString::Format(_("FSM script, action %i: requested move is not valid for this game"), i));
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

static const wxString test_badmove("Charles Pence\nBad Move\n2\nC, 0, 1\nX, 0, 1");
static const wxString test_badtrans("Charles Pence\nBad Transition\n2\nC, 0, 3\nD, 0, 1");
static const wxString test_nan("Charles Pence\nNot a number\n2\nC, 0, asdf\nD, 0, 1");
static const wxString test_notenough("Charles Pence\nNot enough\n2\nC, 0, 0");
static const wxString test_testc("Charles Pence\nNaive\n1\nC, 0, 0");
static const wxString test_testd("Charles Pence\nNaive\n1\nD, 0, 0");
static const wxString test_tft("Charles Pence\nTit-for-Tat\n2\nC, 0, 1\nD, 0, 1");

// FSA player tests
TEST(FSAPlayer, Loading)
{
	MockGame game;
	FSAPlayer *fsa;
	
	// Load the all-C player
	fsa = new FSAPlayer;
	CHECK(fsa->LoadFromString(&game, test_testc));
	delete fsa;
	
	// Load the all-D player, check some stats
	fsa = new FSAPlayer;
	CHECK(fsa->LoadFromString(&game, test_testd));
	CHECK_EQUAL(1, fsa->GetNumLines());
	delete fsa;
}

TEST(FSAPlayer, BadData)
{
	MockGame game;
	FSAPlayer fsa;

	// Various kinds of bad user data: an invalid move	
	CHECK(!fsa.LoadFromString(&game, test_badmove));
	
	// An invalid transition (step out of bounds)
	CHECK(!fsa.LoadFromString(&game, test_badtrans));
	
	// Not enough states specified
	CHECK(!fsa.LoadFromString(&game, test_notenough));
	
	// A non-numerical transition value
	CHECK(!fsa.LoadFromString(&game, test_nan));
}

TEST(FSAPlayer, SimplePlayers)
{
	FSAPlayer allc;
	FSAPlayer alld;
	MockGame game;
	
	// Load the two simple testing players
	CHECK(allc.LoadFromString(&game, test_testc));
	CHECK(alld.LoadFromString(&game, test_testd));
	
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
	CHECK(tft.LoadFromString(&game, test_tft));
	
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

	CHECK(playerOne.LoadFromString(&game, test_testc));
	
	// Make sure FSAPlayer::Clone works
	playerTwo = playerOne.Clone();
	CHECK_TYPES_EQUAL(playerOne, *playerTwo);

	delete playerTwo;
}

/** \endcond */


