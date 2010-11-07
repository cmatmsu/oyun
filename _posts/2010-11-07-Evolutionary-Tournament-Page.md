---
layout: post
title: Evolutionary Tournament Page
---

  The evolutionary tournament page:
 --------------------------------------------------------------
  ![Screenshot](images/evopage.png)


On this page, you can run an Evolutionary Tournament, in which a
virtual "population" is created and evolved over time.

The Evolutionary Tournament Algorithm
=====================================

We begin by creating an initial population containing equal fractions
of each type of player.  Then, we take advantage of the fact that
games are (excepting the built-in random player, which is ignored)
deterministic.  Thus, a given player's score can be calculated as:

    for each possible opponent O (including itself)
        score += (score versus O) * (chance that player will meet O)
    end for

Then, we compute the "total score" that has been scored this round --
adding up the scores achieved by every player.  Finally, an
individual's fractional representation in the next generation is equal
to the fraction of this total score which it accumulated.

Running a Tournament
====================

Begin by setting the number of generations for which the tournament
will run.  The default value is 200, which is usually more than enough
for a given population to stabilize.  Press the "Run Tournament"
button, and the results will be graphed.  When finished, you may press
the "Show Legend..." button to pop up a window showing which graph
colors correspond to which players.

- - -

After you have run the tournament, click the "Next" button to advance
to the next screen.

