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

#include <wx/wxprec.h>
#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#  include <wx/wx.h>
#endif

#include <wx/wizard.h>
#include <wx/config.h>

#include "../game/prisoner.h"

#include "oyunwizard.h"

#include "playerspage.h"
#include "typepage.h"
#include "oneshotpage.h"
#include "oneshotfinishpage.h"
#include "evopage.h"
#include "evofinishpage.h"


#if !defined(__WXMSW__) && !defined(__WXPM__)
  #include "../../build/oyun.xpm"
#endif


IMPLEMENT_DYNAMIC_CLASS(OyunWizard, wxWizard);

DEFINE_EVENT_TYPE(wxEVT_DATA_UPDATE)
DEFINE_EVENT_TYPE(wxEVT_ADD_PLAYER)
DEFINE_EVENT_TYPE(wxEVT_REMOVE_PLAYER)

BEGIN_EVENT_TABLE(OyunWizard, wxWizard)
	EVT_WIZARD_CANCEL(wxID_ANY, OyunWizard::OnFinished)
	EVT_WIZARD_FINISHED(wxID_ANY, OyunWizard::OnFinished)
END_EVENT_TABLE()


OyunWizard::OyunWizard()
{
}

OyunWizard::OyunWizard(wxWindow *parent)
{
	Create(parent);
}

bool OyunWizard::Create(wxWindow *parent)
{
	// Create the game objects
	game = new PrisonerDilemma;
	
	// Get some configuration values
	wxConfigBase *config = wxConfig::Get();
	long xPos = config->Read(wxT("/LastRun/WindowX"), -1);
	long yPos = config->Read(wxT("/LastRun/WindowY"), -1);
	long xWidth = config->Read(wxT("/LastRun/WindowWidth"), -1);
	long yWidth = config->Read(wxT("/LastRun/WindowHeight"), -1);
	
	// Figure out our position and size
	wxPoint windowPos(xPos, yPos);
	wxSize windowSize(xWidth, yWidth);
	
	// Call base-class create
	int style = wxDEFAULT_DIALOG_STYLE | wxDIALOG_NO_PARENT | wxRESIZE_BORDER | \
	            wxMAXIMIZE_BOX | wxMINIMIZE_BOX | wxTAB_TRAVERSAL | wxFULL_REPAINT_ON_RESIZE;
	
	SetExtraStyle(wxWIZARD_EX_HELPBUTTON);
	bool ret = wxWizard::Create(parent, wxID_ANY, wxT("Oyun"), wxNullBitmap, windowPos, style);
	SetDropTarget(new OyunWizardDropTarget(this));
	
	SetIcon(wxICON(oyunicon));
	
	// Create the wizard pages
	pagePlayers = new PlayersPage(this);
	pageType = new TypePage(this);
	pageOneShot = new OneShotPage(this);
	pageEvo = new EvoPage(this);

	// The finish pages need construct-time access to the previous page
	pageOneShotFinish = new OneShotFinishPage(this, pageOneShot);
	pageEvoFinish = new EvoFinishPage(this, pageEvo);
	
	// Link the pages
	wxWizardPageSimple::Chain(pagePlayers, pageType);
	TypePage::Chain(pageType, pageOneShot, pageEvo);
	wxWizardPageSimple::Chain(pageOneShot, pageOneShotFinish);
	wxWizardPageSimple::Chain(pageEvo, pageEvoFinish);

	// Add the pages to the sizer.  Note that the documentation says that
	// adding all pages isn't strictly necessary, but it appears to be
	// lying, at least as of wxWidgets 2.8 on GTK.
	GetPageAreaSizer()->Add(pagePlayers);
	GetPageAreaSizer()->Add(pageType);
	GetPageAreaSizer()->Add(pageOneShot);
	GetPageAreaSizer()->Add(pageOneShotFinish);
	GetPageAreaSizer()->Add(pageEvo);
	GetPageAreaSizer()->Add(pageEvoFinish);
	
	// Set the size hints from the main page area sizer, which now contains
	// all the subpages, their sizers, controls, etc.
	GetPageAreaSizer()->SetSizeHints(this);
	
	// Show the first page
	ShowPage(pagePlayers);

	// Finally, set the size to the requested if it's fully specified
	if (windowSize.IsFullySpecified())
		SetClientSize(windowSize);	
	
	return ret;
}

OyunWizard::~OyunWizard()
{
	delete game;
}


void OyunWizard::OnFinished(wxWizardEvent & WXUNUSED(event))
{
	// Save our data out to the configuration file
	wxConfigBase *config = wxConfig::Get();
	
	wxSize size = GetClientSize();
	wxPoint pos = GetPosition();
	
	config->Write(wxT("/LastRun/WindowX"), pos.x);
	config->Write(wxT("/LastRun/WindowY"), pos.y);
	config->Write(wxT("/LastRun/WindowWidth"), size.GetWidth());
	config->Write(wxT("/LastRun/WindowHeight"), size.GetHeight());
	
	// And destroy the wizard
	Destroy();
}

bool OyunWizard::OnFileDrop(const wxArrayString &files)
{
	// Check the inputs here
	if (!files.GetCount())
		return false;
	
	// We're not doing anything if the playersPage isn't showing
	if (GetCurrentPage() != pagePlayers)
		return false;
	
	// At this point, the players page is the right place for the rest
	// of this loading logic
	return pagePlayers->OnFileDrop(files);
}



//
// Custom message functions
//

void OyunWizard::Update()
{
	wxNotifyEvent event(wxEVT_DATA_UPDATE, wxID_ANY);
	event.SetEventObject(this);
	
	// Send to all pages
	wxPostEvent(pagePlayers, event);
	wxPostEvent(pageType, event);
	wxPostEvent(pageOneShot, event);
	wxPostEvent(pageOneShotFinish, event);
	wxPostEvent(pageEvo, event);
	wxPostEvent(pageEvoFinish, event);
}

void OyunWizard::AddPlayer(Player *player)
{
	wxNotifyEvent event(wxEVT_ADD_PLAYER, wxID_ANY);
	event.SetEventObject(this);
	event.SetClientData(player);
	
	pagePlayers->GetEventHandler()->ProcessEvent(event);
	pageType->GetEventHandler()->ProcessEvent(event);
	pageOneShot->GetEventHandler()->ProcessEvent(event);
	pageOneShotFinish->GetEventHandler()->ProcessEvent(event);
	pageEvo->GetEventHandler()->ProcessEvent(event);
	pageEvoFinish->GetEventHandler()->ProcessEvent(event);
}

void OyunWizard::RemovePlayer(Player *player)
{
	wxNotifyEvent event(wxEVT_REMOVE_PLAYER, wxID_ANY);
	event.SetEventObject(this);
	event.SetClientData(player);
	
	pagePlayers->GetEventHandler()->ProcessEvent(event);
	pageType->GetEventHandler()->ProcessEvent(event);
	pageOneShot->GetEventHandler()->ProcessEvent(event);
	pageOneShotFinish->GetEventHandler()->ProcessEvent(event);
	pageEvo->GetEventHandler()->ProcessEvent(event);
	pageEvoFinish->GetEventHandler()->ProcessEvent(event);
}


