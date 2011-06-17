---
layout: post
title: Player Selection Page
---

  The player-selection page:
 --------------------------------------------------------------
  ![Screenshot](images/playerspage.png){:width="575"}


On this page, you can add all the players you wish to compete in your
tournament.  Two types of players can be added: built-in players and finite 
state machines.  There is no limit (other than your computer's processing
power) to how many players may be added to the tournament, and any individual
player or type of player may be added as many times as you wish.

Built-In Players
================

Currently there are two types of built-in players: the random player, which
makes a random move each turn, and the tit-for-tat player, which implements the
tit-for-tat strategy, copying whatever move its opponent made on the last turn.

To add a built-in player, click the "Add Built-In Player..." button, and select
the built-in player you want from the list.  Click OK to add the player, or
Cancel to return to the player selection screen.

Finite State Machines
=====================

Finite state machines are a file format with which you can design custom players
for use in Oyun.  For a detailed description of their file format and how you
can write one, visit the Getting Started section of the help, where a tutorial
is presented.

To add a finite state machine, click the "Add Finite State Machine..." button,
and select a finite state machine file, which is simply a plain-text file with
extension ".txt".  Click OK to add the player.  If there is an error in the
player's syntax (for example, if it doesn't have as many states as listed, or 
it attempts to make an invalid move or transition to an invalid state), Oyun 
will display an error message and prevent you from adding that player to the
tournament.

Also, you can drag and drop finite state machine text files onto the player 
list, and Oyun will add them automatically.

The Player List
===============

The player list shows you all the players that have been added to the
tournament so far.  To remove a player, select it and press "Remove Player".
Alternately, you can right-click on the player you would like do delete and
select "Remove Player" from the pop-up menu.

If you have added any finite state machine players, you may check out their
source code by right-clicking their entry in the player list and selecting
"View Player Source...".

- - -

When you are done adding players, click the "Next" button to advance to the next
screen.

