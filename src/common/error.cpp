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

#include <wx/wxprec.h>
#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

#include "error.h"

namespace Error
{

static wxArrayString errorStack;

void Set(const wxString &str)
{
	// Add this to the beginning of the string array
	errorStack.Insert(str, 0);
}

const wxString Get(void)
{
	// If no errors have been reported, let the calling function know
	if (!errorStack.GetCount())
		return wxString(_("No error"));
	
	// Pop the top of the stack
	wxString ret = errorStack[0];
	errorStack.RemoveAt(0);
	
	return ret;
}

};


