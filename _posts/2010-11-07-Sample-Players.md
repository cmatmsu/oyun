---
layout: post
title: Sample Players
---

The FSA (finite state automaton) player format is a bit confusing, and a few examples will help you understand how it works.  We begin with the simple "tit-for-tat" strategy -- duplicate the other player's move from the last turn.

    Example Player
    Tit-for-Tat
    2
    C, 0, 1
    D, 0, 1

As you see, the first action is "cooperate," and then we transition again to the first action ("cooperate") if the opponent cooperates, or to the second action, "defect," if the opponent defects.  Pretty simple.

    Example Player
    Tit for Two Tats
    3
    C, 0, 1
    C, 1, 2
    D, 0, 2

Here we have a slightly more complicated player, TF2T, or tit-for-tat with forgiveness.  The player begins by cooperating (C in action 0), and after one defection will proceed to action 1.  Here it will continue cooperating, but after *two* defections, it will defect.

Let's close with some very complicated examples.  This is a combination of a "host" and a "parasite" player, which begin play with a "secret handshake" that, if validated, switches them into an all-cooperate (host) and all-defect (parasite) strategy, thus earning the host maximum points at the expense of the parasite.  First, the code for each:

    Charles Pence
    Host
    12
    C, 1, 11
    C, 2, 11
    C, 3, 11
    D, 7, 4
    C, 5, 6
    C, 5, 5
    C, 6, 6
    C, 8, 8
    C, 9, 9
    C, 10, 10
    C, 10, 11
    D, 10, 11

- - -

    Charles Pence
    Parasite
    12
    C, 1, 11
    C, 2, 11
    C, 3, 11
    D, 7, 4
    D, 5, 6
    D, 5, 5
    C, 6, 6
    C, 8, 8
    C, 9, 9
    C, 10, 10
    C, 10, 11
    D, 10, 11

And now, let's see that same code with some annotations to show you how it works:

    Charles Pence
    Host
    
    # Lead in to handshake -- if this fails, fall back to TfT
    0       C, 1, 11
    1       C, 2, 11
    2       C, 3, 11
    
    # The handshake itself, if it fails, make up and then play TfT
    3       D, 7, 4
    
    # Handshake was successful, this is the identification turn
    # D means we're a parasite, C means we're a host
    4       C, 5, 6
    
    # We're both hosts, cooperation loop
    5       C, 5, 5
    
    # He's a parasite and I'm a host, take advantage loop
    6       C, 6, 6
    
    # Three cooperates -- the apology
    7       C, 8, 8
    8       C, 9, 9
    9       C, 10, 10
    
    # Standard TfT
    10      C, 10, 11
    11      D, 10, 11

- - -

    Charles Pence
    Parasite
    
    # Lead in to handshake -- if this fails, fall back to TfT
    0       C, 1, 11
    1       C, 2, 11
    2       C, 3, 11
    
    # The handshake itself, if it fails, make up and then play TfT
    3       D, 7, 4
    
    # Handshake was successful, this is the identification turn
    # D means we're a parasite, C means we're a host
    4       D, 5, 6
    
    # I'm a parasite and he's a host--take advantage loop
    5       D, 5, 5
    
    # Two parasites--cooperate loop
    6       C, 6, 6
    
    # Three cooperates -- the apology
    7       C, 8, 8
    8       C, 9, 9
    9       C, 10, 10
    
    # Standard TfT
    10      C, 10, 11
    11      D, 10, 11

As you can see, the FSA format is amenable to the production of some pretty complicated strategies!  Good luck, and have fun.