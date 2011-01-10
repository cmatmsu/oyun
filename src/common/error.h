/*
    Copyright (C) 2004-2011 by Charles Pence
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

#ifndef ERROR_H__
#define ERROR_H__


/**
    \namespace Error
    \brief Namespace containing error handling code
*/
namespace Error
{

/**
    \brief Set current error string
    \ingroup common

    \param str Current error string, override old error string
*/
void Set(const wxString &str);


/**
    \brief Get current error string
    \ingroup common

    \returns Current error string
*/
const wxString Get(void);

};

#endif

// Local Variables:
// mode: c++
// End:
