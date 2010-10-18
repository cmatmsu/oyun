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

#ifndef EVOPAGE_H__
#define EVOPAGE_H__

#include <wx/spinctrl.h>
#include <wx/wizard.h>
#include <wx/graphics.h>
#include "tools/logoswizardpage.h"

class EvoTournament;
class EvoGraphWindow;


/**
    \class EvoGraphRenderer
    \ingroup ui
    
    \brief Class which encapsulates \c wxGraphicsContext, image, and SVG drawing
    
    This class accepts rectangle, pen, and text drawing commands, and draws its
    output to a \c wxMemoryDC used to make a bitmap image, a \c wxGraphicsContext
    used for on-screen display, and an SVG buffer used to create an SVG file.
*/
class EvoGraphRenderer
{
public:
	/**
	    \brief Constructor
	    
	    Constructs the renderer and initializes its contents
	    
	    \param imageSize Size of the bitmap image to be rendered
	*/
	EvoGraphRenderer(const wxSize &imageSize);
	
	/**
	    \brief Begin rendering to the graph
	    
	    This function sets the internal size parameters, and gets a
	    \c wxGraphicsContext for use in drawing.
	    
	    \param windowDC The window DC (or paint DC) on which to draw
	    \param windowSize The client size of the window on which to draw
	*/
	void StartDrawing(wxWindowDC &windowDC, const wxSize &windowSize);
	
	
	/**
	    \brief Begin drawing with a particular brush 
	    \param brush The brush to use for drawing
	*/
	void SetBrush(const wxBrush &brush);
	
	/**
	    \brief Begin drawing with a particular pen
	    \param pen The pen to use for drawing
	*/
	void SetPen(const wxPen &pen);
	
	/**
	    \brief Begin drawing in a standard graph color
	    \param color The color to use for drawing, between 0 and 15
	*/
	void SetGraphColor(int color);
	
	/**
	    \brief Begin drawing with a particular font
	    \param font The font to use for drawing
	*/
	void SetFont(const wxFont &font);
	
	
	/**
	    \brief Draw a line on the graph with the current pen or color
	    \param x1 X-coordinate of start, between 0 and 1
	    \param y1 Y-coordinate of start, between 0 and 1
	    \param x2 X-coordinate of end, between 0 and 1
	    \param y2 Y-coordinate of end, between 0 and 1
	*/
	void DrawLine(float x1, float y1, float x2, float y2);
	
	/**
	    \brief Draw a polyline on the graph with the current pen or color
	    \param np Number of points
	    \param x X-coordinates for the points
	    \param y Y-coordinates for the points
	*/
	void DrawPolyLine(size_t np, const float *x, const float *y);
	
	/**
	    \brief Draw a rectangle on the graph with the current brush
	    \param x X-coordinate of top-left of rectangle, between 0 and 1
	    \param y Y-coordinate of top-left of rectangle, between 0 and 1
	    \param w Width of rectangle
	    \param h Height of rectangle
	*/
	void DrawRectangle(float x, float y, float w, float h);
	
	/**
	    \brief Draw some text on the graph in the current font
	    
	    Text will be drawn with the current pen on the graph, but will
	    be printed as black in the SVG file.
	    
	    If \p right is set to true, the point (x, y) will be considered the
	    center-right point of the text to draw.  Otherwise, the point (x, y)
	    will be considered the top-center point of the text to draw.
	    
	    \param x X-coordinate of text, between 0 and 1
	    \param y Y-coordinate of text, between 0 and 1
	    \param right Changes how to interpret (x,y); see documentation
	    \param text String to draw
	*/
	void DrawText(float x, float y, bool right, const wxString &text);
	
	
	/**
	    \brief Finish drawing and save outputs
	    \param bitmapOut Place to save resulting bitmap data
	    \param svgOut Place to save resulting SVG data
	*/
	void FinishDrawing(wxBitmap &bitmapOut, wxString &svgOut);
	
private:
	
	/**
	    \struct SVGColor
	    \brief Representation of an SVG color for drawing
	*/
	struct SVGColor
	{
		const wxChar *svgName;		/**< \brief The SVG-standard name of this color */
		int r;				/**< \brief The red value of this color */
		int g;				/**< \brief The green value of this color */
		int b;				/**< \brief The blue value of this color */
	};
	
	/**
	    \brief The constant list of SVG colors we use for rendering
	*/
	static const struct SVGColor colors[15];
	
	int curR;				/**< \brief Red value of current color */
	int curG;				/**< \brief Green value of current color */
	int curB;				/**< \brief Blue value of current color */
	int curWidth;				/**< \brief Width of the current pen */
	
	wxString curFont;			/**< \brief Current font in use (CSS style format) */
	
	wxString svg;				/**< \brief String in which we're building the SVG */
	
	wxGraphicsRenderer *renderer;		/**< \brief The renderer for drawing on the graph window */
	wxGraphicsContext *gc;			/**< \brief The graphics context for drawing on the graph window */
	wxSize gcSize;				/**< \brief The size of the graphics context */
	
	wxMemoryDC dc;				/**< \brief The memory DC into which to draw the graph */
	wxBitmap bitmap;			/**< \brief The bitmap under \c dc */
	wxSize dcSize;				/**< \brief The size of \c dc */
};
	

/**
    \class EvoPage
    \ingroup ui
    
    \brief Wizard page on which an evolutionary tournament is run
    
    This page runs an evolutionary tournament and draws its results as
    a graph.
*/
class EvoPage : public LogosWizardPage
{
	DECLARE_CLASS(EvoPage)

public:
	/**
	    \brief Constructor
	    
	    Constructs the wizard page and all its controls.
	    
	    \param parent The parent of this wizard page
	    \param prev The previous page in the wizard
	    \param next The next page in the wizard
	*/
	EvoPage(LogosWizard *parent, wxWizardPage *prev = NULL, wxWizardPage *next = NULL);

	virtual ~EvoPage();
	
private:
	DECLARE_EVENT_TABLE()
	
	/**
	    \brief Called when the wizard page is changing
	    \param event The event generated
	*/
	void OnPageChanging(wxWizardEvent &event);
	
	/**
	    \brief Show this wizard page's help documentation
	    \param event The event generated
	*/
	void OnHelp(wxWizardEvent &event);
	
	/**
	    \brief Called when the buttons on the spinner are pressed
	    \param event The event generated
	*/
	void OnSpinner(wxSpinEvent &event);
	
	/**
	    \brief Called when the text field of the spinner is edited
	    \param event The event generated
	*/
	void OnSpinnerText(wxCommandEvent &event);
	
	/**
	    \brief Called when the "Run Tournament" button is pressed
	    \param event The event generated
	*/
	void OnRunTournament(wxCommandEvent &event);
	
	/**
	    \brief Called when the "Show Legend" button is pressed
	    \param event The event generated
	*/
	void OnShowLegend(wxCommandEvent &event);
	
	/**
	    \brief Called when the page receives a 'data update' message
	    \param event The event generated
	*/
	void OnDataUpdate(wxNotifyEvent &event);
	
	/**
	    \brief Add a player to the tournament
	    \param event The event generated
	*/
	void OnAddPlayer(wxNotifyEvent &event);
	
	/**
	    \brief Remove a player from the tournament
	    \param event The event generated
	*/
	void OnRemovePlayer(wxNotifyEvent &event);
	
	
	/**
	    \brief Draw the graph in the graph window
	    
	    This function analyses the EvoTournament::data member and
	    draws it as a graph on this panel.
	    
	    \param dc A \c wxWindowDC, usually a \c wxPaintDC, for 
	              \c graphWindow
	*/
	void PaintGraph(wxWindowDC &dc);
	
	
	friend class EvoFinishPage;
	
	/**
	    \brief The last graph drawn, in SVG format
	    
	    This is used by the ending page to give the user the option of
	    saving in SVG format.  We re-create this SVG every time the
	    graph is redrawn.
	*/
	wxString svgGraph;
	
	/**
	    \brief The last graph drawn, as a wxImage
	    
	    This is used by the ending page to give the user the option of
	    saving in various image formats.  We re-create this image every
	    time the graph is redrawn.
	*/
	wxImage imageGraph;
		
	
	/**
	    \brief The evolutionary tournament to be run
	*/
	EvoTournament *evoTourney;

	wxButton *runTournament;		/**< \brief The run tournament button */
	wxButton *legend;			/**< \brief The show legend button */
	wxStaticText *spinnerLabel;		/**< \brief The text label for the spinner */
	wxSpinCtrl *genSpinner;			/**< \brief The spinner to select generation number */
	
	friend class EvoGraphWindow;
	EvoGraphWindow *graphWindow;		/**< \brief The window in which we draw the graph */
	
	EvoGraphRenderer renderer;		/**< \brief Renderer used to draw on the graph */
	wxBitmap bitmap;			/**< \brief Bitmap where we store result of graph drawing */
};



/**
    \class EvoGraphWindow
    \ingroup ui
    
    \brief A simple wxWindow derivative that handles repainting
    
    This class tells the EvoPage to repaint the graph every time it
    is required.
*/
class EvoGraphWindow : public wxWindow
{
public:
	/**
	    \brief Constructor
	    \param par The parent window
	*/
	EvoGraphWindow(EvoPage *par) : wxWindow(par, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
	                                        wxFULL_REPAINT_ON_RESIZE|wxBORDER_SUNKEN),
	                               parent(par)
	{ }
		
private:
	DECLARE_EVENT_TABLE()
	
	/**
	    \brief The parent wizard page
	*/
	EvoPage *parent;
	
	
	/**
	    \brief Respond to a paint event and pass upward
	    \param event The event generated
	*/
	void OnPaint(wxPaintEvent & WXUNUSED(event))
	{
		wxPaintDC pdc(this);
		parent->PaintGraph(pdc);
	}
};


#endif
