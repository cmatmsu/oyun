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

/**
    \defgroup common Common Module

    This module consists of functions used throughout the source base.
*/

#ifndef RNG_H__
#define RNG_H__


/**
    \namespace Random
    \brief Namespace containing our random number generator
*/
namespace Random
{

/**
    \brief Seed the random number generator
    \ingroup common

    \param s 32-bit seed for the RNG
*/
void Seed(unsigned long s);


/**
    \brief Generate a random integer
    \ingroup common

    \returns A 32-bit random value
*/
unsigned long Generate(void);


/*
    \brief Generate a signed random integer
    \ingroup common

    \returns A 32-bit signed random value
*/
long GenerateSigned(void);


/**
    \brief Generate a random floating-point value
    \ingroup common

    \returns A random double between -1.0 and 1.0
*/
double GenerateFloat(void);


/**
    \brief Generate a random floating-point value with extra entropy
    \ingroup common

    \returns A random double between -1.0 and 1.0
*/
double GenerateFloatHigh(void);

};

#endif

// Local Variables:
// mode: c++
// End:
