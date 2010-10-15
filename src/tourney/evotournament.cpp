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

#include <wx/progdlg.h>

#include "evotournament.h"
#include "match.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(GenerationWeightArray)


EvoTournament::EvoTournament(Game *gm) : played(false), game(gm)
{ }

EvoTournament::~EvoTournament()
{
	for (size_t i = 0 ; i < players.GetCount() ; i++)
		delete players[i];
	players.Clear();
}


void EvoTournament::AddPlayer(const Player *player)
{
	players.Add(player->Clone());
}

void EvoTournament::RemovePlayer(const Player *player)
{
	for (size_t i = 0 ; i < players.GetCount() ; i++)
	{
		Player *t = players[i];
		
		if (player->GetID() == t->GetID())
		{
			players.RemoveAt(i);
			return;
		}
	}
}


bool EvoTournament::Run(int numGenerations)
{
	// If we've already played, reset
	if (played)
		Reset();

        // Create a progress dialog
        wxProgressDialog *progress = new wxProgressDialog(_("Running tournament..."),
                                                          _("Running tournament:"),
                                                          numGenerations + 1);
        progress->Update(1);

	// Create some variables we'll need later: the number of players,
	// a temporary weight object that will get pushed back onto data,
	// and the same indexed by integer not name.
	size_t numPlayers = players.GetCount();
	GenerationWeights weights;
	double *intWeights = new double[numPlayers];
	
	// Seed the population weights
	for (size_t i = 0 ; i < numPlayers ; i++)
	{
		intWeights[i] = 1.0 / (double)numPlayers;
		weights[players[i]->GetID()] = intWeights[i];
	}
	data.push_back(weights);

	// Run it!
	for (int gen = 0 ; gen < numGenerations ; gen++)
	{
		// So, let's leverage the fact that games are deterministic.  A player's
		// score is:
		//
		// Score vs. himself * chance he'll meet himself
		// Score vs. A * chance he'll met A
		// etc.
		//
		// So calculate those weights and use that method to accurately arrive at
		// the evolutionary solution--WITHOUT introducing any roundoff bugs!
		double *newIntWeights = new double[numPlayers];

		for (size_t i = 0 ; i < numPlayers ; i++)
		{
			double roundScore = 0; 

			for (size_t j = 0 ; j < numPlayers ; j++)
			{
				// What's the odds that players i and j will meet?
				double odds = intWeights[i] * intWeights[j];

				// What's the score for this game?
				Player *p1Copy = players[i]->Clone();
				Player *p2Copy = players[j]->Clone();
				Match match(p1Copy, p2Copy);
				
				// Error already set in Match::Play()
				if (!match.Play(game, true))
				{
					progress->Destroy();
					return false;
				}

				// What's that mean for our intrepid warrior?
				double floatScore = odds * match.playerOneScore;

				// Clean up
				delete p1Copy;
				delete p2Copy;

				// Add this on
				roundScore += floatScore;
			}

			// We've looped through every player, so this is our guy's score for this round
			newIntWeights[i] = roundScore;
		}

		// Turn intWeights back into weights
		double sum = 0;
		for (size_t i = 0 ; i < numPlayers ; i++)
			sum += newIntWeights[i];
		for (size_t i = 0 ; i < numPlayers ; i++)
		{
			intWeights[i] = newIntWeights[i] / sum;
			weights[players[i]->GetID()] = intWeights[i];
		}

		// Add it to the data
		delete[] newIntWeights;
		data.push_back(weights);

                progress->Update(gen+1);
	}

	delete[] intWeights;
        progress->Destroy();

	// Set the played variable
	played = true;

	return true;
}

void EvoTournament::Reset()
{
	played = false;
	data.Clear();
}

