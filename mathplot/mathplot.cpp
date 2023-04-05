/////////////////////////////////////////////////////////////////////////////
// Name:            mathplot.cpp
// Purpose:         Framework for plotting in wxWindows
// Original Author: David Schalig
// Maintainer:      Davide Rondini
// Contributors:    Jose Luis Blanco, Val Greene
// Created:         21/07/2003
// Last edit:       09/09/2007
// Last edit:       01/02/2021
// Copyright:       (c) David Schalig, Davide Rondini
// Licence:         wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
// #pragma implementation "plot.h"
#pragma implementation "mathplot.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include <wx/wx.h>
// #include <wx/window.h>
// #include <wx/wxprec.h>

// Comment out for release operation:
// (Added by J.L.Blanco, Aug 2007)
//#define MATHPLOT_DO_LOGGING

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/object.h"
#include "wx/font.h"
#include "wx/colour.h"
#include "wx/settings.h"
#include "wx/sizer.h"
#include "wx/log.h"
#include "wx/intl.h"
#include "wx/dcclient.h"
#include "wx/cursor.h"
#endif

#include "mathplot.h"
#include <wx/bmpbuttn.h>
#include <wx/module.h>
#include <wx/msgdlg.h>
#include <wx/image.h>
#include <wx/tipwin.h>
#include <wx/clipbrd.h>
#include <wx/dcbuffer.h>
#include <wx/filename.h>

#include <cmath>
#include <cstdio> // used only for debug
#include <ctime>  // used for representation of x axes involving date

// If we want icon on the popup menu
#define USE_ICON
#ifdef USE_ICON
#include "Images/Center.h"
#include "Images/Config.h"
#include "Images/Coordinates.h"
#include "Images/Fit.h"
#include "Images/Grid.h"
#include "Images/Load.h"
#include "Images/Mouse.h"
#include "Images/Screenshot.h"
#include "Images/Zoom_in.h"
#include "Images/Zoom_out.h"
#endif

// #include "pixel.xpm"

// Memory leak debugging
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Legend margins
#define mpLEGEND_MARGIN 5
#define mpLEGEND_LINEWIDTH 10

// Minimum axis label separation
#define mpMIN_X_AXIS_LABEL_SEPARATION 64
#define mpMIN_Y_AXIS_LABEL_SEPARATION 32

// Number of pixels to scroll when scrolling by a line
#define mpSCROLL_NUM_PIXELS_PER_LINE  10

// Offset for the text in margin
#define MARGIN_TOP_OFFSET 5
#define MARGIN_BOTTOM_OFFSET 5
#define MARGIN_LEFT_OFFSET 5
#define MARGIN_RIGHT_OFFSET 5

// See doxygen comments.
double mpWindow::zoomIncrementalFactor = 1.5;

//-----------------------------------------------------------------------------
// mpLayer
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpLayer, wxObject)

mpLayer::mpLayer() :
		m_type(mpLAYER_UNDEF)
{
	// Default pen
	SetPen((wxPen const &) *wxBLACK_PEN);
	// Default font
	SetFont((wxFont const &) *wxNORMAL_FONT);
	m_fontcolour = (wxColour const &) *wxBLACK;
	// Default brush
	SetBrush((wxBrush const &) *wxTRANSPARENT_BRUSH);
	m_continuous = false; // Default
	m_showName = false;  // Default
	m_drawOutsideMargins = false;
	m_visible = true;
	m_flags = mpALIGN_NE;
	m_symbol = mpsNone;
	m_symbolSize = 6;
	m_symbolSize2 = 3;
	m_step = 1;
	m_CanDelete = true;
}

void mpLayer::UpdateContext(wxDC &dc)
{
	dc.SetPen(m_pen);
	dc.SetBrush(m_brush);
	dc.SetFont(m_font);
	dc.SetTextForeground(m_fontcolour);
}

wxBitmap mpLayer::GetColourSquare(int side)
{
	wxBitmap square(side, side, -1);
	wxColour filler = m_pen.GetColour();
	wxBrush brush(filler, wxBRUSHSTYLE_SOLID);
	wxMemoryDC dc;
	dc.SelectObject(square);
	dc.SetBackground(brush);
	dc.Clear();
	dc.SelectObject(wxNullBitmap);
	return square;
}

void mpLayer::DrawSymbol(wxDC &dc, wxCoord x, wxCoord y)
{
	switch (m_symbol)
	{
		case mpsCircle:
			dc.DrawCircle(x, y, m_symbolSize);
			break;

		case mpsSquare:
			dc.DrawRectangle(x - m_symbolSize2, y - m_symbolSize2, m_symbolSize, m_symbolSize);
			break;

		case mpsUpTriangle:
		{
			const wxPoint points[] = {wxPoint(x, y - m_symbolSize), wxPoint(x + m_symbolSize, y + m_symbolSize), wxPoint(x - m_symbolSize,
					y + m_symbolSize)};
			dc.DrawPolygon(3, points);
			break;
		}

		case mpsDownTriangle:
		{
			const wxPoint points[] = {wxPoint(x - m_symbolSize, y - m_symbolSize), wxPoint(x + m_symbolSize, y - m_symbolSize), wxPoint(x,
					y + m_symbolSize)};
			dc.DrawPolygon(3, points);
			break;
		}

		case mpsCross:
			dc.DrawLine(x - m_symbolSize, y - m_symbolSize, x + m_symbolSize, y + m_symbolSize);
			dc.DrawLine(x + m_symbolSize, y - m_symbolSize, x - m_symbolSize, y + m_symbolSize);
			break;

		case mpsPlus:
			dc.DrawLine(x, y - m_symbolSize, x, y + m_symbolSize);
			dc.DrawLine(x - m_symbolSize, y, x + m_symbolSize, y);
			break;

		default:
			; // Do nothing
	}
}

//-----------------------------------------------------------------------------
// mpInfoLayer
//-----------------------------------------------------------------------------
IMPLEMENT_ABSTRACT_CLASS(mpInfoLayer, mpLayer)

mpInfoLayer::mpInfoLayer()
{
	m_type = mpLAYER_INFO;
	m_dim = wxRect(0, 0, 1, 1);
	m_brush = *wxTRANSPARENT_BRUSH;
	m_brush.SetColour(*wxWHITE);
	m_reference.x = 0;
	m_reference.y = 0;
	m_winX = 1;
	m_winY = 1;
	m_location = mpMarginNone;
}

mpInfoLayer::mpInfoLayer(wxRect rect, const wxBrush *brush, mpLocation location)
{
	m_type = mpLAYER_INFO;
	m_brush = *brush;
	if (m_brush.GetStyle() == wxBRUSHSTYLE_TRANSPARENT)
		m_brush.SetColour(*wxWHITE);
	m_reference.x = rect.x;
	m_reference.y = rect.y;
	m_winX = 1;
	m_winY = 1;
	m_location = location;
}

mpInfoLayer::~mpInfoLayer()
{

}

void mpInfoLayer::UpdateInfo(mpWindow&WXUNUSED(w), wxEvent& WXUNUSED(event))
{
// Nothing to do here
}

bool mpInfoLayer::Inside(wxPoint &point)
{
#if wxCHECK_VERSION(2, 8, 0)
	return m_dim.Contains(point);
#else
  return m_dim.Inside(point);
#endif
}

void mpInfoLayer::Move(wxPoint delta)
{
	m_dim.SetX(m_reference.x + delta.x);
	m_dim.SetY(m_reference.y + delta.y);
}

void mpInfoLayer::UpdateReference()
{
	m_reference.x = m_dim.x;
	m_reference.y = m_dim.y;
}

void mpInfoLayer::SetInfoRectangle(mpWindow &w, int width, int height)
{
	// Adjust relative position inside the window
	int scrx = w.GetScreenX();
	int scry = w.GetScreenY();
	// Avoid dividing by 0
	if (scrx == 0)
		scrx = 1;
	if (scry == 0)
		scry = 1;

	if (width != 0)
		m_dim.width = width;
	if (m_dim.width > scrx)
		m_dim.width = scrx;
	if (height != 0)
		m_dim.height = height;
	if (m_dim.height > scry)
		m_dim.height = scry;

	if (m_location == mpMarginNone)
	{
		if ((m_winX != scrx) || (m_winY != scry))
		{
#ifdef MATHPLOT_DO_LOGGING
			// wxLogMessage(wxT("mpInfoLayer::Plot() screen size has changed from %d x %d to %d x %d"), m_winX, m_winY, scrx, scry);
#endif
			if (m_winX != 1)
				m_dim.x = (int) floor((double) (m_dim.x * scrx / m_winX));
			if (m_winY != 1)
			{
				m_dim.y = (int) floor((double) (m_dim.y * scry / m_winY));
				UpdateReference();
			}
			// Finally update window size
			m_winX = scrx;
			m_winY = scry;
		}
	}
	else
	{
		switch (m_location)
		{
			case mpMarginLeftCenter:
			{
				m_dim.x = (w.GetMarginLeft() - m_dim.width) / 2;
				m_dim.y = (w.GetScreenY() - m_dim.height) / 2;
				if (m_dim.x < MARGIN_LEFT_OFFSET)
					m_dim.x = MARGIN_LEFT_OFFSET;
				break;
			}
			case mpMarginTopLeft:
			{
				m_dim.x = MARGIN_LEFT_OFFSET;
				m_dim.y = (w.GetMarginTop() - m_dim.height) / 2;
				break;
			}
			case mpMarginTopCenter:
			{
				m_dim.x = (w.GetScreenX() - m_dim.width) / 2;
				m_dim.y = (w.GetMarginTop() - m_dim.height) / 2;
				break;
			}
			case mpMarginTopRight:
			{
				m_dim.x = w.GetScreenX() - m_dim.width - MARGIN_BOTTOM_OFFSET;
				m_dim.y = (w.GetMarginTop() - m_dim.height) / 2;
				break;
			}
			case mpMarginRightCenter:
			{
				m_dim.x = w.GetScreenX() - (w.GetMarginRight() + m_dim.width) / 2;
				m_dim.y = (w.GetScreenY() - m_dim.height) / 2;
				if ((m_dim.x + m_dim.width) > w.GetScreenX())
					m_dim.x = w.GetScreenX() - m_dim.width - MARGIN_RIGHT_OFFSET;
				break;
			}
			case mpMarginBottomLeft:
			{
				m_dim.x = MARGIN_LEFT_OFFSET;
				m_dim.y = w.GetScreenY() - (w.GetMarginBottom() + m_dim.height) / 2;
				break;
			}
			case mpMarginBottomCenter:
			{
				m_dim.x = (w.GetScreenX() - m_dim.width) / 2;
				m_dim.y = w.GetScreenY() - (w.GetMarginBottom() + m_dim.height) / 2;
				break;
			}
			case mpMarginBottomRight:
			{
				m_dim.x = w.GetScreenX() - m_dim.width - MARGIN_BOTTOM_OFFSET;
				m_dim.y = w.GetScreenY() - (w.GetMarginBottom() + m_dim.height) / 2;
				break;
			}
			default:
				;
		}
		if (m_dim.y < MARGIN_TOP_OFFSET)
			m_dim.y = MARGIN_TOP_OFFSET;
		else
			if ((m_dim.y + m_dim.height) > w.GetScreenY())
				m_dim.y = w.GetScreenY() - m_dim.height - MARGIN_BOTTOM_OFFSET;
	}
}

void mpInfoLayer::Plot(wxDC &dc, mpWindow &w)
{
	if (m_visible)
	{
		DoBeforePlot();
		SetInfoRectangle(w);
		UpdateContext(dc);
		dc.DrawRectangle(m_dim.x, m_dim.y, m_dim.width, m_dim.height);
	}
}

wxPoint mpInfoLayer::GetPosition()
{
	return m_dim.GetPosition();
}

wxSize mpInfoLayer::GetSize()
{
	return m_dim.GetSize();
}

//-----------------------------------------------------------------------------
// mpInfoCoords
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpInfoCoords, mpInfoLayer)

mpInfoCoords::mpInfoCoords() :
		mpInfoLayer()
{
	m_labelType = mpX_NORMAL;
	m_timeConv = 0;
	m_mouseX = m_mouseY = 0;
	m_coord_bmp = NULL;
	m_location = mpMarginBottomRight;
	wxBrush coord(wxColour(232, 232, 232), wxBRUSHSTYLE_SOLID);
	SetBrush(coord);
}

mpInfoCoords::mpInfoCoords(mpLocation location) :
		mpInfoLayer()
{
	m_labelType = mpX_NORMAL;
	m_timeConv = 0;
	m_mouseX = m_mouseY = 0;
	m_coord_bmp = NULL;
	m_location = location;
	wxBrush coord(wxColour(232, 232, 232), wxBRUSHSTYLE_SOLID);
	SetBrush(coord);
}

mpInfoCoords::mpInfoCoords(wxRect rect, const wxBrush *brush, mpLocation location) :
		mpInfoLayer(rect, brush, location)
{
	m_labelType = mpX_NORMAL;
	m_timeConv = 0;
	m_mouseX = m_mouseY = 0;
	m_coord_bmp = NULL;
}

mpInfoCoords::~mpInfoCoords()
{
	if (m_coord_bmp)
	{
		delete m_coord_bmp;
		m_coord_bmp = NULL;
	}
}
void mpInfoCoords::SetVisible(bool show)
{
	m_visible = show;
	if (m_coord_bmp)
	{
		delete m_coord_bmp;
		m_coord_bmp = NULL;
	}
}

void mpInfoCoords::UpdateInfo(mpWindow &w, wxEvent &event)
{
	time_t when = 0;
	double xVal = 0.0, yVal = 0.0;
	struct tm timestruct;
	if (event.GetEventType() == wxEVT_MOTION)
	{
		m_mouseX = ((wxMouseEvent&) event).GetX();
		m_mouseY = ((wxMouseEvent&) event).GetY();
		xVal = w.p2x(m_mouseX);
		yVal = w.p2y(m_mouseY);
		/* It seems that Windows port of wxWidgets don't support multi-line text to be drawn in a wxDC.
		 *   wxGTK instead works perfectly with it.
		 *   Info on wxForum: http://wxforum.shadonet.com/viewtopic.php?t=3451&highlight=drawtext+eol */
		// UPDATE 2018-10-04: this seems not to be still valid on latest wxWidgets.
		// #ifdef _WINDOWS
		//     m_content.Printf(wxT("x = %f y = %f"), w.p2x(mouseX), w.p2y(mouseY));
		// #else
		m_content.Clear();

		if (m_labelType == mpX_NORMAL)
		{
			m_content.Printf(wxT("x = %g\ny = %g"), xVal, yVal);
		}
		else
			if (m_labelType == mpX_DATETIME)
			{
				when = (time_t) xVal;
				if (when > 0)
				{
					if (m_timeConv == mpX_LOCALTIME)
					{
						timestruct = *localtime(&when);
					}
					else
					{
						timestruct = *gmtime(&when);
					}
					m_content.Printf(wxT("x = %04.0f-%02.0f-%02.0fT%02.0f:%02.0f:%02.0f\ny = %f"), (double) timestruct.tm_year + 1900,
							(double) timestruct.tm_mon + 1, (double) timestruct.tm_mday, (double) timestruct.tm_hour, (double) timestruct.tm_min,
							(double) timestruct.tm_sec, yVal);
				}
			}
			else
				if (m_labelType == mpX_DATE)
				{
					when = (time_t) xVal;
					if (when > 0)
					{
						if (m_timeConv == mpX_LOCALTIME)
						{
							timestruct = *localtime(&when);
						}
						else
						{
							timestruct = *gmtime(&when);
						}
						m_content.Printf(wxT("x = %04.0f-%02.0f-%02.0f\ny = %f"), (double) timestruct.tm_year + 1900, (double) timestruct.tm_mon + 1,
								(double) timestruct.tm_mday, yVal);
					}
				}
				else
					if ((m_labelType == mpX_TIME) || (m_labelType == mpX_HOURS))
					{
						double sign = 1.0;
						if (xVal < 0)
							sign = -1.0;
						double modulus = fabs(xVal);
						double hh = floor(modulus / 3600);
						double mm = floor((modulus - hh * 3600) / 60);
						double ss = modulus - hh * 3600 - mm * 60;
						m_content.Printf(wxT("x = %02.0f:%02.0f:%02.0f\ny = %f"), sign * hh, mm, floor(ss), yVal);
					}
	}
}

void mpInfoCoords::Plot(wxDC &dc, mpWindow &w)
{
	if (m_visible)
	{
		DoBeforePlot();
		UpdateContext(dc);

		int textX = 0, textY = 0;
		int width = 0, height = 0;

		// It looks like that on Windows, GetTetxExtent function
		// ignores the newline in the calculus of size
#ifdef _WIN32
		// Windows code
		wxString m_contentX, m_contentY;
		int textY_H = 0;
		m_contentX = m_content.BeforeFirst(wxT('\n'));
		m_contentY = m_content.AfterFirst(wxT('\n'));
		dc.GetTextExtent(m_contentX, &textX, &textY);
		dc.GetTextExtent(m_contentY, &textY_H, &textY);
		textX = (textX > textY_H) ? textX : textY_H;
		if (width < textX + 10)
			width = textX + 10;
		if (height < 2 * textY + 10)
			height = 2 * textY + 10;
#else
      // *NIX code
      dc.GetTextExtent(m_content, &textX, &textY);
      if (width < textX + 10) width = textX + 10;
      if (height < textY + 10) height = textY + 10;
#endif

		SetInfoRectangle(w, width, height);
		if (m_location == mpCursor)
		{
			m_dim.x = m_mouseX + 2 * MARGIN_BOTTOM_OFFSET;
			if (m_dim.x + m_dim.width > w.GetScreenX())
				m_dim.x = m_mouseX - m_dim.width;
			m_dim.y = m_mouseY + 2 * MARGIN_BOTTOM_OFFSET;
			if (m_dim.y + m_dim.height > w.GetScreenY())
				m_dim.y = m_mouseY - m_dim.height;
		}

		// Don't use stored bitmap when we repaint all
		if (w.IsRepainting() && m_coord_bmp)
		{
			delete m_coord_bmp;
			m_coord_bmp = NULL;
		}

		// First : restaure stored bitmap
		if (m_coord_bmp)
		{
			wxMemoryDC m_coord_dc(&dc);
			m_coord_dc.SelectObject(*m_coord_bmp);
			dc.Blit(m_oldDim.x, m_oldDim.y, m_oldDim.width, m_oldDim.height, &m_coord_dc, 0, 0);
			m_coord_dc.SelectObject(wxNullBitmap);
			delete m_coord_bmp;
			m_coord_bmp = NULL;
		}

		// Second : store new bitmap
		m_coord_bmp = new wxBitmap(m_dim.width, m_dim.height, dc);
		wxMemoryDC m_coord_dc(&dc);
		m_coord_dc.SelectObject(*m_coord_bmp);
		m_coord_dc.Blit(0, 0, m_dim.width, m_dim.height, &dc, m_dim.x, m_dim.y);
		m_coord_dc.SelectObject(wxNullBitmap);
		m_oldDim = m_dim;

		// Third : draw the coordinate
		dc.DrawRectangle(m_dim.x, m_dim.y, m_dim.width, m_dim.height);
		dc.DrawText(m_content, m_dim.x + 5, m_dim.y + 5);
	}
}

void mpInfoCoords::ErasePlot(wxDC &dc, mpWindow &WXUNUSED(w))
{
	if (m_coord_bmp)
	{
		wxMemoryDC m_coord_dc(&dc);
		m_coord_dc.SelectObject(*m_coord_bmp);
		dc.Blit(m_oldDim.x, m_oldDim.y, m_oldDim.width, m_oldDim.height, &m_coord_dc, 0, 0);
		m_coord_dc.SelectObject(wxNullBitmap);
		delete m_coord_bmp;
		m_coord_bmp = NULL;
	}
}

//-----------------------------------------------------------------------------
// mpInfoLegend
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpInfoLegend, mpInfoLayer)

mpInfoLegend::mpInfoLegend() :
		mpInfoLayer()
{
	m_item_mode = mpLegendLine;
	m_item_direction = mpVertical;
	m_legend_bmp = NULL;
	m_need_update = true;
}

mpInfoLegend::mpInfoLegend(wxRect rect, const wxBrush *brush, mpLocation location) :
		mpInfoLayer(rect, brush, location)
{
	m_item_mode = mpLegendLine;
	m_item_direction = mpVertical;
	m_legend_bmp = NULL;
	m_need_update = true;
}

mpInfoLegend::~mpInfoLegend()
{
	if (m_legend_bmp)
	{
		delete m_legend_bmp;
		m_legend_bmp = NULL;
	}
}

void mpInfoLegend::UpdateBitmap(wxDC &dc, mpWindow &w)
{
	// Create a temporary bitmap to draw the legend
	wxBitmap *buff_bmp = new wxBitmap(w.GetScreenX(), w.GetScreenY(), dc);
	wxMemoryDC buff_dc(&dc);
	buff_dc.SelectObject(*buff_bmp);
	buff_dc.SetPen(*wxTRANSPARENT_PEN);
	if (m_brush.GetStyle() == wxBRUSHSTYLE_TRANSPARENT)
		buff_dc.SetBrush(*wxWHITE_BRUSH);
	else
		buff_dc.SetBrush(m_brush);
	buff_dc.DrawRectangle(0, 0, w.GetScreenX(), w.GetScreenY());

	int posX = 0, posY = 0;
	int tmpX = 0, tmpY = 0;
	int width = 0, height = 0;
	bool first = true;
	mpLayer *ly = NULL;
	wxBrush sqrBrush(*wxWHITE, wxBRUSHSTYLE_SOLID);

	// Delete old bitmap
	if (m_legend_bmp)
	{
		delete m_legend_bmp;
		m_legend_bmp = NULL;
	}

	// Get series name
	for (unsigned int p = 0; p < w.CountAllLayers(); p++)
	{
		ly = w.GetLayer(p);
		if ((ly->GetLayerType() == mpLAYER_PLOT) && (ly->IsVisible()))
		{
			wxString label = ly->GetName();
			wxPen lpen = ly->GetPen();

			buff_dc.SetPen(lpen);
			buff_dc.GetTextExtent(label, &tmpX, &tmpY);

			if (first)
			{
				posX = mpLEGEND_MARGIN;
				posY = mpLEGEND_MARGIN + (tmpY >> 1);
				first = false;
			}

			if (m_item_mode == mpLegendLine)
			{
				buff_dc.DrawLine(posX, posY, posX + mpLEGEND_LINEWIDTH, posY);
			}
			else  // m_item_mode == mpLEGEND_SQUARE
			{
				sqrBrush.SetColour(lpen.GetColour());
				buff_dc.SetBrush(sqrBrush);
				buff_dc.DrawRectangle(posX, posY - (mpLEGEND_LINEWIDTH >> 1),
				mpLEGEND_LINEWIDTH, mpLEGEND_LINEWIDTH);
			}

			posX += mpLEGEND_LINEWIDTH + mpLEGEND_MARGIN;
			buff_dc.DrawText(label, posX, posY - (tmpY >> 1));

			posX += tmpX + mpLEGEND_MARGIN;

			if (m_item_direction == mpVertical)
			{
				if (posX > width)
					width = posX;
				posX = mpLEGEND_MARGIN;
				posY += tmpY;
				height = posY;
				posY += mpLEGEND_MARGIN;
			}
			else
			{
				width = posX;
				if (posY + tmpY > height)
					height = posY + tmpY;
			}
		}
	}

	if ((width != 0) && (height != 0))
	{
		buff_dc.SetPen(*wxBLACK_PEN);
		buff_dc.SetBrush(*wxTRANSPARENT_BRUSH);

		// Adjust size inside the window and trunc legend if necessary
		int scrx = w.GetScreenX();
		int scry = w.GetScreenY();
		if (width > scrx)
			width = scrx;
		if (height > scry)
			height = scry;

		buff_dc.DrawRectangle(0, 0, width, height);
		SetInfoRectangle(w, width, height);

		// Transfert to the legend bitmap
		m_legend_bmp = new wxBitmap(width, height, dc);
		wxMemoryDC buff_dc2(&dc);
		buff_dc2.SelectObject(*m_legend_bmp);
		buff_dc2.Blit(0, 0, width, height, &buff_dc, 0, 0);
		buff_dc2.SelectObject(wxNullBitmap);
		m_need_update = false;
	}

	buff_dc.SelectObject(wxNullBitmap);
	delete buff_bmp;
}

void mpInfoLegend::Plot(wxDC &dc, mpWindow &w)
{
	if (m_visible)
	{
		DoBeforePlot();

		if (m_need_update)
			UpdateBitmap(dc, w);
		else
			// In case we have resize
			SetInfoRectangle(w, m_dim.width, m_dim.height);

		if (m_legend_bmp && (m_dim.width != 0) && (m_dim.height != 0))
		{
			wxMemoryDC buff_dc(&dc);
			buff_dc.SelectObject(*m_legend_bmp);

			// Draw the info bitmap to the current dc
			// wxAND not work on Linux
#ifdef _WIN32
			// Windows code
			if (m_brush.GetStyle() == wxBRUSHSTYLE_TRANSPARENT)
				dc.Blit(m_dim.x, m_dim.y, m_dim.width, m_dim.height, &buff_dc, 0, 0, wxAND);
			else
				dc.Blit(m_dim.x, m_dim.y, m_dim.width, m_dim.height, &buff_dc, 0, 0);
#else
			dc.Blit(m_dim.x, m_dim.y, m_dim.width, m_dim.height, &buff_dc, 0, 0);
#endif
			buff_dc.SelectObject(wxNullBitmap);
		}
	}
}

//-----------------------------------------------------------------------------
// mpLayer implementations - functions
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpFX, mpLayer)

mpFX::mpFX(wxString name, int flags)
{
	m_type = mpLAYER_PLOT;
	SetName(name);
	m_flags = flags;
}

void mpFX::Plot(wxDC &dc, mpWindow &w)
{
	if (m_visible)
	{
		DoBeforePlot();
		UpdateContext(dc);

		wxCoord i, iy, iylast;

		// Get bondaries
		m_plotBondaries = w.GetPlotBondaries(!m_drawOutsideMargins);

		if (m_continuous || (m_pen.GetWidth() > 1))
		{
			if (m_continuous)
			{
				// Get first point
				iylast = w.y2p(GetY(w.p2x(m_plotBondaries.startPx)));
				if (!m_drawOutsideMargins)
				{
					if (iylast < m_plotBondaries.startPy)
						iylast = m_plotBondaries.startPy;
					else
						if (iylast > m_plotBondaries.endPy)
							iylast = m_plotBondaries.endPy;
				}

				for (i = m_plotBondaries.startPx + m_step; i < m_plotBondaries.endPx; i += m_step)
				{
					iy = w.y2p(GetY(w.p2x(i)));

					if (!m_drawOutsideMargins)
					{
						if (iy < m_plotBondaries.startPy)
							iy = m_plotBondaries.startPy;
						else
							if (iy > m_plotBondaries.endPy)
								iy = m_plotBondaries.endPy;
					}
					dc.DrawLine(i - m_step, iylast, i, iy);
					if (m_symbol != mpsNone)
						DrawSymbol(dc, i - m_step, iylast);
					iylast = iy;
				}
				// Last point
				if (m_symbol != mpsNone)
					DrawSymbol(dc, m_plotBondaries.startPx, iylast);
			}
			else
			{
				for (i = m_plotBondaries.startPx; i < m_plotBondaries.endPx; i += m_step)
				{
					iy = w.y2p(GetY(w.p2x(i)));
					if (m_drawOutsideMargins || ((iy >= m_plotBondaries.startPy) && (iy <= m_plotBondaries.endPy)))
					{
						if (m_symbol == mpsNone)
							dc.DrawLine(i, iy, i, iy);
						else
							DrawSymbol(dc, i, iy);
					}
				}
			}
		}
		else
		{
			for (i = m_plotBondaries.startPx; i < m_plotBondaries.endPx; i += m_step)
			{
				iy = w.y2p(GetY(w.p2x(i)));
				// Draw the point only if you can draw outside margins or if the point is inside margins
				if (m_drawOutsideMargins || ((iy >= m_plotBondaries.startPy) && (iy <= m_plotBondaries.endPy)))
				{
					if (m_symbol == mpsNone)
						dc.DrawPoint(i, iy);
					else
						DrawSymbol(dc, i, iy);
				}
			}
		}

		if (m_showName && !m_name.IsEmpty())
		{
			wxCoord tx, ty;
			dc.GetTextExtent(m_name, &tx, &ty);

			switch (m_flags)
			{
				case mpALIGN_RIGHT:
				case mpALIGN_BORDER_RIGHT:
				case mpALIGN_NE:
				case mpALIGN_SE:
					tx = (w.GetScreenX() - tx) - w.GetMarginRight() - 8;
					break;
				case mpALIGN_CENTERY:
					tx = ((w.GetPlotWidth() - tx) / 2) + w.GetMarginLeft();
					break;
				default:  // mpALIGN_LEFT & mpALIGN_BORDER_LEFT
					tx = w.GetMarginLeft() + 8;
			}

			dc.DrawText(m_name, tx, w.y2p(GetY(w.p2x(tx))));
		}
	}
}

//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpFY, mpLayer)

mpFY::mpFY(wxString name, int flags)
{
	m_type = mpLAYER_PLOT;
	SetName(name);
	m_flags = flags;
}

void mpFY::Plot(wxDC &dc, mpWindow &w)
{
	if (m_visible)
	{
		DoBeforePlot();
		UpdateContext(dc);

		wxCoord i, ix, ixlast;

		// Get bondaries
		m_plotBondaries = w.GetPlotBondaries(!m_drawOutsideMargins);

		if (m_continuous || (m_pen.GetWidth() > 1))
		{
			if (m_continuous)
			{
				// Get first point
				ixlast = w.x2p(GetX(w.p2y(m_plotBondaries.startPy)));
				bool inBound = true;
				if (!m_drawOutsideMargins)
				{
					if (ixlast < m_plotBondaries.startPx)
					{
						ixlast = m_plotBondaries.startPx;
						inBound = false;
					}
					else
						if (ixlast > m_plotBondaries.endPx)
						{
							ixlast = m_plotBondaries.endPx;
							inBound = false;
						}
				}

				for (i = m_plotBondaries.startPy + m_step; i < m_plotBondaries.endPy; i += m_step)
				{
					ix = w.x2p(GetX(w.p2y(i)));

					if (!m_drawOutsideMargins)
					{
						if (ix < m_plotBondaries.startPx)
							ix = m_plotBondaries.startPx;
						else
							if (ix > m_plotBondaries.endPx)
								ix = m_plotBondaries.endPx;
					}
					// Avoid vertical draw
					if (ixlast != ix)
					{
						dc.DrawLine(ixlast, i - m_step, ix, i);
						if ((m_symbol != mpsNone) && inBound)
							DrawSymbol(dc, ixlast, i - m_step);
						inBound = true;
					}
					else
						inBound = false;

					ixlast = ix;
				}
				// Last point
				if ((m_symbol != mpsNone) && inBound)
					DrawSymbol(dc, ixlast, i - m_step);
			}
			else
			{
				for (i = m_plotBondaries.startPy; i < m_plotBondaries.endPy; i += m_step)
				{
					ix = w.x2p(GetX(w.p2y(i)));
					if (m_drawOutsideMargins || ((ix >= m_plotBondaries.startPx) && (ix <= m_plotBondaries.endPx)))
					{
						if (m_symbol == mpsNone)
							dc.DrawLine(ix, i, ix, i);
						else
							DrawSymbol(dc, ix, i);
					}
				}
			}
		}
		else
		{
			for (i = m_plotBondaries.startPy; i < m_plotBondaries.endPy; i += m_step)
			{
				ix = w.x2p(GetX(w.p2y(i)));
				if (m_drawOutsideMargins || ((ix >= m_plotBondaries.startPx) && (ix <= m_plotBondaries.endPx)))
				{
					if (m_symbol == mpsNone)
						dc.DrawPoint(ix, i);
					else
						DrawSymbol(dc, ix, i);
				}
			}
		}

		if (m_showName && !m_name.IsEmpty())
		{
			wxCoord tx, ty;
			dc.GetTextExtent(m_name, &tx, &ty);

			switch (m_flags)
			{
				case mpALIGN_TOP:
				case mpALIGN_BORDER_TOP:
				case mpALIGN_NW:
				case mpALIGN_NE:
					ty = w.GetMarginTop() + 8;
					break;
				case mpALIGN_CENTERX:
					ty = ((w.GetPlotHeight() - ty) / 2) + w.GetMarginTop();
					break;
				default:  // mpALIGN_BOTTOM & mpALIGN_BORDER_BOTTOM
					ty = w.GetScreenY() - w.GetMarginBottom() - ty - 8;
			}

			dc.DrawText(m_name, w.x2p(GetX(w.p2y(ty))), ty);
		}
	}
}

//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpFXY, mpLayer)

mpFXY::mpFXY(wxString name, int flags, bool viewAsBar)
{
	m_type = mpLAYER_PLOT;
	SetName(name);
	m_flags = flags;
	maxDrawX = minDrawX = maxDrawY = minDrawY = 0;
	m_deltaX = 1;
	SetViewMode(viewAsBar);
}

void mpFXY::SetViewMode(bool asBar)
{
	m_ViewAsBar = asBar;
	if (m_ViewAsBar)
	{
		// In bar mode, we are not continuous
		m_continuous = false;
	}
}

void mpFXY::UpdateViewBoundary(wxCoord xnew, wxCoord ynew)
{
	// Keep track of how many points have been drawn and the bouding box
	maxDrawX = (xnew > maxDrawX) ? xnew : maxDrawX;
	minDrawX = (xnew < minDrawX) ? xnew : minDrawX;
	maxDrawY = (maxDrawY > ynew) ? maxDrawY : ynew;
	minDrawY = (minDrawY < ynew) ? minDrawY : ynew;
}

void mpFXY::Plot(wxDC &dc, mpWindow &w)
{
	if (!m_visible)
		return;

	DoBeforePlot();
	UpdateContext(dc);

	double x, y;
	// Do this to reset the counters to evaluate bounding box for label positioning
	Rewind();
	// Get first point
	GetNextXY(x, y);
	maxDrawX = minDrawX = x;
	maxDrawY = minDrawY = y;

	wxCoord ix = 0, iy = 0;
	wxCoord ixlast = 0, iylast = 0;

	// Get bondaries
	m_plotBondaries = w.GetPlotBondaries(!m_drawOutsideMargins);

	if (!m_drawOutsideMargins)
	{
		wxRect rect(m_plotBondaries.startPx, m_plotBondaries.startPy, m_plotBondaries.endPx - m_plotBondaries.startPx,
				m_plotBondaries.endPy - m_plotBondaries.startPy);
		dc.SetClippingRegion(rect);
	}

	if (!m_ViewAsBar)
	{
		if (m_continuous || (m_pen.GetWidth() > 1))
		{
			if (m_continuous)
			{
				// Get first point in bound
				ixlast = w.x2p(x);
				iylast = w.y2p(y);

				while (GetNextXY(x, y))
				{
					ix = w.x2p(x);
					iy = w.y2p(y);

					dc.DrawLine(ixlast, iylast, ix, iy);
					UpdateViewBoundary(ix, iy);

					if (m_symbol != mpsNone)
						DrawSymbol(dc, ixlast, iylast);

					ixlast = ix;
					iylast = iy;
				}
				// Last point
				if (m_symbol != mpsNone)
					DrawSymbol(dc, ixlast, iylast);
			}
			else
			{
				ix = w.x2p(x);
				iy = w.y2p(y);
				while (GetNextXY(x, y))
				{
					if (m_drawOutsideMargins
							|| ((ix >= m_plotBondaries.startPx) && (ix <= m_plotBondaries.endPx) && (iy >= m_plotBondaries.startPy)
									&& (iy <= m_plotBondaries.endPy)))
					{
						if (m_symbol == mpsNone)
							dc.DrawLine(ix, iy, ix, iy);
						else
							DrawSymbol(dc, ix, iy);
						UpdateViewBoundary(ix, iy);
					}
					ix = w.x2p(x);
					iy = w.y2p(y);
				}
			}
		}
		else
		{
			// Not continuous and pen width = 1
			ix = w.x2p(x);
			iy = w.y2p(y);
			while (GetNextXY(x, y))
			{

				if (m_drawOutsideMargins
						|| ((ix >= m_plotBondaries.startPx) && (ix <= m_plotBondaries.endPx) && (iy >= m_plotBondaries.startPy)
								&& (iy <= m_plotBondaries.endPy)))
				{
					if (m_symbol == mpsNone)
						dc.DrawPoint(ix, iy);
					else
						DrawSymbol(dc, ix, iy);
					UpdateViewBoundary(ix, iy);
				}
				ix = w.x2p(x);
				iy = w.y2p(y);
			}
		}
	}
	else // View as bar
	{
		double delta = w.GetScreenX() / w.GetScaleX();
		if (m_deltaX < delta)
			delta = m_deltaX;
		int d = (int) ((delta * w.GetScaleX()) / 3.5);
		wxCoord iybase = w.y2p(0);
		ix = w.x2p(x);
		iy = w.y2p(y);
		while (GetNextXY(x, y))
		{
			if (m_drawOutsideMargins
					|| ((ix >= m_plotBondaries.startPx) && (ix <= m_plotBondaries.endPx) && (iy >= m_plotBondaries.startPy)
							&& (iy <= m_plotBondaries.endPy)))
			{
				dc.DrawRectangle(ix - d, iy, 2 * d, iybase - iy);
				UpdateViewBoundary(ix, iy);
			}
			ix = w.x2p(x);
			iy = w.y2p(y);
		}
	}

	// Destroy clipping
	if (!m_drawOutsideMargins)
	{
		dc.DestroyClippingRegion();
	}

	if (m_showName && !m_name.IsEmpty())
	{
		wxCoord tx, ty;
		dc.GetTextExtent(m_name, &tx, &ty);

		switch (m_flags)
		{
			case mpALIGN_NW:
			{
				tx = minDrawX + 8;
				ty = maxDrawY + 8;
				break;
			}
			case mpALIGN_NE:
			{
				tx = maxDrawX - tx - 8;
				ty = maxDrawY + 8;
				break;
			}
			case mpALIGN_SE:
			{
				tx = maxDrawX - tx - 8;
				ty = minDrawY - ty - 8;
				break;
			}
			default:
			{ // mpALIGN_SW
				tx = minDrawX + 8;
				ty = minDrawY - ty - 8;
			}
		}

		dc.DrawText(m_name, tx, ty);
	}
}

//-----------------------------------------------------------------------------
// mpProfile implementation
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpProfile, mpLayer)

mpProfile::mpProfile(wxString name, int flags)
{
	m_type = mpLAYER_PLOT;
	SetName(name);
	m_flags = flags;
}

void mpProfile::Plot(wxDC &dc, mpWindow &w)
{
	if (m_visible)
	{
		DoBeforePlot();
		dc.SetPen(m_pen);
		wxCoord i;

		// Get bondaries
		m_plotBondaries = w.GetPlotBondaries(!m_drawOutsideMargins);

		// Plot profile linking subsequent point of the profile, instead of mpFY, which plots simple points.
		wxCoord c0 = w.y2p(GetY(w.p2x(m_plotBondaries.startPx)));
		if (!m_drawOutsideMargins)
			c0 = (c0 <= m_plotBondaries.endPy) ? ((c0 >= m_plotBondaries.startPy) ? c0 : m_plotBondaries.startPy) : m_plotBondaries.endPy;
		for (i = m_plotBondaries.startPx + m_step; i < m_plotBondaries.endPx; i += m_step)
		{
			wxCoord c1 = w.y2p(GetY(w.p2x(i)));

			if (!m_drawOutsideMargins)
				c1 = (c1 <= m_plotBondaries.endPy) ? ((c1 >= m_plotBondaries.startPy) ? c1 : m_plotBondaries.startPy) : m_plotBondaries.endPy;

			dc.DrawLine(i - m_step, c0, i, c1);
			if (m_symbol != mpsNone)
				DrawSymbol(dc, i - m_step, c0);
			c0 = c1;
		};
		// Last point
		if (m_symbol != mpsNone)
			DrawSymbol(dc, i - m_step, c0);

		if (!m_name.IsEmpty())
		{
			dc.SetFont(m_font);
			dc.SetTextForeground(m_fontcolour);

			wxCoord tx, ty;
			dc.GetTextExtent(m_name, &tx, &ty);

			switch (m_flags)
			{
				case mpALIGN_RIGHT:
				case mpALIGN_BORDER_RIGHT:
				case mpALIGN_NE:
				case mpALIGN_SE:
					tx = (w.GetScreenX() - tx) - w.GetMarginRight() - 8;
					break;
				case mpALIGN_CENTERY:
					tx = ((w.GetPlotWidth() - tx) / 2) + w.GetMarginLeft();
					break;
				default:  // mpALIGN_LEFT & mpALIGN_BORDER_LEFT
					tx = w.GetMarginLeft() + 8;
			}

			dc.DrawText(m_name, tx, w.y2p(GetY(w.p2x(tx))));
		}
	}
}

//-----------------------------------------------------------------------------
// mpLayer implementations - furniture (scales, ...)
//-----------------------------------------------------------------------------

#define mpLN10 2.3025850929940456840179914546844

IMPLEMENT_ABSTRACT_CLASS(mpScale, mpLayer)

mpScale::mpScale(wxString name, int flags, bool grids)
{
	m_type = mpLAYER_AXIS;
	SetName(name);
	SetFont((wxFont const &) *wxSMALL_FONT);
	SetPen((wxPen const &) *wxGREY_PEN);
	m_gridpen = wxPen(*wxLIGHT_GREY, 1, wxPENSTYLE_DOT);
	m_flags = flags;
	m_ticks = true;
	m_grids = grids;
	m_auto = true;
	m_min = -1;
	m_max = 1;
	m_labelFormat = wxT("");
}

IMPLEMENT_DYNAMIC_CLASS(mpScaleX, mpScale)

void mpScaleX::Plot(wxDC &dc, mpWindow &w)
{
	if (!m_visible)
		return;

	DoBeforePlot();
	UpdateContext(dc);

	int orgy = 0;

	// Get bondaries
	m_plotBondaries = w.GetPlotBondaries(!m_drawOutsideMargins);

	switch (m_flags)
	{
		case mpALIGN_BORDER_TOP:
			orgy = 1;
			break;
		case mpALIGN_TOP:
		{
			if (m_drawOutsideMargins)
				orgy = X_BORDER_SEPARATION;
			else
				orgy = w.GetMarginTop();
			break;
		}
		case mpALIGN_CENTERX:
			orgy = w.y2p(0);
			break;
		case mpALIGN_BOTTOM:
		{
			if (m_drawOutsideMargins)
				orgy = w.GetScreenY() - X_BORDER_SEPARATION;
			else
				orgy = w.GetScreenY() - w.GetMarginBottom();
			break;
		}
		case mpALIGN_BORDER_BOTTOM:
			orgy = w.GetScreenY() - 1;
			break;
		default:
			;  // Nothing
	}

	// Draw nothing if we are outside margins
	if (!m_drawOutsideMargins && ((orgy > (w.GetScreenY() - w.GetMarginBottom())) || (orgy < w.GetMarginTop())))
		return;

	dc.DrawLine(m_plotBondaries.startPx, orgy, m_plotBondaries.endPx, orgy);

	const double scaleX = w.GetScaleX();
	const double dig = floor(log(128.0 / scaleX) / mpLN10);
	const double step = exp(mpLN10 * dig);
	const double end = w.GetPosX() + (double) w.GetScreenX() / scaleX;

	wxCoord tx, ty;
	wxString s;
	wxString fmt;
	int tmp = (int) dig;
	if (m_labelType == mpX_NORMAL)
	{
		if (!m_labelFormat.IsEmpty())
		{
			fmt = m_labelFormat;
		}
		else
		{
			if (tmp >= 1)
			{
				fmt = wxT("%.g");
			}
			else
			{
				tmp = 8 - tmp;
				fmt.Printf(wxT("%%.%dg"), tmp >= -1 ? 2 : -tmp);
			}
		}
	}
	else
	{
		// Date and/or time axis representation
		if (m_labelType == mpX_DATETIME)
		{
			fmt = (wxT("%04.0f-%02.0f-%02.0fT%02.0f:%02.0f:%02.0f"));
		}
		else
			if (m_labelType == mpX_DATE)
			{
				fmt = (wxT("%04.0f-%02.0f-%02.0f"));
			}
			else
				if ((m_labelType == mpX_TIME) && (end / 60 < 2))
				{
					fmt = (wxT("%02.0f:%02.3f"));
				}
				else
				{
					fmt = (wxT("%02.0f:%02.0f:%02.0f"));
				}
	}

//		double n0 = floor( (w.GetPosX() - (double)w.GetScreenX() / scaleX) / step ) * step;
	double n0 = floor(w.GetPosX() / step) * step;
	double n = 0;
#ifdef MATHPLOT_DO_LOGGING
	wxLogMessage(wxT("mpScaleX::Plot: dig: %f , step: %f, end: %f, n: %f"), dig, step, end, n0);
#endif

	tmp = -65535;
	int labelH = 0; // Control labels heigth to decide where to put axis name (below labels or on top of axis)
	int maxExtent = 0;
	time_t when = 0;
	struct tm timestruct;
	for (n = n0; n < end; n += step)
	{
		const int p = (int) ((n - w.GetPosX()) * scaleX);
#ifdef MATHPLOT_DO_LOGGING
		wxLogMessage(wxT("mpScaleX::Plot: n: %f -> p = %d"), n, p);
#endif
		if ((p >= m_plotBondaries.startPx) && (p <= m_plotBondaries.endPx))
		{
			if (m_ticks)
			{ // draw axis ticks
				dc.SetPen(m_pen);
				if (m_flags == mpALIGN_BORDER_BOTTOM)
					dc.DrawLine(p, orgy, p, orgy - 4);
				else
					dc.DrawLine(p, orgy, p, orgy + 4);
			}
			if (m_grids)
			{ // draw grid
				dc.SetPen(m_gridpen);
				dc.DrawLine(p, m_plotBondaries.startPy, p, m_plotBondaries.endPy);
			}
			// Write ticks labels in s string
			if (m_labelType == mpX_NORMAL)
				s.Printf(fmt, n);
			else
				if (m_labelType == mpX_DATETIME)
				{
					when = (time_t) n;
					if (when > 0)
					{
						if (m_timeConv == mpX_LOCALTIME)
						{
							timestruct = *localtime(&when);
						}
						else
						{
							timestruct = *gmtime(&when);
						}
						s.Printf(fmt, (double) timestruct.tm_year + 1900, (double) timestruct.tm_mon + 1, (double) timestruct.tm_mday,
								(double) timestruct.tm_hour, (double) timestruct.tm_min, (double) timestruct.tm_sec);
					}
				}
				else
					if (m_labelType == mpX_DATE)
					{
						when = (time_t) n;
						if (when > 0)
						{
							if (m_timeConv == mpX_LOCALTIME)
							{
								timestruct = *localtime(&when);
							}
							else
							{
								timestruct = *gmtime(&when);
							}
							s.Printf(fmt, (double) timestruct.tm_year + 1900, (double) timestruct.tm_mon + 1, (double) timestruct.tm_mday);
						}
					}
					else
						if ((m_labelType == mpX_TIME) || (m_labelType == mpX_HOURS))
						{
							double modulus = fabs(n);
							double sign = n / modulus;
							double hh = floor(modulus / 3600);
							double mm = floor((modulus - hh * 3600) / 60);
							double ss = modulus - hh * 3600 - mm * 60;
#ifdef MATHPLOT_DO_LOGGING
							wxLogMessage(wxT("%02.0f Hours, %02.0f minutes, %02.0f seconds"), sign * hh, mm, ss);
#endif // MATHPLOT_DO_LOGGING
							if (fmt.Len() == 20) // Format with hours has 11 chars
								s.Printf(fmt, sign * hh, mm, floor(ss));
							else
								s.Printf(fmt, sign * mm, ss);
						}
			dc.GetTextExtent(s, &tx, &ty);
			labelH = (labelH <= ty) ? ty : labelH;
			/*				if ((p-tx/2-tmp) > 64) { // Problem about non-regular axis labels
			 if ((m_flags == mpALIGN_BORDER_BOTTOM) || (m_flags == mpALIGN_TOP)) {
			 dc.DrawText( s, p-tx/2, orgy-4-ty);
			 } else {
			 dc.DrawText( s, p-tx/2, orgy+4);
			 }
			 tmp=p+tx/2;
			 }
			 */
			maxExtent = (tx > maxExtent) ? tx : maxExtent; // Keep in mind max label width
		}
	}

	// Actually draw labels, taking care of not overlapping them, and distributing them regularly
	double labelStep = ceil((maxExtent + mpMIN_X_AXIS_LABEL_SEPARATION) / (scaleX * step)) * step;

	for (n = n0; n < end; n += labelStep)
	{
		// To have a real zero
		if (fabs(n) < 1e-10)
			n = 0;
		const int p = (int) ((n - w.GetPosX()) * scaleX);
#ifdef MATHPLOT_DO_LOGGING
		wxLogMessage(wxT("mpScaleX::Plot: n_label = %f -> p_label = %d"), n, p);
#endif
		if ((p >= m_plotBondaries.startPx) && (p <= m_plotBondaries.endPx))
		{
			// Write ticks labels in s string
			if (m_labelType == mpX_NORMAL)
				s.Printf(fmt, n);
			else
				if (m_labelType == mpX_DATETIME)
				{
					when = (time_t) n;
					if (when > 0)
					{
						if (m_timeConv == mpX_LOCALTIME)
						{
							timestruct = *localtime(&when);
						}
						else
						{
							timestruct = *gmtime(&when);
						}
						s.Printf(fmt, (double) timestruct.tm_year + 1900, (double) timestruct.tm_mon + 1, (double) timestruct.tm_mday,
								(double) timestruct.tm_hour, (double) timestruct.tm_min, (double) timestruct.tm_sec);
					}
				}
				else
					if (m_labelType == mpX_DATE)
					{
						when = (time_t) n;
						if (when > 0)
						{
							if (m_timeConv == mpX_LOCALTIME)
							{
								timestruct = *localtime(&when);
							}
							else
							{
								timestruct = *gmtime(&when);
							}
							s.Printf(fmt, (double) timestruct.tm_year + 1900, (double) timestruct.tm_mon + 1, (double) timestruct.tm_mday);
						}
					}
					else
						if ((m_labelType == mpX_TIME) || (m_labelType == mpX_HOURS))
						{
							double modulus = fabs(n);
							double sign = n / modulus;
							double hh = floor(modulus / 3600);
							double mm = floor((modulus - hh * 3600) / 60);
							double ss = modulus - hh * 3600 - mm * 60;
#ifdef MATHPLOT_DO_LOGGING
							wxLogMessage(wxT("%02.0f Hours, %02.0f minutes, %02.0f seconds"), sign * hh, mm, ss);
#endif // MATHPLOT_DO_LOGGING
							if (fmt.Len() == 20) // Format with hours has 11 chars
								s.Printf(fmt, sign * hh, mm, floor(ss));
							else
								s.Printf(fmt, sign * mm, ss);
						}
			dc.GetTextExtent(s, &tx, &ty);
			if ((m_flags == mpALIGN_BORDER_BOTTOM) || (m_flags == mpALIGN_TOP))
			{
				dc.DrawText(s, p - tx / 2, orgy - ty - 4);
			}
			else
			{
				dc.DrawText(s, p - tx / 2, orgy + 4);
			}
		}
	}

	// Draw axis name
	dc.GetTextExtent(m_name, &tx, &ty);
	switch (m_flags)
	{
		case mpALIGN_BORDER_BOTTOM:
			dc.DrawText(m_name, m_plotBondaries.endPx - tx - 4, orgy - ty - labelH - 8);
			break;
		case mpALIGN_BOTTOM:
		{
			if ((!m_drawOutsideMargins) && (w.GetMarginBottom() > (ty + labelH + 8)))
			{
//				dc.DrawText(m_name, (m_plotBondaries.endPx + m_plotBondaries.startPx - tx) >> 1, orgy + labelH + 6);
				dc.DrawText(m_name, m_plotBondaries.endPx - tx - 4, orgy + labelH + 6);
			}
			else
			{
				dc.DrawText(m_name, m_plotBondaries.endPx - tx - 4, orgy - ty - 4);
			}
			break;
		}
		case mpALIGN_CENTERX:
			dc.DrawText(m_name, m_plotBondaries.endPx - tx - 4, orgy - ty - 4);
			break;
		case mpALIGN_TOP:
		{
			if ((!m_drawOutsideMargins) && (w.GetMarginTop() > (ty + labelH + 8)))
			{
//				dc.DrawText(m_name, (m_plotBondaries.endPx + m_plotBondaries.startPx - tx) >> 1, orgy - ty - labelH - 6);
				dc.DrawText(m_name, m_plotBondaries.endPx - tx - 4, orgy - ty - labelH - 8);
			}
			else
			{
				dc.DrawText(m_name, m_plotBondaries.endPx - tx - 4, orgy + 4);
			}
			break;
		}
		case mpALIGN_BORDER_TOP:
			dc.DrawText(m_name, m_plotBondaries.endPx - tx - 4, orgy + labelH + 6);
			break;
		default:
			break;
	}
}

IMPLEMENT_DYNAMIC_CLASS(mpScaleY, mpScale)

void mpScaleY::Plot(wxDC &dc, mpWindow &w)
{
	if (!m_visible)
		return;

	DoBeforePlot();
	UpdateContext(dc);

	int orgx = 0;

	// Get bondaries
	m_plotBondaries = w.GetPlotBondaries(!m_drawOutsideMargins);

	switch (m_flags)
	{
		case mpALIGN_BORDER_LEFT:
			orgx = 1;
			break;
		case mpALIGN_LEFT:
		{
			if (m_drawOutsideMargins)
				orgx = Y_BORDER_SEPARATION;
			else
				orgx = w.GetMarginLeft();
			break;
		}
		case mpALIGN_CENTERY:
			orgx = w.x2p(0);
			break;
		case mpALIGN_RIGHT:
		{
			if (m_drawOutsideMargins)
				orgx = w.GetScreenX() - Y_BORDER_SEPARATION;
			else
				orgx = w.GetScreenX() - w.GetMarginRight();
			break;
		}
		case mpALIGN_BORDER_RIGHT:
			orgx = w.GetScreenX() - 1;
			break;
		default:
			;  // Nothing
	}

	// Draw nothing if we are outside margins
	if (!m_drawOutsideMargins && ((orgx > (w.GetScreenX() - w.GetMarginRight())) || (orgx + 1 < w.GetMarginLeft())))
		return;

	// Draw line
	dc.DrawLine(orgx + 1, m_plotBondaries.startPy, orgx + 1, m_plotBondaries.endPy);

	const double scaleY = w.GetScaleY();
	const double dig = floor(log(128.0 / scaleY) / mpLN10);
	const double step = exp(mpLN10 * dig);
	const double end = w.GetPosY() + (double) w.GetScreenY() / scaleY;

	wxCoord tx = 0, ty = 0;
	wxString s;
	wxString fmt;
	int tmp = (int) dig;
	double maxScaleAbs = fabs(w.GetDesiredYmax());
	double minScaleAbs = fabs(w.GetDesiredYmin());
	double endscale = (maxScaleAbs > minScaleAbs) ? maxScaleAbs : minScaleAbs;
	if (m_labelFormat.IsEmpty())
	{
		if ((endscale < 1e4) && (endscale > 1e-3))
			fmt = wxT("%.2f");
		else
			fmt = wxT("%.2e");
	}
	else
	{
		fmt = m_labelFormat;
	}

	double n = floor((w.GetPosY() - (double) (w.GetScreenY()) / scaleY) / step) * step;

	tmp = 65536;
	int labelW = 0;
	// Before staring cycle, calculate label height
	int labelHeigth = 0;
	s.Printf(fmt, n);
	dc.GetTextExtent(s, &tx, &labelHeigth);
	labelHeigth /= 2;
	for (; n < end; n += step)
	{
		// To have a real zero
		if (fabs(n) < 1e-10)
			n = 0;
		const int p = (int) ((w.GetPosY() - n) * scaleY);
		if ((p > m_plotBondaries.startPy + labelHeigth) && (p < m_plotBondaries.endPy - labelHeigth))
		{
			if (m_ticks)
			{ // Draw axis ticks
				dc.SetPen(m_pen);
				if (m_flags == mpALIGN_BORDER_LEFT)
				{
					dc.DrawLine(orgx, p, orgx + 4, p);
				}
				else
				{
					dc.DrawLine(orgx - 4, p, orgx, p);
				}
			}
			if (m_grids)
			{ // Draw axis grids
				dc.SetPen(m_gridpen);
				dc.DrawLine(m_plotBondaries.startPx, p, m_plotBondaries.endPx, p);
			}

			// Print ticks labels
			s.Printf(fmt, n);
			dc.GetTextExtent(s, &tx, &ty);
#ifdef MATHPLOT_DO_LOGGING
			if (ty != labelHeigth)
				wxLogMessage(wxT("mpScaleY::Plot: ty(%d) and labelHeigth(%d) differ!"), ty, labelHeigth);
#endif
			labelW = (labelW <= tx) ? tx : labelW;
			if ((tmp - p + labelHeigth) > mpMIN_Y_AXIS_LABEL_SEPARATION)
			{
				if ((m_flags == mpALIGN_BORDER_LEFT) || (m_flags == mpALIGN_RIGHT))
					dc.DrawText(s, orgx + 4, p - ty / 2);
				else
					dc.DrawText(s, orgx - tx - 4, p - ty / 2);
				tmp = p - labelHeigth;
			}
		}
	}

	// Draw axis name
	dc.GetTextExtent(m_name, &tx, &ty);
	switch (m_flags)
	{
		case mpALIGN_BORDER_LEFT:
			dc.DrawText(m_name, labelW + 8, m_plotBondaries.startPy + 4);
			break;
		case mpALIGN_LEFT:
		{
			if ((!m_drawOutsideMargins) && (w.GetMarginLeft() > (ty + labelW + 8)))
			{
				dc.DrawRotatedText(m_name, orgx - labelW - ty - 6, (m_plotBondaries.endPy + m_plotBondaries.startPy + tx) >> 1, 90);
			}
			else
			{
				dc.DrawText(m_name, orgx + 4, m_plotBondaries.startPy + 4);
			}
			break;
		}
		case mpALIGN_CENTERY:
			dc.DrawText(m_name, orgx + 4, m_plotBondaries.startPy + 4);
			break;
		case mpALIGN_RIGHT:
		{
			if ((!m_drawOutsideMargins) && (w.GetMarginRight() > (ty + labelW + 8)))
			{
				dc.DrawRotatedText(m_name, orgx + labelW + 6, (m_plotBondaries.endPy + m_plotBondaries.startPy + tx) >> 1, 90);
			}
			else
			{
				dc.DrawText(m_name, orgx - tx - 4, m_plotBondaries.startPy + 4);
			}
			break;
		}
		case mpALIGN_BORDER_RIGHT:
			dc.DrawText(m_name, orgx - tx - labelW - 6, m_plotBondaries.startPy + 4);
			break;
		default:
			break;
	}
}

//-----------------------------------------------------------------------------
// mpWindow
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpWindow, wxWindow)

BEGIN_EVENT_TABLE(mpWindow, wxWindow)
EVT_PAINT(mpWindow::OnPaint)
EVT_SIZE(mpWindow::OnSize)
EVT_SCROLLWIN_THUMBTRACK(mpWindow::OnScrollThumbTrack)
EVT_SCROLLWIN_PAGEUP(mpWindow::OnScrollPageUp)
EVT_SCROLLWIN_PAGEDOWN(mpWindow::OnScrollPageDown)
EVT_SCROLLWIN_LINEUP(mpWindow::OnScrollLineUp)
EVT_SCROLLWIN_LINEDOWN(mpWindow::OnScrollLineDown)
EVT_SCROLLWIN_TOP(mpWindow::OnScrollTop)
EVT_SCROLLWIN_BOTTOM(mpWindow::OnScrollBottom)

EVT_MIDDLE_UP(mpWindow::OnShowPopupMenu)
EVT_RIGHT_DOWN(mpWindow::OnMouseRightDown) // JLB
EVT_RIGHT_UP (mpWindow::OnShowPopupMenu)
EVT_MOUSEWHEEL(mpWindow::OnMouseWheel )// JLB
EVT_MOTION(mpWindow::OnMouseMove)// JLB
EVT_LEAVE_WINDOW(mpWindow::OnMouseLeave)
EVT_LEFT_DOWN(mpWindow::OnMouseLeftDown)
EVT_LEFT_UP(mpWindow::OnMouseLeftRelease)

EVT_MENU(mpID_CENTER, mpWindow::OnCenter)
EVT_MENU(mpID_FIT, mpWindow::OnFit)
EVT_MENU(mpID_TOGGLE_GRID, mpWindow::OnToggleGrids)
EVT_MENU(mpID_TOGGLE_COORD, mpWindow::OnToggleCoords)
EVT_MENU(mpID_SCREENSHOT, mpWindow::OnScreenShot)
EVT_MENU(mpID_CONFIG, mpWindow::OnConfiguration)
EVT_MENU(mpID_LOAD_FILE, mpWindow::OnLoadFile)
EVT_MENU(mpID_ZOOM_IN, mpWindow::OnZoomIn)
EVT_MENU(mpID_ZOOM_OUT, mpWindow::OnZoomOut)
EVT_MENU(mpID_LOCKASPECT, mpWindow::OnLockAspect)
EVT_MENU(mpID_HELP_MOUSE, mpWindow::OnMouseHelp)
EVT_MENU(mpID_FULLSCREEN, mpWindow::OnFullScreen)
END_EVENT_TABLE()

mpWindow::mpWindow(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long flag) :
		wxWindow(parent, id, pos, size, flag, wxT("Mathplot"))
{
	// Fullscreen only if parent is a frame
	wxString classname = parent->GetClassInfo()->GetClassName();
	if (classname.IsSameAs(_("wxFrame")))
		m_parent = (wxFrame*) parent;
	else
		m_parent = NULL;
	m_fullscreen = false;

	SetBackgroundStyle(wxBG_STYLE_PAINT);

	InitParameters();

	// For unix screenshoot
#ifdef _WIN32
#else
	wxImage::AddHandler(new wxPNGHandler);
#endif

#ifdef USE_ICON
	wxImage::AddHandler(new wxPNGHandler);

	wxBitmap icon[] = {wxBITMAP_PNG_FROM_DATA(Center_24x24), wxBITMAP_PNG_FROM_DATA(Fit_24x24),
			wxBITMAP_PNG_FROM_DATA(Zoom_in_24x24), wxBITMAP_PNG_FROM_DATA(Zoom_out_24x24),
			wxBITMAP_PNG_FROM_DATA(Screenshot_24x24), wxBITMAP_PNG_FROM_DATA(Grid_24x24),
			wxBITMAP_PNG_FROM_DATA(Coordinates_24x24), wxBITMAP_PNG_FROM_DATA(Config_24x24),
			wxBITMAP_PNG_FROM_DATA(Load_24x24), wxBITMAP_PNG_FROM_DATA(Mouse_24x24)
	};

	// Because of GTK problem, we must set bitmap BEFORE append the menu item
	wxMenuItem *mymenu = new wxMenuItem(&m_popmenu, mpID_CENTER, _T("Center to this position"), _T("Center plot view to this position"));
	mymenu->SetBitmap(icon[0]);
	m_popmenu.Append(mymenu);
	mymenu = new wxMenuItem(&m_popmenu, mpID_FIT, _T("Fit"), _T("Set plot view to show all items"));
	mymenu->SetBitmap(icon[1]);
	m_popmenu.Append(mymenu);
	mymenu = new wxMenuItem(&m_popmenu, mpID_ZOOM_IN, _T("Zoom in"), _T("Zoom in plot view."));
	mymenu->SetBitmap(icon[2]);
	m_popmenu.Append(mymenu);
	mymenu = new wxMenuItem(&m_popmenu, mpID_ZOOM_OUT, _T("Zoom out"), _T("Zoom out plot view."));
	mymenu->SetBitmap(icon[3]);
	m_popmenu.Append(mymenu);
	m_popmenu.AppendCheckItem(mpID_LOCKASPECT, _T("Lock aspect"), _T("Lock horizontal and vertical zoom aspect."));
	mymenu = new wxMenuItem(&m_popmenu, mpID_SCREENSHOT, _T("Screen shot"), _T("Copy a screen shot to the clipboard"));
	mymenu->SetBitmap(icon[4]);
	m_popmenu.Append(mymenu);
	mymenu = new wxMenuItem(&m_popmenu, mpID_TOGGLE_GRID, _T("Toggle grids"), _T("Show/Hide grids"));
	mymenu->SetBitmap(icon[5]);
	m_popmenu.Append(mymenu);
	mymenu = new wxMenuItem(&m_popmenu, mpID_TOGGLE_COORD, _T("Toggle info coords"), _T("Show/Hide info coordinates"));
	mymenu->SetBitmap(icon[6]);
	m_popmenu.Append(mymenu);
	mymenu = new wxMenuItem(&m_popmenu, mpID_CONFIG, _T("Configuration"), _T("Plot configuration"));
	mymenu->SetBitmap(icon[7]);
	m_popmenu.Append(mymenu);
	mymenu = new wxMenuItem(&m_popmenu, mpID_LOAD_FILE, _T("Load file"), _T("Load data file"));
	mymenu->SetBitmap(icon[8]);
	m_popmenu.Append(mymenu);
	mymenu = new wxMenuItem(&m_popmenu, mpID_HELP_MOUSE, _T("Show mouse commands ..."), _T("Show help about the mouse commands."));
	mymenu->SetBitmap(icon[9]);
	m_popmenu.Append(mymenu);
	if (m_parent)
	{
		mymenu = new wxMenuItem(&m_popmenu, mpID_FULLSCREEN, _T("Toggle fullscreen"), _T("Toggle fullscreen."));
//		mymenu->SetBitmap(icon[10]);
		m_popmenu.Append(mymenu);
	}
#else
	m_popmenu.Append(mpID_CENTER, _T("Center to this position"), _T("Center plot view to this position"));
	m_popmenu.Append(mpID_FIT, _T("Fit"), _T("Set plot view to show all items"));
	m_popmenu.Append(mpID_ZOOM_IN, _T("Zoom in"), _T("Zoom in plot view."));
	m_popmenu.Append(mpID_ZOOM_OUT, _T("Zoom out"), _T("Zoom out plot view."));
	m_popmenu.AppendCheckItem(mpID_LOCKASPECT, _T("Lock aspect"), _T("Lock horizontal and vertical zoom aspect."));
	m_popmenu.Append(mpID_SCREENSHOT, _T("Screen shot"), _T("Copy a screen shot to the clipboard"));
	m_popmenu.Append(mpID_TOGGLE_GRID, _T("Toggle grids"), _T("Show/Hide grids"));
	m_popmenu.Append(mpID_TOGGLE_COORD, _T("Toggle info coords"), _T("Show/Hide info coordinates"));
	m_popmenu.Append(mpID_CONFIG, _T("Configuration"), _T("Plot configuration"));
	m_popmenu.Append(mpID_LOAD_FILE, _T("Load file"), _T("Load data file"));
	m_popmenu.Append(mpID_HELP_MOUSE, _T("Show mouse commands..."), _T("Show help about the mouse commands."));
	if (m_parent)
		m_popmenu.Append(mpID_FULLSCREEN, _T("Toggle fullscreen"), _T("Toggle fullscreen."));
#endif

	m_layers.clear();
	SetBackgroundColour(*wxWHITE);
	m_bgColour = *wxWHITE;
	m_fgColour = *wxBLACK;
	m_drawBox = true;

	m_enableScrollBars = false;
	SetSizeHints(128, 128);

	UpdateAll();
}

mpWindow::~mpWindow()
{
	// Free all the layers:
	DelAllLayers(true, false);

	if (m_buff_bmp)
	{
		delete m_buff_bmp;
		m_buff_bmp = NULL;
	}
	if (m_Screenshot_bmp)
	{
		delete m_Screenshot_bmp;
		m_Screenshot_bmp = NULL;
	}
	if (m_configWindow)
	{
		delete m_configWindow;
		m_configWindow = NULL;
	}
}

void mpWindow::InitParameters()
{
	m_scaleX = m_scaleY = 1.0;
	m_posX = m_posY = 0;
	m_desired.Xmin = m_desired.Ymin = 0;
	m_desired.Xmax = m_desired.Ymax = 0;
	m_scrX = m_scrY = 64; // Fixed from m_scrX = m_scrX = 64;
	m_bound.Xmin = m_bound.Ymin = 0;
	m_bound.Xmax = m_bound.Ymax = 0;
	m_last_lx = m_last_ly = 0;
	m_XAxis = NULL;
	m_YAxis = NULL;
	m_repainting = false;
	m_buff_bmp = NULL;
	m_Screenshot_bmp = NULL;
	m_enableDoubleBuffer = true;
	m_enableMouseNavigation = true;
	m_mouseMovedAfterRightClick = false;
	m_movingInfoLayer = NULL;
	m_InfoCoords = NULL;
	// Set all margins to 50
	SetMargins(50, 50, 50, 50);

	m_lockaspect = false;
}

// Mouse handler, for detecting when the user drag with the right button or just "clicks" for the menu
// JLB
void mpWindow::OnMouseRightDown(wxMouseEvent &event)
{
	m_mouseMovedAfterRightClick = false;
	m_mouseRClick_X = event.GetX();
	m_mouseRClick_Y = event.GetY();
	if (m_enableMouseNavigation)
	{
		SetCursor(*wxCROSS_CURSOR);
	}
}

// Process mouse wheel events
// JLB
void mpWindow::OnMouseWheel(wxMouseEvent &event)
{
	if (!m_enableMouseNavigation)
	{
		event.Skip();
		return;
	}

	// Zoom in and out
	if (!event.m_controlDown && !event.m_shiftDown)
	{
		wxPoint clickPt(event.GetX(), event.GetY());
		// CTRL key hold: Zoom in/out:
		if (event.GetWheelRotation() > 0)
			ZoomIn(clickPt);
		else
			ZoomOut(clickPt);
	}
	else
	{
		// Scroll vertically or horizontally (this is SHIFT is hold down).
		int change = -event.GetWheelRotation(); // Opposite direction (More intuitive)!
		double changeUnitsX = change / m_scaleX;
		double changeUnitsY = change / m_scaleY;

		if (event.m_shiftDown)
		{
			m_posX += changeUnitsX;
			m_desired.Xmax += changeUnitsX;
			m_desired.Xmin += changeUnitsX;
		}
		else
			if (event.m_controlDown)
			{
				m_posY -= changeUnitsY;
				m_desired.Ymax -= changeUnitsY;
				m_desired.Ymax -= changeUnitsY;
			}

		UpdateAll();
	}
}

// If the user "drags" with the right buttom pressed, do "pan"
// JLB
void mpWindow::OnMouseMove(wxMouseEvent &event)
{
	if (!m_enableMouseNavigation)
	{
		event.Skip();
		return;
	}

	if (event.m_rightDown)
	{
		m_mouseMovedAfterRightClick = true; // Hides the popup menu after releasing the button!

		// The change:
		int Ax = m_mouseRClick_X - event.GetX();
		int Ay = m_mouseRClick_Y - event.GetY();

		// For the next event, use relative to this coordinates.
		m_mouseRClick_X = event.GetX();
		m_mouseRClick_Y = event.GetY();

		double Ax_units = Ax / m_scaleX;
		double Ay_units = -Ay / m_scaleY;

		m_posX += Ax_units;
		m_posY += Ay_units;
		m_desired.Xmax += Ax_units;
		m_desired.Xmin += Ax_units;
		m_desired.Ymax += Ay_units;
		m_desired.Ymin += Ay_units;

		UpdateAll();

#ifdef MATHPLOT_DO_LOGGING
		wxLogMessage(wxT("[mpWindow::OnMouseMove] Ax:%i Ay:%i m_posX:%f m_posY:%f"), Ax, Ay, m_posX, m_posY);
#endif
	}
	else
	{
		if (event.m_leftDown)
		{
			if (m_movingInfoLayer == NULL)
			{
				wxClientDC dc(this);
				wxPen pen(*wxBLACK, 1, wxPENSTYLE_DOT);  // wxDOT
				dc.SetPen(pen);
				dc.SetBrush(*wxTRANSPARENT_BRUSH);
				dc.DrawRectangle(m_mouseLClick_X, m_mouseLClick_Y, event.GetX() - m_mouseLClick_X, event.GetY() - m_mouseLClick_Y);
			}
			else
			{
				wxPoint moveVector(event.GetX() - m_mouseLClick_X, event.GetY() - m_mouseLClick_Y);
				m_movingInfoLayer->Move(moveVector);
			}
			UpdateAll();
		}
		else
		{
			// Mouse move coordinate
			if (m_InfoCoords && m_InfoCoords->IsVisible())
			{
				m_InfoCoords->UpdateInfo(*this, event);
				wxClientDC dc(this);
				m_InfoCoords->Plot(dc, *this);
			}

			/* if (m_coordTooltip) {
			 wxString toolTipContent;
			 toolTipContent.Printf(_("X = %f\nY = %f"), p2x(event.GetX()), p2y(event.GetY()));
			 wxTipWindow** ptr = NULL;
			 wxRect rectBounds(event.GetX(), event.GetY(), 5, 5);
			 wxTipWindow* tip = new wxTipWindow(this, toolTipContent, 100, ptr, &rectBounds);

			 } */
		}
	}
	event.Skip();
}

void mpWindow::OnMouseLeave(wxMouseEvent& WXUNUSED(event))
{
	if (m_InfoCoords && m_InfoCoords->IsVisible())
	{
		wxClientDC dc(this);
		m_InfoCoords->ErasePlot(dc, *this);
	}
}

void mpWindow::OnMouseLeftDown(wxMouseEvent &event)
{
	m_mouseLClick_X = event.GetX();
	m_mouseLClick_Y = event.GetY();
#ifdef MATHPLOT_DO_LOGGING
	wxLogMessage(wxT("mpWindow::OnMouseLeftDown() X = %d , Y = %d"), event.GetX(), event.GetY());
#endif
	wxPoint pointClicked = event.GetPosition();
	m_movingInfoLayer = IsInsideInfoLayer(pointClicked);
#ifdef MATHPLOT_DO_LOGGING
	if (m_movingInfoLayer != NULL)
	{
		wxLogMessage(wxT("mpWindow::OnMouseLeftDown() started moving layer %p"), m_movingInfoLayer);
	}
#endif
	event.Skip();
}

void mpWindow::OnMouseLeftRelease(wxMouseEvent &event)
{
	wxPoint release(event.GetX(), event.GetY());
	wxPoint press(m_mouseLClick_X, m_mouseLClick_Y);
	if (m_movingInfoLayer != NULL)
	{
		m_movingInfoLayer->UpdateReference();
		m_movingInfoLayer = NULL;
	}
	else
	{
		if (release != press)
		{
			ZoomRect(press, release);
		} /*else {
		 if (m_coordTooltip) {
		 wxString toolTipContent;
		 toolTipContent.Printf(_("X = %f\nY = %f"), p2x(event.GetX()), p2y(event.GetY()));
		 SetToolTip(toolTipContent);
		 }
		 } */
	}
	event.Skip();
}

void mpWindow::Fit()
{
	if (UpdateBBox())
	{
		mpFloatRect bound = m_bound;
		if (m_XAxis && (!m_XAxis->GetAuto()))
		{
			bound.Xmin = m_XAxis->GetMinScale();
			bound.Xmax = m_XAxis->GetMaxScale();
		}
		if (m_YAxis && (!m_YAxis->GetAuto()))
		{
			bound.Ymin = m_YAxis->GetMinScale();
			bound.Ymax = m_YAxis->GetMaxScale();
		}
		Fit(bound);
	}
}

// JL
void mpWindow::Fit(const mpFloatRect &rect, wxCoord *printSizeX, wxCoord *printSizeY)
{
	// Save desired borders:
	m_desired = rect;

	if (printSizeX != NULL && printSizeY != NULL)
	{
		// Printer:
		SetScreen(*printSizeX, *printSizeY);
	}
	else
	{
		// Normal case (screen):
		int h, w;
		GetClientSize(&w, &h);
		SetScreen(w, h);
	}

	double Ax, Ay;

	Ax = rect.Xmax - rect.Xmin;
	Ay = rect.Ymax - rect.Ymin;

	m_scaleX = ISNOTNULL(Ax) ? m_plotWidth / Ax : 1;
	m_scaleY = ISNOTNULL(Ay) ? m_plotHeight / Ay : 1;

	if (m_lockaspect)
	{
#ifdef MATHPLOT_DO_LOGGING
		wxLogMessage(wxT("mpWindow::Fit()(lock) m_scaleX=%f,m_scaleY=%f"), m_scaleX, m_scaleY);
#endif
		// Keep the lowest "scale" to fit the whole range required by that axis (to actually "fit"!):
		double s = m_scaleX < m_scaleY ? m_scaleX : m_scaleY;
		m_scaleX = s;
		m_scaleY = s;
	}

	// Adjusts corner coordinates: This should be simply:
	//   m_posX = m_minX;
	//   m_posY = m_maxY;
	// But account for centering if we have lock aspect:
	m_posX = (rect.Xmin + rect.Xmax) / 2 - (m_plotWidth / 2 + m_margin.left) / m_scaleX;
	m_posY = (rect.Ymin + rect.Ymax) / 2 + (m_plotHeight / 2 + m_margin.top) / m_scaleY;

#ifdef MATHPLOT_DO_LOGGING
	wxLogMessage(wxT("mpWindow::Fit() m_desired.Xmin=%f m_desired.Xmax=%f  m_desired.Ymin=%f m_desired.Ymax=%f"),
			m_desired.Xmin, m_desired.Xmax, m_desired.Ymin, m_desired.Ymax);
	wxLogMessage(wxT("mpWindow::Fit() m_scaleX = %f , m_scrX = %d,m_scrY=%d, Ax=%f, Ay=%f, m_posX=%f, m_posY=%f"),
			m_scaleX, m_scrX, m_scrY, Ax, Ay, m_posX, m_posY);
#endif

	// It is VERY IMPORTANT to DO NOT call Refresh if we are drawing to the printer!!
	// Otherwise, the DC dimensions will be those of the window instead of the printer device
	if (printSizeX == NULL || printSizeY == NULL)
		UpdateAll();
}

// Patch ngpaton
void mpWindow::DoZoomInXCalc(const int staticXpixel)
{
	// Preserve the position of the clicked point:
	double staticX = p2x(staticXpixel);
	// Zoom in:
	m_scaleX = m_scaleX * zoomIncrementalFactor;
	// Adjust the new m_posx
	m_posX = staticX - (staticXpixel / m_scaleX);
	// Adjust desired
	m_desired.Xmin = m_posX;
	m_desired.Xmax = m_posX + m_plotWidth / m_scaleX;
#ifdef MATHPLOT_DO_LOGGING
	wxLogMessage(wxT("mpWindow::DoZoomInXCalc() prior X coord: (%f), new X coord: (%f) SHOULD BE EQUAL!!"), staticX, p2x(staticXpixel));
#endif
}

void mpWindow::DoZoomInYCalc(const int staticYpixel)
{
	// Preserve the position of the clicked point:
	double staticY = p2y(staticYpixel);
	// Zoom in:
	m_scaleY = m_scaleY * zoomIncrementalFactor;
	// Adjust the new m_posy:
	m_posY = staticY + (staticYpixel / m_scaleY);
	// Adjust desired
	m_desired.Ymax = m_posY;
	m_desired.Ymin = m_posY - m_plotHeight / m_scaleY;
#ifdef MATHPLOT_DO_LOGGING
	wxLogMessage(wxT("mpWindow::DoZoomInYCalc() prior Y coord: (%f), new Y coord: (%f) SHOULD BE EQUAL!!"), staticY, p2y(staticYpixel));
#endif
}

void mpWindow::DoZoomOutXCalc(const int staticXpixel)
{
	// Preserve the position of the clicked point:
	double staticX = p2x(staticXpixel);
	// Zoom out:
	m_scaleX = m_scaleX / zoomIncrementalFactor;
	// Adjust the new m_posx/y:
	m_posX = staticX - (staticXpixel / m_scaleX);
	// Adjust desired
	m_desired.Xmin = m_posX;
	m_desired.Xmax = m_posX + m_plotWidth / m_scaleX;
#ifdef MATHPLOT_DO_LOGGING
	wxLogMessage(wxT("mpWindow::DoZoomOutXCalc() prior X coord: (%f), new X coord: (%f) SHOULD BE EQUAL!!"), staticX, p2x(staticXpixel));
#endif
}

void mpWindow::DoZoomOutYCalc(const int staticYpixel)
{
	// Preserve the position of the clicked point:
	double staticY = p2y(staticYpixel);
	// Zoom out:
	m_scaleY = m_scaleY / zoomIncrementalFactor;
	// Adjust the new m_posx/y:
	m_posY = staticY + (staticYpixel / m_scaleY);
	// Adjust desired
	m_desired.Ymax = m_posY;
	m_desired.Ymin = m_posY - m_plotHeight / m_scaleY;
#ifdef MATHPLOT_DO_LOGGING
	wxLogMessage(wxT("mpWindow::DoZoomOutYCalc() prior Y coord: (%f), new Y coord: (%f) SHOULD BE EQUAL!!"), staticY, p2y(staticYpixel));
#endif
}

void mpWindow::ZoomIn(const wxPoint &centerPoint)
{
	wxPoint c(centerPoint);
	if (c == wxDefaultPosition)
	{
		int h, w;
		GetClientSize(&w, &h);
		SetScreen(w, h);
		c.x = m_plotWidth / 2 + m_margin.left;
		c.y = m_plotHeight / 2 - m_margin.top;
	}

	// Preserve the position of the clicked point:
	double prior_layer_x = p2x(c.x);
	double prior_layer_y = p2y(c.y);

	// Zoom in:
	m_scaleX = m_scaleX * zoomIncrementalFactor;
	m_scaleY = m_scaleY * zoomIncrementalFactor;

	// Adjust the new m_posx/y:
	m_posX = prior_layer_x - c.x / m_scaleX;
	m_posY = prior_layer_y + c.y / m_scaleY;

	m_desired.Xmin = m_posX;
	m_desired.Xmax = m_posX + m_plotWidth / m_scaleX;
	m_desired.Ymax = m_posY;
	m_desired.Ymin = m_posY - m_plotHeight / m_scaleY;

#ifdef MATHPLOT_DO_LOGGING
	wxLogMessage(wxT("mpWindow::ZoomIn() prior coords: (%f,%f), new coords: (%f,%f) SHOULD BE EQUAL!!"),
			prior_layer_x, prior_layer_y, p2x(c.x), p2y(c.y));
#endif

	UpdateAll();
}

void mpWindow::ZoomOut(const wxPoint &centerPoint)
{
	wxPoint c(centerPoint);
	if (c == wxDefaultPosition)
	{
		int h, w;
		GetClientSize(&w, &h);
		SetScreen(w, h);
		c.x = m_plotWidth / 2 + m_margin.left;
		c.y = m_plotHeight / 2 - m_margin.top;
	}

	// Preserve the position of the clicked point:
	double prior_layer_x = p2x(c.x);
	double prior_layer_y = p2y(c.y);

	// Zoom out:
	m_scaleX = m_scaleX / zoomIncrementalFactor;
	m_scaleY = m_scaleY / zoomIncrementalFactor;

	// Adjust the new m_posx/y:
	m_posX = prior_layer_x - c.x / m_scaleX;
	m_posY = prior_layer_y + c.y / m_scaleY;

	m_desired.Xmin = m_posX;
	m_desired.Xmax = m_posX + m_plotWidth / m_scaleX;
	m_desired.Ymax = m_posY;
	m_desired.Ymin = m_posY - m_plotHeight / m_scaleY;

#ifdef MATHPLOT_DO_LOGGING
	wxLogMessage(wxT("mpWindow::ZoomOut() prior coords: (%f,%f), new coords: (%f,%f) SHOULD BE EQUAL!!"),
			prior_layer_x, prior_layer_y, p2x(c.x), p2y(c.y));
#endif
	UpdateAll();
}

void mpWindow::ZoomInX()
{
	m_scaleX = m_scaleX * zoomIncrementalFactor;
	UpdateAll();
}

void mpWindow::ZoomOutX()
{
	m_scaleX = m_scaleX / zoomIncrementalFactor;
	UpdateAll();
}

void mpWindow::ZoomInY()
{
	m_scaleY = m_scaleY * zoomIncrementalFactor;
	UpdateAll();
}

void mpWindow::ZoomOutY()
{
	m_scaleY = m_scaleY / zoomIncrementalFactor;
	UpdateAll();
}

void mpWindow::ZoomRect(wxPoint p0, wxPoint p1)
{
	// Compute the 2 corners in graph coordinates:
	double p0x = p2x(p0.x);
	double p0y = p2y(p0.y);
	double p1x = p2x(p1.x);
	double p1y = p2y(p1.y);

	// Order them:
	mpFloatRect zoom;
	zoom.Xmin = p0x < p1x ? p0x : p1x;
	zoom.Xmax = p0x > p1x ? p0x : p1x;
	zoom.Ymin = p0y < p1y ? p0y : p1y;
	zoom.Ymax = p0y > p1y ? p0y : p1y;

#ifdef MATHPLOT_DO_LOGGING
	wxLogMessage(wxT("Zoom: (%f,%f)-(%f,%f)"), zoom.Xmin, zoom.Ymin, zoom.Xmax, zoom.Ymax);
#endif

	Fit(zoom);
}

void mpWindow::LockAspect(bool enable)
{
	m_lockaspect = enable;
	m_popmenu.Check(mpID_LOCKASPECT, enable);

	// Try to fit again with the new config:
	Fit(m_desired);
}

void mpWindow::OnShowPopupMenu(wxMouseEvent &event)
{
	// Only display menu if the user has not "dragged" the figure
	if (m_enableMouseNavigation)
	{
		SetCursor(*wxSTANDARD_CURSOR);
	}

	if (!m_mouseMovedAfterRightClick)   // JLB
	{
		m_clickedX = event.GetX();
		m_clickedY = event.GetY();
		PopupMenu(&m_popmenu, event.GetX(), event.GetY());
	}
}

void mpWindow::OnLockAspect(wxCommandEvent& WXUNUSED(event))
{
	LockAspect(!m_lockaspect);
}

void mpWindow::OnMouseHelp(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(
			_T(
					"Supported Mouse commands:\n \
        - Left button down + Mark area: Rectangular zoom\n \
        - Right button down + Move: Pan (Move)\n \
        - Wheel: Zoom in/out\n \
        - Wheel + SHIFT: Horizontal scroll\n \
        - Wheel + CTRL: Vertical scroll"),
			_T("wxMathPlot help"), wxOK, this);
}

void mpWindow::OnFit(wxCommandEvent& WXUNUSED(event))
{
	Fit();
}

void mpWindow::OnToggleGrids(wxCommandEvent& WXUNUSED(event))
{
	mpScaleX *scaleX = (mpScaleX*) GetLayerByClassName(_("mpScaleX"));
	if (scaleX)
	{
		bool grid = scaleX->GetShowGrids();
		scaleX->ShowGrids(!grid);
	}
	mpScaleY *scaleY = (mpScaleY*) GetLayerByClassName(_("mpScaleY"));
	if (scaleY)
	{
		bool grid = scaleY->GetShowGrids();
		scaleY->ShowGrids(!grid);
	}
	UpdateAll();
}

void mpWindow::OnToggleCoords(wxCommandEvent& WXUNUSED(event))
{
	mpInfoCoords *coords = (mpInfoCoords*) GetLayerByClassName(_("mpInfoCoords"));
	if (coords != NULL)
	{
		coords->SetVisible(!coords->IsVisible());
		UpdateAll();
	}
}

void mpWindow::OnScreenShot(wxCommandEvent& WXUNUSED(event))
{
	ClipboardScreenshot();
}

void mpWindow::OnFullScreen(wxCommandEvent& WXUNUSED(event))
{
	m_fullscreen = !m_fullscreen;
	m_parent->ShowFullScreen(m_fullscreen);
}

/**
 * Create series from a data file.
 * The file is not formated. For example a simple txt file or csv file
 * First data is x abscissa and next is up to 9 ordinates
 * Separator is : space or ; or tab
 */
void mpWindow::OnLoadFile(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog OpenFile(this, _T("Select file"), wxEmptyString, wxEmptyString,
			_T("Data files (*.dat)|*.dat|Csv files (csv.*)|csv.*|All files (*.*)|*.*"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);

	if (OpenFile.ShowModal() == wxID_OK)
	{
		FILE* file = fopen(OpenFile.GetPath().c_str(), "r");
		if (file == NULL)
			return;

		unsigned int nb_series = this->CountLayersPlot();
		wxFileName filename(OpenFile.GetPath());
		wxString name = filename.GetName();

		// Max line length
		char line[1024];
		// Data separator : space or ; or tab
		char seps[] = " ;\t\n";
		char *token;
		// Maximum, 10 data by line. First data is x coordinate.
		double data[10];
		int i = 0, j = 0;
		do
		{
			if (fgets(line, 1024, file) == NULL) continue; // EOF

			token = strtok(line, seps);
			i = 0;
			while (token != NULL)
			{
				data[i++] = atof(token);
				token = strtok(NULL, seps);
			}

			for (j = 1; j < i; j++)
				GetXYSeries(nb_series + j - 1, name)->AddData(data[0], data[j], true);

		} while (!feof(file));

		fclose(file);

		mpInfoLegend *legend = (mpInfoLegend*) GetLayerByClassName(_("mpInfoLegend"));
		if (legend != NULL)
		{
			legend->SetNeedUpdate();
		}
	}
}

void mpWindow::OnConfiguration(wxCommandEvent& WXUNUSED(event))
{
	if (m_configWindow == NULL)
		m_configWindow = new MathPlotConfigDialog(this);

	m_configWindow->Initialize();
	m_configWindow->Show();
}

void mpWindow::OnCenter(wxCommandEvent& WXUNUSED(event))
{
	int h, w;
	GetClientSize(&w, &h);
	SetScreen(w, h);
	int centerX = m_plotWidth / 2;
	int centerY = m_plotHeight / 2;
	SetPos(p2x(m_clickedX - centerX - m_margin.left), p2y(m_clickedY - centerY - m_margin.top));
}

void mpWindow::OnZoomIn(wxCommandEvent& WXUNUSED(event))
{
	ZoomIn(wxPoint(m_mouseRClick_X, m_mouseRClick_Y));
}

void mpWindow::OnZoomOut(wxCommandEvent& WXUNUSED(event))
{
	ZoomOut();
}

void mpWindow::OnSize(wxSizeEvent& WXUNUSED(event))
{
	// Try to fit again with the new window size:
	Fit(m_desired);
#ifdef MATHPLOT_DO_LOGGING
	wxLogMessage(wxT("mpWindow::OnSize() m_scrX = %d, m_scrY = %d"), m_scrX, m_scrY);
#endif // MATHPLOT_DO_LOGGING
}

bool mpWindow::AddLayer(mpLayer *layer, bool refreshDisplay)
{
	if (layer != NULL)
	{
		mpInfoType info;
		mpScaleType scale;
		mpFunctionType function;

		if (layer->IsInfo(&info) && (info == mpiCoords))
		{
			// Only one info coords is allowed
			if (m_InfoCoords)
				return false;
			m_InfoCoords = (mpInfoCoords*) layer;
		}

		if (layer->IsScale(&scale))
		{
			if ((scale == mpsScaleX) && (m_XAxis == NULL))
			{
				// Only the first X axis
				m_XAxis = (mpScaleX*) layer;
			}

			if ((scale == mpsScaleY) && (m_YAxis == NULL))
			{
				// Only the first Y axis
				m_YAxis = (mpScaleY*) layer;
			}
		}

		// We just add a function, so we need to update the legend
		if (layer->IsFunction(&function))
		{
			mpInfoLegend *legend = (mpInfoLegend*) this->GetLayerByClassName(_("mpInfoLegend"));
			if (legend)
				legend->SetNeedUpdate();
		}

		layer->SetWindow(*this);
		m_layers.push_back(layer);

		if (refreshDisplay)
			UpdateAll();
		return true;
	}
	return false;
}

bool mpWindow::DelLayer(mpLayer *layer, bool alsoDeleteObject, bool refreshDisplay)
{
	wxLayerList::iterator layIt;
	for (layIt = m_layers.begin(); layIt != m_layers.end(); layIt++)
	{
		if (*layIt == layer)
		{
			bool cancel = false;
			if (m_OnDeleteLayer != NULL)
				m_OnDeleteLayer(this, (*layIt)->GetClassInfo()->GetClassName(), cancel);
			if (!cancel)
			{
				if (layer == m_InfoCoords)
					m_InfoCoords = NULL;
				if (layer == m_movingInfoLayer)
					m_movingInfoLayer = NULL;
				// Also delete the object?
				if (alsoDeleteObject)
					delete *layIt;
				m_layers.erase(layIt); // this deleted the reference only
				if (refreshDisplay)
					UpdateAll();
				return true;
			}
			else return false;
		}
	}
	return false;
}

void mpWindow::DelAllLayers(bool alsoDeleteObject, bool refreshDisplay)
{
	while (m_layers.size() > 0)
	{
		// Also delete the object?
		if (alsoDeleteObject)
			delete m_layers[0];
		m_layers.erase(m_layers.begin()); // this deleted the reference only
	}
	m_InfoCoords = NULL;
	m_movingInfoLayer = NULL;
	if (refreshDisplay)
		UpdateAll();
}

void mpWindow::OnPaint(wxPaintEvent& WXUNUSED(event))
{
#ifdef _WIN32
	wxPaintDC dc(this);
#else
	wxAutoBufferedPaintDC dc(this);
#endif
	int h, w;
	dc.GetSize(&w, &h);   // This is the size of the visible area only!
	SetScreen(w, h);
	wxMemoryDC *m_buff_dc = NULL;

#ifdef MATHPLOT_DO_LOGGING
	{
		int px = 0, py = 0;
//    GetViewStart( &px, &py );
		wxLogMessage(wxT("[mpWindow::OnPaint] vis.area:%ix%i px=%i py=%i"), m_scrX, m_scrY, px, py);
	}
#endif

	// Selects direct or buffered draw:
	wxDC *trgDc;

	// J.L.Blanco @ Aug 2007: Added double buffer support
	if (m_enableDoubleBuffer)
	{
		// Recreate Bitmap if sizes have changed
		if (m_last_lx != m_scrX || m_last_ly != m_scrY)
		{
			if (m_buff_bmp)
				delete m_buff_bmp;
			m_buff_bmp = new wxBitmap(m_scrX, m_scrY, dc);
			m_last_lx = m_scrX;
			m_last_ly = m_scrY;
		}
		m_buff_dc = new wxMemoryDC(&dc);
		m_buff_dc->SelectObject(*m_buff_bmp);
		trgDc = m_buff_dc;
	}
	else
	{
		trgDc = &dc;
	}

	// Draw background
	// Clean the screen
	trgDc->Clear();
	trgDc->SetPen(*wxTRANSPARENT_PEN);
	trgDc->SetBrush(*wxWHITE_BRUSH);
	trgDc->DrawRectangle(0, 0, m_scrX, m_scrY);

	if (m_drawBox)
	{
		trgDc->SetPen(*wxBLACK);
		trgDc->DrawRectangle(0, 0, m_scrX, m_scrY);
	}

	trgDc->SetBrush(m_bgColour);
	trgDc->SetTextForeground(m_fgColour);
	trgDc->DrawRectangle(m_margin.left, m_margin.top, m_plotWidth, m_plotHeight);

	// Draw all the layers:
	m_repainting = true;
	wxLayerList::iterator li;
	for (li = m_layers.begin(); li != m_layers.end(); li++)
	{
		(*li)->Plot(*trgDc, *this);
	}

	// If doublebuffer, draw now to the window:
	if (m_enableDoubleBuffer)
	{
		dc.Blit(0, 0, m_scrX, m_scrY, trgDc, 0, 0, wxCOPY);
		m_buff_dc->SelectObject(wxNullBitmap);
		delete m_buff_dc;
	}
	m_repainting = false;
}

void mpWindow::SetMPScrollbars(bool status)
{
	// Temporary behaviour: always disable scrollbars
	m_enableScrollBars = status; //false;
	if (status == false)
	{
		SetScrollbar(wxHORIZONTAL, 0, 0, 0);
		SetScrollbar(wxVERTICAL, 0, 0, 0);
	}
	// else the scroll bars will be updated in UpdateAll();
	UpdateAll();
}

bool mpWindow::UpdateBBox()
{
	bool first = true;

	for (wxLayerList::iterator li = m_layers.begin(); li != m_layers.end(); li++)
	{
		mpLayer *f = *li;

		if (f->HasBBox() && f->IsVisible())
		{
			if (first)
			{
				first = false;
				m_bound.Xmin = f->GetMinX();
				m_bound.Xmax = f->GetMaxX();
				m_bound.Ymin = f->GetMinY();
				m_bound.Ymax = f->GetMaxY();
			}
			else
			{
				if (f->GetMinX() < m_bound.Xmin)
					m_bound.Xmin = f->GetMinX();
				if (f->GetMaxX() > m_bound.Xmax)
					m_bound.Xmax = f->GetMaxX();
				if (f->GetMinY() < m_bound.Ymin)
					m_bound.Ymin = f->GetMinY();
				if (f->GetMaxY() > m_bound.Ymax)
					m_bound.Ymax = f->GetMaxY();
			}
		}
	}
#ifdef MATHPLOT_DO_LOGGING
	wxLogDebug
	(wxT("[mpWindow::UpdateBBox] Bounding box: Xmin = %f, Xmax = %f, Ymin = %f, YMax = %f"), m_bound.Xmin, m_bound.Xmax, m_bound.Ymin,
			m_bound.Ymax);
#endif // MATHPLOT_DO_LOGGING
	return first == false;
}

void mpWindow::UpdateAll()
{
	if (UpdateBBox())
	{
		if (m_enableScrollBars)
		{
			int cx, cy;
			GetClientSize(&cx, &cy);
			// Do x scroll bar
			{
				// Convert margin sizes from pixels to coordinates
				double leftMargin = m_margin.left / m_scaleX;
				// Calculate the range in coords that we want to scroll over
				double maxX = (m_desired.Xmax > m_bound.Xmax) ? m_desired.Xmax : m_bound.Xmax;
				double minX = (m_desired.Xmin < m_bound.Xmin) ? m_desired.Xmin : m_bound.Xmin;
				if ((m_posX + leftMargin) < minX)
					minX = m_posX + leftMargin;
				// Calculate scroll bar size and thumb position
				int sizeX = (int) ((maxX - minX) * m_scaleX);
				int thumbX = (int) (((m_posX + leftMargin) - minX) * m_scaleX);
				SetScrollbar(wxHORIZONTAL, thumbX, cx - (m_margin.right + m_margin.left), sizeX);
			}
			// Do y scroll bar
			{
				// Convert margin sizes from pixels to coordinates
				double topMargin = m_margin.top / m_scaleY;
				// Calculate the range in coords that we want to scroll over
				double maxY = (m_desired.Ymax > m_bound.Ymax) ? m_desired.Ymax : m_bound.Ymax;
				if ((m_posY - topMargin) > maxY)
					maxY = m_posY - topMargin;
				double minY = (m_desired.Ymin < m_bound.Ymin) ? m_desired.Ymin : m_bound.Ymin;
				// Calculate scroll bar size and thumb position
				int sizeY = (int) ((maxY - minY) * m_scaleY);
				int thumbY = (int) ((maxY - (m_posY - topMargin)) * m_scaleY);
				SetScrollbar(wxVERTICAL, thumbY, cy - (m_margin.top + m_margin.bottom), sizeY);
			}
		}
	}

	Refresh(false);
}

void mpWindow::DoScrollCalc(const int position, const int orientation)
{
	if (orientation == wxVERTICAL)
	{
		// Y axis
		// Get top margin in coord units
		double topMargin = m_margin.top / m_scaleY;
		// Calculate maximum Y coord to be shown in the graph
		double maxY = m_desired.Ymax > m_bound.Ymax ? m_desired.Ymax : m_bound.Ymax;
		// Set new position
		SetPosY((maxY - (position / m_scaleY)) + topMargin);
	}
	else
	{
		// X Axis
		// Get left margin in coord units
		double leftMargin = m_margin.left / m_scaleX;
		// Calculate minimum X coord to be shown in the graph
		double minX = (m_desired.Xmin < m_bound.Xmin) ? m_desired.Xmin : m_bound.Xmin;
		// Set new position
		SetPosX((minX + (position / m_scaleX)) - leftMargin);
	}
}

void mpWindow::OnScrollThumbTrack(wxScrollWinEvent &event)
{
	DoScrollCalc(event.GetPosition(), event.GetOrientation());
}

void mpWindow::OnScrollPageUp(wxScrollWinEvent &event)
{
	int scrollOrientation = event.GetOrientation();
	// Get position before page up
	int position = GetScrollPos(scrollOrientation);
	// Get thumb size
	int thumbSize = GetScrollThumb(scrollOrientation);
	// Need to adjust position by a page
	position -= thumbSize;
	if (position < 0)
		position = 0;

	DoScrollCalc(position, scrollOrientation);
}

void mpWindow::OnScrollPageDown(wxScrollWinEvent &event)
{
	int scrollOrientation = event.GetOrientation();
	// Get position before page up
	int position = GetScrollPos(scrollOrientation);
	// Get thumb size
	int thumbSize = GetScrollThumb(scrollOrientation);
	// Get scroll range
	int scrollRange = GetScrollRange(scrollOrientation);
	// Need to adjust position by a page
	position += thumbSize;
	if (position > (scrollRange - thumbSize))
		position = scrollRange - thumbSize;

	DoScrollCalc(position, scrollOrientation);
}

void mpWindow::OnScrollLineUp(wxScrollWinEvent &event)
{
	int scrollOrientation = event.GetOrientation();
	// Get position before page up
	int position = GetScrollPos(scrollOrientation);
	// Need to adjust position by a line
	position -= mpSCROLL_NUM_PIXELS_PER_LINE;
	if (position < 0)
		position = 0;

	DoScrollCalc(position, scrollOrientation);
}

void mpWindow::OnScrollLineDown(wxScrollWinEvent &event)
{
	int scrollOrientation = event.GetOrientation();
	// Get position before page up
	int position = GetScrollPos(scrollOrientation);
	// Get thumb size
	int thumbSize = GetScrollThumb(scrollOrientation);
	// Get scroll range
	int scrollRange = GetScrollRange(scrollOrientation);
	// Need to adjust position by a page
	position += mpSCROLL_NUM_PIXELS_PER_LINE;
	if (position > (scrollRange - thumbSize))
		position = scrollRange - thumbSize;

	DoScrollCalc(position, scrollOrientation);
}

void mpWindow::OnScrollTop(wxScrollWinEvent &event)
{
	DoScrollCalc(0, event.GetOrientation());
}

void mpWindow::OnScrollBottom(wxScrollWinEvent &event)
{
	int scrollOrientation = event.GetOrientation();
	// Get thumb size
	int thumbSize = GetScrollThumb(scrollOrientation);
	// Get scroll range
	int scrollRange = GetScrollRange(scrollOrientation);

	DoScrollCalc(scrollRange - thumbSize, scrollOrientation);
}
// End patch ngpaton

// New methods implemented by Davide Rondini

unsigned int mpWindow::CountLayers()
{
	unsigned int layerNo = 0;
	for (wxLayerList::iterator li = m_layers.begin(); li != m_layers.end(); li++)
	{
		if ((*li)->HasBBox())
			layerNo++;
	}
	return layerNo;
}

unsigned int mpWindow::CountLayersPlot()
{
	unsigned int layerNo = 0;
	for (wxLayerList::iterator li = m_layers.begin(); li != m_layers.end(); li++)
	{
		if ((*li)->GetLayerType() == mpLAYER_PLOT)
			layerNo++;
	}
	return layerNo;
}

mpLayer* mpWindow::GetLayer(int position)
{
	if ((position >= (int) m_layers.size()) || position < 0)
		return NULL;
	return m_layers[position];
}

mpLayer* mpWindow::GetLayerPlot(int position, mpFunctionType func)
{
	int layerNo = -1;
	mpFunctionType function;
	if (position < 0)
		return NULL;
	for (wxLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
	{
		if ((*it)->IsFunction(&function) && ((func == mpfAllType) || (function == func)))
		{
			if (++layerNo == position)
				return *it;
		}
	}
	return NULL;
}

mpFXYVector* mpWindow::GetXYSeries(unsigned int n, const wxString &name, bool create)
{
	mpFXYVector *serie = (mpFXYVector*) this->GetLayerPlot(n, mpfFXYVector);
	if ((serie == NULL) && create)
	{
		serie = new mpFXYVector(wxString::Format(wxT("%s %d"), name, n));
		serie->SetContinuity(true);
		wxPen pen(wxIndexColour(n), 2, wxPENSTYLE_SOLID);
		serie->SetPen(pen);
		this->AddLayer(serie);
	}
	return serie;
}

mpLayer* mpWindow::GetLayerByName(const wxString &name)
{
	for (wxLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
		if ((*it)->GetName().IsSameAs(name))
			return *it;
	return NULL;    // Not found
}

mpLayer* mpWindow::GetLayerByClassName(const wxString &name)
{
	for (wxLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
	{
		wxString classname = (*it)->GetClassInfo()->GetClassName();
		if (classname.IsSameAs(name))
			return *it;
	}
	return NULL;    // Not found
}

mpInfoLayer* mpWindow::IsInsideInfoLayer(wxPoint &point)
{
	mpInfoType info;
	wxLayerList::iterator li;
	for (li = m_layers.begin(); li != m_layers.end(); li++)
	{
#ifdef MATHPLOT_DO_LOGGING
		wxLogMessage(wxT("mpWindow::IsInsideInfoLayer() examinining layer = %p"), (*li));
#endif // MATHPLOT_DO_LOGGING
		if ((*li)->IsInfo(&info))
		{
			mpInfoLayer *tmpLyr = (mpInfoLayer*) (*li);
#ifdef MATHPLOT_DO_LOGGING
			wxLogMessage(wxT("mpWindow::IsInsideInfoLayer() layer = %p"), (*li));
#endif // MATHPLOT_DO_LOGGING
			if (tmpLyr->Inside(point))
			{
				return tmpLyr;
			}
		}
	}
	return NULL;
}

void mpWindow::SetLayerVisible(const wxString &name, bool viewable)
{
	mpLayer *lx = GetLayerByName(name);
	if (lx)
	{
		lx->SetVisible(viewable);
		UpdateAll();
	}
}

bool mpWindow::IsLayerVisible(const wxString &name)
{
	mpLayer *lx = GetLayerByName(name);
	return (lx) ? lx->IsVisible() : false;
}

bool mpWindow::IsLayerVisible(const unsigned int position)
{
	mpLayer *lx = GetLayer(position);
	return (lx) ? lx->IsVisible() : false;
}

void mpWindow::SetLayerVisible(const unsigned int position, bool viewable)
{
	mpLayer *lx = GetLayer(position);
	if (lx)
	{
		lx->SetVisible(viewable);
		UpdateAll();
	}
}

void mpWindow::GetBoundingBox(double *bbox)
{
	bbox[0] = m_bound.Xmin;
	bbox[1] = m_bound.Xmax;
	bbox[2] = m_bound.Ymin;
	bbox[3] = m_bound.Ymax;
}

void mpWindow::SetMargins(int top, int right, int bottom, int left)
{
	m_margin.top = top;
	m_margin.right = right;
	m_margin.bottom = bottom;
	m_margin.left = left;
	m_plotWidth = m_scrX - (m_margin.left + m_margin.right);
	m_plotHeight = m_scrY - (m_margin.top + m_margin.bottom);

	m_plotBondaries.startPx = 0;
	m_plotBondariesMargin.startPx = m_margin.left;
	m_plotBondaries.endPx = m_scrX;
	m_plotBondariesMargin.endPx = m_scrX - m_margin.right;

	m_plotBondaries.startPy = 0;
	m_plotBondariesMargin.startPy = m_margin.top;
	m_plotBondaries.endPy = m_scrY;
	m_plotBondariesMargin.endPy = m_scrY - m_margin.bottom;
}

wxBitmap *mpWindow::BitmapScreenshot(wxSize imageSize, bool fit)
{
	int sizeX, sizeY;
	int bk_scrX = m_scrX;
	int bk_scrY = m_scrY;
	mpFloatRect bk_m_desired = m_desired;
	wxMemoryDC m_Screenshot_dc;

	if (imageSize == wxDefaultSize)
	{
		sizeX = m_scrX;
		sizeY = m_scrY;
	}
	else
	{
		sizeX = imageSize.x;
		sizeY = imageSize.y;
		SetScreen(sizeX, sizeY);
	}

	if (m_Screenshot_bmp)
		delete m_Screenshot_bmp;
	m_Screenshot_bmp = new wxBitmap(sizeX, sizeY);
	m_Screenshot_dc.SelectObject(*m_Screenshot_bmp);

	// Clean the screen
	m_Screenshot_dc.SetPen(*wxTRANSPARENT_PEN);
	m_Screenshot_dc.SetBrush(*wxWHITE_BRUSH);
	m_Screenshot_dc.DrawRectangle(0, 0, m_scrX, m_scrY);

	if (m_drawBox)
	{
		m_Screenshot_dc.SetPen(*wxBLACK);
		m_Screenshot_dc.DrawRectangle(0, 0, m_scrX, m_scrY);
	}

	m_Screenshot_dc.SetBrush(m_bgColour);
	m_Screenshot_dc.SetTextForeground(m_fgColour);
	m_Screenshot_dc.DrawRectangle(m_margin.left, m_margin.top, m_plotWidth, m_plotHeight);

	if (fit)
	{
		Fit(m_bound, &sizeX, &sizeY);
	}
	else
	{
		if (imageSize != wxDefaultSize)
			Fit(m_desired, &sizeX, &sizeY);
	}

	// Draw all the layers:
	wxLayerList::iterator li;
	for (li = m_layers.begin(); li != m_layers.end(); li++)
	{
		(*li)->Plot(m_Screenshot_dc, *this);
	}
	m_Screenshot_dc.SelectObject(wxNullBitmap);

	// Restore dimensions
	if (fit || (imageSize != wxDefaultSize))
	{
		SetScreen(bk_scrX, bk_scrY);
		Fit(bk_m_desired, &bk_scrX, &bk_scrY);
		UpdateAll();
	}
	return m_Screenshot_bmp;
}

void mpWindow::ClipboardScreenshot(wxSize imageSize, bool fit)
{
	BitmapScreenshot(imageSize, fit);

	wxImage screenImage = m_Screenshot_bmp->ConvertToImage();

	if (wxTheClipboard->Open())
	{
		// CAUTION : Data objects are held by the clipboard,
		// so do not delete them in the app.
		if (!wxTheClipboard->SetData(new wxBitmapDataObject(screenImage)))
			wxMessageBox(_T("Failed to copy image to clipboard"));
		wxTheClipboard->Flush();
		wxTheClipboard->Close();
	}
}

bool mpWindow::SaveScreenshot(const wxString &filename, int type, wxSize imageSize, bool fit)
{
	BitmapScreenshot(imageSize, fit);

	wxImage screenImage = m_Screenshot_bmp->ConvertToImage();
	return screenImage.SaveFile(filename, (wxBitmapType) type);
}

void mpWindow::SetColourTheme(const wxColour &bgColour, const wxColour &drawColour, const wxColour &axesColour)
{
	SetForegroundColour(drawColour);
	m_bgColour = bgColour;
	m_fgColour = drawColour;
	m_axColour = axesColour;
	// cycle between layers to set colours and properties to them
	wxLayerList::iterator li;
	for (li = m_layers.begin(); li != m_layers.end(); li++)
	{
		if ((*li)->GetLayerType() == mpLAYER_AXIS)
		{
			wxPen axisPen = (*li)->GetPen(); // Get the old pen to modify only colour, not style or width
			axisPen.SetColour(axesColour);
			(*li)->SetPen(axisPen);
		}
		if ((*li)->GetLayerType() == mpLAYER_INFO)
		{
			wxPen infoPen = (*li)->GetPen(); // Get the old pen to modify only colour, not style or width
			infoPen.SetColour(drawColour);
			(*li)->SetPen(infoPen);
		}
	}
}

//-----------------------------------------------------------------------------
// mpFXYVector implementation - by Jose Luis Blanco (AGO-2007)
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(mpFXYVector, mpFXY)

// Constructor
mpFXYVector::mpFXYVector(wxString name, int flags, bool viewAsBar) :
		mpFXY(name, flags, viewAsBar)
{
	m_type = mpLAYER_PLOT;
	m_index = 0;
	m_minX = -1;
	m_maxX = 1;
	m_minY = -1;
	m_maxY = 1;
	m_deltaX = 1e+308;
	m_xs.clear();
	m_ys.clear();
	SetReserve(1000);
}

bool mpFXYVector::GetNextXY(double &x, double &y)
{
	if (m_index >= m_xs.size())
		return false;
	else
	{
		x = m_xs[m_index];
		y = m_ys[m_index];
		m_index += m_step;
		return m_index <= m_xs.size();
	}
}

void mpFXYVector::DrawAddedPoint(const double x, const double y)
{
	// If we are here, new point is always in bound
	if (!m_visible)
		return;

	// Direct access to the dc
	wxClientDC dc(m_win);
	dc.SetPen(m_pen);
	dc.SetBrush(m_brush);

	wxCoord ix = m_win->x2p(x);
	wxCoord iy = m_win->y2p(y);

	if (!m_ViewAsBar)
	{
		if (m_continuous)
		{
			// Last coordinates (we assume that m_step = 1 in this context)
			wxCoord ixlast = m_win->x2p(m_xs[m_index - 1]);
			wxCoord iylast = m_win->y2p(m_ys[m_index - 1]);
			m_index++;
			dc.DrawLine(ixlast, iylast, ix, iy);
			if (m_symbol != mpsNone)
				DrawSymbol(dc, ix, iy);
		}
		else
		{
			if (m_symbol == mpsNone)
			{
				if (m_pen.GetWidth() > 1)
					dc.DrawLine(ix, iy, ix, iy);
				else
					dc.DrawPoint(ix, iy);
			}
			else
				DrawSymbol(dc, ix, iy);
		}
	}
	else
	{
		double delta = m_win->GetScreenX() / m_win->GetScaleX();
		if (m_deltaX < delta)
			delta = m_deltaX;
		int d = (int) ((delta * m_win->GetScaleX()) / 3.5);
		wxCoord iybase = m_win->y2p(0);
		dc.DrawRectangle(ix - d, iy, 2 * d, iybase - iy);
	}
}

void mpFXYVector::Clear()
{
	m_xs.clear();
	m_ys.clear();
	// Default min max
	m_minX = -1;
	m_maxX = 1;
	m_minY = -1;
	m_maxY = 1;
	m_deltaX = 1e+308;
	Rewind();
}

void mpFXYVector::SetData(const std::vector<double> &xs, const std::vector<double> &ys)
{
	// We add 2% to the limit
#define LIMIT 0.02
	// Check if the data vectora are of the same size
	if (xs.size() != ys.size())
	{
		wxLogError(_T("wxMathPlot error: X and Y vector are not of the same length!"));
		return;
	}
	// Copy the data:
	m_xs = xs;
	m_ys = ys;

	// Update internal variables for the bounding box.
	if (xs.size() > 0)
	{
		bool first = true;
		m_minX = xs[0] - (fabs(xs[0]) * LIMIT);
		m_maxX = xs[0] + (fabs(xs[0]) * LIMIT);
		m_minY = ys[0] - (fabs(ys[0]) * LIMIT);
		m_maxY = ys[0] + (fabs(ys[0]) * LIMIT);

		std::vector<double>::const_iterator it;

		for (it = xs.begin(); it != xs.end(); it++)
		{
			if (*it < m_minX)
				m_minX = (*it) - (fabs((*it)) * LIMIT);
			else
				if (*it > m_maxX)
					m_maxX = (*it) + (fabs((*it)) * LIMIT);

			if (first)
			{
				m_lastX = (*it);
				first = false;
			}
			else
			{
				if (abs((*it) - m_lastX) < m_deltaX)
					m_deltaX = abs((*it) - m_lastX);

				m_lastX = *it;
			}
		}

		for (it = ys.begin(); it != ys.end(); it++)
		{
			if (*it < m_minY)
				m_minY = (*it) - (fabs((*it)) * LIMIT);
			else
				if (*it > m_maxY)
					m_maxY = (*it) + (fabs((*it)) * LIMIT);
		}
	}
	else
	{
		m_minX = -1;
		m_maxX = 1;
		m_minY = -1;
		m_maxY = 1;
	}
	Rewind();
}

/** Add data to the internal vector. This method DOES NOT refresh the mpWindow; do it manually
 * by calling UpdateAll() or just Fit() if we want to adjust plot
 * We add 2% to the limit
 */
bool mpFXYVector::AddData(const double x, const double y, bool updatePlot)
{
#define LIMIT 0.02
	bool new_limit = false;
	double bbox[4];
	m_win->GetBoundingBox(bbox);

	m_xs.push_back(x);
	m_ys.push_back(y);

	// We have exactly 2 points, we can update min max
	if (m_xs.size() == 2)
	{
		// X scale
		if (m_xs[0] > m_xs[1])
		{
			m_minX = m_xs[1] - (fabs(m_xs[1]) * LIMIT);
			m_maxX = m_xs[0] + (fabs(m_xs[0]) * LIMIT);
		}
		else
		{
			m_minX = m_xs[0] - (fabs(m_xs[0]) * LIMIT);
			m_maxX = m_xs[1] + (fabs(m_xs[1]) * LIMIT);
		}
		m_deltaX = abs(m_xs[1] - m_lastX);
		m_lastX = m_xs[1];

		// Y scale
		if (m_ys[0] > m_ys[1])
		{
			m_minY = m_ys[1] - (fabs(m_ys[1]) * LIMIT);
			m_maxY = m_ys[0] + (fabs(m_ys[0]) * LIMIT);
		}
		else
		{
			m_minY = m_ys[0] - (fabs(m_ys[0]) * LIMIT);
			m_maxY = m_ys[1] + (fabs(m_ys[1]) * LIMIT);
		}
		new_limit = true;
	}
	else
	{
		// X scale
		if (x < m_minX)
		{
			m_minX = x - (fabs(x) * LIMIT);
			if (m_minX < bbox[0])
				new_limit = true;
		}
		else
			if (x > m_maxX)
			{
				m_maxX = x + (fabs(x) * LIMIT);
				if (m_maxX > bbox[1])
					new_limit = true;
			}

		if (m_xs.size() == 1)
			m_lastX = m_xs[0];
		else
		{
			if (abs(x - m_lastX) < m_deltaX)
				m_deltaX = abs(x - m_lastX);
			m_lastX = x;
		}

		// Y scale
		if (y < m_minY)
		{
			m_minY = y - (fabs(y) * LIMIT);
			if (m_minY < bbox[2])
				new_limit = true;
		}
		else
			if (y > m_maxY)
			{
				m_maxY = y + (fabs(y) * LIMIT);
				if (m_maxY > bbox[3])
					new_limit = true;
			}
	}

	if (updatePlot && !new_limit)
	{
		DrawAddedPoint(x, y);
	}
//	else
//		Rewind();
	return new_limit;
}

//-----------------------------------------------------------------------------
// mpText - provided by Val Greene
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpText, mpLayer)

/** @param name text to be displayed
 @param offsetx x position in percentage (0-100)
 @param offsetx y position in percentage (0-100)
 */
mpText::mpText(wxString name, int offsetx, int offsety)
{
	m_type = mpLAYER_INFO;
	SetName(name);
	m_location = mpMarginNone;

	if (offsetx >= 0 && offsetx <= 100)
		m_offsetx = offsetx;
	else
		m_offsetx = 5;

	if (offsety >= 0 && offsety <= 100)
		m_offsety = offsety;
	else
		m_offsety = 50;
}

/** @param name text to be displayed
 @param location in the margin
 */
mpText::mpText(wxString name, mpLocation marginLocation)
{
	m_type = mpLAYER_INFO;
	SetName(name);
	m_location = marginLocation;
	m_offsetx = 5;
	m_offsety = 50;
}

/** mpText Layer plot handler.
 This implementation will plot the text adjusted to the visible area.
 */

void mpText::Plot(wxDC &dc, mpWindow &w)
{
	if (m_visible)
	{
		DoBeforePlot();
		UpdateContext(dc);

		wxCoord tw = 0, th = 0;
		dc.GetTextExtent(GetName(), &tw, &th);

		//     int left = -dc.LogicalToDeviceX(0);
		//     int width = dc.LogicalToDeviceX(0) - left;
		//     int bottom = dc.LogicalToDeviceY(0);
		//     int height = bottom - -dc.LogicalToDeviceY(0);

		/*    dc.DrawText( GetName(),
		 (int)((((float)width/100.0) * m_offsety) + left - (tw/2)),
		 (int)((((float)height/100.0) * m_offsetx) - bottom) );*/
//		int px = m_offsetx * (w.GetScreenX() - w.GetMarginLeft() - w.GetMarginRight()) / 100;
//		int py = m_offsety * (w.GetScreenY() - w.GetMarginTop() - w.GetMarginBottom()) / 100;
		int px = 0, py = 0;
		if (m_location == mpMarginNone)
		{
			px = m_offsetx * w.GetPlotWidth() / 100;
			py = m_offsety * w.GetPlotHeight() / 100;
		}
		else
		{
			switch (m_location)
			{
				case mpMarginLeftCenter:
				{
					px = (w.GetMarginLeft() - tw) / 2;
					py = (w.GetScreenY() - th) / 2;
					break;
				}
				case mpMarginTopLeft:
				{
					px = MARGIN_LEFT_OFFSET;
					py = (w.GetMarginTop() - th) / 2;
					break;
				}
				case mpMarginTopCenter:
				{
					px = (w.GetScreenX() - tw) / 2;
					py = (w.GetMarginTop() - th) / 2;
					break;
				}
				case mpMarginTopRight:
				{
					px = w.GetScreenX() - tw - MARGIN_BOTTOM_OFFSET;
					py = (w.GetMarginTop() - th) / 2;
					break;
				}
				case mpMarginRightCenter:
				{
					px = w.GetScreenX() - (w.GetMarginRight() + tw) / 2;
					py = (w.GetScreenY() - th) / 2;
					break;
				}
				case mpMarginBottomLeft:
				{
					px = MARGIN_LEFT_OFFSET;
					py = w.GetScreenY() - (w.GetMarginBottom() + th) / 2;
					break;
				}
				case mpMarginBottomCenter:
				{
					px = (w.GetScreenX() - tw) / 2;
					py = w.GetScreenY() - (w.GetMarginBottom() + th) / 2;
					break;
				}
				case mpMarginBottomRight:
				{
					px = w.GetScreenX() - tw - MARGIN_BOTTOM_OFFSET;
					py = w.GetScreenY() - (w.GetMarginBottom() + th) / 2;
					break;
				}
				default:
					;
			}
		}

		dc.DrawText(GetName(), px, py);
	}
}

//-----------------------------------------------------------------------------
// mpTitle
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpTitle, mpText)

mpTitle::mpTitle()
{
	m_location = mpMarginTopCenter;
}

//-----------------------------------------------------------------------------
// mpPrintout - provided by Davide Rondini
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpPrintout, wxPrintout)

mpPrintout::mpPrintout(mpWindow *drawWindow, const wxChar *title, int factor) :
		wxPrintout(title)
{
	drawn = false;
	plotWindow = drawWindow;
	stretch_factor = factor;
}

bool mpPrintout::OnPrintPage(int page)
{
	if (plotWindow == NULL)
		return false;

	wxDC *trgDc = GetDC();

	if ((trgDc) && (page == 1))
	{
		wxCoord marginX = 100;
		wxCoord marginY;
		wxCoord m_prnX, m_prnY;
		trgDc->GetSize(&m_prnX, &m_prnY);

		// The future size of the plot window
		wxCoord m_prnXw, m_prnYw;
		m_prnXw = m_prnX - 2 * marginX;

		// Ratio between printer and actual window on x size
		double ratio = (double) (m_prnXw) / plotWindow->GetScreenX();

		// Same ratio for y
		m_prnYw = (wxCoord) (ratio * plotWindow->GetScreenY());
		// Then compute y margin
		marginY = (m_prnY - m_prnYw) / 2;

#ifdef MATHPLOT_DO_LOGGING
		wxLogMessage(wxT("Printer Size: %d x %d\n"), m_prnX, m_prnY);
		wxLogMessage(wxT("Actual Screen Size: %d x %d\n"), plotWindow->GetScreenX(), plotWindow->GetScreenY());
		wxLogMessage(wxT("Screen Size for Printer: %d x %d\n"), m_prnXw, m_prnYw);
#endif

		plotWindow->BitmapScreenshot(wxSize(m_prnXw / stretch_factor, m_prnYw / stretch_factor));
		wxMemoryDC bitmap_dc(trgDc);
		bitmap_dc.SelectObject(*(plotWindow->m_Screenshot_bmp));

		if (stretch_factor == 1)
			trgDc->Blit(marginX, marginY, m_prnXw, m_prnYw, &bitmap_dc, 0, 0);
		else
			trgDc->StretchBlit(marginX, marginY, m_prnXw, m_prnYw, &bitmap_dc, 0, 0, m_prnXw / stretch_factor,
					m_prnYw / stretch_factor);
		bitmap_dc.SelectObject(wxNullBitmap);
	}
	return true;
}

bool mpPrintout::HasPage(int page)
{
	return (page == 1);
}

//-----------------------------------------------------------------------------
// mpMovableObject - provided by Jose Luis Blanco
//-----------------------------------------------------------------------------
//#define ASSEMBLER
inline void SinCos(double Angle, double *sinA, double *cosA)
{
#ifdef ASSEMBLER
	// https://gcc.gnu.org/onlinedocs/gcc-4.9.2/gcc/Extended-Asm.html#Extended-Asm
	__asm__ ("fsincos" : "=t" (*cosA), "=u" (*sinA) : "0" (Angle));
#else
	#if __GNUC__
		// Use GNU extension
		sincos(Angle, sinA, cosA);
	#else
		*sinA = sin(Angle);
		*cosA = cos(Angle);
	#endif
#endif
}

IMPLEMENT_DYNAMIC_CLASS(mpMovableObject, mpLayer)

void mpMovableObject::TranslatePoint(double x, double y, double &out_x, double &out_y)
{
	//    double ccos = cos( m_reference_phi );  // Avoid computing cos/sin twice.
	//    double csin = sin( m_reference_phi );

	double ccos, csin;
	SinCos(m_reference_phi, &csin, &ccos);

	out_x = m_reference_x + ccos * x - csin * y;
	out_y = m_reference_y + csin * x + ccos * y;
}

// This method updates the buffers m_trans_shape_xs/ys, and the precomputed bounding box.
void mpMovableObject::ShapeUpdated()
{
	// Just in case...
	if (m_shape_xs.size() != m_shape_ys.size())
	{
		wxLogError(wxT("[mpMovableObject::ShapeUpdated] Error, m_shape_xs and m_shape_ys have different lengths!"));
	}
	else
	{
		//        double ccos = cos( m_reference_phi );  // Avoid computing cos/sin twice.
		//        double csin = sin( m_reference_phi );

		double ccos, csin;
		SinCos(m_reference_phi, &csin, &ccos);

		m_trans_shape_xs.resize(m_shape_xs.size());
		m_trans_shape_ys.resize(m_shape_xs.size());

		std::vector<double>::iterator itXi, itXo;
		std::vector<double>::iterator itYi, itYo;

		m_bbox_min_x = 1e300;
		m_bbox_max_x = -1e300;
		m_bbox_min_y = 1e300;
		m_bbox_max_y = -1e300;

		for (itXo = m_trans_shape_xs.begin(), itYo = m_trans_shape_ys.begin(), itXi = m_shape_xs.begin(), itYi = m_shape_ys.begin();
				itXo != m_trans_shape_xs.end(); itXo++, itYo++, itXi++, itYi++)
		{
			*itXo = m_reference_x + ccos * (*itXi) - csin * (*itYi);
			*itYo = m_reference_y + csin * (*itXi) + ccos * (*itYi);

			// Keep BBox:
			if (*itXo < m_bbox_min_x)
				m_bbox_min_x = *itXo;
			if (*itXo > m_bbox_max_x)
				m_bbox_max_x = *itXo;
			if (*itYo < m_bbox_min_y)
				m_bbox_min_y = *itYo;
			if (*itYo > m_bbox_max_y)
				m_bbox_max_y = *itYo;
		}
	}
}

void mpMovableObject::Plot(wxDC &dc, mpWindow &w)
{
	if (m_visible)
	{
		DoBeforePlot();
		dc.SetPen(m_pen);

		std::vector<double>::iterator itX = m_trans_shape_xs.begin();
		std::vector<double>::iterator itY = m_trans_shape_ys.begin();

		if (!m_continuous)
		{
			// for some reason DrawPoint does not use the current pen,
			// so we use DrawLine for fat pens
			if (m_pen.GetWidth() <= 1)
			{
				while (itX != m_trans_shape_xs.end())
				{
					dc.DrawPoint(w.x2p(*(itX++)), w.y2p(*(itY++)));
				}
			}
			else
			{
				while (itX != m_trans_shape_xs.end())
				{
					wxCoord cx = w.x2p(*(itX++));
					wxCoord cy = w.y2p(*(itY++));
					dc.DrawLine(cx, cy, cx, cy);
				}
			}
		}
		else
		{
			wxCoord cx0 = 0, cy0 = 0;
			bool first = true;
			while (itX != m_trans_shape_xs.end())
			{
				wxCoord cx = w.x2p(*(itX++));
				wxCoord cy = w.y2p(*(itY++));
				if (first)
				{
					first = false;
					cx0 = cx;
					cy0 = cy;
				}
				dc.DrawLine(cx0, cy0, cx, cy);
				cx0 = cx;
				cy0 = cy;
			}
		}

		if (!m_name.IsEmpty() && m_showName)
		{
			dc.SetFont(m_font);
			dc.SetTextForeground(m_fontcolour);

			wxCoord tx, ty;
			dc.GetTextExtent(m_name, &tx, &ty);

			if (HasBBox())
			{
				wxCoord sx = (wxCoord) ((m_bbox_max_x - w.GetPosX()) * w.GetScaleX());
				wxCoord sy = (wxCoord) ((w.GetPosY() - m_bbox_max_y) * w.GetScaleY());

				tx = sx - tx - 8;
				ty = sy - 8 - ty;
			}
			else
			{
				const int sx = w.GetScreenX() >> 1;
				const int sy = w.GetScreenY() >> 1;

				switch (m_flags)
				{
					case mpALIGN_NW:
					{
						tx = -sx + 8;
						ty = -sy + 8;
						break;
					}
					case mpALIGN_NE:
					{
						tx = sx - tx - 8;
						ty = -sy + 8;
						break;
					}
					case mpALIGN_SE:
					{
						tx = sx - tx - 8;
						ty = sy - 8 - ty;
						break;
					}
					default:
					{ // mpALIGN_SW
						tx = -sx + 8;
						ty = sy - 8 - ty;
					}
				}
			}

			dc.DrawText(m_name, tx, ty);
		}
	}
}

//-----------------------------------------------------------------------------
// mpCovarianceEllipse - provided by Jose Luis Blanco
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpCovarianceEllipse, mpMovableObject)

// Called to update the m_shape_xs, m_shape_ys vectors, whenever a parameter changes.
void mpCovarianceEllipse::RecalculateShape()
{
	m_shape_xs.clear();
	m_shape_ys.clear();

	// Preliminar checks:
	if (m_quantiles < 0)
	{
		wxLogError(wxT("[mpCovarianceEllipse] Error: quantiles must be non-negative"));
		return;
	}
	if (m_cov_00 < 0)
	{
		wxLogError(wxT("[mpCovarianceEllipse] Error: cov(0,0) must be non-negative"));
		return;
	}
	if (m_cov_11 < 0)
	{
		wxLogError(wxT("[mpCovarianceEllipse] Error: cov(1,1) must be non-negative"));
		return;
	}

	m_shape_xs.resize(m_segments, 0);
	m_shape_ys.resize(m_segments, 0);

	// Compute the two eigenvalues of the covariance:
	// -------------------------------------------------
	double b = -m_cov_00 - m_cov_11;
	double c = m_cov_00 * m_cov_11 - m_cov_01 * m_cov_01;

	double D = b * b - 4 * c;

	if (D < 0)
	{
		wxLogError(wxT("[mpCovarianceEllipse] Error: cov is not positive definite"));
		return;
	}

	double eigenVal0 = 0.5 * (-b + sqrt(D));
	double eigenVal1 = 0.5 * (-b - sqrt(D));

	// Compute the two corresponding eigenvectors:
	// -------------------------------------------------
	double eigenVec0_x, eigenVec0_y;
	double eigenVec1_x, eigenVec1_y;

	if (fabs(eigenVal0 - m_cov_00) > EPSILON)
	{
		double k1x = m_cov_01 / (eigenVal0 - m_cov_00);
		eigenVec0_y = 1;
		eigenVec0_x = eigenVec0_y * k1x;
	}
	else
	{
		double k1y = m_cov_01 / (eigenVal0 - m_cov_11);
		eigenVec0_x = 1;
		eigenVec0_y = eigenVec0_x * k1y;
	}

	if (fabs(eigenVal1 - m_cov_00) > EPSILON)
	{
		double k2x = m_cov_01 / (eigenVal1 - m_cov_00);
		eigenVec1_y = 1;
		eigenVec1_x = eigenVec1_y * k2x;
	}
	else
	{
		double k2y = m_cov_01 / (eigenVal1 - m_cov_11);
		eigenVec1_x = 1;
		eigenVec1_y = eigenVec1_x * k2y;
	}

	// Normalize the eigenvectors:
	double len = sqrt(eigenVec0_x * eigenVec0_x + eigenVec0_y * eigenVec0_y);
	eigenVec0_x /= len;  // It *CANNOT* be zero
	eigenVec0_y /= len;

	len = sqrt(eigenVec1_x * eigenVec1_x + eigenVec1_y * eigenVec1_y);
	eigenVec1_x /= len;  // It *CANNOT* be zero
	eigenVec1_y /= len;

	// Take the sqrt of the eigenvalues (required for the ellipse scale):
	eigenVal0 = sqrt(eigenVal0);
	eigenVal1 = sqrt(eigenVal1);

	// Compute the 2x2 matrix M = diag(eigVal) * (~eigVec)  (each eigen vector is a row):
	double M_00 = eigenVec0_x * eigenVal0;
	double M_01 = eigenVec0_y * eigenVal0;

	double M_10 = eigenVec1_x * eigenVal1;
	double M_11 = eigenVec1_y * eigenVal1;

	// The points of the 2D ellipse:
	double ang;
	double Aang = 6.283185308 / (m_segments - 1);
	int i;
	for (i = 0, ang = 0; i < m_segments; i++, ang += Aang)
	{
		double ccos, csin;
		SinCos(ang, &csin, &ccos);

		m_shape_xs[i] = m_quantiles * (ccos * M_00 + csin * M_10);
		m_shape_ys[i] = m_quantiles * (ccos * M_01 + csin * M_11);
	} // end for points on ellipse

	ShapeUpdated();
}

//-----------------------------------------------------------------------------
// mpPolygon - provided by Jose Luis Blanco
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpPolygon, mpMovableObject)

void mpPolygon::setPoints(const std::vector<double> &points_xs, const std::vector<double> &points_ys, bool closedShape)
{
	if (points_xs.size() != points_ys.size())
	{
		wxLogError(wxT("[mpPolygon] Error: points_xs and points_ys must have the same number of elements"));
	}
	else
	{
		m_shape_xs = points_xs;
		m_shape_ys = points_ys;

		if (closedShape && points_xs.size())
		{
			m_shape_xs.push_back(points_xs[0]);
			m_shape_ys.push_back(points_ys[0]);
		}

		ShapeUpdated();
	}
}

//-----------------------------------------------------------------------------
// mpBitmapLayer - provided by Jose Luis Blanco
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpBitmapLayer, mpLayer)

void mpBitmapLayer::GetBitmapCopy(wxImage &outBmp) const
{
	if (m_validImg)
		outBmp = m_bitmap;
}

void mpBitmapLayer::SetBitmap(const wxImage &inBmp, double x, double y, double lx, double ly)
{
	if (!inBmp.Ok())
	{
		wxLogError(wxT("[mpBitmapLayer] Assigned bitmap is not Ok()!"));
	}
	else
	{
		m_bitmap = inBmp; //.GetSubBitmap( wxRect(0, 0, inBmp.GetWidth(), inBmp.GetHeight()));
		m_min_x = x;
		m_min_y = y;
		m_max_x = x + lx;
		m_max_y = y + ly;
		m_validImg = true;
	}
}

void mpBitmapLayer::Plot(wxDC &dc, mpWindow &w)
{
	if (m_visible && m_validImg)
	{
		DoBeforePlot();
		/*	1st: We compute (x0,y0)-(x1,y1), the pixel coordinates of the real outer limits
		 of the image rectangle within the (screen) mpWindow. Note that these coordinates
		 might fall well far away from the real view limits when the user zoom in.

		 2nd: We compute (dx0,dy0)-(dx1,dy1), the pixel coordinates the rectangle that will
		 be actually drawn into the mpWindow, i.e. the clipped real rectangle that
		 avoids the non-visible parts. (offset_x,offset_y) are the pixel coordinates
		 that correspond to the window point (dx0,dy0) within the image "m_bitmap", and
		 (b_width,b_height) is the size of the bitmap patch that will be drawn.

		 (x0,y0) .................  (x1,y0)
		 .                          .
		 .                          .
		 (x0,y1) ................   (x1,y1)
		 (In pixels!!)
		 */

		// 1st step -------------------------------
		wxCoord x0 = w.x2p(m_min_x);
		wxCoord y0 = w.y2p(m_max_y);
		wxCoord x1 = w.x2p(m_max_x);
		wxCoord y1 = w.y2p(m_min_y);

		// 2nd step -------------------------------
		// Precompute the size of the actual bitmap pixel on the screen (e.g. will be >1 if zoomed in)
		double screenPixelX = (x1 - x0) / (double) m_bitmap.GetWidth();
		double screenPixelY = (y1 - y0) / (double) m_bitmap.GetHeight();

		// The minimum number of pixels that the streched image will overpass the actual mpWindow borders:
		wxCoord borderMarginX = (wxCoord) (screenPixelX + 1); // ceil
		wxCoord borderMarginY = (wxCoord) (screenPixelY + 1); // ceil

		// The actual drawn rectangle (dx0,dy0)-(dx1,dy1) is (x0,y0)-(x1,y1) clipped:
		wxCoord dx0 = x0, dx1 = x1, dy0 = y0, dy1 = y1;
		if (dx0 < 0)
			dx0 = -borderMarginX;
		if (dy0 < 0)
			dy0 = -borderMarginY;
		if (dx1 > w.GetScreenX())
			dx1 = w.GetScreenX() + borderMarginX;
		if (dy1 > w.GetScreenY())
			dy1 = w.GetScreenY() + borderMarginY;

		// For convenience, compute the width/height of the rectangle to be actually drawn:
		wxCoord d_width = dx1 - dx0 + 1;
		wxCoord d_height = dy1 - dy0 + 1;

		// Compute the pixel offsets in the internally stored bitmap:
		wxCoord offset_x = (wxCoord) ((dx0 - x0) / screenPixelX);
		wxCoord offset_y = (wxCoord) ((dy0 - y0) / screenPixelY);

		// and the size in pixel of the area to be actually drawn from the internally stored bitmap:
		wxCoord b_width = (wxCoord) ((dx1 - dx0 + 1) / screenPixelX);
		wxCoord b_height = (wxCoord) ((dy1 - dy0 + 1) / screenPixelY);

#ifdef MATHPLOT_DO_LOGGING
		wxLogMessage(wxT("[mpBitmapLayer::Plot] screenPixel: x=%f y=%f  d_width=%ix%i"),
				screenPixelX, screenPixelY, d_width, d_height);
		wxLogMessage(wxT("[mpBitmapLayer::Plot] offset: x=%i y=%i  bmpWidth=%ix%i"),
				offset_x, offset_y, b_width, b_height);
#endif

		// Is there any visible region?
		if (d_width > 0 && d_height > 0)
		{
			// Build the scaled bitmap from the image, only if it has changed:
			if (m_scaledBitmap.GetWidth() != d_width || m_scaledBitmap.GetHeight() != d_height || m_scaledBitmap_offset_x != offset_x
					|| m_scaledBitmap_offset_y != offset_y)
			{
				wxRect r(wxRect(offset_x, offset_y, b_width, b_height));
				// Just for the case....
				if (r.x < 0)
					r.x = 0;
				if (r.y < 0)
					r.y = 0;
				if (r.width > m_bitmap.GetWidth())
					r.width = m_bitmap.GetWidth();
				if (r.height > m_bitmap.GetHeight())
					r.height = m_bitmap.GetHeight();

				m_scaledBitmap = wxBitmap(wxBitmap(m_bitmap).GetSubBitmap(r).ConvertToImage().Scale(d_width, d_height));
				m_scaledBitmap_offset_x = offset_x;
				m_scaledBitmap_offset_y = offset_y;
			}

			// Draw it:
			dc.DrawBitmap(m_scaledBitmap, dx0, dy0, true);
		}
	}

	// Draw the name label
	if (!m_name.IsEmpty() && m_showName)
	{
		dc.SetFont(m_font);
		dc.SetTextForeground(m_fontcolour);

		wxCoord tx, ty;
		dc.GetTextExtent(m_name, &tx, &ty);

		if (HasBBox())
		{
			wxCoord sx = (wxCoord) ((m_max_x - w.GetPosX()) * w.GetScaleX());
			wxCoord sy = (wxCoord) ((w.GetPosY() - m_max_y) * w.GetScaleY());

			tx = sx - tx - 8;
			ty = sy - 8 - ty;
		}
		else
		{
			const int sx = w.GetScreenX() >> 1;
			const int sy = w.GetScreenY() >> 1;

			switch (m_flags)
			{
				case mpALIGN_NW:
				{
					tx = -sx + 8;
					ty = -sy + 8;
					break;
				}
				case mpALIGN_NE:
				{
					tx = sx - tx - 8;
					ty = -sy + 8;
					break;
				}
				case mpALIGN_SE:
				{
					tx = sx - tx - 8;
					ty = sy - 8 - ty;
					break;
				}
				default:
				{ // mpALIGN_SW
					tx = -sx + 8;
					ty = sy - 8 - ty;
				}
			}
		}

		dc.DrawText(m_name, tx, ty);
	}
}

//-----------------------------------------------------------------------------
// End of file
//-----------------------------------------------------------------------------

