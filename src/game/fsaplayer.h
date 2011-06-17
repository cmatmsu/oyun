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

#include "player.h"

#ifndef FSAPLAYER_H__
#define FSAPLAYER_H__


/**
    \class FSAState
    \ingroup game
    
    \brief An individual state object in a finite state machine
    
    This class encapsulates an action as well as transitions to subsequent
    states based on the move taken by an opponent.
    
    \see FSAPlayer
*/
class FSAState
{
public:
	/**
	    \brief Constructor
	    
	    Sets the \c action and \c transitions values.
	    
	    \param newAction Initial value of the \c action member
	    \param newTransitions Initial values of the \c transitions member
	*/
	FSAState(wxChar newAction, const long *newTransitions) :
		action (newAction)
	{
		transitions[0] = newTransitions[0];
		transitions[1] = newTransitions[1];
	}

	/**
	    \brief The action to take at this node
	*/
	wxChar action;

	/**
	    \brief Array of transitions (where to go based on opponent move)
	*/
	long transitions[2];
};

/**
    \typedef FSAStateArray
    \ingroup game
    \brief A variable-length array of FSAState objects
*/
WX_DECLARE_OBJARRAY(FSAState, FSAStateArray);


/**
    \class FSAPlayer
    \ingroup game
    
    \brief A player that plays according to a Finite State Automaton
    
    Players instantiated using this class will make decisions based upon
    a text representation of a finite state machine.  The player's move is
    specified as a sequence of states:
    
\verbatim Machine Author
Machine Name
Number of states (unsigned int)
State 0
State 1
...
State N-1 \endverbatim
    
    The format for a given state is as follows:

\verbatim <move>, <next state if opponent moves C>, <next state if opponent moves D> \endverbatim

    The list of states is zero-indexed (that is, the first state played in a
    given game is state 0, not state 1).

    A simple FSA implementation of the tit-for-tat algorithm would be 
    expressed as follows:
    
\verbatim Charles Pence
TitForTat
2
C, 0, 1
D, 0, 1 \endverbatim
    
    Hopefully this is enough documentation that the machines are reasonably
    understandable.
*/
class FSAPlayer : public Player
{
public:
	/**
	    \brief Constructor
	    
	    The constructor initializes the instruction pointer to an invalid
	    value (which will be reset on machine load).
	*/
	FSAPlayer() :
	    Player(),
	    eip(-1)
	{ }

	/**
	    \brief Copy constructor
	    \param p Player to be copied
	*/	
	FSAPlayer(const FSAPlayer &p) :
	    Player(p),
	    playerName(p.playerName),
	    playerAuthor(p.playerAuthor),
	    playerSource(p.playerSource),
	    machine(p.machine),
	    eip(p.eip)
	{ }
	
	/**
	    \brief Load a finite state machine script
	    
	    Load the source code for this player's finite state machine from the
	    given external file.
	    
	    \param game The game against which to check the player moves
	    \param fileName The script file to be loaded
	    \returns True if the file is successfully loaded, false otherwise
	*/
	bool Load(const Game *game, const wxString &fileName);

	/**
	    \brief Load a finite state machine script from a string

	    Load the source code for this player's finite state machine from the
	    given string.  This is useful primarily for testing.

	    \param game The game against which to check the player moves
	    \param fsaScript The finite state script to be loaded
	    \returns True if the file is successfully loaded, false otherwise
	*/
	bool LoadFromString(const Game *game, const wxString &fsaScript);

	
	virtual Player *Clone() const
	{ return new FSAPlayer(*this); }

	virtual bool Think(const Game *gamePlayed, const Player *nextOpponent);
	
	/**
	    \brief Reset the internal FSAPlayer state
	    
	    The FSAPlayer must override the Reset method in order to clear
	    the \c eip variable.
	*/
	virtual void Reset()
	{
		eip = 0;
		Player::Reset();
	}

	virtual const wxString &GetPlayerName() const { return playerName; }
	virtual const wxString &GetPlayerAuthor() const { return playerAuthor; }

	/**
	    \brief Get the source code for this player
	    \returns Source code for player
	*/
	const wxString &GetSource() { return playerSource; }
	
	/**
	    \brief Get the number of machine states
	    \returns Number of machine states
	*/
	int GetNumLines() { return machine.size(); }

private:
	/**
	    \brief Internal function which actually loads the FSA script

	    This is the generalized loading code, called by both Load()
	    and LoadFromString().

		\param game The game against which to check the player moves
	    \param fsaScript The script file to be loaded, an array of lines
	    \returns True if the file is successfully loaded, false otherwise
	*/
	bool DoLoad (const Game *game, const wxArrayString &fsaScript);
	
	
	/**
	    \brief Name of this player
	*/
	wxString playerName;

	/**
	    \brief Author of this player
	*/
	wxString playerAuthor;

	/**
	    \brief Source code for this player
	*/
	wxString playerSource;


	/**
	    \brief List of machine states
	*/
	FSAStateArray machine;
	
	/**
	    \brief The current state being executed in the finite state
	           machine
	*/
	unsigned int eip;
};


#endif

// Local Variables:
// mode: c++
// End:
