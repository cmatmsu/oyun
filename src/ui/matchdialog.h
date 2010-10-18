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

#ifndef MATCHDIALOG_H__
#define MATCHDIALOG_H__

class Match;


/**
    \class MatchDialog
    \ingroup ui
    
    \brief Dialog box which shows detailed information about a match
*/
class MatchDialog : public wxDialog
{
	DECLARE_CLASS(MatchDialog)
	
public:
	/**
	    \brief Constructor
	    
	    Constructs the dialog and fills its controls with information
	    about the match
	    
	    \param parent The parent of this dialog box
	    \param match The match to be displayed
	*/
	MatchDialog(wxWindow *parent, Match *match);
	
private:
	wxBoxSizer *sizer;		/**< \brief The sizer for the dialog controls */
	
	wxStaticText *header;		/**< \brief The header text */
	
	wxStaticText *labels[5];	/**< \brief The labels for the text controls */
	wxTextCtrl *games[5];		/**< \brief The five text controls for the games */
	wxButton *ok;			/**< \brief The OK button */
};


#endif


