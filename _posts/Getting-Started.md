
To get a tournament going in Logos, you'll need to (i) write a set of players,
(ii) load them up, and (iii) run a tournament of your choice.

Writing Players
===============

Tournaments in Logos can be run with two different sorts of players: built-in
players and external players written as finite state machines.  There are only
two sorts of built-in players available: one player which makes random moves,
and one which plays the tit-for-tat strategy.

If you want to write a player, you'll need to learn the Logos finite state
machine format.  A finite state machine is like a very simple programming
language.  In this format, you tell your player which move to make, and then
how to respond to moves made by its opponent.

Let's move to some more concrete explanation.  First of all, a finite state 
machine is just a text file that looks like the following:

	Machine Author
	Machine Name
	Number of states
	State 0
	State 1
	...
	State N-1 

A few things to note: the machine author and machine name are *not* optional.
Further, which will be important later, the states are numbered *starting with
zero*.  

Each state in the state machine looks like this:

	<move>, <next state if opponent moves C>, <next state if opponent moves D>

So you can now see how play works for a finite state machine.  The player
begins in state 0, making the move specified by that state.  On the next turn,
it looks to see what move its opponent made on the previous turn.  Depending on
this move, it decides which state to switch to.  It then makes the move
specified in that new state, and the process continues.

Here's an example.  A finite state machine version of the tit-for-tat strategy
would look like this:

	Charles Pence
	Tit for Tat
	2
	C, 0, 1
	D, 0, 1

As specified, the player was written by me, and its name is "Tit for Tat."  It
has two states.  Play begins in state 0, and the player chooses to cooperate.
If the opponent cooperates, our player will continue to cooperate (that is, it
will stay in state 0).  If the opponent defects, our player will switch to state
1.  When it gets there, it will defect.  Again, in this state, it will copy the
move of its opponent.

More examples of finite state machine players are available on the wiki at the
Logos website.

Loading Players
===============

Load the players you've written on the first page of Logos.  You can also drag
and drop finite state machine text files onto the Logos window.  If there are
any syntax errors or invalid states in your finite state machine (did you
remember that the first state is state 0 and not state 1?), Logos will warn you
upon loading the file.

Click "Next" to continue.

Running a Tournament
====================

You now need to choose which type of tournament to run.  You can either choose
a round-robin (or "one-shot") tournament, in which each player plays every 
other player for a total five games, and the winner is the player which
accumulates the highest score, or you can choose an evolutionary tournament, in
which the players seed an initial "population," and their scores against one
another are used to determine their percentage in the next "generation."  For
more information on these two tournament types, see the documentation under
"Logos Task Pages" for the "One-Shot Tournament" and the "Evolutionary
Tournament."

Finally, depending on which tournament you have run, there will be several
options for how you can save the results -- for a one-shot tournament, you can
save a spreadsheet of results or a word-processor document detailing the moves
each player made throughout, and for an evolutionary tournament, you can save a
spreadsheet of the contents of the population at each generation, or the graph
of the results in a variety of image formats.

Hopefully this is enough information to get you started using Logos!  For more
information on any of these steps, see the detailed documentation under "Logos 
Task Pages" above.
