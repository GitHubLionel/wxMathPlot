/////////////////////////////////////////////////////////////////////////////
// Name:            mathplot.cpp
// Purpose:         Framework for plotting in wxWindows
// Original Author: David Schalig
// Maintainer:      Davide Rondini
// Contributors:    Jose Luis Blanco, Val Greene, Lionel Reynaud, Dave Nadler
// Created:         21/07/2003
// Last edit:       25/12/2024
// Copyright:       (c) David Schalig, Davide Rondini
// Licence:         wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "mathplot.h"
#endif

#include <algorithm>

// For compilers that support precompilation, includes "wx.h".
#include <wx/wx.h>
// #include <wx/window.h>
// #include <wx/wxprec.h>

// Comment out for release operation:
// (Added by J.L.Blanco, Aug 2007)
//#define MATHPLOT_DO_LOGGING
//#define MATHPLOT_LOG_SCALE  // For log scale debug

// To help debug
#define DEBUG_COUT(message) std::cout << #message << std::endl;
#define DEBUG_COUT2(message, test)  std::cout << #message << ((test) ? " true":" false") << std::endl;

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
#include "Images/Fullscreen.h"
#include "Images/Grid.h"
#include "Images/Load.h"
#include "Images/Mouse.h"
#include "Images/Screenshot.h"
#include "Images/Zoom_in.h"
#include "Images/Zoom_out.h"
#endif

wxString Popup_string[mpID_FULLSCREEN - mpID_FIT + 1][2] = {_T("")};

// List of string message used
wxString MESS_HELP0 = _T("");
wxString MESS_HELP1 = _T("");
wxString MESS_HELP2 = _T("");
wxString MESS_HELP3 = _T("");
wxString MESS_HELP4 = _T("");
wxString MESS_HELP5 = _T("");
wxString MESS_HELP6 = _T("");

wxString MESS_LOAD = _T("");
wxString MESS_WILDCARD = _T("");

wxString MESS_CLIPFAIL = _T("");

void mpWindow::FillI18NString()
{
  Popup_string[0][0] = _("Fit");
  Popup_string[0][1] = _("Set plot view to show all items");
  Popup_string[1][0] = _("Zoom in");
  Popup_string[1][1] = _("Zoom in plot view.");
  Popup_string[2][0] = _("Zoom out");
  Popup_string[2][1] = _("Zoom out plot view.");
  Popup_string[3][0] = _("Center to this position");
  Popup_string[3][1] = _("Center plot view to this position");
  Popup_string[4][0] = _("Lock aspect");
  Popup_string[4][1] = _("Lock horizontal and vertical zoom aspect.");
  Popup_string[5][0] = _("Toggle grids");
  Popup_string[5][1] = _("Show/Hide grids");
  Popup_string[6][0] = _("Toggle info coords");
  Popup_string[6][1] = _("Show/Hide info coordinates");
  Popup_string[7][0] = _("Screen shot");
  Popup_string[7][1] = _("Copy a screen shot to the clipboard");
  Popup_string[8][0] = _("Configuration");
  Popup_string[8][1] = _("Plot configuration");
  Popup_string[9][0] = _("Load file");
  Popup_string[9][1] = _("Load data file");
  Popup_string[10][0] = _("Show mouse commands...");
  Popup_string[10][1] = _("Show help about the mouse commands.");
  Popup_string[11][0] = _("Toggle fullscreen");
  Popup_string[11][1] = _("Toggle fullscreen.");

  // List of string message used
  MESS_HELP0 = _("wxMathPlot help");
  MESS_HELP1 = _("Supported Mouse commands:");
  MESS_HELP2 = _(" - Left button down + Mark area: Rectangular zoom");
  MESS_HELP3 = _(" - Right button down + Move: Pan (Move)");
  MESS_HELP4 = _(" - Wheel: Zoom in/out");
  MESS_HELP5 = _(" - Wheel + SHIFT: Horizontal scroll");
  MESS_HELP6 = _(" - Wheel + CTRL: Vertical scroll");

  MESS_LOAD = _("Select file");
  MESS_WILDCARD = _("Data files (*.dat)|*.dat|Csv files (csv.*)|csv.*|All files (*.*)|*.*");

  MESS_CLIPFAIL = _("Failed to copy image to clipboard");
}

// Memory leak debugging
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Legend margins
#define MARGIN_LEGEND 5 // The margin around the legend (the name of the function) in pixel
#define LEGEND_LINEWIDTH 10 // The size of the decoration in front of the legend (line or square)

// Minimum axis label separation
#define MIN_X_AXIS_LABEL_SEPARATION 64
#define MIN_Y_AXIS_LABEL_SEPARATION 32

// Number of pixels to scroll when scrolling by a line
#define SCROLL_NUM_PIXELS_PER_LINE  10

// Offset for the text in margin
#define MARGIN_TOP_OFFSET 5
#define MARGIN_BOTTOM_OFFSET 5
#define MARGIN_LEFT_OFFSET 5
#define MARGIN_RIGHT_OFFSET 5

// Margin for the coordinates
#define MARGIN_COORD  5
#define MARGIN_COORD_X2 2*MARGIN_COORD

// See doxygen comments.
double mpWindow::m_zoomIncrementalFactor = 1.5;

// Delete and null pointer
#define DeleteAndNull(ptr)  \
  { if (ptr) \
    { delete(ptr);  \
      (ptr) = NULL; }}

//-----------------------------------------------------------------------------
// Date time conversion
//-----------------------------------------------------------------------------

bool DoubleToTimeStruct(double val, unsigned int timeConv, struct tm *timestruct)
{
  time_t when = (time_t)val;

  if (when > 0)
  {
    if (timeConv == mpX_LOCALTIME)
    {
      *timestruct = *localtime(&when);
    }
    else
    {
      *timestruct = *gmtime(&when);
    }
    return true;
  }
  return false;
}

//-----------------------------------------------------------------------------
// mpLayer
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpLayer, wxObject)

mpLayer::mpLayer(mpLayerType layerType) :
    m_type(layerType)
{
  // The wxWindow handle is not yet available
  m_win = NULL;
  // Default pen
  SetPen((wxPen const&)*wxBLACK_PEN);
  // Default font
  SetFont((wxFont const&)*wxNORMAL_FONT);
  m_fontcolour = (wxColour const&)*wxBLACK;
  // Default brush
  SetBrush((wxBrush const&)*wxTRANSPARENT_BRUSH);
  m_showName = false;  // Default
  m_drawOutsideMargins = false;
  m_visible = true;
  m_tractable = false;
  m_flags = mpALIGN_NE;
  m_CanDelete = true;
  m_busy = false;
  m_ZIndex = mpZIndex_BACKGROUND;
}

void mpLayer::GetBBox(mpFloatRect *m_bound)
{
  m_bound->Xmin = GetMinX();
  m_bound->Xmax = GetMaxX();
  m_bound->Ymin = GetMinY();
  m_bound->Ymax = GetMaxY();
}

void mpLayer::Plot(wxDC &dc, mpWindow &w)
{
  if ((!m_visible) || m_busy)
    return;

  m_busy = true;
  DoBeforePlot();
  UpdateContext(dc);
  DoPlot(dc, w);
  m_busy = false;
}

void mpLayer::UpdateContext(wxDC &dc) const
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

void mpLayer::CheckLog(double *x, double *y)
{
  if (m_win == NULL)
    return;
  if (m_win->IsLogXaxis())
    *x = log10(*x);
  if (m_win->IsLogYaxis())
    *y = log10(*y);
}

//-----------------------------------------------------------------------------
// mpInfoLayer
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpInfoLayer, mpLayer)

mpInfoLayer::mpInfoLayer() : mpLayer(mpLAYER_INFO)
{
  m_subtype = mpiInfo;
  m_dim = wxRect(0, 0, 1, 1);
  m_oldDim = m_dim;
  m_info_bmp = NULL;
  m_brush = *wxTRANSPARENT_BRUSH;
  m_brush.SetColour(*wxWHITE);
  m_reference.x = 0;
  m_reference.y = 0;
  m_winX = 1;
  m_winY = 1;
  m_location = mpMarginNone;
  m_ZIndex = mpZIndex_INFO;
}

mpInfoLayer::mpInfoLayer(wxRect rect, const wxBrush &brush, mpLocation location) :
    mpInfoLayer()
{
  m_brush = brush;
  if (m_brush.GetStyle() == wxBRUSHSTYLE_TRANSPARENT)
    m_brush.SetColour(*wxWHITE);
  m_reference.x = rect.x;
  m_reference.y = rect.y;
  m_location = location;
}

mpInfoLayer::~mpInfoLayer()
{
  DeleteAndNull(m_info_bmp);
}

void mpInfoLayer::SetVisible(bool show)
{
  m_visible = show;
  DeleteAndNull(m_info_bmp);
}

void mpInfoLayer::UpdateInfo(mpWindow &WXUNUSED(w), wxEvent &WXUNUSED(event))
{
// Nothing to do here
}

bool mpInfoLayer::Inside(const wxPoint &point)
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
//      wxLogMessage(_T("mpInfoLayer::Plot() screen size has changed from %d x %d to %d x %d"), m_winX, m_winY, scrx, scry);
#endif
      if (m_winX != 1)
        m_dim.x = (int)floor((double)(m_dim.x * scrx / m_winX));
      if (m_winY != 1)
      {
        m_dim.y = (int)floor((double)(m_dim.y * scry / m_winY));
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

void mpInfoLayer::DoPlot(wxDC &dc, mpWindow &w)
{
  SetInfoRectangle(w);
  dc.DrawRectangle(m_dim);
}

void mpInfoLayer::ErasePlot(wxDC &dc, mpWindow &WXUNUSED(w))
{
  if (m_info_bmp)
  {
    wxMemoryDC m_info_dc(&dc);
    m_info_dc.SelectObject(*m_info_bmp);
    dc.Blit(m_oldDim.x, m_oldDim.y, m_oldDim.width, m_oldDim.height, &m_info_dc, 0, 0);
    m_info_dc.SelectObject(wxNullBitmap);
    DeleteAndNull(m_info_bmp);
  }
}

//-----------------------------------------------------------------------------
// mpInfoCoords
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpInfoCoords, mpInfoLayer)

mpInfoCoords::mpInfoCoords() :
    mpInfoLayer()
{
  m_subtype = mpiCoords;
  m_labelType = mpX_NORMAL;
  m_timeConv = 0;
  m_mouseX = m_mouseY = 0;
  m_location = mpMarginBottomRight;
  wxBrush coord(wxColour(232, 232, 232), wxBRUSHSTYLE_SOLID);
  SetBrush(coord);
  m_series_coord = false;
  // Default pen
  SetPenSeries((wxPen const&)*wxBLACK_PEN);
}

mpInfoCoords::mpInfoCoords(mpLocation location) :
    mpInfoLayer()
{
  m_subtype = mpiCoords;
  m_labelType = mpX_NORMAL;
  m_timeConv = 0;
  m_mouseX = m_mouseY = 0;
  m_location = location;
  m_series_coord = false;
  wxBrush coord(wxColour(232, 232, 232), wxBRUSHSTYLE_SOLID);
  SetBrush(coord);
  m_content = _T("");
}

mpInfoCoords::mpInfoCoords(wxRect rect, const wxBrush &brush, mpLocation location) :
    mpInfoLayer(rect, brush, location)
{
  m_subtype = mpiCoords;
  m_labelType = mpX_NORMAL;
  m_timeConv = 0;
  m_mouseX = m_mouseY = 0;
  m_series_coord = false;
  m_content = _T("");
}

void mpInfoCoords::UpdateInfo(mpWindow &w, wxEvent &event)
{
  m_content.Clear();

  if (event.GetEventType() == wxEVT_MOTION)
  {
    double xVal = 0.0, yVal = 0.0, y2Val = 0.0;
    bool isY2Axis = false;

    m_mouseX = ((wxMouseEvent&)event).GetX();
    m_mouseY = ((wxMouseEvent&)event).GetY();

    if (m_series_coord)
    {
      mpLayer* layer = w.GetClosestPlot(m_mouseX, m_mouseY, &xVal, &yVal, &isY2Axis);
      if (layer)
      {
        // Just change the colour
        m_penSeries.SetColour(layer->GetPen().GetColour());
      }
      else
        return;
    }
    else
    {
      xVal = w.p2x(m_mouseX);
      yVal = w.p2y(m_mouseY);
      y2Val = w.p2y(m_mouseY, true);
    }

    // Log axis
    if (m_win->IsLogXaxis())
      xVal = pow(10, xVal);
    if (m_win->IsLogYaxis())
      yVal = pow(10, yVal);

    m_content = GetInfoCoordsText(xVal, yVal, y2Val, isY2Axis);
  }
}

wxString mpInfoCoords::GetInfoCoordsText(double xVal, double yVal, double y2Val, bool isY2Axis) {
  wxString result;
  struct tm timestruct;
  // Format X part
  switch (m_labelType)
  {
    case mpX_NORMAL:
    case mpX_USER:
      result.Printf(_T("x = %g"), xVal);
      break;
    case mpX_DATETIME:
    {
      if (DoubleToTimeStruct(xVal, m_timeConv, &timestruct))
        result.Printf(_T("x = %04d-%02d-%02dT%02d:%02d:%02d"), timestruct.tm_year + 1900, timestruct.tm_mon + 1, timestruct.tm_mday,
            timestruct.tm_hour, timestruct.tm_min, timestruct.tm_sec);
      break;
    }
    case mpX_DATE:
    {
      if (DoubleToTimeStruct(xVal, m_timeConv, &timestruct))
        result.Printf(_T("x = %04d-%02d-%02d"), timestruct.tm_year + 1900, timestruct.tm_mon + 1, timestruct.tm_mday);
      break;
    }
    case mpX_TIME:
    case mpX_HOURS:
    {
      double sign = (xVal < 0) ? -1.0 : 1.0;
      double modulus = fabs(xVal);
      double hh = floor(modulus / 3600);
      double mm = floor((modulus - hh * 3600) / 60);
      double ss = modulus - hh * 3600 - mm * 60;
      result.Printf(_T("x = %02.0f:%02.0f:%02.0f"), sign * hh, mm, floor(ss));
      break;
    }
    default:
      ;
  }

  // Format Y part
  if (m_win->Y2AxisExist())
  {
    if (m_series_coord)
    {
      if (isY2Axis) // The value is on y2 axis
        result.Printf(_T("%s\ny2 = %g"), result, y2Val);
      else
        result.Printf(_T("%s\ny = %g"), result, yVal);
    } else {
      result.Printf(_T("%s\ny = %g\ny2 = %g"), result, yVal, y2Val);
    }
  } else {
    result.Printf(_T("%s\ny = %g"), result, yVal);
  }

  return result;
}

void mpInfoCoords::ErasePlot(wxDC &dc, mpWindow &w)
{
  mpInfoLayer::ErasePlot(dc, w);
  m_content.Empty();
}

void mpInfoCoords::DoPlot(wxDC &dc, mpWindow &w)
{
  if (m_content.IsEmpty())
  {
    ErasePlot(dc, w);
    return;
  }

  int textX = 0, textY = 0;
  int width = 0, height = 0;
  int offset = (m_series_coord) ? LEGEND_LINEWIDTH : 0;

// Should be work on Windows and Linux. If no, use GetTextExtent for Linux
  dc.GetMultiLineTextExtent(m_content, &textX, &textY);
  if (width < textX + MARGIN_COORD_X2 + offset)
    width = textX + MARGIN_COORD_X2 + offset;
  if (height < textY + MARGIN_COORD_X2)
    height = textY + MARGIN_COORD_X2;
  textY /= 2;

  SetInfoRectangle(w, width, height);
  if (m_location == mpCursor)
  {
    m_dim.x = m_mouseX + 2 * MARGIN_BOTTOM_OFFSET;
    if (m_dim.x + m_dim.width > w.GetScreenX())
      m_dim.x = m_mouseX - m_dim.width - 5;
    m_dim.y = m_mouseY + 2 * MARGIN_BOTTOM_OFFSET;
    if (m_dim.y + m_dim.height > w.GetScreenY())
      m_dim.y = m_mouseY - m_dim.height - 5;
  }

  // Don't use stored bitmap when we repaint all
  if (w.IsRepainting())
    DeleteAndNull(m_info_bmp);

  // First : restore stored bitmap
  if (m_info_bmp)
  {
    wxMemoryDC m_coord_dc(&dc);
    m_coord_dc.SelectObject(*m_info_bmp);
    dc.Blit(m_oldDim.x, m_oldDim.y, m_oldDim.width, m_oldDim.height, &m_coord_dc, 0, 0);
    m_coord_dc.SelectObject(wxNullBitmap);
    DeleteAndNull(m_info_bmp);
  }

  // Second : store new bitmap
  m_info_bmp = new wxBitmap(m_dim.width, m_dim.height, dc);
  wxMemoryDC m_coord_dc(&dc);
  m_coord_dc.SelectObject(*m_info_bmp);
  m_coord_dc.Blit(0, 0, m_dim.width, m_dim.height, &dc, m_dim.x, m_dim.y);
  m_coord_dc.SelectObject(wxNullBitmap);
  m_oldDim = m_dim;

  // Third : draw the coordinate
  dc.DrawRectangle(m_dim);
  dc.DrawText(m_content, m_dim.x + MARGIN_COORD + offset, m_dim.y + MARGIN_COORD);
  if (m_series_coord)
  {
    textY = m_dim.y + MARGIN_COORD + textY + (textY / 2) + 2;
    dc.SetPen(m_penSeries);
    wxBrush sqrBrush(m_penSeries.GetColour(), wxBRUSHSTYLE_SOLID);
    dc.SetBrush(sqrBrush);
    dc.DrawRectangle(m_dim.x + 2, textY - (LEGEND_LINEWIDTH / 2),
    LEGEND_LINEWIDTH, LEGEND_LINEWIDTH);
  }
}

//-----------------------------------------------------------------------------
// mpInfoLegend
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpInfoLegend, mpInfoLayer)

mpInfoLegend::mpInfoLegend() :
    mpInfoLayer()
{
  m_subtype = mpiLegend;
  SetBrush(*wxWHITE_BRUSH);
  m_item_mode = mpLegendLine;
  m_item_direction = mpVertical;
  m_location = mpMarginBottomCenter;
  m_needs_update = true;
}

mpInfoLegend::mpInfoLegend(wxRect rect, const wxBrush &brush, mpLocation location) :
    mpInfoLayer(rect, brush, location)
{
  m_subtype = mpiLegend;
  m_item_mode = mpLegendLine;
  m_item_direction = mpVertical;
  m_needs_update = true;
}

void mpInfoLegend::UpdateBitmap(wxDC &dc, mpWindow &w)
{
  // Create a temporary bitmap to draw the legend
  wxBitmap* buff_bmp = new wxBitmap(w.GetScreenX(), w.GetScreenY(), dc);
  wxMemoryDC buff_dc(&dc);
  buff_dc.SelectObject(*buff_bmp);

  buff_dc.SetFont(m_font);
  buff_dc.SetTextForeground(m_fontcolour);

  buff_dc.SetPen(*wxTRANSPARENT_PEN);
  if (m_brush.GetStyle() == wxBRUSHSTYLE_TRANSPARENT)
    buff_dc.SetBrush(*wxWHITE_BRUSH);
  else
    buff_dc.SetBrush(m_brush);
  buff_dc.DrawRectangle(0, 0, w.GetScreenX(), w.GetScreenY());

  int posX = 0, posY = 0; // position of the current label
  int width = 0, height = 0; // accumulated dimensions of complete legend
  bool first = true;

  // Get series name and create new bitmap legend
  m_LegendDetailList.clear();
  unsigned int layerIdx = 0;
  for (unsigned int p = 0; p < w.CountAllLayers(); p++)
  {
    mpLayer* ly = w.GetLayer(p);
    if (ly->GetLayerType() == mpLAYER_PLOT)
    {
      if (ly->IsVisible())
      {
        int labelWidth = 0, labelHeight = 0;
        wxString label = ly->GetName();
        wxPen lpen = ly->GetPen();
        lpen.SetWidth(2);
        buff_dc.SetPen(lpen);
        buff_dc.GetTextExtent(label, &labelWidth, &labelHeight);

        if (first)
        {
          posX = MARGIN_LEGEND;
          posY = MARGIN_LEGEND + (labelHeight / 2);
          // Since labelHeight is constant (all label layers use same legend font), we can initialise height of the legend bitmap
          height = posY + labelHeight;
          first = false;
        }

        // Draw the decoration
        if (m_item_mode == mpLegendLine)
        {
          buff_dc.DrawLine(posX, posY + 1, posX + LEGEND_LINEWIDTH, posY + 1);
        }
        else  // m_item_mode == mpLEGEND_SQUARE
        {
          wxBrush sqrBrush(*wxWHITE, wxBRUSHSTYLE_SOLID);
          sqrBrush.SetColour(lpen.GetColour());
          buff_dc.SetBrush(sqrBrush);
          buff_dc.DrawRectangle(posX, posY - (LEGEND_LINEWIDTH / 2) + 1,
                LEGEND_LINEWIDTH, LEGEND_LINEWIDTH);
        }

        // Draw the name of the function after the decoration
        posX += LEGEND_LINEWIDTH + MARGIN_LEGEND;
        buff_dc.DrawText(label, posX, posY - (labelHeight / 2));

        posX += labelWidth + 2 * MARGIN_LEGEND;

        // Adjust the full size of the Legend and store the end (bottom or right) of this legend component
        LegendDetail ld;
        if (m_item_direction == mpVertical)
        {
          width = std::max(width, posX);
          posX = MARGIN_LEGEND;
          posY += labelHeight;
          height = ld.legendEnd = posY;
          posY += 2 * MARGIN_LEGEND;
        }
        else
        {
          width = ld.legendEnd = posX;
        }
        ld.layerIdx = layerIdx;
        m_LegendDetailList.push_back(ld);
      }
      layerIdx++;
    }
  }

  // Delete old bitmap
  DeleteAndNull(m_info_bmp);

  // Copy new bitmap to m_legend_bmp
  if ((width != 0) && (height != 0))
  {
    buff_dc.SetPen(*wxBLACK_PEN);
    buff_dc.SetBrush(*wxTRANSPARENT_BRUSH);

    // If necessary, truncate legend to fit in window
    width = std::min(width, w.GetScreenX());
    height = std::min(height, w.GetScreenY());

    buff_dc.DrawRectangle(0, 0, width, height);
    SetInfoRectangle(w, width, height);

    // Transfer to the legend bitmap
    m_info_bmp = new wxBitmap(width, height, dc);
    wxMemoryDC buff_dc2(&dc);
    buff_dc2.SelectObject(*m_info_bmp);
    buff_dc2.Blit(0, 0, width, height, &buff_dc, 0, 0);
    buff_dc2.SelectObject(wxNullBitmap);
    m_needs_update = false;
  }
  else
    // To avoid mouse focus on empty legend
    SetInfoRectangle(w, 1, 1);

  buff_dc.SelectObject(wxNullBitmap);
  delete buff_bmp;
}

void mpInfoLegend::DoPlot(wxDC &dc, mpWindow &w)
{
  if (m_needs_update)
    UpdateBitmap(dc, w);
  else
    // In case we have resize
    SetInfoRectangle(w, m_dim.width, m_dim.height);

  if (m_info_bmp && (m_dim.width != 0) && (m_dim.height != 0))
  {
    wxMemoryDC buff_dc(&dc);
    buff_dc.SelectObject(*m_info_bmp);

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

int mpInfoLegend::GetPointed(mpWindow &WXUNUSED(w), wxPoint eventPoint)
{
  // Adjust clicked point coordinates for legend bitmap's offset within plot area
  wxCoord side;
  if (m_item_direction == mpVertical)
    side = eventPoint.y - m_dim.y;
  else
    side = eventPoint.x - m_dim.x;
  // Find which series legend we have clicked
  // We only need test against right or bottom side of the rectangle (stored in UpdateBitmap function).
  for (std::vector<LegendDetail>::iterator it = m_LegendDetailList.begin(); it != m_LegendDetailList.end(); it++)
  {
    const LegendDetail& ld = *it;
    if (side < ld.legendEnd)
      return ld.layerIdx;
  }
  return -1;
}

//-----------------------------------------------------------------------------
// mpFunction implementations - functions
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpFunction, mpLayer)

mpFunction::mpFunction(mpLayerType layerType /*=mpLAYER_PLOT*/, const wxString &name/*=wxEmptyString*/, bool useY2Axis/*=false*/) :
    mpLayer(layerType)
{
  m_subtype = mpfAllType;
  SetName(name);
  m_symbol = mpsNone;
  m_symbolSize = 6;
  m_symbolSize2 = 3;
  m_continuous = false; // Default
  m_step = 1;
  m_UseY2Axis = useY2Axis;
  m_ZIndex = mpZIndex_PLOT;
}

void mpFunction::DrawSymbol(wxDC &dc, wxCoord x, wxCoord y)
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
// mpHorizontalLine implementations - functions
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpLine, mpFunction)

mpLine::mpLine(double value, const wxPen &pen) :
    mpFunction(mpLAYER_LINE)
{
  m_value = value;
  m_subtype = mpfLine;
  m_ZIndex = mpZIndex_LINE;
  m_IsHorizontal = false;
  SetPen(pen);
  SetDrawOutsideMargins(false);
}

//-----------------------------------------------------------------------------
// mpHorizontalLine implementations - functions
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpHorizontalLine, mpFunction)

mpHorizontalLine::mpHorizontalLine(double yvalue, const wxPen &pen, bool useY2Axis) :
    mpLine(yvalue, pen)
{
  SetName(wxT("Horizontal line"));
  m_IsHorizontal = true;
  SetY2Axis(useY2Axis);
}

void mpHorizontalLine::DoPlot(wxDC &dc, mpWindow &w)
{
  // Get boundaries
  m_plotBoundaries = w.GetPlotBoundaries(!m_drawOutsideMargins);

  wxCoord iy;
  if (m_win->IsLogYaxis())
    iy = w.y2p(log10(m_value), m_UseY2Axis);
  else
    iy = w.y2p(m_value, m_UseY2Axis);

  // Draw nothing if we are outside margins
  if (!m_drawOutsideMargins && ((iy > (w.GetScreenY() - w.GetMarginBottom())) || (iy < w.GetMarginTop())))
    return;

  // Draw horizontal line from boundary minX to boundary maxX
  dc.DrawLine(m_plotBoundaries.startPx, iy, m_plotBoundaries.endPx, iy);

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

    dc.DrawText(m_name, tx, iy);
  }
}

//-----------------------------------------------------------------------------
// mpVerticalLine implementations - functions
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpVerticalLine, mpFunction)

mpVerticalLine::mpVerticalLine(double xvalue, const wxPen &pen) :
    mpLine(xvalue, pen)
{
  SetName(wxT("Vertical line"));
  m_IsHorizontal = false;
}

void mpVerticalLine::DoPlot(wxDC &dc, mpWindow &w)
{
  // Get boundaries
  m_plotBoundaries = w.GetPlotBoundaries(!m_drawOutsideMargins);

  wxCoord ix;
  if (m_win->IsLogXaxis())
    ix = w.x2p(log10(m_value));
  else
    ix = w.x2p(m_value);

  // Draw nothing if we are outside margins
  if (!m_drawOutsideMargins && ((ix > (w.GetScreenX() - w.GetMarginRight())) || (ix + 1 < w.GetMarginLeft())))
    return;

  // Draw vertical line from boundary minX to boundary maxX
  dc.DrawLine(ix, m_plotBoundaries.startPy, ix, m_plotBoundaries.endPy);

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

    dc.DrawText(m_name, ix, ty);
  }
}

//-----------------------------------------------------------------------------
// mpFX implementations - functions
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpFX, mpFunction)

mpFX::mpFX(const wxString &name, int flags, bool useY2Axis) :
    mpFunction(mpLAYER_PLOT, name, useY2Axis)
{
  m_subtype = mpfFX;
  m_flags = flags;
}

double mpFX::DoGetY(double x)
{
  double y = GetY(x);
  if (m_win->IsLogYaxis())
    y = log10(y);
  return y;
}

void mpFX::DoPlot(wxDC &dc, mpWindow &w)
{
  wxCoord i, iy, iylast;

  // Get boundaries
  m_plotBoundaries = w.GetPlotBoundaries(!m_drawOutsideMargins);

  if (!m_drawOutsideMargins)
  {
    wxRect rect(m_plotBoundaries.startPx, m_plotBoundaries.startPy,
        m_plotBoundaries.endPx - m_plotBoundaries.startPx,
        m_plotBoundaries.endPy - m_plotBoundaries.startPy);
    dc.SetClippingRegion(rect);
  }

  if (m_continuous || (m_pen.GetWidth() > 1))
  {
    if (m_continuous)
    {
      // Get first point
      iylast = w.y2p(DoGetY(w.p2x(m_plotBoundaries.startPx)), m_UseY2Axis);

      for (i = m_plotBoundaries.startPx + m_step; i < m_plotBoundaries.endPx; i += m_step)
      {
        iy = w.y2p(DoGetY(w.p2x(i)), m_UseY2Axis);
        dc.DrawLine(i - m_step, iylast, i, iy);
        if (m_symbol != mpsNone)
          DrawSymbol(dc, i - m_step, iylast);
        iylast = iy;
      }
      // Last point
      if (m_symbol != mpsNone)
        DrawSymbol(dc, i - m_step, iylast);
    }
    else
    {
      for (i = m_plotBoundaries.startPx; i < m_plotBoundaries.endPx; i += m_step)
      {
        iy = w.y2p(DoGetY(w.p2x(i)), m_UseY2Axis);
        if (m_symbol == mpsNone)
          dc.DrawLine(i, iy, i, iy);
        else
          DrawSymbol(dc, i, iy);
      }
    }
  }
  else
  {
    for (i = m_plotBoundaries.startPx; i < m_plotBoundaries.endPx; i += m_step)
    {
      iy = w.y2p(DoGetY(w.p2x(i)), m_UseY2Axis);
      if (m_symbol == mpsNone)
        dc.DrawPoint(i, iy);
      else
        DrawSymbol(dc, i, iy);
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

    dc.DrawText(m_name, tx, w.y2p(DoGetY(w.p2x(tx)), m_UseY2Axis));
  }
}

//-----------------------------------------------------------------------------
// mpFY implementations - functions
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpFY, mpFunction)

mpFY::mpFY(const wxString &name, int flags, bool useY2Axis) :
    mpFunction(mpLAYER_PLOT, name, useY2Axis)
{
  m_subtype = mpfFY;
  m_flags = flags;
}

double mpFY::DoGetX(double y)
{
  double x = GetX(y);
  if (m_win->IsLogXaxis())
    x = log10(x);
  return x;
}

void mpFY::DoPlot(wxDC &dc, mpWindow &w)
{
  wxCoord i, ix, ixlast;

  // Get boundaries
  m_plotBoundaries = w.GetPlotBoundaries(!m_drawOutsideMargins);

  if (!m_drawOutsideMargins)
  {
    wxRect rect(m_plotBoundaries.startPx, m_plotBoundaries.startPy,
        m_plotBoundaries.endPx - m_plotBoundaries.startPx,
        m_plotBoundaries.endPy - m_plotBoundaries.startPy);
    dc.SetClippingRegion(rect);
  }

  if (m_continuous || (m_pen.GetWidth() > 1))
  {
    if (m_continuous)
    {
      // Get first point
      ixlast = w.x2p(DoGetX(w.p2y(m_plotBoundaries.startPy, m_UseY2Axis)));

      for (i = m_plotBoundaries.startPy + m_step; i < m_plotBoundaries.endPy; i += m_step)
      {
        ix = w.x2p(DoGetX(w.p2y(i, m_UseY2Axis)));
        dc.DrawLine(ixlast, i - m_step, ix, i);
        if (m_symbol != mpsNone)
          DrawSymbol(dc, ixlast, i - m_step);
        ixlast = ix;
      }
      // Last point
      if (m_symbol != mpsNone)
        DrawSymbol(dc, ixlast, i - m_step);
    }
    else
    {
      for (i = m_plotBoundaries.startPy; i < m_plotBoundaries.endPy; i += m_step)
      {
        ix = w.x2p(DoGetX(w.p2y(i, m_UseY2Axis)));
        if (m_symbol == mpsNone)
          dc.DrawLine(ix, i, ix, i);
        else
          DrawSymbol(dc, ix, i);
      }
    }
  }
  else
  {
    for (i = m_plotBoundaries.startPy; i < m_plotBoundaries.endPy; i += m_step)
    {
      ix = w.x2p(DoGetX(w.p2y(i, m_UseY2Axis)));
      if (m_symbol == mpsNone)
        dc.DrawPoint(ix, i);
      else
        DrawSymbol(dc, ix, i);
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

    dc.DrawText(m_name, w.x2p(DoGetX(w.p2y(ty, m_UseY2Axis))), ty);
  }
}

//-----------------------------------------------------------------------------
// mpFXY implementations - functions
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpFXY, mpFunction)

mpFXY::mpFXY(const wxString &name, int flags, bool viewAsBar, bool useY2Axis) :
    mpFunction(mpLAYER_PLOT, name, useY2Axis)
{
  m_subtype = mpfFXY;
  m_flags = flags;
  maxDrawX = minDrawX = maxDrawY = minDrawY = 0;
  m_deltaX = m_deltaY = 1e+308; // Big number
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
  // Keep track of how many points have been drawn into the bounding box
  if (xnew > maxDrawX)
    maxDrawX = xnew;
  else
    if (xnew < minDrawX)
      minDrawX = xnew;
  if (ynew > maxDrawY)
    maxDrawY = ynew;
  else
    if (ynew < minDrawY)
      minDrawY = ynew;
}

bool mpFXY::DoGetNextXY(double *x, double *y)
{
  bool result = GetNextXY(x, y);
  if (result)
  { // only log-scale result if there is actually a result...
    CheckLog(x, y);
  }
  return result;
}

void mpFXY::DoPlot(wxDC &dc, mpWindow &w)
{
  double x, y;
  // Do this to reset the counters to evaluate bounding box for label positioning
  Rewind();
  // Get first point
  DoGetNextXY(&x, &y);
  maxDrawX = minDrawX = (int)x;
  maxDrawY = minDrawY = (int)y;

  wxCoord ix = 0, iy = 0;
  wxCoord ixlast = 0, iylast = 0;

  // Get boundaries
  m_plotBoundaries = w.GetPlotBoundaries(!m_drawOutsideMargins);

  if (!m_drawOutsideMargins)
  {
    wxRect rect(m_plotBoundaries.startPx , m_plotBoundaries.startPy,
        m_plotBoundaries.endPx - m_plotBoundaries.startPx,
        m_plotBoundaries.endPy - m_plotBoundaries.startPy);
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
        iylast = w.y2p(y, m_UseY2Axis);

        while (DoGetNextXY(&x, &y))
        {
          ix = w.x2p(x);
          iy = w.y2p(y, m_UseY2Axis);

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
        Rewind();
        while (DoGetNextXY(&x, &y))
        {
          ix = w.x2p(x);
          iy = w.y2p(y, m_UseY2Axis);
          if (m_symbol == mpsNone)
            dc.DrawLine(ix, iy, ix, iy);
          else
            DrawSymbol(dc, ix, iy);
          UpdateViewBoundary(ix, iy);
        }
      }
    }
    else
    {
      // Not continuous and pen width = 1
      Rewind();
      while (DoGetNextXY(&x, &y))
      {
        ix = w.x2p(x);
        iy = w.y2p(y, m_UseY2Axis);
        if (m_symbol == mpsNone)
          dc.DrawPoint(ix, iy);
        else
          DrawSymbol(dc, ix, iy);
        UpdateViewBoundary(ix, iy);
      }
    }
  }
  else // View as bar
  {
    double delta = w.GetScreenX() / w.GetScaleX();
    if (m_deltaX < delta)
      delta = m_deltaX;
    m_BarWidth = (int)((delta * w.GetScaleX()) / 3.5);
    if (m_BarWidth == 0)
      m_BarWidth = 1;
    wxCoord iybase = w.y2p(0, m_UseY2Axis);
    Rewind();
    while (DoGetNextXY(&x, &y))
    {
      ix = w.x2p(x);
      iy = w.y2p(y, m_UseY2Axis);
      dc.DrawRectangle(ix - m_BarWidth, iy, 2 * m_BarWidth, iybase - iy);
      UpdateViewBoundary(ix, iy);
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
// mpFXYVector implementation - by Jose Luis Blanco (AGO-2007)
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(mpFXYVector, mpFXY)

// Constructor
mpFXYVector::mpFXYVector(const wxString &name, int flags, bool viewAsBar, bool useY2Axis) :
    mpFXY(name, flags, viewAsBar, useY2Axis)
{
  m_subtype = mpfFXYVector;
  m_index = 0;
  m_minX = -1;
  m_maxX = 1;
  m_minY = -1;
  m_maxY = 1;
  m_xs.clear();
  m_ys.clear();
  SetReserve(1000);
}

bool mpFXYVector::GetNextXY(double *x, double *y)
{
  if (m_index >= m_xs.size())
    return false;
  else
  {
    *x = m_xs[m_index];
    *y = m_ys[m_index];
    m_index += m_step;
    return m_index <= m_xs.size();
  }
}

/**
 * Draw the newly added point at m_index, optionally drawing the continuous-mode line.
 * The bounds are already known.
 */
void mpFXYVector::DrawAddedPoint(double x, double y)
{
  // If we are here, new point is always in bound
  if (!m_visible)
    return;

  // Direct access to the dc
  wxClientDC dc(m_win);
  dc.SetPen(m_pen);
  dc.SetBrush(m_brush);

  CheckLog(&x, &y);
  wxCoord ix = m_win->x2p(x);
  wxCoord iy = m_win->y2p(y, m_UseY2Axis);

  if (!m_ViewAsBar)
  {
    if (m_continuous)
    {
      if (m_index > 0)
      { // Do not draw continuous-mode line unless there is a prior point
        // Last point coordinates
        size_t lastPtIdx = m_index - 1; // we assume that m_step = 1 in this context
        double xlast = m_xs[lastPtIdx];
        double ylast = m_ys[lastPtIdx];
        CheckLog(&xlast, &ylast);
        wxCoord ixlast = m_win->x2p(xlast);
        wxCoord iylast = m_win->y2p(ylast, m_UseY2Axis);
        dc.DrawLine(ixlast, iylast, ix, iy);
      };

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
    wxCoord iybase = m_win->y2p(0, m_UseY2Axis);
    dc.DrawRectangle(ix - m_BarWidth, iy, 2 * m_BarWidth, iybase - iy);
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
  m_deltaX = m_deltaY = 1e+308; // Big number
  Rewind();
}

void mpFXYVector::First_Point(double x, double y)
{
  m_minX = m_maxX = x;
  m_lastX = x;
  m_deltaX = 1e+308; // Big number
  m_minY = m_maxY = y;
  m_lastY = y;
  m_deltaY = 1e+308; // Big number
}

void mpFXYVector::Check_Limit(double val, double *min, double *max, double *last, double *delta)
{
  if (abs(val - *last) < *delta)
    *delta = abs(val - *last);
  *last = val;

  if (val < *min)
    *min = val - *delta;
  else
    if (val > *max)
      *max = val + *delta;
}

void mpFXYVector::SetData(const std::vector<double> &xs, const std::vector<double> &ys)
{
  // Check if the data vectors are of the same size
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
    First_Point(xs[0], ys[0]);

    std::vector<double>::const_iterator it;

    // X scale
    it = xs.begin();
    it++;
    for (; it != xs.end(); it++)
    {
      Check_Limit((*it), &m_minX, &m_maxX, &m_lastX, &m_deltaX);
    }

    // Y scale
    it = ys.begin();
    it++;
    for (; it != ys.end(); it++)
    {
      Check_Limit((*it), &m_minY, &m_maxY, &m_lastY, &m_deltaY);
    }
    Rewind();
  }
  else
  {
    Clear();
  }
}

/** Add data to the internal vector. This method DOES NOT refresh the mpWindow unless updatePlot = true;
 * Do it manually by calling UpdateAll() or just Fit() if we want to adjust plot
 * BEWARE : mpFXYVector layer must be added to the mpWindow layers list for update Plot (logic !)
 */
bool mpFXYVector::AddData(const double x, const double y, bool updatePlot)
{
  bool new_limit = true;

  m_xs.push_back(x);
  m_ys.push_back(y);

  // first point
  if (m_xs.size() == 1)
  {
    First_Point(x, y);
  }
  else
  {
    // X scale
    Check_Limit(x, &m_minX, &m_maxX, &m_lastX, &m_deltaX);

    // Y scale
    Check_Limit(y, &m_minY, &m_maxY, &m_lastY, &m_deltaY);
  }

  if (m_win)
  {
    const mpFloatRect* bbox = m_win->GetBoundingBox();
    new_limit = (m_minX < bbox->Xmin) || (m_maxX > bbox->Xmax) || (m_minY < bbox->Ymin) || (m_maxY > bbox->Ymax);

    if (updatePlot && !new_limit)
    {
      DrawAddedPoint(x, y);
    }
  }
  // In all cases, we increment the index
  m_index++;

  return new_limit;
}

//-----------------------------------------------------------------------------
// mpProfile implementation
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpProfile, mpFunction)

mpProfile::mpProfile(const wxString &name, int flags) :
    mpFunction(mpLAYER_PLOT, name)
{
  m_flags = flags;
}

void mpProfile::DoPlot(wxDC &dc, mpWindow &w)
{
  wxCoord i;

  // Get boundaries
  m_plotBoundaries = w.GetPlotBoundaries(!m_drawOutsideMargins);

  // Plot profile linking subsequent point of the profile, instead of mpFY, which plots simple points.
  wxCoord c0 = w.y2p(GetY(w.p2x(m_plotBoundaries.startPx)));
  if (!m_drawOutsideMargins)
    c0 = (c0 <= m_plotBoundaries.endPy) ? ((c0 >= m_plotBoundaries.startPy) ? c0 : m_plotBoundaries.startPy) : m_plotBoundaries.endPy;
  for (i = m_plotBoundaries.startPx + m_step; i < m_plotBoundaries.endPx; i += m_step)
  {
    wxCoord c1 = w.y2p(GetY(w.p2x(i)));

    if (!m_drawOutsideMargins)
      c1 = (c1 <= m_plotBoundaries.endPy) ? ((c1 >= m_plotBoundaries.startPy) ? c1 : m_plotBoundaries.startPy) : m_plotBoundaries.endPy;

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

//-----------------------------------------------------------------------------
// mpChart implementation
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpChart, mpFunction)

mpChart::mpChart(const wxString &name) :
    mpFunction(mpLAYER_CHART, name)
{
  m_subtype = mpcChartNone;
  m_max_value = 0;
  m_total_value = 0;
}

void mpChart::SetChartValues(const std::vector<double> &data)
{
  values.clear();
  values = data;

  // Found max value
  m_max_value = 0;
  m_total_value = 0;
  for (size_t ii = 0; ii < values.size(); ii++)
  {
    if (m_max_value < values[ii])
      m_max_value = values[ii];
    m_total_value += values[ii];
  }
}

void mpChart::SetChartLabels(const std::vector<std::string> &labelArray)
{
  labels.clear();
  labels = labelArray;
}

void mpChart::AddData(const double &data, const std::string &label)
{
  values.push_back(data);
  labels.push_back(label);
  if (values.size() == 1)
  {
    m_max_value = data;
    m_total_value = data;
  }
  else
  {
    if (m_max_value < data)
      m_max_value = data;
    m_total_value += data;
  }
}

void mpChart::Clear()
{
  values.clear();
  labels.clear();
  m_max_value = 0;
  m_total_value = 0;
}

//-----------------------------------------------------------------------------
// mpBarChart implementation
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpBarChart, mpChart)

mpBarChart::mpBarChart(const wxString &name, double width) :
    mpChart(name)
{
  // already set in base class mpChart's ctor: m_type = mpLAYER_CHART;
  m_subtype = mpcBarChart;
  m_width = width;
  SetBarColour(wxColour(0, 0, 255));
  SetBarLabelPosition(mpBAR_NONE);
}

void mpBarChart::DoPlot(wxDC &dc, mpWindow &w)
{
  int labelX = 0, labelY = 0;
  int labelW = 0, labelH = 0;
  bool drawLabels = (labels.size() == values.size()) && (m_labelPos != mpBAR_NONE);
  wxCoord rect_x_tl = 0, rect_y_tl = 0, rect_width = 0, rect_height = 0;
  wxString currentLabel;

  if (values.size() > 0)
  {
    for (size_t binIndex = 0; binIndex < values.size(); binIndex++)
    {
      rect_x_tl = w.x2p(((double)binIndex) + 1.0 - 0.5 * m_width);
      rect_y_tl = w.y2p(values[binIndex]);
      rect_width = m_width * w.GetScaleX();
      rect_height = values[binIndex] * w.GetScaleY();
      dc.DrawRectangle(rect_x_tl, rect_y_tl, rect_width, rect_height);
      if (drawLabels)
      {
        currentLabel = wxConvUTF8.cMB2WX(labels[binIndex].c_str());
        dc.GetTextExtent(currentLabel, &labelW, &labelH);
        switch (m_labelPos)
        {
          case mpBAR_AXIS_H:
            labelX = rect_x_tl;
            labelY = w.y2p(0.0) + labelH;
            break;
          case mpBAR_AXIS_V:
            labelX = w.x2p(((double)binIndex) + 1.0) - (labelH / 2);
            labelY = w.y2p(-0.05 * m_max_value) + labelW;
            break;
          case mpBAR_INSIDE:
            labelX = w.x2p(((double)binIndex) + 1.0) - (labelH / 2);
            labelY = w.y2p(0.05 * m_max_value);
            break;
          case mpBAR_TOP:
            labelX = rect_x_tl;
            labelY = rect_y_tl - 10 - labelH;
            break;
          default:
            // Default: omit labels
            break;
        }
        dc.DrawRotatedText(currentLabel, labelX, labelY, m_labelAngle); // bar label
      }
    }
  }
}

void mpBarChart::SetBarColour(const wxColour &colour)
{
  m_barColour = colour;
  wxBrush brush(m_barColour, wxBRUSHSTYLE_SOLID);
  SetBrush(brush);
}

void mpBarChart::SetBarLabelPosition(int position)
{
  m_labelPos = position;
  switch (m_labelPos)
  {
    case mpBAR_AXIS_H:
    case mpBAR_TOP:
      m_labelAngle = 0;
      break;
    case mpBAR_AXIS_V:
    case mpBAR_INSIDE:
      m_labelAngle = 90;
      break;
    default:
      m_labelAngle = 0;
      break;
  }
}

double mpBarChart::GetMinX()
{
  if (values.size() > 0)
  {
    return 1.0 - m_width;
  }
  return 0.0;
}

double mpBarChart::GetMaxX()
{
  return (((double)values.size()) + m_width * 0.5);
}

double mpBarChart::GetMinY()
{
  return 0.0;
}

double mpBarChart::GetMaxY()
{
  return m_max_value;
}

//-----------------------------------------------------------------------------
// mpPieChart implementation
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpPieChart, mpChart)

mpPieChart::mpPieChart(const wxString &name, double radius) :
    mpChart(name)
{
  // already set in base class mpChart's ctor: m_type = mpLAYER_CHART;
  m_subtype = mpcBarChart;
  m_radius = radius;
  wxBrush brush(wxColour(125, 200, 255), wxBRUSHSTYLE_SOLID);
  SetBrush(brush);
}

void mpPieChart::DoPlot(wxDC &dc, mpWindow &w)
{
  int labelX = 0, labelY = 0;
  int labelW = 0, labelH = 0;
  bool drawLabels = (labels.size() == values.size());
  wxCoord x1 = 0, y1 = 0, x2 = 0, y2 = 0, xc = 0, yc = 0;
  double angle = 0;
  double anglepie = 0;
  double angletxt = 0;
  double scale = w.GetScaleY() / w.GetScaleX();
  wxCoord offset = (scale == 1 ? 0 : (wxCoord)(w.x2p(0) / 2));
  wxString currentLabel;

  if (values.size() > 0)
  {
    xc = w.x2p(0) * scale + offset;
    x1 = w.x2p(m_radius) * scale + offset;
    y1 = yc = w.y2p(0);

    for (size_t binIndex = 0; binIndex < values.size(); binIndex++)
    {
      angle = values[binIndex] / m_total_value * (2.0 * M_PI);
      anglepie += angle;
      x2 = w.x2p(m_radius * cos(anglepie)) * scale + offset;
      y2 = w.y2p(m_radius * sin(anglepie));
      wxBrush brush(GetColour(binIndex), wxBRUSHSTYLE_SOLID);
      dc.SetBrush(brush);
      dc.DrawArc(x1, y1, x2, y2, xc, yc);
      if (drawLabels)
      {
        currentLabel = wxConvUTF8.cMB2WX(labels[binIndex].c_str());
        dc.GetTextExtent(currentLabel, &labelW, &labelH);
        labelX = w.x2p(m_radius * cos(angletxt + angle / 2.0)) * scale + offset + 10;
        labelY = w.y2p(m_radius * sin(angletxt + angle / 2.0));
        dc.DrawText(currentLabel, labelX, labelY);
        angletxt += angle;
      }
      x1 = x2;
      y1 = y2;
    }
  }
}

void mpPieChart::SetPieColours(const std::vector<wxColour> &colourArray)
{
  colours.clear();
  colours = colourArray;
}

const wxColour& mpPieChart::GetColour(unsigned int id)
{
  // Create colours if necessary
  while (id >= colours.size())
  {
    colours.push_back(wxIndexColour(colours.size()));
  }
  return colours[id];
}

//-----------------------------------------------------------------------------
// mpScale implementations
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(mpScale, mpLayer)

mpScale::mpScale(const wxString &name, int flags, bool grids) : mpLayer(mpLAYER_AXIS)
{
  m_subtype = mpsScaleNone;
  SetName(name);
  SetFont((wxFont const&)*wxSMALL_FONT);
  SetPen((wxPen const&)*wxGREY_PEN);
  m_gridpen = wxPen(*wxLIGHT_GREY, 1, wxPENSTYLE_DOT);
  m_flags = flags;
  m_ticks = true;
  m_grids = grids;
  m_auto = true;
  m_min = -1;
  m_max = 1;
  m_labelFormat = _T("");
  m_ZIndex = mpZIndex_AXIS;
}

double mpScale::GetStep(double scale)
{
  const double DIGIT = 128.0;
  const double DIGIT_LOG = 128.0;

  double dig;
  if (IsLogAxis())
  {
    dig = floor(log10(DIGIT_LOG / scale));
    if (scale > DIGIT_LOG)
      dig += 1;
  }
  else
    dig = floor(log10(DIGIT / scale));
  return pow(10, dig);
}

wxString mpScale::FormatLogValue(double n)
{
  // Special format for log axis : 10 ^ exponent
  wxString s = _T("");

  if (n - floor(n) == 0)
  {
    int exp = (int)n;
    if (exp == 0)
      s = _T("1");
    else
      if (exp == 1)
        s = _T("10");
      else
        s.Printf(_T("10^%d"), (int)exp);
  }
  return s;
}

//-----------------------------------------------------------------------------
// mpScaleX
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpScaleX, mpScale)

/**
 * Get the origin of axis and initialize the plot boundaries
 */
int mpScaleX::GetOrigin(mpWindow &w)
{
  int origin = 0;
  // Get boundaries
  m_plotBoundaries = w.GetPlotBoundaries(!m_drawOutsideMargins);

  switch (m_flags)
  {
    // Scale X : horizontal axis
    case mpALIGN_BORDER_TOP:
      origin = 1;
      break;
    case mpALIGN_TOP:
    {
      if (m_drawOutsideMargins)
        origin = X_BORDER_SEPARATION;
      else
        origin = w.GetMarginTop() - EXTRA_MARGIN;
      break;
    }
    case mpALIGN_CENTERX:
      origin = w.y2p(0);
      // Draw nothing if we are outside margins
      if (!m_drawOutsideMargins && ((origin > (w.GetScreenY() - w.GetMarginBottom())) || (origin < w.GetMarginTop())))
        origin = -1;
      break;
    case mpALIGN_BOTTOM:
    {
      if (m_drawOutsideMargins)
        origin = w.GetScreenY() - X_BORDER_SEPARATION;
      else
        origin = w.GetScreenY() - w.GetMarginBottom() + EXTRA_MARGIN - 1;
      break;
    }
    case mpALIGN_BORDER_BOTTOM:
      origin = w.GetScreenY() - 1;
      break;

    default:
      ;  // Nothing
  }
  return origin;
}

void mpScaleX::DrawScaleName(wxDC &dc, mpWindow &w, int origin, int labelSize)
{
  wxCoord tx, ty;

  // Draw axis name
  dc.GetTextExtent(m_name, &tx, &ty);
  switch (m_flags)
  {
    // Scale X : horizontal axis
    case mpALIGN_BORDER_BOTTOM:
      dc.DrawText(m_name, m_plotBoundaries.endPx - tx - 4, origin - ty - labelSize - 8);
      break;
    case mpALIGN_BOTTOM:
    {
      if ((!m_drawOutsideMargins) && (w.GetMarginBottom() > (ty + labelSize + 8)))
      {
//        dc.DrawText(m_name, (m_plotBoundaries.endPx + m_plotBoundaries.startPx - tx) / 2, orgy + labelH + 6);
        dc.DrawText(m_name, m_plotBoundaries.endPx - tx - 4, origin + labelSize + 6);
      }
      else
      {
        dc.DrawText(m_name, m_plotBoundaries.endPx - tx - 4, origin - ty - 4);
      }
      break;
    }
    case mpALIGN_CENTERX:
      dc.DrawText(m_name, m_plotBoundaries.endPx - tx - 4, origin - ty - 4);
      break;
    case mpALIGN_TOP:
    {
      if ((!m_drawOutsideMargins) && (w.GetMarginTop() > (ty + labelSize + 8)))
      {
//        dc.DrawText(m_name, (m_plotBoundaries.endPx + m_plotBoundaries.startPx - tx) / 2, orgy - ty - labelH - 6);
        dc.DrawText(m_name, m_plotBoundaries.endPx - tx - 4, origin - ty - labelSize - 8);
      }
      else
      {
        dc.DrawText(m_name, m_plotBoundaries.endPx - tx - 4, origin + 4);
      }
      break;
    }
    case mpALIGN_BORDER_TOP:
      dc.DrawText(m_name, m_plotBoundaries.endPx - tx - 4, origin + labelSize + 6);
      break;

    default:
      ;
  }
}

wxString mpScaleX::FormatValue(const wxString &fmt, double n)
{
  wxString s = _T("");
  struct tm timestruct;

  switch (m_labelType)
  {
    case mpX_NORMAL:
    case mpX_USER:
      if (IsLogAxis())
        s = FormatLogValue(n);
      else
        s.Printf(fmt, n);
      break;
    case mpX_DATETIME:
    {
      if (DoubleToTimeStruct(n, m_timeConv, &timestruct))
        s.Printf(fmt, timestruct.tm_year + 1900, timestruct.tm_mon + 1, timestruct.tm_mday, timestruct.tm_hour, timestruct.tm_min,
            timestruct.tm_sec);

      break;
    }
    case mpX_DATE:
    {
      if (DoubleToTimeStruct(n, m_timeConv, &timestruct))
        s.Printf(fmt, timestruct.tm_year + 1900, timestruct.tm_mon + 1, timestruct.tm_mday);
      break;
    }
    case mpX_TIME:
    case mpX_HOURS:
    {
      double sign = (n < 0) ? -1.0 : 1.0;
      double modulus = fabs(n);
      double hh = floor(modulus / 3600);
      double mm = floor((modulus - hh * 3600) / 60);
      double ss = modulus - hh * 3600 - mm * 60;
#if defined(MATHPLOT_DO_LOGGING) && defined(MATHPLOT_LOG_SCALE)
      wxLogMessage(_T("%02.0f Hours, %02.0f minutes, %02.0f seconds"), sign * hh, mm, ss);
#endif // MATHPLOT_DO_LOGGING
      if (fmt.Len() == 20) // Format with hours has 11 chars
        s.Printf(fmt, sign * hh, mm, floor(ss));
      else
        s.Printf(fmt, sign * mm, ss);
      break;
    }
    default:
      ;
  }
  return s;
}

void mpScaleX::DoPlot(wxDC &dc, mpWindow &w)
{
  int orgy = GetOrigin(w);

  // Draw nothing if we are outside margins
  if (orgy == -1)
    return;

  // Draw X axis
  dc.DrawLine(m_plotBoundaries.startPx, orgy, m_plotBoundaries.endPx, orgy);

  const double scaleX = w.GetScaleX();
  const double step = GetStep(scaleX);
  const double end = w.GetPosX() + (double)w.GetScreenX() / scaleX;

  // Get string format
  wxString fmt = _T("");
  switch (m_labelType)
  {
    case mpX_NORMAL:
    case mpX_USER:
    {
      if ((m_labelType == mpX_USER) && (!m_labelFormat.IsEmpty()))
      {
        fmt = m_labelFormat;
      }
      else
      {
        int tmp = (int)log10(step);
        if (tmp > 1)
        {
          fmt = _T("%.g");
        }
        else
        {
          tmp = 8 - tmp;
          fmt.Printf(_T("%%.%dg"), tmp >= -1 ? 2 : -tmp);
        }
      }
      break;
    }
    case mpX_DATETIME:
      fmt = (_T("%04d-%02d-%02dT%02d:%02d:%02d"));
      break;
    case mpX_DATE:
      fmt = (_T("%04d-%02d-%02d"));
      break;
    case mpX_TIME:
    {
      if (end / 60 < 2)
        fmt = (_T("%02.0f:%02.3f"));
      else
        fmt = (_T("%02.0f:%02.0f:%02.0f"));
      break;
    }
    case mpX_HOURS:
      fmt = (_T("%02.0f:%02.0f:%02.0f"));
      break;
    default:
      ;
  }

  double n0 = floor(w.GetPosX() / step) * step;
  double n = 0;
#if defined(MATHPLOT_DO_LOGGING) && defined(MATHPLOT_LOG_SCALE)
  wxLogMessage(_T("mpScaleX::Plot: step: %f, end: %f, n: %f"), step, end, n0);
#endif

  int labelH = 0; // Control labels height to decide where to put axis name (below labels or on top of axis)
  int maxExtent = 0;
  wxCoord tx, ty;
  wxString s;

  // Draw grid, ticks and compute max label length
  for (n = n0; n < end; n += step)
  {
    const int p = (int)((n - w.GetPosX()) * scaleX);
#if defined(MATHPLOT_DO_LOGGING) && defined(MATHPLOT_LOG_SCALE)
    wxLogMessage(_T("mpScaleX::Plot: n: %f -> p = %d"), n, p);
#endif
    if ((p >= m_plotBoundaries.startPx) && (p <= m_plotBoundaries.endPx))
    {
      // draw grid
      if (m_grids)
      {
        dc.SetPen(m_gridpen);
        dc.DrawLine(p, m_plotBoundaries.startPy + 1, p, m_plotBoundaries.endPy - 1);
      }

      // draw axis ticks
      if (m_ticks)
      {
        dc.SetPen(m_pen);
        if (m_flags == mpALIGN_BORDER_BOTTOM)
          dc.DrawLine(p, orgy, p, orgy - 4);
        else
          dc.DrawLine(p, orgy, p, orgy + 4);
      }

      // Write ticks labels in s string : compute size
      s = FormatValue(fmt, n);

      dc.GetTextExtent(s, &tx, &ty);
      labelH = (labelH <= ty) ? ty : labelH;
      maxExtent = (tx > maxExtent) ? tx : maxExtent; // Keep in mind max label width
    }
  }

  // Actually draw labels, taking care of not overlapping them, and distributing them regularly
  double labelStep = ceil((maxExtent + MIN_X_AXIS_LABEL_SEPARATION) / (scaleX * step)) * step;

  for (n = n0; n < end; n += labelStep)
  {
    // To have a real zero
    if (fabs(n) < 1e-10)
      n = 0;

    const int p = (int)((n - w.GetPosX()) * scaleX);
#if defined(MATHPLOT_DO_LOGGING) && defined(MATHPLOT_LOG_SCALE)
    wxLogMessage(_T("mpScaleX::Plot: n_label = %f -> p_label = %d"), n, p);
#endif
    if ((p >= m_plotBoundaries.startPx) && (p <= m_plotBoundaries.endPx))
    {
      // Write ticks labels in s string
      s = FormatValue(fmt, n);

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
  DrawScaleName(dc, w, orgy, labelH);
}

bool mpScaleX::IsLogAxis()
{
  if (m_win)
    return m_win->IsLogXaxis();
  else
    return false;
}

void mpScaleX::SetLogAxis(bool log)
{
  if (m_win)
    m_win->SetLogXaxis(log);
}

//-----------------------------------------------------------------------------
// mpScaleY
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpScaleY, mpScale)

/**
 * Get the origin of axis and initialize the plot boundaries
 */
int mpScaleY::GetOrigin(mpWindow &w)
{
  int origin = 0;
  // Get boundaries
  m_plotBoundaries = w.GetPlotBoundaries(!m_drawOutsideMargins);

  switch (m_flags)
  {
    // Scale Y : vertical axis
    case mpALIGN_BORDER_LEFT:
      origin = 1;
      break;
    case mpALIGN_LEFT:
    {
      if (m_drawOutsideMargins)
        origin = Y_BORDER_SEPARATION;
      else
        origin = w.GetMarginLeft() - EXTRA_MARGIN - 1;
      break;
    }
    case mpALIGN_CENTERY:
      origin = w.x2p(0);
      if (!m_drawOutsideMargins && ((origin > (w.GetScreenX() - w.GetMarginRight())) || (origin + 1 < w.GetMarginLeft())))
        origin = -1;
      break;
    case mpALIGN_RIGHT:
    {
      if (m_drawOutsideMargins)
        origin = w.GetScreenX() - Y_BORDER_SEPARATION;
      else
        origin = w.GetScreenX() - w.GetMarginRight() + EXTRA_MARGIN - 2;
      break;
    }
    case mpALIGN_BORDER_RIGHT:
      origin = w.GetScreenX() - 1;
      break;

    default:
      ;  // Nothing
  }
  return origin;
}

void mpScaleY::DrawScaleName(wxDC &dc, mpWindow &w, int origin, int labelSize)
{
  wxCoord tx, ty;

  // Draw axis name
  dc.GetTextExtent(m_name, &tx, &ty);
  switch (m_flags)
  {
    // Scale Y : vertical axis
    case mpALIGN_BORDER_LEFT:
      dc.DrawText(m_name, labelSize + 8, m_plotBoundaries.startPy + 4);
      break;
    case mpALIGN_LEFT:
    {
      if ((!m_drawOutsideMargins) && (w.GetMarginLeft() > (ty + labelSize + 8)))
      {
        dc.DrawRotatedText(m_name, origin - labelSize - ty - 6, (m_plotBoundaries.endPy + m_plotBoundaries.startPy + tx) / 2, 90);
      }
      else
      {
        dc.DrawText(m_name, origin + 4, m_plotBoundaries.startPy + 4);
      }
      break;
    }
    case mpALIGN_CENTERY:
      dc.DrawText(m_name, origin + 4, m_plotBoundaries.startPy + 4);
      break;
    case mpALIGN_RIGHT:
    {
      if ((!m_drawOutsideMargins) && (w.GetMarginRight() > (ty + labelSize + 8)))
      {
        dc.DrawRotatedText(m_name, origin + labelSize + 6, (m_plotBoundaries.endPy + m_plotBoundaries.startPy + tx) / 2, 90);
      }
      else
      {
        dc.DrawText(m_name, origin - tx - 4, m_plotBoundaries.startPy + 4);
      }
      break;
    }
    case mpALIGN_BORDER_RIGHT:
      dc.DrawText(m_name, origin - tx - labelSize - 6, m_plotBoundaries.startPy + 4);
      break;

    default:
      ;
  }
}

void mpScaleY::DoPlot(wxDC &dc, mpWindow &w)
{
  int orgx = GetOrigin(w);

  // Draw nothing if we are outside margins
  if (orgx == -1)
    return;

  // Draw Y axis
  dc.DrawLine(orgx + 1, m_plotBoundaries.startPy, orgx + 1, m_plotBoundaries.endPy);

  const double scaleY = w.GetScaleY(m_isY2Axis);
  const double step = GetStep(scaleY);
  const double posY = w.GetPosY(m_isY2Axis);
  const double end = posY + (double)w.GetScreenY() / scaleY;

  wxString fmt;
  if (m_labelFormat.IsEmpty())
  {
    double maxScaleAbs = fabs(w.GetDesiredYmax());
    double minScaleAbs = fabs(w.GetDesiredYmin());
    double endscale = (maxScaleAbs > minScaleAbs) ? maxScaleAbs : minScaleAbs;
    if ((endscale < 1e4) && (endscale > 1e-3))
      fmt = _T("%.2f");
    else
      fmt = _T("%.2e");
  }
  else
  {
    fmt = m_labelFormat;
  }

  double n = floor((posY - (double)(w.GetScreenY()) / scaleY) / step) * step;

  wxCoord tmp = 65536;
  wxCoord labelW = 0;
  // Before staring cycle, calculate label height
  wxCoord labelHeigth = 0;
  wxString s;
  wxCoord tx = 0, ty = 0;
  s.Printf(fmt, n);
  dc.GetTextExtent(s, &tx, &labelHeigth);
  labelHeigth /= 2;

  // Draw grid, ticks and label
  for (; n < end; n += step)
  {
    // To have a real zero
    if (fabs(n) < 1e-10)
      n = 0;
    const int p = (int)((posY - n) * scaleY);
    if ((p > m_plotBoundaries.startPy + labelHeigth) && (p < m_plotBoundaries.endPy - labelHeigth))
    {
      // Draw axis grids
      if (m_grids && (n != 0))
      {
        dc.SetPen(m_gridpen);
        dc.DrawLine(m_plotBoundaries.startPx + 1, p, m_plotBoundaries.endPx - 1, p);
      }

      // Draw axis ticks
      if (m_ticks)
      {
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

      if (IsLogAxis())
      {
        s = FormatLogValue(n);
        if (s.IsEmpty())
          continue;
      }
      else
        s.Printf(fmt, n);

      // Print ticks labels
      dc.GetTextExtent(s, &tx, &ty);
#if defined(MATHPLOT_DO_LOGGING) && defined(MATHPLOT_LOG_SCALE)
      if (ty != labelHeigth)
        wxLogMessage(_T("mpScaleY::Plot: ty(%d) and labelHeigth(%d) differ!"), ty, labelHeigth);
#endif
      labelW = (labelW <= tx) ? tx : labelW;
      if ((tmp - p + labelHeigth) > MIN_Y_AXIS_LABEL_SEPARATION)
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
  DrawScaleName(dc, w, orgx, labelW);
}

bool mpScaleY::IsLogAxis()
{
  if (m_win)
    return m_win->IsLogYaxis();
  else
    return false;
}

void mpScaleY::SetLogAxis(bool log)
{
  if (m_win)
    m_win->SetLogYaxis(log);
}

void mpScaleY::SetY2Axis(bool y2Axis)
{
  if (m_isY2Axis != y2Axis)
  {
    m_isY2Axis = y2Axis;
    if (m_win)
      m_win->Update_CountY2Axis(y2Axis);
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
    wxWindow(parent, id, pos, size, flag, _T("Mathplot"))
{
  // Fill i18n string
  FillI18NString();

  // Search the top parent
  wxWindow* pWin = parent;
  while (true)
  {
    if (pWin->GetParent())
      pWin = pWin->GetParent();
    else
      break;
  }
  m_parent = (wxTopLevelWindow*)pWin;
  m_fullscreen = false;

  SetBackgroundStyle(wxBG_STYLE_PAINT);

  InitParameters();

  // For unix screenshoot
#ifdef _WIN32
#else
  wxImage::AddHandler(new wxPNGHandler);
#endif

  int nbItem = mpID_HELP_MOUSE - mpID_FIT + 1;
  if (m_parent) // Fullscreen available (last item)
    nbItem++;

#ifdef USE_ICON
  wxImage::AddHandler(new wxPNGHandler);

  wxBitmap icon[] = {wxBITMAP_PNG_FROM_DATA(Fit_24x24), wxBITMAP_PNG_FROM_DATA(Zoom_in_24x24), wxBITMAP_PNG_FROM_DATA(Zoom_out_24x24),
      wxBITMAP_PNG_FROM_DATA(Center_24x24), wxNullBitmap, wxBITMAP_PNG_FROM_DATA(Grid_24x24), wxBITMAP_PNG_FROM_DATA(Coordinates_24x24),
      wxBITMAP_PNG_FROM_DATA(Screenshot_24x24), wxBITMAP_PNG_FROM_DATA(Config_24x24), wxBITMAP_PNG_FROM_DATA(Load_24x24),
      wxBITMAP_PNG_FROM_DATA(Mouse_24x24), wxBITMAP_PNG_FROM_DATA(Fullscreen_24x24)};

  // Because of GTK problem, we must set bitmap BEFORE append the menu item
  wxMenuItem* mymenu;
  for (int i = 0; i < nbItem; i++)
  {
    if (mpID_FIT + i != mpID_LOCKASPECT)
    {
      mymenu = new wxMenuItem(&m_popmenu, mpID_FIT + i, Popup_string[i][0], Popup_string[i][1]);
      mymenu->SetBitmap(icon[i]);
      m_popmenu.Append(mymenu);
    }
    else
      m_popmenu.AppendCheckItem(mpID_LOCKASPECT, Popup_string[i][0], Popup_string[i][1]);
  }
#else
  for (int i = 0; i < nbItem; i++)
  {
    if (mpID_FIT + i != mpID_LOCKASPECT)
      m_popmenu.Append(mpID_FIT + i, Popup_string[i][0], Popup_string[i][1]);
    else
      m_popmenu.AppendCheckItem(mpID_LOCKASPECT, Popup_string[i][0], Popup_string[i][1]);
  }
#endif

  m_layers.clear();
  SetBackgroundColour(*wxWHITE);
  m_bgColour = *wxWHITE;
  m_fgColour = *wxBLACK;
  m_drawBox = true;

  SetSizeHints(128, 128);

  UpdateAll();
}

mpWindow::~mpWindow()
{
  // Free all the layers:
  DelAllLayers(true, false);

  DeleteAndNull(m_buff_bmp);
  DeleteAndNull(m_zoom_bmp);
  DeleteAndNull(m_Screenshot_bmp);
  DeleteAndNull(m_configWindow);
}

void mpWindow::InitParameters()
{
  m_scaleX = m_scaleY = m_scaleY2 = 1.0;
  m_posX = m_posY = m_posY2 = 0.0;
  m_bound = mpFloatRect();
  m_desired = mpFloatRect();
  m_scrX = m_scrY = 64;
  m_last_lx = m_last_ly = 0;
  m_XAxis = NULL;
  m_YAxis = NULL;
  m_Y2Axis = NULL;
  m_repainting = false;
  m_buff_bmp = NULL;
  m_Screenshot_bmp = NULL;
  m_enableDoubleBuffer = true;
  m_enableMouseNavigation = true;
  m_mouseMovedAfterRightClick = false;
  m_movingInfoLayer = NULL;
  m_InfoCoords = NULL;
  m_InfoLegend = NULL;
  m_InInfoLegend = false;
  m_zoom_bmp = NULL;
  m_magnetize = false;
  m_enableScrollBars = false;

  // Set all margins to 50
  SetMargins(50, 50, 50, 50);

  m_lockaspect = false;
}

bool mpWindow::CheckUserMouseAction(wxMouseEvent &event)
{
  if (m_OnUserMouseAction != NULL)
  {
    bool cancel = true;
    m_OnUserMouseAction(this, event, cancel);
    if (cancel)
    {
      event.Skip();
      return true;
    }
  }
  return false;
}

void mpWindow::OnMouseLeftDown(wxMouseEvent &event)
{
  if (CheckUserMouseAction(event))
    return;

  m_mouseLClick = event.GetPosition();
#ifdef MATHPLOT_DO_LOGGING
  wxLogMessage(_T("mpWindow::OnMouseLeftDown() X = %d , Y = %d"), event.GetX(), event.GetY());
#endif
  m_movingInfoLayer = IsInsideInfoLayer(m_mouseLClick);
#ifdef MATHPLOT_DO_LOGGING
  if (m_movingInfoLayer != NULL)
  {
    wxLogMessage(_T("mpWindow::OnMouseLeftDown() started moving layer %p"), m_movingInfoLayer);
  }
#endif
  if (m_InInfoLegend)
  {
    int select = m_InfoLegend->GetPointed(*this, m_mouseLClick);
    if (m_configWindow == NULL)
      m_configWindow = new MathPlotConfigDialog(this);

    m_configWindow->Initialize(3);
    m_configWindow->SelectChoiceSerie(select);
    m_configWindow->Show();
  }

  event.Skip();
}

// Mouse handler, for detecting when the user drag with the right button or just "clicks" for the menu
// JLB
void mpWindow::OnMouseRightDown(wxMouseEvent &event)
{
  if (CheckUserMouseAction(event))
    return;

  m_mouseMovedAfterRightClick = false;
  m_mouseRClick = wxPoint(event.GetX(), event.GetY());
  if (m_magnetize)
    m_magnet.SetRightClick();

  if (m_enableMouseNavigation)
    SetCursor(*wxCROSS_CURSOR);
}

// If the user "drags" with the right button pressed, do "pan"
// JLB
void mpWindow::OnMouseMove(wxMouseEvent &event)
{
  if (CheckUserMouseAction(event))
    return;

  if (!m_enableMouseNavigation)
  {
    event.Skip();
    return;
  }

  // The current mouse position
  wxPoint eventPoint = wxPoint(event.GetX(), event.GetY());

  // pan
  if (event.m_rightDown)
  {
    m_mouseMovedAfterRightClick = true; // Hides the popup menu after releasing the button!

    // The change:
    wxPoint Axy = m_mouseRClick - eventPoint;

    // For the next event, use relative to this coordinates.
    m_mouseRClick = eventPoint;

    double Ax_units = Axy.x / m_scaleX;
    double Ay_units = -Axy.y / m_scaleY;
    double Ay2_units = -Axy.y / m_scaleY2;

    m_posX += Ax_units;
    m_posY += Ay_units;
    m_posY2 += Ay2_units;
    m_desired.Xmax += Ax_units;
    m_desired.Xmin += Ax_units;
    m_desired.Ymax += Ay_units;
    m_desired.Ymin += Ay_units;
    m_desired.Y2max += Ay2_units;
    m_desired.Y2min += Ay2_units;

    UpdateAll();
    CheckAndReportDesiredBoundsChanges();

#ifdef MATHPLOT_DO_LOGGING
    wxLogMessage(_T("[mpWindow::OnMouseMove] Ax:%i Ay:%i m_posX:%f m_posY:%f"), Axy.x, Axy.y, m_posX, m_posY);
#endif
  }
  else
  {
    wxClientDC dc(this);

    // First need to clean the plot.
    if (m_magnetize && (!m_repainting))
      m_magnet.ClearPlot(dc);

    // zoom select rectangle
    if (event.m_leftDown)
    {
      wxPoint moveVector = eventPoint - m_mouseLClick;
      if (m_movingInfoLayer == NULL)
      {
        // First : restore stored bitmap
        if (m_zoom_bmp)
        {
          wxMemoryDC m_coord_dc(&dc);
          m_coord_dc.SelectObject(*m_zoom_bmp);
          dc.Blit(m_zoom_oldDim.x, m_zoom_oldDim.y, m_zoom_oldDim.width, m_zoom_oldDim.height, &m_coord_dc, 0, 0);
          m_coord_dc.SelectObject(wxNullBitmap);
          DeleteAndNull(m_zoom_bmp);
        }

        // Second : store new bitmap
        m_zoom_dim = wxRect(m_mouseLClick, wxSize(moveVector.x, moveVector.y));
        if ((m_zoom_dim.width != 0) && (m_zoom_dim.height != 0))
        {
          if (m_zoom_dim.width < 0)
          {
            m_zoom_dim.x += m_zoom_dim.width;
            m_zoom_dim.width = abs(m_zoom_dim.width);
          }
          if (m_zoom_dim.height < 0)
          {
            m_zoom_dim.y += m_zoom_dim.height;
            m_zoom_dim.height = abs(m_zoom_dim.height);
          }

          m_zoom_bmp = new wxBitmap(m_zoom_dim.width, m_zoom_dim.height, dc);
          wxMemoryDC m_coord_dc(&dc);
          m_coord_dc.SelectObject(*m_zoom_bmp);
          m_coord_dc.Blit(0, 0, m_zoom_dim.width, m_zoom_dim.height, &dc, m_zoom_dim.x, m_zoom_dim.y);
          m_coord_dc.SelectObject(wxNullBitmap);
          m_zoom_oldDim = m_zoom_dim;

          // Draw the rectangle that focus the selected region
          wxPen pen(*wxBLACK, 1, wxPENSTYLE_DOT);  // wxDOT
          dc.SetPen(pen);
          dc.SetBrush(*wxTRANSPARENT_BRUSH);
          dc.DrawRectangle(m_zoom_dim);
        }

        if (m_magnetize && (!m_repainting))
          m_magnet.Plot(dc, eventPoint);
      }
      else
        m_movingInfoLayer->Move(moveVector);
    }
    else
    {
      // Mouse move coordinate
      if (m_InfoCoords && m_InfoCoords->IsVisible())
      {
        if ((m_InfoCoords->GetDrawOutsideMargins()) || (m_PlotArea.Contains(eventPoint)))
        {
          m_InfoCoords->UpdateInfo(*this, event);
          m_InfoCoords->Plot(dc, *this);
        }
        else
          m_InfoCoords->ErasePlot(dc, *this);
      }

      // Mouse move on legend
      if (m_InfoLegend && m_InfoLegend->IsVisible())
      {
        m_InInfoLegend = m_InfoLegend->Inside(eventPoint);
        if (m_InInfoLegend)
          SetCursor(wxCursor(wxCURSOR_HAND));
        else
          SetCursor(*wxSTANDARD_CURSOR);
      }

      if (m_magnetize && (!m_repainting) && (event.GetEventType() == wxEVT_MOTION))
        m_magnet.Plot(dc, eventPoint);
    }
  }
  event.Skip();
}

void mpWindow::OnMouseLeftRelease(wxMouseEvent &event)
{
  if (CheckUserMouseAction(event))
    return;

  if (m_movingInfoLayer != NULL)
  {
    m_movingInfoLayer->UpdateReference();
    m_movingInfoLayer = NULL;
  }
  else
  {
    DeleteAndNull(m_zoom_bmp);
    wxPoint release(event.GetX(), event.GetY());
    // Zoom if we have a real rectangle
    if ((release.x != m_mouseLClick.x) && (release.y != m_mouseLClick.y))
    {
      ZoomRect(m_mouseLClick, release);
    }
  }
  event.Skip();
}

// Process mouse wheel events
// JLB
void mpWindow::OnMouseWheel(wxMouseEvent &event)
{
  if (CheckUserMouseAction(event))
    return;

  if (!m_enableMouseNavigation)
  {
    event.Skip();
    return;
  }

  // Zoom in and out
  if (!event.m_controlDown && !event.m_shiftDown)
  {
    // CTRL key hold: Zoom in/out:
    Zoom((event.GetWheelRotation() > 0), wxPoint(event.GetX(), event.GetY()));
  }
  else
  {
    // Scroll vertically or horizontally (this is SHIFT held down).
    int change = -event.GetWheelRotation(); // Opposite direction (More intuitive)!

    if (event.m_shiftDown)
    {
      double changeUnitsX = change / m_scaleX;
      m_posX += changeUnitsX;
      m_desired.Xmax += changeUnitsX;
      m_desired.Xmin += changeUnitsX;
    }
    else
      if (event.m_controlDown)
      {
        double changeUnitsY = change / m_scaleY;
        double changeUnitsY2 = change / m_scaleY2;
        m_posY -= changeUnitsY;
        m_posY2 -= changeUnitsY2;
        m_desired.Ymax -= changeUnitsY;
        m_desired.Ymax -= changeUnitsY;
        m_desired.Y2max -= changeUnitsY2;
        m_desired.Y2max -= changeUnitsY2;
      }

    UpdateAll();
    CheckAndReportDesiredBoundsChanges();
  }
}

/**
 * Mouse leave the plot area
 */
void mpWindow::OnMouseLeave(wxMouseEvent &event)
{
  if (CheckUserMouseAction(event))
    return;

  wxClientDC dc(this);
  if (m_InfoCoords && m_InfoCoords->IsVisible())
  {
    m_InfoCoords->ErasePlot(dc, *this);
  }
  if (m_zoom_bmp)
  {
    wxMemoryDC m_coord_dc(&dc);
    m_coord_dc.SelectObject(*m_zoom_bmp);
    dc.Blit(m_zoom_oldDim.x, m_zoom_oldDim.y, m_zoom_oldDim.width, m_zoom_oldDim.height, &m_coord_dc, 0, 0);
    m_coord_dc.SelectObject(wxNullBitmap);
    DeleteAndNull(m_zoom_bmp);
  }
  if (m_magnetize)
  {
    m_magnet.ClearPlot(dc);
  }
}

/**
 * General fit
 * Use global layer bounding box
 */
void mpWindow::Fit()
{
  UpdateBBox();
  Fit(m_bound);
}

void mpWindow::Fit(const mpFloatRect &rect, wxCoord *printSizeX, wxCoord *printSizeY)
{ // JL
  bool weArePrinting = printSizeX!=NULL && printSizeY!=NULL;
  if (m_magnetize)
  {
    // Avoid paint cross if mouse move
    m_repainting = true;
  }

  // Save desired borders:
  m_desired = rect;

  if (weArePrinting)
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

  double Ax, Ay, Ay2;

  Ax = rect.Xmax - rect.Xmin;
  Ay = rect.Ymax - rect.Ymin;
  Ay2 = rect.Y2max - rect.Y2min;

  m_scaleX = ISNOTNULL(Ax) ? m_plotWidth / Ax : 1;
  m_scaleY = ISNOTNULL(Ay) ? m_plotHeight / Ay : 1;
  m_scaleY2 = ISNOTNULL(Ay2) ? m_plotHeight / Ay2 : 1;

  if (m_lockaspect)
  {
#ifdef MATHPLOT_DO_LOGGING
    wxLogMessage(_T("mpWindow::Fit()(lock) m_scaleX=%f,m_scaleY=%f"), m_scaleX, m_scaleY);
#endif
    // Keep the lowest "scale" to fit the whole range required by that axis (to actually "fit"!):
    double s = m_scaleX < m_scaleY ? m_scaleX : m_scaleY;
    m_scaleX = s;
    m_scaleY = s;
    m_scaleY2 = s;
  }

  // Adjusts corner coordinates: This should be simply:
  //   m_posX = m_minX;
  //   m_posY = m_maxY;
  // But account for centering if we have lock aspect:
  m_posX = (rect.Xmin + rect.Xmax) / 2 - (m_plotWidth / 2 + m_margin.left) / m_scaleX;
  m_posY = (rect.Ymin + rect.Ymax) / 2 + (m_plotHeight / 2 + m_margin.top) / m_scaleY;
  m_posY2 = (rect.Y2min + rect.Y2max) / 2 + (m_plotHeight / 2 + m_margin.top) / m_scaleY2;

#ifdef MATHPLOT_DO_LOGGING
  wxLogMessage(_T("mpWindow::Fit() m_desired.Xmin=%f m_desired.Xmax=%f  m_desired.Ymin=%f m_desired.Ymax=%f"),
      m_desired.Xmin, m_desired.Xmax, m_desired.Ymin, m_desired.Ymax);
  wxLogMessage(_T("mpWindow::Fit() m_scaleX = %f , m_scrX = %d,m_scrY=%d, Ax=%f, Ay=%f, m_posX=%f, m_posY=%f"),
      m_scaleX, m_scrX, m_scrY, Ax, Ay, m_posX, m_posY);
#endif

  // It is VERY IMPORTANT to DO NOT call Refresh if we are drawing to the printer!!
  // Otherwise, the DC dimensions will be those of the window instead of the printer device
  if (!weArePrinting) {
    // We are NOT drawing to a printer...
    UpdateAll();
    CheckAndReportDesiredBoundsChanges();
  }
}

void mpWindow::CheckAndReportDesiredBoundsChanges() {
  if (m_desired.IsNotSet()) return; // nothing to do until useful info in m_desired
  if (!m_initialDesiredBoundsRecorded) {
    m_initialDesiredBoundsRecorded = true;
  } else if ( !(m_desired == m_lastDesiredReportedBounds) ) {
    DesiredBoundsHaveChanged();
  }
  m_lastDesiredReportedBounds = m_desired;
}

// Patch ngpaton
void mpWindow::DoZoomInXCalc(const int staticXpixel)
{
  // Preserve the position of the clicked point:
  double staticX = p2x(staticXpixel);
  // Zoom in:
  m_scaleX *= m_zoomIncrementalFactor;
  // Adjust the new m_posx
  m_posX = staticX - (staticXpixel / m_scaleX);
  // Adjust desired
  m_desired.Xmin = m_posX;
  m_desired.Xmax = m_posX + m_plotWidth / m_scaleX;
  CheckAndReportDesiredBoundsChanges();
#ifdef MATHPLOT_DO_LOGGING
  wxLogMessage(_T("mpWindow::DoZoomInXCalc() prior X coord: (%f), new X coord: (%f) SHOULD BE EQUAL!!"), staticX, p2x(staticXpixel));
#endif
}

void mpWindow::DoZoomInYCalc(const int staticYpixel)
{
  // Preserve the position of the clicked point:
  double staticY = p2y(staticYpixel);
  double staticY2 = p2y(staticYpixel, true);
  // Zoom in:
  m_scaleY *= m_zoomIncrementalFactor;
  m_scaleY2 *= m_zoomIncrementalFactor;
  // Adjust the new m_posy:
  m_posY = staticY + (staticYpixel / m_scaleY);
  m_posY2 = staticY2 + (staticYpixel / m_scaleY2);
  // Adjust desired
  m_desired.Ymax = m_posY;
  m_desired.Ymin = m_posY - m_plotHeight / m_scaleY;
  m_desired.Y2max = m_posY2;
  m_desired.Y2min = m_posY2 - m_plotHeight / m_scaleY2;
  CheckAndReportDesiredBoundsChanges();
#ifdef MATHPLOT_DO_LOGGING
  wxLogMessage(_T("mpWindow::DoZoomInYCalc() prior Y coord: (%f), new Y coord: (%f) SHOULD BE EQUAL!!"), staticY, p2y(staticYpixel));
#endif
}

void mpWindow::DoZoomOutXCalc(const int staticXpixel)
{
  // Preserve the position of the clicked point:
  double staticX = p2x(staticXpixel);
  // Zoom out:
  m_scaleX /= m_zoomIncrementalFactor;
  // Adjust the new m_posx/y:
  m_posX = staticX - (staticXpixel / m_scaleX);
  // Adjust desired
  m_desired.Xmin = m_posX;
  m_desired.Xmax = m_posX + m_plotWidth / m_scaleX;
  CheckAndReportDesiredBoundsChanges();
#ifdef MATHPLOT_DO_LOGGING
  wxLogMessage(_T("mpWindow::DoZoomOutXCalc() prior X coord: (%f), new X coord: (%f) SHOULD BE EQUAL!!"), staticX, p2x(staticXpixel));
#endif
}

void mpWindow::DoZoomOutYCalc(const int staticYpixel)
{
  // Preserve the position of the clicked point:
  double staticY = p2y(staticYpixel);
  double staticY2 = p2y(staticYpixel, true);
  // Zoom out:
  m_scaleY /= m_zoomIncrementalFactor;
  m_scaleY2 /= m_zoomIncrementalFactor;
  // Adjust the new m_posx/y:
  m_posY = staticY + (staticYpixel / m_scaleY);
  m_posY2 = staticY2 + (staticYpixel / m_scaleY2);
  // Adjust desired
  m_desired.Ymax = m_posY;
  m_desired.Ymin = m_posY - m_plotHeight / m_scaleY;
  m_desired.Y2max = m_posY2;
  m_desired.Y2min = m_posY2 - m_plotHeight / m_scaleY2;
  CheckAndReportDesiredBoundsChanges();
#ifdef MATHPLOT_DO_LOGGING
  wxLogMessage(_T("mpWindow::DoZoomOutYCalc() prior Y coord: (%f), new Y coord: (%f) SHOULD BE EQUAL!!"), staticY, p2y(staticYpixel));
#endif
}

void mpWindow::ZoomIn(const wxPoint &centerPoint)
{
  Zoom(true, centerPoint);
}

void mpWindow::ZoomOut(const wxPoint &centerPoint)
{
  Zoom(false, centerPoint);
}

void mpWindow::Zoom(bool zoomIn, const wxPoint &centerPoint)
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
  double prior_layer_y2 = p2y(c.y, true);

  double zoomFactor = zoomIn?
      m_zoomIncrementalFactor : 1.0/m_zoomIncrementalFactor;
  m_scaleX *= zoomFactor;
  m_scaleY *= zoomFactor;
  m_scaleY2 *= zoomFactor;

  // Adjust the new m_posx/y:
  m_posX = prior_layer_x - c.x / m_scaleX;
  m_posY = prior_layer_y + c.y / m_scaleY;
  m_posY2 = prior_layer_y2 + c.y / m_scaleY2;

  m_desired.Xmin = m_posX;
  m_desired.Xmax = m_posX + m_plotWidth / m_scaleX;
  m_desired.Ymax = m_posY;
  m_desired.Ymin = m_posY - m_plotHeight / m_scaleY;
  m_desired.Y2max = m_posY2;
  m_desired.Y2min = m_posY2 - m_plotHeight / m_scaleY2;

#ifdef MATHPLOT_DO_LOGGING
  wxLogMessage(_T("mpWindow::Zoom() prior coords: (%f,%f), new coords: (%f,%f) SHOULD BE EQUAL!!"),
      prior_layer_x, prior_layer_y, p2x(c.x), p2y(c.y));
#endif
  UpdateAll();
  CheckAndReportDesiredBoundsChanges();
}

void mpWindow::ZoomInX()
{
  m_scaleX *= m_zoomIncrementalFactor;
  UpdateAll();
  CheckAndReportDesiredBoundsChanges();
}

void mpWindow::ZoomOutX()
{
  m_scaleX /= m_zoomIncrementalFactor;
  UpdateAll();
  CheckAndReportDesiredBoundsChanges();
}

void mpWindow::ZoomInY()
{
  m_scaleY *= m_zoomIncrementalFactor;
  m_scaleY2 *= m_zoomIncrementalFactor;
  UpdateAll();
  CheckAndReportDesiredBoundsChanges();
}

void mpWindow::ZoomOutY()
{
  m_scaleY /= m_zoomIncrementalFactor;
  m_scaleY2 /= m_zoomIncrementalFactor;
  UpdateAll();
  CheckAndReportDesiredBoundsChanges();
}

void mpWindow::ZoomRect(wxPoint p0, wxPoint p1)
{
  // Compute the 2 corners in graph coordinates:
  double p0x = p2x(p0.x);
  double p0y = p2y(p0.y);
  double p0y2 = p2y(p0.y, true);
  double p1x = p2x(p1.x);
  double p1y = p2y(p1.y);
  double p1y2 = p2y(p1.y, true);

  // Order them:
  mpFloatRect zoom;
  zoom.Xmin = p0x < p1x ? p0x : p1x;
  zoom.Xmax = p0x > p1x ? p0x : p1x;
  zoom.Ymin = p0y < p1y ? p0y : p1y;
  zoom.Ymax = p0y > p1y ? p0y : p1y;
  zoom.Y2min = p0y2 < p1y2 ? p0y2 : p1y2;
  zoom.Y2max = p0y2 > p1y2 ? p0y2 : p1y2;

#ifdef MATHPLOT_DO_LOGGING
  wxLogMessage(_T("Zoom: (%f,%f)-(%f,%f)"), zoom.Xmin, zoom.Ymin, zoom.Xmax, zoom.Ymax);
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
  if (CheckUserMouseAction(event))
    return;

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

void mpWindow::OnLockAspect(wxCommandEvent &WXUNUSED(event))
{
  LockAspect(!m_lockaspect);
}

void mpWindow::OnMouseHelp(wxCommandEvent &WXUNUSED(event))
{
  wxMessageBox(
      MESS_HELP1 + _T("\n") + MESS_HELP2 + _T("\n") + MESS_HELP3 + _T("\n") + MESS_HELP4 + _T("\n") + MESS_HELP5 + _T("\n") + MESS_HELP6,
      MESS_HELP0, wxOK, this);
}

void mpWindow::OnFit(wxCommandEvent &WXUNUSED(event))
{
  Fit();
}

void mpWindow::OnToggleGrids(wxCommandEvent &WXUNUSED(event))
{
  mpScaleX* scaleX = GetLayerXAxis();
  if (scaleX)
  {
    scaleX->ShowGrids(!scaleX->GetShowGrids());
  }
  mpScaleY* scaleY = GetLayerYAxis();
  if (scaleY)
  {
    scaleY->ShowGrids(!scaleY->GetShowGrids());
  }
  UpdateAll();
}

void mpWindow::OnToggleCoords(wxCommandEvent &WXUNUSED(event))
{
  mpInfoCoords* coords = (mpInfoCoords*)GetLayerByClassName(_T("mpInfoCoords"));
  if (coords)
  {
    coords->SetVisible(!coords->IsVisible());
    UpdateAll();
  }
}

void mpWindow::OnScreenShot(wxCommandEvent &WXUNUSED(event))
{
  ClipboardScreenshot();
}

void mpWindow::OnFullScreen(wxCommandEvent &WXUNUSED(event))
{
  m_fullscreen = !m_fullscreen;
  m_parent->ShowFullScreen(m_fullscreen);
}

/**
 * Create series from a data file.
 * The file is not formatted. For example a simple txt file or csv file
 * First data is x abscissa and next is up to 9 ordinates
 * Separator is : space or ; or tab
 */
void mpWindow::OnLoadFile(wxCommandEvent &WXUNUSED(event))
{
  wxFileDialog OpenFile(this, MESS_LOAD, wxEmptyString, wxEmptyString, MESS_WILDCARD, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (OpenFile.ShowModal() == wxID_OK)
  {
    LoadFile(OpenFile.GetPath());
    Fit();
  }
}

void mpWindow::OnConfiguration(wxCommandEvent &WXUNUSED(event))
{
  if (m_configWindow == NULL)
    m_configWindow = new MathPlotConfigDialog(this);

  m_configWindow->Initialize();
  m_configWindow->Show();
}

void mpWindow::OnCenter(wxCommandEvent &WXUNUSED(event))
{
  int h, w;
  GetClientSize(&w, &h);
  SetScreen(w, h);
  int centerX = m_plotWidth / 2;
  int centerY = m_plotHeight / 2;
  SetPos(p2x(m_clickedX - centerX - m_margin.left), p2y(m_clickedY - centerY - m_margin.top),
      p2y(m_clickedY - centerY - m_margin.top, true));
}

void mpWindow::OnZoomIn(wxCommandEvent &WXUNUSED(event))
{
  Zoom(true, m_mouseRClick);
}

void mpWindow::OnZoomOut(wxCommandEvent &WXUNUSED(event))
{
  Zoom(false, wxDefaultPosition);
}

void mpWindow::OnSize(wxSizeEvent &WXUNUSED(event))
{
  // Need to redraw the legend bitmap
  RefreshLegend();
  // Try to fit again with the new window size:
  Fit(m_desired);
#ifdef MATHPLOT_DO_LOGGING
  wxLogMessage(_T("mpWindow::OnSize() m_scrX = %d, m_scrY = %d"), m_scrX, m_scrY);
#endif // MATHPLOT_DO_LOGGING
}

bool mpWindow::AddLayer(mpLayer *layer, bool refreshDisplay)
{
  if (layer != NULL)
  {
    int info;
    int scale;

    if (layer->IsLayerType(mpLAYER_INFO, &info))
    {
      if (info == mpiCoords)
      {
        // Only one info coords is allowed
        if (m_InfoCoords)
          return false;
        m_InfoCoords = (mpInfoCoords*)layer;
      }

      if (info == mpiLegend)
      {
        // Only one info legend is allowed
        if (m_InfoLegend)
          return false;
        m_InfoLegend = (mpInfoLegend*)layer;
      }
    }

    // add the layer to the layer list
    m_layers.push_back(layer);

    if (layer->IsLayerType(mpLAYER_AXIS, &scale))
    {
      if ((scale == mpsScaleX) && (m_XAxis == NULL))
      {
        // Only the first X axis
        m_XAxis = (mpScaleX*)layer;
      }

      if (scale == mpsScaleY)
      {
        if (((mpScaleY*)layer)->IsY2Axis())
        {
          Update_CountY2Axis(true);
        }
        else
        {
          if (m_YAxis == NULL)
          {
            // Only the first Y axis
            m_YAxis = (mpScaleY*)layer;
          }
        }
      }
    }

    // We just add a function, so we need to update the legend
    if (layer->GetLayerType() == mpLAYER_PLOT)
    {
      RefreshLegend();
    }

    layer->SetWindow(*this);

    if (refreshDisplay)
      UpdateAll();
    RefreshConfigWindow();

    return true;
  }
  return false;
}

bool mpWindow::DelLayer(mpLayer *layer, bool alsoDeleteObject, bool refreshDisplay)
{
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
    if (*it == layer)
    {
      bool cancel = false;
      if (m_OnDeleteLayer != NULL)
        m_OnDeleteLayer(this, (*it)->GetClassInfo()->GetClassName(), cancel);
      if (!cancel)
      {
        if (layer == m_InfoCoords)
          m_InfoCoords = NULL;
        if (layer == m_InfoLegend)
          m_InfoLegend = NULL;
        if (layer == m_movingInfoLayer)
          m_movingInfoLayer = NULL;
        if (layer == m_XAxis)
          m_XAxis = NULL;
        if (layer == m_YAxis)
          m_YAxis = NULL;
        // In case we suppress an Y2 axis
        int scale;
        if ((layer->IsLayerType(mpLAYER_AXIS, &scale)) && (scale == mpsScaleY))
        {
          if (((mpScaleY*)layer)->IsY2Axis())
            Update_CountY2Axis(false);
        }
        if (layer == m_Y2Axis)
          m_Y2Axis = NULL;
        // Also delete the object?
        if (alsoDeleteObject)
          delete *it; // delete the object pointed at by the iterator
        // Remove pointer to the object from m_layers.
        // WARNING: 'erase' invalidates 'it' and all m_layers iterators in existence
        m_layers.erase(it);
        // No need to reset 'it' as we return without any further reference.
        // Refresh
        RefreshLegend();
        if (refreshDisplay)
          UpdateAll();
        RefreshConfigWindow();
        return true;
      }
      else
        return false;
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
    m_layers.erase(m_layers.begin()); // remove ptr to object from m_layers
  }
  m_InfoCoords = NULL;
  m_InfoLegend = NULL;
  m_movingInfoLayer = NULL;
  m_XAxis = NULL;
  m_YAxis = NULL;
  m_Y2Axis = NULL;
  m_countY2Axis = 0;
  if (refreshDisplay)
    UpdateAll();
  DeleteAndNull(m_configWindow);
}

void mpWindow::DelAllPlot(bool alsoDeleteObject, mpFunctionType func, bool refreshDisplay)
{
  int function;
  mpLayerList::iterator it = m_layers.begin();
  if(it == m_layers.end()) return; // Don't blow up if no layers were added yet, nothing to do here.
  do
  {
    if ((*it)->IsLayerType(mpLAYER_PLOT, &function) && ((func == mpfAllType) || (function == func)))
    {
      DelLayer((mpLayer*)(*it), alsoDeleteObject, false); // may invalidate all extant m_layers iterators
      it = m_layers.begin(); // ...so reset iterator to begin of m_layers vector
    }
    else it++;
  } while (it != m_layers.end());
  RefreshLegend();
  if (refreshDisplay)
    UpdateAll();
  RefreshConfigWindow();
}

void mpWindow::OnPaint(wxPaintEvent &WXUNUSED(event))
{
#ifdef _WIN32
  wxPaintDC dc(this);
#else
  wxAutoBufferedPaintDC dc(this);
#endif
  int h, w;
  dc.GetSize(&w, &h);   // This is the size of the visible area only!
  if (w == 0 || h == 0)
    return;
  SetScreen(w, h);
  wxMemoryDC* m_buff_dc = NULL;

#ifdef MATHPLOT_DO_LOGGING
  {
    int px = 0, py = 0;
//    GetViewStart( &px, &py );
    wxLogMessage(_T("[mpWindow::OnPaint] vis.area:%ix%i px=%i py=%i"), m_scrX, m_scrY, px, py);
  }
#endif

  m_repainting = true;

  // Selects direct or buffered draw:
  wxDC* trgDc;

  // J.L.Blanco @ Aug 2007: Added double buffer support
  if (m_enableDoubleBuffer)
  {
    // Recreate Bitmap if sizes have changed
    if (m_last_lx != m_scrX || m_last_ly != m_scrY)
    {
      DeleteAndNull(m_buff_bmp);
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
  if (m_drawBox)
    trgDc->SetPen(*wxBLACK);
  else
    trgDc->SetPen(*wxTRANSPARENT_PEN);
  trgDc->SetBrush(*wxWHITE_BRUSH);
  trgDc->DrawRectangle(0, 0, m_scrX, m_scrY);

  // Draw background plot area
  trgDc->SetBrush(m_bgColour);
  trgDc->SetTextForeground(m_fgColour);
  trgDc->DrawRectangle(m_PlotArea);

  // Draw all the layers in Z order
  for (int i = mpZIndex_BACKGROUND; i < mpZIndex_END; i++)
  {
    for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
    {
      if ((*it)->GetZIndex() == i)
        (*it)->Plot(*trgDc, *this);
    }
  }

  // If doublebuffer, draw now to the window:
  if (m_enableDoubleBuffer)
  {
    dc.Blit(0, 0, m_scrX, m_scrY, trgDc, 0, 0, wxCOPY);
    m_buff_dc->SelectObject(wxNullBitmap);
    delete m_buff_dc;
  }

  // We redraw the cross if necessary. We pass the mouse position if we do a pan operation.
  if (m_magnetize)
    m_magnet.UpdatePlot(dc, m_mouseRClick);

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

/// Deprecated: Incomplete! Use UpdateBBox!
void mpWindow::SetBound()
{
  bool HaveXAxis = (m_XAxis && (!m_XAxis->GetAuto()));
  bool HaveYAxis = (m_YAxis && (!m_YAxis->GetAuto()));

  if (HaveXAxis || HaveYAxis)
  {
    int function;
    for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
    {
      if ((*it)->IsLayerType(mpLAYER_PLOT, &function))
      {
        if ((function == mpfFX) && HaveXAxis)
        {
          m_bound.Ymin = ((mpFX*)(*it))->GetY(m_XAxis->GetMinScale());
          m_bound.Ymax = ((mpFX*)(*it))->GetY(m_XAxis->GetMaxScale());
        }
        else
          if ((function == mpfFY) && HaveYAxis)
          {
            m_bound.Xmin = ((mpFY*)(*it))->GetX(m_YAxis->GetMinScale());
            m_bound.Xmax = ((mpFY*)(*it))->GetX(m_YAxis->GetMaxScale());
          }
      }
    }
  }
}

bool mpWindow::UpdateBBox()
{
  bool first = true;
  bool firstY = true;
  bool firstY2 = true;

  // Deprecated: To update bound of mpFX and mpFY functions: SetBound();
#ifdef MATHPLOT_DO_LOGGING
  wxLogMessage
  (_T("[mpWindow::UpdateBBox] Bounding box enter: Xmin = %f, Xmax = %f, Ymin = %f, YMax = %f"), m_bound.Xmin, m_bound.Xmax, m_bound.Ymin,
      m_bound.Ymax);
#endif // MATHPLOT_DO_LOGGING

  // Find minimum bounding box to fit all visible layers
  mpFloatRect f_bound;
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
    mpLayer* f = *it;
    if (!f->HasBBox() || !f->IsVisible()) continue; // this layer isn't used for bounding box

    f->GetBBox(&f_bound);
    // X
    if (first)
    {
      first = false;
      m_bound.Xmin = f_bound.Xmin;
      m_bound.Xmax = f_bound.Xmax;
    }
    else
    {
      if (f_bound.Xmin < m_bound.Xmin)
        m_bound.Xmin = f_bound.Xmin;
      if (f_bound.Xmax > m_bound.Xmax)
        m_bound.Xmax = f_bound.Xmax;
    }
    if ((f->GetLayerType() == mpLAYER_PLOT) && (((mpFunction*)f)->GetY2Axis()))
    {
      // Y2
      if (firstY2)
      {
        m_bound.Y2min = f_bound.Ymin;
        m_bound.Y2max = f_bound.Ymax;
        firstY2 = false;
      }
      else
      {
        if (f_bound.Ymin < m_bound.Y2min)
          m_bound.Y2min = f_bound.Ymin;
        if (f_bound.Ymax > m_bound.Y2max)
          m_bound.Y2max = f_bound.Ymax;
      }
    }
    else
    {
      // Y
      if (firstY)
      {
        m_bound.Ymin = f_bound.Ymin;
        m_bound.Ymax = f_bound.Ymax;
        firstY = false;
      }
      else
      {
        if (f_bound.Ymin < m_bound.Ymin)
          m_bound.Ymin = f_bound.Ymin;
        if (f_bound.Ymax > m_bound.Ymax)
          m_bound.Ymax = f_bound.Ymax;
      }
    }
  } // end iteration over all layers

  // Take care of scale : restrict bound
  if (m_XAxis && (!m_XAxis->GetAuto()))
  {
    m_bound.Xmin = m_XAxis->GetMinScale();
    m_bound.Xmax = m_XAxis->GetMaxScale();
  }
  if (m_YAxis && (!m_YAxis->GetAuto()))
  {
    m_bound.Ymin = m_YAxis->GetMinScale();
    m_bound.Ymax = m_YAxis->GetMaxScale();
  }
  if (m_Y2Axis && (!m_Y2Axis->GetAuto()))
  {
    m_bound.Y2min = m_Y2Axis->GetMinScale();
    m_bound.Y2max = m_Y2Axis->GetMaxScale();
  }

  // To always have a bound rectangle
  if (m_bound.Xmin == m_bound.Xmax)
  {
    if (m_bound.Xmax > 0)
      m_bound.Xmin = 0;
    else
      m_bound.Xmax = 0;
  }

  if (m_bound.Ymin == m_bound.Ymax)
  {
    if (m_bound.Ymax > 0)
      m_bound.Ymin = 0;
    else
      m_bound.Ymax = 0;
  }

  if (m_bound.Y2min == m_bound.Y2max)
  {
    if (m_bound.Y2max > 0)
      m_bound.Y2min = 0;
    else
      m_bound.Y2max = 0;
  }

  // Log X axis
  if (m_LogXaxis)
  {
    if (m_bound.Xmin > 0)
      m_bound.Xmin = log10(m_bound.Xmin);
    else
      m_bound.Xmin = 0;
    if (m_bound.Xmax > 0)
      m_bound.Xmax = log10(m_bound.Xmax);
    else
      m_bound.Xmax = 0;
  }

  // Log Y axis
  if (m_LogYaxis)
  {
    if (m_bound.Ymin > 0)
      m_bound.Ymin = log10(m_bound.Ymin);
    else
      m_bound.Ymin = 0;
    if (m_bound.Ymax > 0)
      m_bound.Ymax = log10(m_bound.Ymax);
    else
      m_bound.Ymax = 0;
  }

#ifdef MATHPLOT_DO_LOGGING
  wxLogMessage
  (_T("[mpWindow::UpdateBBox] Bounding box exit: Xmin = %f, Xmax = %f, Ymin = %f, YMax = %f"), m_bound.Xmin, m_bound.Xmax, m_bound.Ymin,
      m_bound.Ymax);
#endif // MATHPLOT_DO_LOGGING
  return first == false;
}

void mpWindow::UpdateAll()
{
  if (m_magnetize)
  {
    // To be sure to skip events that may occur before OnPaint
    m_repainting = true;
    m_magnet.SaveDrawState();
  }

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
        int sizeX = (int)((maxX - minX) * m_scaleX);
        int thumbX = (int)(((m_posX + leftMargin) - minX) * m_scaleX);
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
        int sizeY = (int)((maxY - minY) * m_scaleY);
        int thumbY = (int)((maxY - (m_posY - topMargin)) * m_scaleY);
        SetScrollbar(wxVERTICAL, thumbY, cy - (m_margin.top + m_margin.bottom), sizeY);
      }
    }
  }

  Refresh();
}

void mpWindow::DoScrollCalc(const int position, const int orientation)
{
  if (orientation == wxVERTICAL)
  {
    // Y axis
    // Get top margin in coord units
    double topMargin = m_margin.top / m_scaleY;
    double topMargin2 = m_margin.top / m_scaleY2;
    // Calculate maximum Y coord to be shown in the graph
    double maxY = m_desired.Ymax > m_bound.Ymax ? m_desired.Ymax : m_bound.Ymax;
    double maxY2 = m_desired.Y2max > m_bound.Y2max ? m_desired.Y2max : m_bound.Y2max;
    // Set new position
    SetPosY((maxY - (position / m_scaleY)) + topMargin, (maxY2 - (position / m_scaleY2)) + topMargin2);
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
  position -= SCROLL_NUM_PIXELS_PER_LINE;
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
  position += SCROLL_NUM_PIXELS_PER_LINE;
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
/**
 * Several layer operations
 * - CountLayers() : return the number of layer with BBox
 * - CountLayersType() : return the number of layer who are of selected type
 * - CountLayersFXYPlot() : return the number of layer who are FXYVector function plot
 * - GetLayer(int position) : return the layer at position
 * - GetLayerPlot(int position, mpFunctionType func) : return the layer at position if his function type is func
 */
unsigned int mpWindow::CountLayers()
{
  unsigned int layerNo = 0;
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
    if ((*it)->HasBBox())
      layerNo++;
  }
  return layerNo;
}

unsigned int mpWindow::CountLayersType(mpLayerType type)
{
  unsigned int layerNo = 0;
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
    if ((*it)->GetLayerType() == type)
      layerNo++;
  }
  return layerNo;
}

mpLayer* mpWindow::GetLayersType(int position, mpLayerType type)
{
  int layerNo = -1;
  if (position < 0)
    return NULL;
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
    if ((*it)->GetLayerType() == type)
    {
      if (++layerNo == position)
        return *it;
    }
  }
  return NULL;
}

unsigned int mpWindow::CountLayersFXYPlot()
{
  unsigned int layerNo = 0;
  int function;
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
    if ((*it)->IsLayerType(mpLAYER_PLOT, &function) && (function == mpfFXYVector))
      layerNo++;
  }
  return layerNo;
}

mpLayer* mpWindow::GetLayer(int position)
{
  if ((position >= (int)m_layers.size()) || position < 0)
    return NULL;
  return m_layers[position];
}

mpLayer* mpWindow::GetLayerPlot(int position, mpFunctionType func)
{
  int layerNo = -1;
  int function;
  if (position < 0)
    return NULL;
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
    if ((*it)->IsLayerType(mpLAYER_PLOT, &function) && ((func == mpfAllType) || (function == func)))
    {
      if (++layerNo == position)
        return *it;
    }
  }
  return NULL;
}

mpLayer* mpWindow::GetLayerAxis(int position, mpScaleType scale)
{
  int layerNo = -1;
  int thescale;
  if (position < 0)
    return NULL;
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
    if ((*it)->IsLayerType(mpLAYER_AXIS, &thescale) && ((scale == mpsAllType) || (thescale == scale)))
    {
      if (++layerNo == position)
        return *it;
    }
  }
  return NULL;
}

mpFXYVector* mpWindow::GetXYSeries(unsigned int n, const wxString &name, bool create)
{
  mpFXYVector* serie = (mpFXYVector*)this->GetLayerPlot(n, mpfFXYVector);
  if ((serie == NULL) && create)
  {
    serie = new mpFXYVector(wxString::Format(_T("%s %d"), name, n));
    serie->SetContinuity(true);
    wxPen pen(wxIndexColour(n), 2, wxPENSTYLE_SOLID);
    serie->SetPen(pen);
    this->AddLayer(serie);
  }
  return serie;
}

mpLayer* mpWindow::GetClosestPlot(wxCoord ix, wxCoord iy, double *xnear, double *ynear, bool *isY2Axis)
{
#define NEAR_AREA 8
  int function;
  mpLayer* result = NULL;

  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
    if ((*it)->IsTractable() && (*it)->IsVisible() && (*it)->IsLayerType(mpLAYER_PLOT, &function))
    {
      switch (function)
      {
        case mpfFX:
        {
          mpFX* fx = (mpFX*)(*it);
          double fy = fx->DoGetY(this->p2x(ix));
          if (abs(this->y2p(fy, fx->GetY2Axis()) - iy) < NEAR_AREA)
          {
            *xnear = this->p2x(ix);
            *ynear = fy;
            *isY2Axis = fx->GetY2Axis();
            result = (*it);
          }
          break;
        }
        case mpfFY:
        {
          mpFY* fy = (mpFY*)(*it);
          double fx = fy->DoGetX(this->p2y(iy, fy->GetY2Axis()));
          if (abs(this->x2p(fx) - ix) < NEAR_AREA)
          {
            *xnear = fx;
            *ynear = this->p2y(iy, fy->GetY2Axis());
            *isY2Axis = fy->GetY2Axis();
            result = (*it);
          }
          break;
        }
        case mpfFXY:
        case mpfFXYVector:
        {
          mpFXY* fxy = (mpFXY*)(*it);
          double xx, yy;
          if (fxy->ViewAsBar())
          {
            double zero = this->y2p(0.0, fxy->GetY2Axis());
            fxy->Rewind();
            while (fxy->DoGetNextXY(&xx, &yy))
            {
              // We are in the x bar range
              if (abs(this->x2p(xx) - ix) < fxy->GetBarWidth())
              {
                wxCoord yyp = this->y2p(yy, fxy->GetY2Axis());
                // Check if we are over the bar
                if (((yy < 0) && ((iy >= zero) && (iy < yyp + NEAR_AREA))) || ((yy > 0) && ((iy <= zero) && (iy > yyp - NEAR_AREA))))
                {
                  *xnear = xx;
                  *ynear = yy;
                  *isY2Axis = fxy->GetY2Axis();
                  result = (*it);
                  break;
                }
              }
            }
          }
          else
          {
            fxy->Rewind();
            while (fxy->DoGetNextXY(&xx, &yy))
            {
              if ((abs(this->x2p(xx) - ix) < NEAR_AREA) && (abs(this->y2p(yy, fxy->GetY2Axis()) - iy) < NEAR_AREA))
              {
                *xnear = xx;
                *ynear = yy;
                *isY2Axis = fxy->GetY2Axis();
                result = (*it);
                break;
              }
            }
          }
          break;
        }

        default: // nothing to do in others cases
          ;
      }
    }
    if (result)
      break;
  }
  return result;
}

mpLayer* mpWindow::GetLayerByName(const wxString &name)
{
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
    if ((*it)->GetName().IsSameAs(name))
      return *it;
  return NULL;    // Not found
}

mpLayer* mpWindow::GetLayerByClassName(const wxString &name)
{
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
    wxString classname = (*it)->GetClassInfo()->GetClassName();
    if (classname.IsSameAs(name))
      return *it;
  }
  return NULL;    // Not found
}

void mpWindow::RefreshLegend(void)
{
  mpInfoLegend* legend = (mpInfoLegend*)GetLayerByClassName(_T("mpInfoLegend"));
  if (legend)
    legend->SetNeedUpdate();
}

/**
 * Get the first scale X layer (X axis) or NULL if not found
 */
mpScaleX* mpWindow::GetLayerXAxis()
{
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
    wxString classname = (*it)->GetClassInfo()->GetClassName();
    if (classname.IsSameAs(_T("mpScaleX")))
      return (mpScaleX*)(*it);
  }
  return NULL;    // Not found
}

/**
 * Get the first scale Y layer (Y axis) or NULL if not found
 */
mpScaleY* mpWindow::GetLayerYAxis()
{
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
    wxString classname = (*it)->GetClassInfo()->GetClassName();
    if ((classname.IsSameAs(_T("mpScaleY"))) && (!(((mpScaleY*)(*it))->IsY2Axis())))
      return (mpScaleY*)(*it);
  }
  return NULL;    // Not found
}

/**
 * Get the first scale Y2 layer (Y2 axis) or NULL if not found
 */
mpScaleY* mpWindow::GetLayerY2Axis()
{
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
    wxString classname = (*it)->GetClassInfo()->GetClassName();
    if ((classname.IsSameAs(_T("mpScaleY"))) && (((mpScaleY*)(*it))->IsY2Axis()))
      return (mpScaleY*)(*it);
  }
  return NULL;    // Not found
}

void mpWindow::Update_CountY2Axis(bool Y2Axis)
{
  if (Y2Axis)
    m_countY2Axis++;
  else
    m_countY2Axis--;

  // Update the pointer to the Y and Y2 axis
  m_YAxis = GetLayerYAxis();
  m_Y2Axis = GetLayerY2Axis();

  // If we have no Y2Axis, we must verify that no plot is on Y2 axis
  if (m_countY2Axis == 0)
  {
    for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
    {
      if ((*it)->GetLayerType() == mpLAYER_PLOT)
        ((mpFunction*)(*it))->SetY2Axis(false);
    }
  }
}

mpInfoLayer* mpWindow::IsInsideInfoLayer(const wxPoint &point)
{
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
#ifdef MATHPLOT_DO_LOGGING
    wxLogMessage(_T("mpWindow::IsInsideInfoLayer() examinining layer = %p"), (*it));
#endif // MATHPLOT_DO_LOGGING
    if ((*it)->GetLayerType() == mpLAYER_INFO)
    {
      mpInfoLayer* tmpLyr = (mpInfoLayer*)(*it);
#ifdef MATHPLOT_DO_LOGGING
      wxLogMessage(_T("mpWindow::IsInsideInfoLayer() layer = %p"), (*it));
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
  mpLayer* lx = GetLayerByName(name);
  if (lx)
  {
    lx->SetVisible(viewable);
    UpdateAll();
  }
}

bool mpWindow::IsLayerVisible(const wxString &name)
{
  mpLayer* lx = GetLayerByName(name);
  if (lx)
    return (lx) ? lx->IsVisible() : false;
  else
    return false;
}

bool mpWindow::IsLayerVisible(const unsigned int position)
{
  mpLayer* lx = GetLayer(position);
  if (lx)
    return (lx) ? lx->IsVisible() : false;
  else
    return false;
}

void mpWindow::SetLayerVisible(const unsigned int position, bool viewable)
{
  mpLayer* lx = GetLayer(position);
  if (lx)
  {
    lx->SetVisible(viewable);
    UpdateAll();
  }
}

void mpWindow::GetBoundingBox(double *bbox) const
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

  m_plotBoundaries.startPx = 0;
  m_plotBoundariesMargin.startPx = m_margin.left;
  m_plotBoundaries.endPx = m_scrX;
  m_plotBoundariesMargin.endPx = m_scrX - m_margin.right;

  m_plotBoundaries.startPy = 0;
  m_plotBoundariesMargin.startPy = m_margin.top;
  m_plotBoundaries.endPy = m_scrY;
  m_plotBoundariesMargin.endPy = m_scrY - m_margin.bottom;

  m_PlotArea = wxRect(m_margin.left - EXTRA_MARGIN, m_margin.top - EXTRA_MARGIN,
      m_plotWidth + 2 * EXTRA_MARGIN, m_plotHeight + 2 * EXTRA_MARGIN);
}

wxBitmap* mpWindow::BitmapScreenshot(wxSize imageSize, bool fit)
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

  DeleteAndNull(m_Screenshot_bmp);
  m_Screenshot_bmp = new wxBitmap(sizeX, sizeY);
  m_Screenshot_dc.SelectObject(*m_Screenshot_bmp);

  // Clean the screen
  if (m_drawBox)
    m_Screenshot_dc.SetPen(*wxBLACK);
  else
    m_Screenshot_dc.SetPen(*wxTRANSPARENT_PEN);
  m_Screenshot_dc.SetBrush(*wxWHITE_BRUSH);
  m_Screenshot_dc.DrawRectangle(0, 0, m_scrX, m_scrY);

  m_Screenshot_dc.SetBrush(m_bgColour);
  m_Screenshot_dc.SetTextForeground(m_fgColour);
  m_Screenshot_dc.DrawRectangle(m_PlotArea);

  if (fit)
  {
    Fit(m_bound, &sizeX, &sizeY);
  }
  else
  {
    if (imageSize != wxDefaultSize)
      Fit(m_desired, &sizeX, &sizeY);
  }

  // Draw all the layers in Z order
  for (int i = mpZIndex_BACKGROUND; i < mpZIndex_END; i++)
  {
    for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
    {
      if ((*it)->GetZIndex() == i)
        (*it)->Plot(m_Screenshot_dc, *this);
    }
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
      wxMessageBox(MESS_CLIPFAIL);
    wxTheClipboard->Flush();
    wxTheClipboard->Close();
  }
}

bool mpWindow::SaveScreenshot(const wxString &filename, int type, wxSize imageSize, bool fit)
{
  BitmapScreenshot(imageSize, fit);

  wxImage screenImage = m_Screenshot_bmp->ConvertToImage();
  return screenImage.SaveFile(filename, (wxBitmapType)type);
}

bool mpWindow::LoadFile(const wxString &filename)
{
  FILE* file = fopen(filename.c_str(), "r");
  if (file == NULL)
    return false;

  unsigned int nb_series = this->CountLayersFXYPlot();
  wxFileName thefile(filename);
  wxString name = thefile.GetName();

  // Max line length
  char line[1024];
  // Data separator : space or ; or tab
  char seps[] = " ;\t\n";
  char* token;
  // Maximum, 10 data by line. First data is x coordinate.
  double data[10];
  int i = 0, j = 0;
  do
  {
    if (fgets(line, 1024, file) == NULL)
      continue; // EOF

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

  RefreshLegend();
  return true;
}

void mpWindow::SetColourTheme(const wxColour &bgColour, const wxColour &drawColour, const wxColour &axesColour)
{
  SetForegroundColour(drawColour);
  m_bgColour = bgColour;
  m_fgColour = drawColour;
  m_axColour = axesColour;

  // cycle between layers to set colours and properties to them
  for (mpLayerList::iterator it = m_layers.begin(); it != m_layers.end(); it++)
  {
    mpLayerType type = (*it)->GetLayerType();
    wxPen pen = (*it)->GetPen(); // Get the old pen to modify only colour, not style or width

    if (type == mpLAYER_AXIS)
    {
      pen.SetColour(axesColour);
    }
    if ((type == mpLAYER_INFO) || (type == mpLAYER_TEXT))
    {
      pen.SetColour(drawColour);
    }
    (*it)->SetPen(pen);
  }
}

void mpWindow::RefreshConfigWindow()
{
  if (m_configWindow)
    m_configWindow->Initialize();
}

MathPlotConfigDialog* mpWindow::GetConfigWindow(bool Create)
{
  if (Create && (m_configWindow == NULL))
    m_configWindow = new MathPlotConfigDialog(this);

  return m_configWindow;
}

//-----------------------------------------------------------------------------
// mpText - provided by Val Greene
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpText, mpLayer)

/** @param name text to be displayed
 @param offsetx x position in percentage (0-100)
 @param offsetx y position in percentage (0-100)
 */
mpText::mpText(const wxString &name, int offsetx, int offsety) :
    mpText(name)
{
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
mpText::mpText(const wxString &name, mpLocation marginLocation) :
    mpText(name)
{
  m_location = marginLocation;
}

/** mpText Layer plot handler.
 This implementation will plot the text adjusted to the visible area.
 */
void mpText::DoPlot(wxDC &dc, mpWindow &w)
{
  wxCoord px = 0, py = 0;
  wxCoord tw = 0, th = 0;
  dc.GetTextExtent(GetName(), &tw, &th);

  switch (m_location)
  {
    case mpMarginNone:
    {
      px = m_offsetx * w.GetPlotWidth() / 100;
      py = m_offsety * w.GetPlotHeight() / 100;
      break;
    }
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

  // Keep at least one pixel
  if (px < 3)
    px = 3;
  if (py < 3)
    py = 3;

  // Erase background of text with current brush and pen
  dc.DrawRectangle(px - 2, py - 2, tw + 4, th + 4);
  dc.DrawText(GetName(), px, py);
}

//-----------------------------------------------------------------------------
// mpTitle
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpTitle, mpText)

mpTitle::mpTitle()
{
  m_subtype = mptTitle;
  m_location = mpMarginTopCenter;
  SetPen(*wxWHITE_PEN);
  SetBrush(*wxWHITE_BRUSH);
}

//-----------------------------------------------------------------------------
// mpPrintout - provided by Davide Rondini
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpPrintout, wxPrintout)

mpPrintout::mpPrintout(mpWindow *drawWindow, const wxString &title, int factor) :
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

  wxDC* trgDc = GetDC();

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
    double ratio = (double)(m_prnXw) / plotWindow->GetScreenX();

    // Same ratio for y
    m_prnYw = (wxCoord)(ratio * plotWindow->GetScreenY());
    // Then compute y margin
    marginY = (m_prnY - m_prnYw) / 2;

#ifdef MATHPLOT_DO_LOGGING
    wxLogMessage(_T("Printer Size: %d x %d\n"), m_prnX, m_prnY);
    wxLogMessage(_T("Actual Screen Size: %d x %d\n"), plotWindow->GetScreenX(), plotWindow->GetScreenY());
    wxLogMessage(_T("Screen Size for Printer: %d x %d\n"), m_prnXw, m_prnYw);
#endif

    plotWindow->BitmapScreenshot(wxSize(m_prnXw / stretch_factor, m_prnYw / stretch_factor));
    wxMemoryDC bitmap_dc(trgDc);
    bitmap_dc.SelectObject(*(plotWindow->m_Screenshot_bmp));

    if (stretch_factor == 1)
      trgDc->Blit(marginX, marginY, m_prnXw, m_prnYw, &bitmap_dc, 0, 0);
    else
      trgDc->StretchBlit(marginX, marginY, m_prnXw, m_prnYw, &bitmap_dc, 0, 0, m_prnXw / stretch_factor, m_prnYw / stretch_factor);
    bitmap_dc.SelectObject(wxNullBitmap);
  }
  return true;
}

bool mpPrintout::HasPage(int page)
{
  return (page == 1);
}

//-----------------------------------------------------------------------------
// SinCos - retrieve sin/cos pair
//-----------------------------------------------------------------------------
//#define ASSEMBLER
inline void SinCos(double angleRadians, double *sinA, double *cosA)
{
#ifdef ASSEMBLER
  // https://gcc.gnu.org/onlinedocs/gcc-4.9.2/gcc/Extended-Asm.html#Extended-Asm
  __asm__ ("fsincos" : "=t" (*cosA), "=u" (*sinA) : "0" (angleRadians));
#else
#if __GNUC__
  // Use GNU extension
  sincos(angleRadians, sinA, cosA);
#else
    *sinA = sin(angleRadians);
    *cosA = cos(angleRadians);
  #endif
#endif
}

//-----------------------------------------------------------------------------
// mpMovableObject - provided by Jose Luis Blanco
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(mpMovableObject, mpLayer)

void mpMovableObject::TranslatePoint(double x, double y, double &out_x, double &out_y) const
{
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
    wxLogError(_T("[mpMovableObject::ShapeUpdated] Error, m_shape_xs and m_shape_ys have different lengths!"));
  }
  else
  {
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

void mpMovableObject::DoPlot(wxDC &dc, mpWindow &w)
{
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
        if (m_symbol != mpsNone)
          DrawSymbol(dc, w.x2p(*(itX++)), w.y2p(*(itY++)));
        else
          dc.DrawPoint(w.x2p(*(itX++)), w.y2p(*(itY++)));
      }
    }
    else
    {
      while (itX != m_trans_shape_xs.end())
      {
        wxCoord cx = w.x2p(*(itX++));
        wxCoord cy = w.y2p(*(itY++));
        if (m_symbol != mpsNone)
          DrawSymbol(dc, cx, cy);
        else
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
      if (m_symbol != mpsNone)
        DrawSymbol(dc, cx0, cy0);
      cx0 = cx;
      cy0 = cy;
    }
    // Last point
    if (m_symbol != mpsNone)
      DrawSymbol(dc, cx0, cy0);
  }

  if (m_showName && !m_name.IsEmpty())
  {
    wxCoord tx, ty;
    dc.GetTextExtent(m_name, &tx, &ty);

    if (HasBBox())
    {
      wxCoord sx = (wxCoord)((m_bbox_max_x - w.GetPosX()) * w.GetScaleX());
      wxCoord sy = (wxCoord)((w.GetPosY() - m_bbox_max_y) * w.GetScaleY());

      tx = sx - tx - 8;
      ty = sy - 8 - ty;
    }
    else
    {
      const int sx = w.GetScreenX() / 2;
      const int sy = w.GetScreenY() / 2;

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
    wxLogError(_T("[mpCovarianceEllipse] Error: quantiles must be non-negative"));
    return;
  }
  if (m_cov_00 < 0)
  {
    wxLogError(_T("[mpCovarianceEllipse] Error: cov(0,0) must be non-negative"));
    return;
  }
  if (m_cov_11 < 0)
  {
    wxLogError(_T("[mpCovarianceEllipse] Error: cov(1,1) must be non-negative"));
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
    wxLogError(_T("[mpCovarianceEllipse] Error: cov is not positive definite"));
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
    wxLogError(_T("[mpPolygon] Error: points_xs and points_ys must have the same number of elements"));
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
    wxLogError(_T("[mpBitmapLayer] Assigned bitmap is not Ok()!"));
  }
  else
  {
    m_bitmap = inBmp; //.GetSubBitmap( wxRect(0, 0, inBmp.GetWidth(), inBmp.GetHeight()));
    m_min_x = x;
    m_min_y = y;
    m_max_x = x + lx;
    m_max_y = y + ly;
    m_validImg = true;
    m_bitmapChanged = true;
  }
}

void mpBitmapLayer::DoPlot(wxDC &dc, mpWindow &w)
{
  if (!m_validImg)
    return;

  /*  1st: We compute (x0,y0)-(x1,y1), the pixel coordinates of the real outer limits
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
  double screenPixelX = (x1 - x0) / (double)m_bitmap.GetWidth();
  double screenPixelY = (y1 - y0) / (double)m_bitmap.GetHeight();

  // The minimum number of pixels that the streched image will overpass the actual mpWindow borders:
  wxCoord borderMarginX = (wxCoord)(screenPixelX + 1); // ceil
  wxCoord borderMarginY = (wxCoord)(screenPixelY + 1); // ceil

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
  wxCoord offset_x = (wxCoord)((dx0 - x0) / screenPixelX);
  wxCoord offset_y = (wxCoord)((dy0 - y0) / screenPixelY);

  // and the size in pixel of the area to be actually drawn from the internally stored bitmap:
  wxCoord b_width = (wxCoord)((dx1 - dx0 + 1) / screenPixelX);
  wxCoord b_height = (wxCoord)((dy1 - dy0 + 1) / screenPixelY);

  if (b_width + offset_x > m_bitmap.GetWidth())
    b_width = m_bitmap.GetWidth() - offset_x;
  if (b_height + offset_y > m_bitmap.GetHeight())
    b_height = m_bitmap.GetHeight() - offset_y;

#ifdef MATHPLOT_DO_LOGGING
  wxLogMessage(_T("[mpBitmapLayer::Plot] screenPixel: x=%f y=%f  d_width=%ix%i"),
      screenPixelX, screenPixelY, d_width, d_height);
  wxLogMessage(_T("[mpBitmapLayer::Plot] offset: x=%i y=%i  bmpWidth=%ix%i"),
      offset_x, offset_y, b_width, b_height);
#endif

  // Is there any visible region?
  if ((d_width > 0) && (d_height > 0) && (b_width > 0) && (b_height > 0))
  {
    // Build the scaled bitmap from the image, only if it has changed:
    if (m_scaledBitmap.GetWidth() != d_width || m_scaledBitmap.GetHeight() != d_height || m_scaledBitmap_offset_x != offset_x
        || m_scaledBitmap_offset_y != offset_y || m_bitmapChanged)
    {
      wxRect r = wxRect(offset_x, offset_y, b_width, b_height);
      m_scaledBitmap = wxBitmap(wxBitmap(m_bitmap).GetSubBitmap(r).ConvertToImage().Scale(d_width, d_height));
      m_scaledBitmap_offset_x = offset_x;
      m_scaledBitmap_offset_y = offset_y;
      m_bitmapChanged = false;
    }

    // Draw it:
    dc.DrawBitmap(m_scaledBitmap, dx0, dy0, true);
  }

  // Draw the name label
  if (m_showName && !m_name.IsEmpty())
  {
    wxCoord tx, ty;
    dc.GetTextExtent(m_name, &tx, &ty);

    if (HasBBox())
    {
      wxCoord sx = (wxCoord)((m_max_x - w.GetPosX()) * w.GetScaleX());
      wxCoord sy = (wxCoord)((w.GetPosY() - m_max_y) * w.GetScaleY());

      tx = sx - tx - 8;
      ty = sy - 8 - ty;
    }
    else
    {
      const int sx = w.GetScreenX() / 2;
      const int sy = w.GetScreenY() / 2;

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
// mpMagnet
//-----------------------------------------------------------------------------

void mpMagnet::Plot(wxClientDC &dc, const wxPoint &mousePos)
{
  if (m_domain.Contains(mousePos))
  {
    if ((m_mousePosition.x != mousePos.x) || (m_mousePosition.y != mousePos.y))
    {
      // Not draw the cross when we just right click
      if (m_rightClick)
      {
        m_rightClick = false;
      }
      else
      {
        m_mousePosition = mousePos;
        DrawCross(dc);
        m_IsDrawn = true;
      }
    }
  }
}

void mpMagnet::ClearPlot(wxClientDC &dc)
{
  if (m_IsDrawn)
  {
    DrawCross(dc);
    m_IsDrawn = false;
  }
  // In any cases
  m_IsWasDrawn = false;
}

void mpMagnet::UpdatePlot(wxClientDC &dc, const wxPoint &mousePos)
{
  if (m_IsWasDrawn)
  {
    // Mouse position has changed when pan operation
    if (m_rightClick)
      m_mousePosition = mousePos;
    DrawCross(dc);
    m_IsDrawn = true;
    m_IsWasDrawn = false;
  }
}

void mpMagnet::DrawCross(wxClientDC &dc) const
{
  // Note : wxINVERT not work on Linux GTK
  dc.SetPen(*wxBLACK_PEN);
  dc.SetLogicalFunction(wxINVERT);
  dc.DrawLine(m_mousePosition.x, m_plot_size.y, m_mousePosition.x, m_plot_size.height);
  dc.DrawLine(m_plot_size.x, m_mousePosition.y, m_plot_size.width, m_mousePosition.y);
  dc.SetLogicalFunction(wxCOPY);
}

//-----------------------------------------------------------------------------
// End of file
//-----------------------------------------------------------------------------
