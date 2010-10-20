/*
    Copyright (C) 2004-2010 by Charles Pence
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

#ifndef TOURNEY_MATCH_H__
#define TOURNEY_MATCH_H__

class Game;
class Player;


/**
    \class Match
    \ingroup tourney
    
    \brief Defines a match, a sequence of games between two players
    
    This class will run a match, a series of five games.  The scores are
    accumulated, and the results can be queried for later display.
*/
class Match
{
public:
	/**
	    \brief Constructor
	    
	    Sets the \c playerOne and \c playerTwo values.
	    
	    \param one Initial value of the \c playerOne member
	    \param two Initial value of the \c playerTwo member
	*/
	Match(Player *one, Player *two) : playerOne(one), playerTwo(two),
	                                  playerOneScore(0), playerTwoScore(0)
	{ }

	/**
	    \brief Play a match between two players
	    
	    This plays a match of the given game file between two players,
	    storing the players and the result in the match structure.  The number of
	    games in the match is predetermined according to previously run trials 
	    using Axelrod's random exit coefficient (0.00346).  The match lengths are 
	    predetermined in order to make the score values for each match
	    deterministic.
	    
	    The \p quick parameter allows for a one-game match to be played, which
	    is useful in environments where match speed is critical (like the
	    evolutionary tournament).
	    
	    \param game The game to be played
	    \param quick If true, only one game will be played (rather than five)
	*/
	bool Play(Game *game, bool quick = false);

	/**
	    \brief The first game player
	*/
	Player *playerOne;

	/**
	    \brief The second game player
	*/
	Player *playerTwo;

	/**
	    \brief First player's match score
	*/
	int playerOneScore;

	/**
	    \brief Second player's match score
	*/
	int playerTwoScore;
	
	
	/**
	    \brief The entire list of moves for this match
	*/
	wxArrayString matchHistory[5];
};


/**
    \typedef MatchPtrArray
    \ingroup tourney
    \brief A variable-length array of matches
*/
WX_DEFINE_ARRAY_PTR(Match *, MatchPtrArray);
	

#endif

// Local Variables:
// mode: c++
// End:
