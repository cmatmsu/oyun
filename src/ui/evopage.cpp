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

#include <wx/wizard.h>

#include "../common/error.h"
#include "../tourney/evotournament.h"
#include "../game/player.h"

#include "tools/htmldialog.h"
#include "logosapp.h"
#include "logoswizard.h"
#include "evopage.h"


IMPLEMENT_CLASS(EvoPage, LogosWizardPage)

enum
{
	ID_RUN_TOURNAMENT = wxID_HIGHEST,
	ID_SHOW_LEGEND,
	ID_GEN_SPINNER,
};

BEGIN_EVENT_TABLE(EvoPage, LogosWizardPage)
	EVT_WIZARD_PAGE_CHANGING(wxID_ANY, EvoPage::OnPageChanging)
	EVT_WIZARD_HELP(wxID_ANY, EvoPage::OnHelp)
	
	EVT_SPINCTRL(ID_GEN_SPINNER, EvoPage::OnSpinner)
	EVT_TEXT(ID_GEN_SPINNER, EvoPage::OnSpinnerText)
	
	EVT_BUTTON(ID_RUN_TOURNAMENT, EvoPage::OnRunTournament)
	EVT_BUTTON(ID_SHOW_LEGEND, EvoPage::OnShowLegend)
	
	EVT_NOTIFY(wxEVT_DATA_UPDATE, wxID_ANY, EvoPage::OnDataUpdate)
	EVT_NOTIFY(wxEVT_ADD_PLAYER, wxID_ANY, EvoPage::OnAddPlayer)
	EVT_NOTIFY(wxEVT_REMOVE_PLAYER, wxID_ANY, EvoPage::OnRemovePlayer)
END_EVENT_TABLE()


EvoPage::EvoPage(LogosWizard *parent, wxWizardPage *prev, wxWizardPage *next) :
	LogosWizardPage(_("Evolutionary Tournament"),
                        _("This tournament uses scores as weights for future generations in a population."),
                        parent, prev, next),
	renderer(wxSize(800, 800)) // FIXME: configure this?
{
	evoTourney = new EvoTournament(parent->game);	
	evoTourney->Reset();
	
	// Create the controls in the appropriate tab order
	runTournament = new wxButton(this, ID_RUN_TOURNAMENT, _("&Run Tournament"));

	legend = new wxButton(this, ID_SHOW_LEGEND, _("Show &Legend..."));
	legend->Disable();

	spinnerLabel = new wxStaticText(this, wxID_ANY, _("Number of &Generations:"));

	genSpinner = new wxSpinCtrl(this, ID_GEN_SPINNER, wxT("200"), wxDefaultPosition, wxDefaultSize,
	                            wxSP_ARROW_KEYS, 10, 10000, 200);

	// The spinner seems to come out small unless we call this
	genSpinner->SetInitialSize(genSpinner->GetBestSize());
			
	graphWindow = new EvoGraphWindow(this);

	// Make a sizer and add the controls to it
	wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
	
	topSizer->Add(runTournament, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 12);
	topSizer->Add(legend, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 12);
	topSizer->AddStretchSpacer();
	topSizer->Add(spinnerLabel, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 12);
	topSizer->Add(genSpinner, 0, wxTOP | wxBOTTOM | wxRIGHT | wxALIGN_CENTER_VERTICAL, 12);
	
	sizer->Add(topSizer, 0, wxEXPAND);
	
	sizer->Add(graphWindow, 1, wxEXPAND | wxALL, 12);
	
	SetSizerAndFit(sizer);
}

EvoPage::~EvoPage()
{
	delete evoTourney;
}


void EvoPage::OnHelp(wxWizardEvent & WXUNUSED(event))
{
	wxGetApp().ShowHelp(this, wxT("evopage.html"));
}

void EvoPage::OnRunTournament(wxCommandEvent & WXUNUSED(event))
{
	wxBusyCursor busy;
	int generations = genSpinner->GetValue();
	
	if (!evoTourney->Run(generations))
	{
		wxString errStr(wxString::Format(_("Could not run tournament.  Error reported:\n\n%s"), Error::Get().c_str()));
		wxMessageBox(errStr, _("Logos: Error"), wxOK | wxICON_ERROR, this);
		return;
	}
	
	// This qualifies as a data update
	parent->Update();
}


void EvoPage::OnDataUpdate(wxNotifyEvent & WXUNUSED(event))
{
	// Invalidate the graph window, which will repaint it on the next
	// event loop
	graphWindow->Refresh();

	// Update control states if we're visible
	if (IsShownOnScreen())
	{
		if (evoTourney->IsPlayed())
		{
			// If the tournament has been played, then default button and
			// focus should snap to the 'Next' button
			wxWindow *win = wxWindow::FindWindowById(wxID_FORWARD, GetParent());
			wxButton *nextButton = wxDynamicCast(win, wxButton);
			
			if (nextButton)
			{
				nextButton->SetDefault();
				nextButton->SetFocus();
			}
			
			// Enable the legend button
			legend->Enable();
		}
		else
		{
			// Otherwise, snap focus and default to the 'Run' button
			runTournament->SetDefault();
			runTournament->SetFocus();
			
			// Disable the legend button
			legend->Disable();
		}
	}
}

void EvoPage::OnAddPlayer(wxNotifyEvent &event)
{
	// Send this to the tournamnent
	evoTourney->AddPlayer((Player *)event.GetClientData());
}

void EvoPage::OnRemovePlayer(wxNotifyEvent &event)
{
	// Send this to the tournamnent
	evoTourney->RemovePlayer((Player *)event.GetClientData());
}


void EvoPage::OnSpinner(wxSpinEvent & WXUNUSED(event))
{
	evoTourney->Reset();
	graphWindow->Refresh();
}

void EvoPage::OnSpinnerText(wxCommandEvent & WXUNUSED(event))
{
	evoTourney->Reset();
	graphWindow->Refresh();
}



void EvoPage::OnShowLegend(wxCommandEvent & WXUNUSED(event))
{
	if (!evoTourney->IsPlayed())
	{
		wxMessageBox(_("You must play the tournament first!"), _("Logos: Error"),
		             wxOK | wxICON_ERROR, this);
		return;
	}
	
	wxString graphColorNames[] = 
	{ _("Black"), _("Red"), _("Yellow"), _("Green"), _("Cyan"), 
	  _("Pink"), _("Tan"), _("Grey"), _("Dark Red"),  _("Dark Blue"), 
	  _("Purple"), _("Light Cyan"), _("Light Blue"), _("Orchid"), 
	  _("Mauve") };
	
	wxString html = wxT("<html><body><font size=\"-1\"><p>");
	for (size_t i = 0 ; i < evoTourney->players.GetCount() ; i++)
	{
		html.Append(graphColorNames[i % 15]);
		html.Append(wxT(": "));
		html.Append(evoTourney->players[i]->GetPlayerName());
		html.Append(wxT(" ("));
		html.Append(evoTourney->players[i]->GetPlayerAuthor());
		html.Append(wxT(")<br>"));
	}
	
	html.Append(wxT("</font></p></body></html>"));
	
	HtmlDialog *dialog = new HtmlDialog(this, wxID_ANY, _("Graph Legend"));
	dialog->SetHtml(html);
	dialog->ShowModal();
	dialog->Destroy();
}



void EvoPage::OnPageChanging(wxWizardEvent &event)
{
	// We only care if we're moving forward
	if (!event.GetDirection())
	{
		event.Skip();
		return;
	}
	
	// Don't allow them to go forward if we haven't run the tournament
	if (!evoTourney->IsPlayed())
	{
		wxMessageBox(_("You must run the tournament to continue."),
		             _("Logos: Error"), wxOK | wxICON_ERROR, this);
		event.Veto();
		return;
	}
	
	// Turn the image into a bitmap here, because this is a painfully
	// slow operation.  Also, set very high quality for JPGs.
	imageGraph = bitmap.ConvertToImage();
	imageGraph.SetOption(wxT("quality"), 100);
	
	// We don't need to hold this event up
	event.Skip();
}


const struct EvoGraphRenderer::SVGColor EvoGraphRenderer::colors[15] =
{
	{ wxT("Black"), 0, 0, 0},
	{ wxT("Red"), 255, 0, 0},
	{ wxT("Yellow"), 255, 255, 0},
	{ wxT("Green"), 0, 128, 0},
	{ wxT("Aquamarine"), 127, 255, 212},
	{ wxT("Pink"), 255, 192, 203},
	{ wxT("Wheat"), 245, 222, 179},
	{ wxT("Gray"), 128, 128, 128},
	{ wxT("SaddleBrown"), 139, 69, 19},
	{ wxT("Blue"), 0, 0, 255},
	{ wxT("BlueViolet"), 138, 43, 226},
	{ wxT("Cyan"), 0, 255, 255},
	{ wxT("Turquoise"), 64, 224, 208},
	{ wxT("Magenta"), 255, 0, 255},
	{ wxT("Salmon"), 250, 128, 114}
};

EvoGraphRenderer::EvoGraphRenderer(const wxSize &imageSize) : dcSize(imageSize)
{
	renderer = wxGraphicsRenderer::GetDefaultRenderer();
	
	bitmap.Create(dcSize.GetWidth(), dcSize.GetHeight());
}

void EvoGraphRenderer::StartDrawing(wxWindowDC &windowDC, const wxSize &windowSize)
{
	gc = renderer->CreateContext(windowDC);
	gcSize = windowSize;
	dc.SelectObject(bitmap);
	
	svg.Clear();
	
	static const wxChar *svgHeader =
		wxT("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\"\n")
		wxT(" \"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n")
		wxT("<svg xmlns=\"http://www.w3.org/2000/svg\"\n")
		wxT(" xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n")
		wxT(" width='%dpx' height='%dpx'>\n\n")
		wxT("<title>Logos Tournament Graph</title>\n\n");
	
	wxString header;
	header.Printf(svgHeader, dcSize.GetWidth(), dcSize.GetHeight());
	
	svg = header;
}

void EvoGraphRenderer::FinishDrawing(wxBitmap &bitmapOut, wxString &svgOut)
{
	// This is a robust copy, which defeats the wxWidgets reference counting
	dc.SelectObject(wxNullBitmap);
	bitmapOut = bitmap.GetSubBitmap(wxRect(0, 0, dcSize.GetWidth(), dcSize.GetHeight()));
	
	// Copy the string out for the SVG
	svg += wxT("\n\n</svg>\n\n");
	svgOut = svg;
}


void EvoGraphRenderer::SetPen(const wxPen &pen)
{
	gc->SetPen(pen);
	dc.SetPen(pen);
	
	wxColor c(pen.GetColour());
	curR = c.Red();
	curG = c.Green();
	curB = c.Blue();
	curWidth = pen.GetWidth();
}

void EvoGraphRenderer::SetBrush(const wxBrush &brush)
{
	gc->SetBrush(brush);
	dc.SetBrush(brush);
	
	// Assume we're setting a brush because we're about to fill
	// something, and save out the color for the SVG file
	wxColor c = brush.GetColour();
	curR = c.Red();
	curG = c.Green();
	curB = c.Blue();
}

void EvoGraphRenderer::SetGraphColor(int col)
{
	// Make sure we have a safe index
	col = col % 15;
	
	SetPen(wxPen(wxColour(colors[col].r, colors[col].g, colors[col].b), 2));
}

void EvoGraphRenderer::SetFont(const wxFont &font)
{
	gc->SetFont(font, *wxBLACK);
	dc.SetFont(font);
	
	curFont = font.GetFaceName();
}

void EvoGraphRenderer::DrawLine(float x1, float y1, float x2, float y2)
{
	gc->StrokeLine(x1 * (float)gcSize.GetWidth(), y1 * (float)gcSize.GetHeight(),
	               x2 * (float)gcSize.GetWidth(), y2 * (float)gcSize.GetHeight());
	
	dc.DrawLine(x1 * dcSize.GetWidth(), y1 * dcSize.GetHeight(),
            x2 * dcSize.GetWidth(), y2 * dcSize.GetHeight());
	
	wxString line;
	line.Printf(wxT("<polyline points='%d %d, %d %d' ")
	            wxT("stroke-width='%d' stroke='rgb(%d, %d, %d)' style='fill: none;' />\n"),
	            (int)(x1 * dcSize.GetWidth()), (int)(y1 * dcSize.GetHeight()),
	            (int)(x2 * dcSize.GetWidth()), (int)(y2 * dcSize.GetHeight()),
	            curWidth, curR, curG, curB);
	svg += line;
}

void EvoGraphRenderer::DrawPolyLine(size_t np, const float *x, const float *y)
{
	wxString polyline;
	polyline.Printf(wxT("<polyline stroke-width='%d' stroke='rgb(%d, %d, %d)' style='fill: none;' points='%d %d"),
	                curWidth, curR, curG, curB,
	                (int)(x[0] * dcSize.GetWidth()), (int)(y[0] * dcSize.GetHeight()));
	
	for (size_t i = 1 ; i < np ; i++)
	{
		gc->StrokeLine(x[i-1] * (float)gcSize.GetWidth(), y[i-1] * (float)gcSize.GetHeight(),
		               x[i] * (float)gcSize.GetWidth(), y[i] * (float)gcSize.GetHeight());
	
		dc.DrawLine(x[i-1] * dcSize.GetWidth(), y[i-1] * dcSize.GetHeight(),
	            x[i] * dcSize.GetWidth(), y[i] * dcSize.GetHeight());

		wxString point(wxString::Format(wxT(", %d %d"), 
		               (int)(x[i] * dcSize.GetWidth()), (int)(y[i] * dcSize.GetHeight())));
		polyline += point;
	}
	
	polyline += wxT("' />\n");
	svg += polyline;
}

void EvoGraphRenderer::DrawRectangle(float x, float y, float w, float h)
{
	gc->DrawRectangle(x * (float)gcSize.GetWidth(), y * (float)gcSize.GetHeight(),
	                  w * (float)gcSize.GetWidth(), h * (float)gcSize.GetHeight());
	
	dc.DrawRectangle(x * dcSize.GetWidth(), y * dcSize.GetHeight(),
	                 w * dcSize.GetWidth(), h * dcSize.GetHeight());
	
	wxString rect;
	rect.Printf(wxT("<rect x='%d' y='%d' width='%dpx' height='%dpx' style='fill: rgb(%d, %d, %d);'/>\n"),
	            (int)(x * dcSize.GetWidth()), (int)(y * dcSize.GetHeight()),
	            (int)(w * dcSize.GetWidth()), (int)(h * dcSize.GetHeight()),
	            curR, curG, curB);
	svg += rect;
}

void EvoGraphRenderer::DrawText(float x, float y, bool right, const wxString &text)
{
	// If right is true, then (x, y) is the center-right-hand point on the
	// text, otherwise it's the center-top poit on the text
	wxDouble gcw, gch;
	wxCoord dcw, dch;
	gc->GetTextExtent(text, &gcw, &gch, NULL, NULL);
	dc.GetTextExtent(text, &dcw, &dch, NULL, NULL);
	
	if (right)
	{
		// x, y = center-right-hand, so we need to subtract width and
		// half of height
		gc->DrawText(text, (x * (float)gcSize.GetWidth()) - gcw, (y * (float)gcSize.GetHeight()) - (gch / 2.0));
		dc.DrawText(text, (x * dcSize.GetWidth()) - dcw, (y * dcSize.GetHeight()) - (dch / 2.0));
		
		// Set alignment attributes: horizontally the point is at the right, vertically positioned in middle
		wxString t;
		t.Printf(wxT("<text x='%d' y='%d' fill='black' font-family='%s' font-size='8pt' text-anchor='end' style='dominant-baseline: central'>%s</text>\n"),
		         (int)(x * dcSize.GetWidth()), (int)(y * dcSize.GetHeight()),
		         curFont.c_str(), text.c_str());
		svg += t;
	}
	else
	{
		// x, y = top-center, so we only need to subtract half width
		gc->DrawText(text, (x * (float)gcSize.GetWidth()) - (gcw / 2.0), y * (float)gcSize.GetHeight());
		dc.DrawText(text, (x * dcSize.GetWidth()) - (dcw / 2.0), y * dcSize.GetHeight());
		
		// Set alignment attributes: horizontally centered, vertically positioned at top-line
		wxString t;
		t.Printf(wxT("<text x='%d' y='%d' fill='black' font-family='%s' font-size='8pt' text-anchor='middle' style='dominant-baseline: text-before-edge'>%s</text>\n"),
		         (int)(x * dcSize.GetWidth()), (int)(y * dcSize.GetHeight()),
		         curFont.c_str(), text.c_str());
		svg += t;
	}
}



void EvoPage::PaintGraph(wxWindowDC &pdc)
{
	// This will get called occasionally by sizers when we aren't actually
	// shown
	if (!IsEnabled() || !IsShownOnScreen())
		return;

	size_t numGenerations = genSpinner->GetValue();
	
	renderer.StartDrawing(pdc, graphWindow->GetClientSize());
	
	// Clear the graph
	renderer.SetPen(*wxWHITE_PEN);
	renderer.SetBrush(*wxWHITE_BRUSH);
	renderer.DrawRectangle(0, 0, 1, 1);
	
	if (!evoTourney->IsPlayed())
		return;

	// Set default pen and font
	renderer.SetPen(*wxBLACK_PEN);
	
	wxFont font(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New"));
	renderer.SetFont(font);
	
	// Here's some constants for graph formatting
	const float sideSeparatorX = 0.05f;
	const float tickWidth = 0.01f;
	const float sideSeparatorY = 0.1f;
	const float tickHeight = 0.01f;
	
	// Draw a frame for the graph
	renderer.DrawLine(sideSeparatorX, sideSeparatorY, 1.0 - sideSeparatorX, sideSeparatorY);
	renderer.DrawLine(sideSeparatorX, sideSeparatorY, sideSeparatorX, 1.0 - sideSeparatorY);
	renderer.DrawLine(1.0 - sideSeparatorX, sideSeparatorY, 1.0 - sideSeparatorX, 1.0 - sideSeparatorY);
	renderer.DrawLine(sideSeparatorX, 1.0 - sideSeparatorY, 1.0 - sideSeparatorX, 1.0 - sideSeparatorY);
	
	// Draw ten tick marks on the Y-axis, print out 0, 0.5, and 1.0
	for (int i = 0 ; i <= 10 ; i++)
	{
		float y = sideSeparatorY + (i * (1.0 - 2 * sideSeparatorY) / 10.0f);
		renderer.DrawLine(sideSeparatorX - tickWidth, y, sideSeparatorX, y);
		
		if (i == 0 || i == 5 || i == 10)
		{
			float val = fabs(1.0f - (i * 0.1f));
			wxString str(wxString::Format(wxT("%1.1f"), val));
			renderer.DrawText(sideSeparatorX - 1.5 * tickWidth, y, true, str);
		}
	}
	
	// Draw ten ticks across the X-axis, print every other value
	for (int i = 0 ; i <= 10 ; i++)
	{
		float x = sideSeparatorX + (i * (1.0 - 2 * sideSeparatorX) / 10.0f);
		renderer.DrawLine(x, 1.0 - sideSeparatorY, x, 1.0 - sideSeparatorY + tickHeight);
		
		if ((i % 2) == 0)
		{
			int val = (int)((float)i / 10.0f * (float)numGenerations);
			wxString str(wxString::Format(wxT("%d"), val));
			renderer.DrawText(x, 1.0 - sideSeparatorY + 1.5 * tickHeight, false, str);
		}
	}
	
	// Make an array of X-coordinates
	float *x = new float[numGenerations + 1];
	for (size_t i = 0 ; i <= numGenerations ; i++)
		x[i] = sideSeparatorX + ((1.0 - 2 * sideSeparatorX) * ((float)i / numGenerations));
	
	// Convert the hash map to arrays of Y-coordinates
	size_t numPlayers = evoTourney->players.GetCount();
	float **y = new float *[numPlayers];
	
	for (size_t i = 0 ; i < numPlayers ; i++)
	{
		y[i] = new float[numGenerations + 1];
		
		// What's the ID of this player?
		int id = evoTourney->players[i]->GetID();
		
		for (size_t j = 0 ; j <= numGenerations ; j++)
		{
			// Get the data for this player from the tournament
			GenerationWeights &weights = evoTourney->data[j];
			y[i][j] = sideSeparatorY + ((1.0 - weights[id]) * (1.0 - 2 * sideSeparatorY));
		}
	}
	
	// Draw the curves and clean up
	for (size_t i = 0 ; i < numPlayers ; i++)
	{
		renderer.SetGraphColor(i % 15);
		renderer.DrawPolyLine(numGenerations + 1, x, y[i]);
		
		delete[] y[i];
	}
	
	delete[] x;
	delete[] y;
	
	renderer.FinishDrawing(bitmap, svgGraph);
}





BEGIN_EVENT_TABLE(EvoGraphWindow, wxWindow)
	EVT_PAINT(EvoGraphWindow::OnPaint)
END_EVENT_TABLE()


