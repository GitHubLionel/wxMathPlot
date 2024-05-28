/////////////////////////////////////////////////////////////////////////////
// Name:            mathplot.cpp
// Purpose:         Framework for plotting in wxWindows
// Original Author: David Schalig
// Maintainer:      Davide Rondini
// Contributors:    Jose Luis Blanco, Val Greene, Lionel Reynaud
// Created:         21/07/2003
// Last edit:       22/02/2009
// Last edit:       16/06/2023
// Copyright:       (c) David Schalig, Davide Rondini
// Licence:         wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MP_MATHPLOT_H_
#define _MP_MATHPLOT_H_

/** @file mathplot.h */
/** @mainpage wxMathPlot
 wxMathPlot is a framework for mathematical graph plotting in wxWindows.

 The framework is designed for convenience and ease of use.

 @section screenshots Screenshots
 <a href="http://wxmathplot.sourceforge.net/screenshot.shtml">Go to the screenshots page.</a>

 @section overview Overview
 The heart of wxMathPlot is mpWindow, which is a 2D canvas for plot layers.
 mpWindow can be embedded as subwindow in a wxPane, a wxFrame, or any other wxWindow.
 mpWindow provides a zoomable and moveable view of the layers. The current view can
 be controlled with the mouse, the scrollbars, and a context menu.

 Plot layers are implementations of the abstract base class mpLayer. Those can
 be function plots, scale rulers, or any other vector data visualisation. wxMathPlot provides two mpLayer
 implementations for plotting horizontal and vertical rulers: mpScaleX and mpScaleY.
 For convenient function plotting a series of classes derived from mpLayer are provided, like mpFX, mpProfile,
 mpLegend and so on. These base classes already come with plot code, user's own functions can be implemented
 by overriding just one member for retrieving a function value.

 mpWindow has built-in support for mouse-based pan and zoom through intuitive combinations of buttons and
 the mouse wheel. It also incorporates an optional double buffering mechanism to avoid flicker. Plots can be
 easily sent to printer evices or exported in bitmap formats like PNG, BMP or JPEG.

 @section coding Coding conventions
 wxMathPlot sticks to wxWindow's coding conventions. All entities defined by wxMathPlot have the prefix <i>mp</i>.

 @section author Author and license
 wxMathPlot is published under the terms of the wxWindow license.<br>
 The original author is David Schalig <mrhill@users.sourceforge.net>.<br>
 From June 2007 the project is maintained by Davide Rondini <cdron77@users.sourceforge.net>.<br>
 Authors can be contacted via the wxMathPlot's homepage at
 https://sourceforge.net/projects/wxmathplot<br>
 Contributors:<br>
 Jose Luis Blanco, Val Greene.<br>
 */

//this definition uses windows dll to export function.
//WXDLLIMPEXP_MATHPLOT definition definition changed to WXDLLIMPEXP_MATHPLOT
//mathplot_EXPORTS will be defined by cmake
#ifdef mathplot_EXPORTS
#define WXDLLIMPEXP_MATHPLOT WXEXPORT
#define WXDLLIMPEXP_DATA_MATHPLOT(type) WXEXPORT type
#else // not making DLL
#define WXDLLIMPEXP_MATHPLOT
#define WXDLLIMPEXP_DATA_MATHPLOT(type) type
#endif

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "mathplot.h"
#endif

#include <vector>

// #include <wx/wx.h>
#include <wx/defs.h>
#include <wx/menu.h>
#include <wx/scrolwin.h>
#include <wx/event.h>
#include <wx/dynarray.h>
#include <wx/pen.h>
#include <wx/dcmemory.h>
#include <wx/string.h>
#include <wx/print.h>
#include <wx/image.h>
#include <wx/intl.h>

#include <cmath>
#include <deque>

#include "MathPlotConfig.h"

/**
 * A simple plot component for wxWidgets
 */
namespace MathPlot
{

// For memory leak debug
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#else
#define DEBUG_NEW new
#endif // _DEBUG
#endif // _WINDOWS

// Separation for axes when set close to border
#define X_BORDER_SEPARATION 40
#define Y_BORDER_SEPARATION 60

/** When setting x labels in date/time format, convert input time to local time. */
#define mpX_LOCALTIME 0x10
/** When setting x labels in date/time format, convert input time to UTC time (or just leave it raw). */
#define mpX_UTCTIME 0x20
#define mpX_RAWTIME mpX_UTCTIME

// An epsilon for float comparison to 0
#define EPSILON   1e-8
#define ISNOTNULL(x)  (fabs(x) > EPSILON)

// A small extra margin for the plot boundary
#define EXTRA_MARGIN  8

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_MATHPLOT mpLayer;
class WXDLLIMPEXP_MATHPLOT mpFunction;
class WXDLLIMPEXP_MATHPLOT mpHorizontalLine;
class WXDLLIMPEXP_MATHPLOT mpVerticalLine;
class WXDLLIMPEXP_MATHPLOT mpFX;
class WXDLLIMPEXP_MATHPLOT mpFY;
class WXDLLIMPEXP_MATHPLOT mpFXY;
class WXDLLIMPEXP_MATHPLOT mpFXYVector;
class WXDLLIMPEXP_MATHPLOT mpProfile;
class WXDLLIMPEXP_MATHPLOT mpScale;
class WXDLLIMPEXP_MATHPLOT mpScaleX;
class WXDLLIMPEXP_MATHPLOT mpScaleY;
class WXDLLIMPEXP_MATHPLOT mpInfoLayer;
class WXDLLIMPEXP_MATHPLOT mpInfoCoords;
class WXDLLIMPEXP_MATHPLOT mpInfoLegend;
class WXDLLIMPEXP_MATHPLOT mpWindow;
class WXDLLIMPEXP_MATHPLOT mpText;
class WXDLLIMPEXP_MATHPLOT mpTitle;
class WXDLLIMPEXP_MATHPLOT mpPrintout;
class WXDLLIMPEXP_MATHPLOT mpMovableObject;
class WXDLLIMPEXP_MATHPLOT mpCovarianceEllipse;
class WXDLLIMPEXP_MATHPLOT mpPolygon;
class WXDLLIMPEXP_MATHPLOT mpBitmapLayer;

class MathPlotConfigDialog;

// a rectangle structure in several flavor
typedef union
{
    struct
    {
        wxCoord startPx;
        wxCoord endPx;
        wxCoord startPy;
        wxCoord endPy;
    };
    struct
    {
        wxCoord left;
        wxCoord top;
        wxCoord right;
        wxCoord bottom;
    };
    struct
    {
        wxCoord x1;
        wxCoord y1;
        wxCoord x2;
        wxCoord y2;
    };
    wxCoord tab[4];
} mpRect;

typedef union
{
    struct
    {
        double Xmin;
        double Xmax;
        double Ymin;
        double Ymax;
        double Y2min;
        double Y2max;
    };
    double tab[6];
} mpFloatRect;

/** Command IDs used by mpWindow
 * Same order for the popup menu
 */
enum
{
  mpID_FIT = 2000,         //!< Fit view to match bounding box of all layers
  mpID_ZOOM_IN,            //!< Zoom into view at clickposition / window center
  mpID_ZOOM_OUT,           //!< Zoom out
  mpID_CENTER,             //!< Center view on click position
  mpID_LOCKASPECT,         //!< Lock x/y scaling aspect
  mpID_TOGGLE_GRID,        //!< Show/Hide grids
  mpID_TOGGLE_COORD,       //!< Show/Hide info coord
  mpID_SCREENSHOT,         //!< Copy a screen shot to the clipboard
  mpID_CONFIG,             //!< Configuration
  mpID_LOAD_FILE,          //!< Load a file
  mpID_HELP_MOUSE,         //!< Shows information about the mouse commands
  mpID_FULLSCREEN          //!< Toggle fullscren only if parent is a frame windows
};

// Location for the Info layer
typedef enum __mp_Location_Type
{
  mpMarginLeftCenter,
  mpMarginTopLeft,
  mpMarginTopCenter,
  mpMarginTopRight,
  mpMarginRightCenter,
  mpMarginBottomLeft,
  mpMarginBottomCenter,
  mpMarginBottomRight,
  mpMarginNone,
  mpCursor // only for mpInfoCoords
} mpLocation;

// Alignment for X axis
typedef enum __XAxis_Align_Type
{
  mpALIGN_BORDER_BOTTOM = 10,
  mpALIGN_BOTTOM,
  mpALIGN_CENTERX,
  mpALIGN_TOP,
  mpALIGN_BORDER_TOP
} mpXAxis_Align;

// Alignment for Y axis
typedef enum __YAxis_Align_Type
{
  mpALIGN_BORDER_LEFT = 20,
  mpALIGN_LEFT,
  mpALIGN_CENTERY,
  mpALIGN_RIGHT,
  mpALIGN_BORDER_RIGHT
} mpYAxis_Align;

typedef enum __Plot_Align_Name_Type
{
  mpALIGN_NW = 5,
  mpALIGN_NE,
  mpALIGN_SE,
  mpALIGN_SW
} mpPlot_Align;

// Style for the Legend layer
/** Show legend items with line with the same pen of referred mpLayer  */
/** Show legend items with small square with the same color of referred mpLayer  */
typedef enum __mp_Style_Type
{
  mpLegendLine,
  mpLegendSquare
} mpLegendStyle;

// Direction for the Legend layer
typedef enum __mp_Direction_Type
{
  mpVertical,
  mpHorizontal
} mpLegendDirection;

typedef enum __Symbol_Type
{
  mpsNone,
  mpsCircle,
  mpsSquare,
  mpsUpTriangle,
  mpsDownTriangle,
  mpsCross,
  mpsPlus
} mpSymbol;

typedef enum __Info_Type
{
  mpiNone,
  mpiInfo,
  mpiCoords,
  mpiLegend
} mpInfoType;

typedef enum __Function_Type
{
  mpfNone,
  mpfFX,
  mpfFY,
  mpfFXY,
  mpfFXYVector,
  mpfBar,
  mpfMovable,
  mpfAllType
} mpFunctionType;

typedef enum __Scale_Type
{
  mpsScaleNone,
  mpsScaleX,
  mpsScaleY,
  mpsAllType
} mpScaleType;

//-----------------------------------------------------------------------------
// mpLayer
//-----------------------------------------------------------------------------

typedef enum __mp_Layer_Type
{
  mpLAYER_UNDEF,   //!< Layer type undefined
  mpLAYER_AXIS,    //!< Axis type layer
  mpLAYER_PLOT,    //!< Plot type layer
  mpLAYER_INFO,    //!< Info box type layer
  mpLAYER_BITMAP,  //!< Bitmap type layer
  mpLAYER_CUSTOM   //!< Custom type layer
} mpLayerType;

/**
 * Z order for drawing layer
 * Background is the deeper (bitmap layer)
 * Then draw axis, custom layer, function, info (info, coord, legend, ...) and finally text (text, title)
 */
typedef enum __mp_Layer_ZOrder
{
  mpZIndex_BACKGROUND,   //!< Bitmap type layer
  mpZIndex_AXIS,         //!< Axis type layer
  mpZIndex_CUSTOM,       //!< Custom type layer
  mpZIndex_PLOT,         //!< Plot (function) type layer
  mpZIndex_INFO,         //!< Info box type layer
  mpZIndex_TEXT,         //!< Text box type layer
  mpZIndex_END           //!< Just the end of ZOrder
} mpLayerZOrder;

/** Plot layer, abstract base class.
 Any number of mpLayer implementations can be attached to mpWindow.
 Examples for mpLayer implementations are function graphs, or scale rulers.

 For convenience mpLayer defines a name, a font (wxFont), a pen (wxPen),
 and a continuity property (bool) as class members.
 The default values at constructor are the default font, a black pen, and
 continuity set to false (draw separate points).
 These may or may not be used by implementations.
 */
class WXDLLIMPEXP_MATHPLOT mpLayer: public wxObject
{
  public:
    mpLayer();

    virtual ~mpLayer()
    {
      ;
    }

    /** Set the wxWindow handle
     * Usefull to acces the method of mpWindow
     */
    void SetWindow(mpWindow &w)
    {
      m_win = &w;
    }

    /** Check whether this layer has a bounding box.
     The default implementation returns \a TRUE. Override and return
     FALSE if your mpLayer implementation should be ignored by the calculation
     of the global bounding box for all layers in a mpWindow.
     @retval TRUE Has bounding box
     @retval FALSE Has not bounding box
     */
    virtual bool HasBBox()
    {
      return true;
    }

    /**
     * Return the bounding box, ie minX, maxX, minY and maxY of the object
     * Here we don't use extra Y2 values
     */
    virtual void GetBBox(mpFloatRect *m_bound);

    /** Check whether the layer is a special object.
     The default implementation returns \a FALSE. It is overrided to \a TRUE in object
     class and its derivative.
     @return whether the layer is a specific object */
    virtual bool IsInfo(mpInfoType *info)
    {
      *info = mpiNone;
      return false;
    }

    virtual bool IsFunction(mpFunctionType *function)
    {
      *function = mpfNone;
      return false;
    }

    virtual bool IsScale(mpScaleType *scale)
    {
      *scale = mpsScaleNone;
      return false;
    }

    virtual bool IsText()
    {
      return false;
    }

    virtual bool IsTitle()
    {
      return false;
    }

    /** Get inclusive left border of bounding box.
     @return Value
     */
    virtual double GetMinX()
    {
      return -1.0;
    }

    /** Get inclusive right border of bounding box.
     @return Value
     */
    virtual double GetMaxX()
    {
      return 1.0;
    }

    /** Get inclusive bottom border of bounding box.
     @return Value
     */
    virtual double GetMinY()
    {
      return -1.0;
    }

    /** Get inclusive top border of bounding box.
     @return Value
     */
    virtual double GetMaxY()
    {
      return 1.0;
    }

    /** Plot given view of layer to the given device context.
     An implementation of this function has to transform layer coordinates to
     wxDC coordinates based on the view parameters retrievable from the mpWindow
     passed in \a w.
     Note that the public methods of mpWindow: x2p,y2p and p2x,p2y are already provided
     which transform layer coordinates to DC pixel coordinates, and <b>user code should rely
     on them</b> for portability and future changes to be applied transparently, instead of
     implementing the following formulas manually.

     The passed device context \a dc has its coordinate origin set to the top-left corner
     of the visible area (the default). The coordinate orientation is as shown in the
     following picture:
     <pre>
     (wxDC origin 0,0)
     x-------------> ascending X ----------------+
     |                                           |
     |                                           |
     V ascending Y                               |
     |                                           |
     |                                           |
     |                                           |
     +-------------------------------------------+  <-- right-bottom corner of the mpWindow visible area.
     </pre>
     Note that Y ascends in downward direction, whereas the usual vertical orientation
     for mathematical plots is vice versa. Thus Y-orientation will be swapped usually,
     when transforming between wxDC and mpLayer coordinates. This change of coordinates
     is taken into account in the methods p2x, p2y, x2p, y2p.

     <b> Rules for transformation between mpLayer and wxDC coordinates </b>
     @code
     dc_X = (layer_X - mpWindow::GetPosX()) * mpWindow::GetScaleX()
     dc_Y = (mpWindow::GetPosY() - layer_Y) * mpWindow::GetScaleY() // swapping Y-orientation

     layer_X = (dc_X / mpWindow::GetScaleX()) + mpWindow::GetPosX() // scale guaranteed to be not 0
     layer_Y = mpWindow::GetPosY() - (dc_Y / mpWindow::GetScaleY()) // swapping Y-orientation
     @endcode

     @param dc Device context to plot to.
     @param w  View to plot. The visible area can be retrieved from this object.
     @sa mpWindow::p2x, mpWindow::p2y, mpWindow::x2p, mpWindow::y2p
     */
    void Plot(wxDC &dc, mpWindow &w);

    virtual void DoPlot(wxDC &dc, mpWindow &w) = 0;

    /**
     * If we need to do something before plot like reinitialize some parameters ...
     */
    virtual void DoBeforePlot()
    {
      ;
    }

    /** Set layer name
     @param name Name, will be copied to internal class member
     */
    void SetName(const wxString &name)
    {
      m_name = name;
    }

    /** Get layer name.
     @return Name
     */
    const wxString& GetName() const
    {
      return m_name;
    }

    /** Set layer font
     @param font Font, will be copied to internal class member
     */
    void SetFont(const wxFont &font)
    {
      m_font = font;
    }

    /** Get font set for this layer.
     @return Font
     */
    const wxFont& GetFont() const
    {
      return m_font;
    }

    /** Set layer font foreground colour
     @param colour
     */
    void SetFontColour(const wxColour &colour)
    {
      m_fontcolour = colour;
    }

    /** Get font foreground colour set for this layer.
     @return colour
     */
    const wxColour& GetFontColour() const
    {
      return m_fontcolour;
    }

    /** Set layer pen
     @param pen Pen, will be copied to internal class member
     */
    void SetPen(const wxPen &pen)
    {
      m_pen = pen;
    }

    /** Get pen set for this layer.
     @return Pen
     */
    const wxPen& GetPen() const
    {
      return m_pen;
    }

    /** Set layer brush
     @param brush brush, will be copied to internal class member  */
    void SetBrush(const wxBrush &brush)
    {
      if (brush == wxNullBrush)
        m_brush = *wxTRANSPARENT_BRUSH;
      else
        m_brush = brush;
    }

    /** Get brush set for this layer.
     @return brush. */
    const wxBrush& GetBrush() const
    {
      return m_brush;
    }

    /** Set Name visibility.
     @param show visibility bool. */
    void SetShowName(bool show)
    {
      m_showName = show;
    }

    /** Get Name visibility.
     @return \a true if visible */
    inline bool GetShowName()
    {
      return m_showName;
    }

    /** Set Draw mode: inside or outside margins. Default is outside (TRUE), which allows the layer to draw up to the mpWindow border.
     @param drawModeOutside The draw mode to be set */
    void SetDrawOutsideMargins(bool drawModeOutside)
    {
      m_drawOutsideMargins = drawModeOutside;
    }

    /** Get Draw mode: inside or outside margins.
     @return The draw mode */
    bool GetDrawOutsideMargins() const
    {
      return m_drawOutsideMargins;
    }

    /** Get a small square bitmap filled with the colour of the pen used in the layer.
     Useful to create legends or similar reference to the layers.
     @param side side length in pixels
     @return a wxBitmap filled with layer's colour */
    wxBitmap GetColourSquare(int side = 16);

    /** Get layer type: a Layer can be of different types: plot lines, axis, info boxes, etc,
     this method returns the right value.
     @return An integer indicating layer type */
    mpLayerType GetLayerType() const
    {
      return m_type;
    }

    /** Checks whether the layer is visible or not.
     @return \a true if visible */
    inline bool IsVisible()
    {
      return m_visible;
    }

    /** Sets layer visibility.
     @param show visibility bool. */
    virtual void SetVisible(bool show)
    {
      m_visible = show;
    }

    /** Checks whether the layer is tractable or not.
     @return \a true if visible */
    inline bool IsTractable()
    {
      return m_tractable;
    }

    /** Sets layer tractability.
     @param track tractability bool. */
    virtual void SetTractable(bool track)
    {
      m_tractable = track;
    }

    /** Set X/Y alignment.
     @param align alignment */
    void SetAlign(int align)
    {
      m_flags = align;
    }

    /** Get X/Y alignment.
     @return align alignment */
    int GetAlign() const
    {
      return m_flags;
    }

    /** Set CanDelete for plot.
     @param CanDelete */
    void SetCanDelete(bool canDelete)
    {
      m_CanDelete = canDelete;
    }

    /** Get CanDelete for plot.
     @return CanDelete*/
    bool GetCanDelete(void) const
    {
      return m_CanDelete;
    }

    /** Get the ZIndex of the plot.
     @return m_ZIndex*/
    mpLayerZOrder GetZIndex(void)
    {
      return m_ZIndex;
    }

  protected:
    mpWindow* m_win;            //!< The wxWindow handle
    mpLayerType m_type;         //!< Define layer type, which is assigned by constructor
    wxFont m_font;              //!< Layer's font
    wxColour m_fontcolour;      //!< Layer's font foreground colour
    wxPen m_pen;                //!< Layer's pen. Default Colour = Black, width = 1, style = wxPENSTYLE_SOLID
    wxBrush m_brush;            //!< Layer's brush. Default wxTRANSPARENT_BRUSH
    wxString m_name;            //!< Layer's name
    bool m_showName;            //!< States whether the name of the layer must be shown. Default : false
    bool m_drawOutsideMargins;  //!< Select if the layer should draw only inside margins or over all DC. Default : false
    bool m_visible;             //!< Toggles layer visibility. Default : true
    bool m_tractable;           //!< Is the layer tractable
    int m_flags;                //!< Holds label alignment. Default : mpALIGN_NE
    mpRect m_plotBondaries;     //!< The bondaries for plotting curve calculated by mpWindow
    bool m_CanDelete;           //!< Is the layer can be deleted
    mpLayerZOrder m_ZIndex;     //!< The index in Z-Order to draw the layer

    /** Initialize the context
     */
    void UpdateContext(wxDC &dc);

  private:
    bool m_busy;                //!< Test if we are busy (plot operation)

  DECLARE_DYNAMIC_CLASS(mpLayer)
};

//-----------------------------------------------------------------------------
// mpInfoLayer
//-----------------------------------------------------------------------------

/** @class mpInfoLayer
 @brief Base class to create small rectangular info boxes
 mpInfoLayer is the base class to create a small rectangular info box in transparent overlay over plot layers.
 It is used to implement objects like legends.
 */
class WXDLLIMPEXP_MATHPLOT mpInfoLayer: public mpLayer
{
  public:
    /** Default constructor. */
    mpInfoLayer();

    /** Complete constructor.
     @param rect Sets the initial size rectangle of the layer.
     @param brush pointer to a fill brush. Default is transparent
     @param location to place in the margin or free */
    mpInfoLayer(wxRect rect, const wxBrush &brush = *wxTRANSPARENT_BRUSH, mpLocation location = mpMarginNone);

    /** Destructor */
    virtual ~mpInfoLayer();

    /** Updates the content of the info box. Should be overidden by derived classes.
     Update may behave in different ways according to the type of event which called it.
     @param w parent mpWindow from which to obtain informations
     @param event The event which called the update. */
    virtual void UpdateInfo(mpWindow &w, wxEvent &event);

    /** mpInfoLayer has not bounding box. @sa mpLayer::HasBBox
     @return always \a FALSE */
    virtual bool HasBBox()
    {
      return false;
    }

    /** Plot method. Can be overidden by derived classes.
     @param dc the device content where to plot
     @param w the window to plot
     @sa mpLayer::Plot */
    virtual void DoPlot(wxDC &dc, mpWindow &w);

    /** Specifies that this is an Info box layer.
     @return always \a TRUE
     @sa mpLayer::IsInfo */
    virtual bool IsInfo(mpInfoType *info)
    {
      *info = mpiInfo;
      return true;
    }

    /** Checks whether a point is inside the info box rectangle.
     @param point The point to be checked
     @return \a true if the point is inside the bounding box */
    virtual bool Inside(const wxPoint &point);

    /** Moves the layer rectangle of given pixel deltas.
     @param delta The wxPoint container for delta coordinates along x and y. Units are in pixels. */
    virtual void Move(wxPoint delta);

    /** Updates the rectangle reference point. Used by internal methods of mpWindow to correctly move mpInfoLayers. */
    virtual void UpdateReference();

    /** Returns the position of the upper left corner of the box (in pixels)
     @return The rectangle position */
    wxPoint GetPosition();

    /** Returns the size of the box (in pixels)
     @return The rectangle size */
    wxSize GetSize();

    /** Returns the current rectangle coordinates.
     @return The info layer rectangle */
    const wxRect& GetRectangle() const
    {
      return m_dim;
    }

    /** Set the location of the box
     */
    void SetLocation(mpLocation location)
    {
      m_location = location;
    }

    /** Returns the location of the box
     @return location */
    mpLocation GetLocation() const
    {
      return m_location;
    }

  protected:
    wxRect m_dim;           //!< The bounding rectangle of the box. It may be resized dynamically by the Plot method.
    wxPoint m_reference;    //!< Holds the reference point for movements
    int m_winX, m_winY;     //!< Holds the mpWindow size. Used to rescale position when window is resized.
    mpLocation m_location;  //!< Location of the box in the margin. Default mpMarginNone = use coordinates

    /** Compute the dimensions and position of the rectangle info
     */
    void SetInfoRectangle(mpWindow &w, int width = 0, int height = 0);

  DECLARE_DYNAMIC_CLASS(mpInfoLayer)
};

/** @class mpInfoCoords
 @brief Implements an overlay box which shows the mouse coordinates in plot units.
 When an mpInfoCoords layer is activated, when mouse is moved over the mpWindow,
 its coordinates (in mpWindow units, not pixels) are continuously reported inside the layer box. */
class WXDLLIMPEXP_MATHPLOT mpInfoCoords: public mpInfoLayer
{
  public:
    /** Default constructor */
    mpInfoCoords();

    /** Limited constructor */
    mpInfoCoords(mpLocation location);

    /** Complete constructor, setting initial rectangle and background brush.
     @param rect The initial bounding rectangle.
     @param brush The wxBrush to be used for box background: default is transparent
     @param location to place in the margin or free */
    mpInfoCoords(wxRect rect, const wxBrush &brush = *wxTRANSPARENT_BRUSH, mpLocation location = mpMarginNone);

    /** Default destructor */
    ~mpInfoCoords();

    virtual void SetVisible(bool show);

    /** Updates the content of the info box. It is used to update coordinates.
     @param w parent mpWindow from which to obtain information
     @param event The event which called the update. */
    virtual void UpdateInfo(mpWindow &w, wxEvent &event);

    /** Plot method.
     @param dc the device content where to plot
     @param w the window to plot
     @sa mpLayer::Plot */
    virtual void DoPlot(wxDC &dc, mpWindow &w);

    void ErasePlot(wxDC &dc, mpWindow &w);

    /** Set X axis label view mode.
     @param mode mpX_NORMAL for normal labels, mpX_TIME for time axis in hours, minutes, seconds. */
    void SetLabelMode(unsigned int mode, unsigned int time_conv = mpX_RAWTIME)
    {
      m_labelType = mode;
      m_timeConv = time_conv;
    }

    /** Specifies that this is an InfoCoords box layer.
     @return always \a TRUE
     @sa mpLayer::IsInfoCoords */
    virtual bool IsInfo(mpInfoType *info)
    {
      *info = mpiCoords;
      return true;
    }

    /** Set the series coordinates of the mouse position (if tractable set)
     */
    void SetSeriesCoord(bool show)
    {
      m_series_coord = show;
    }

    /** Return if we show the series coordinates
     @return bool */
    bool IsSeriesCoord() const
    {
      return m_series_coord;
    }

    /** Pen series for tractable
     */
    void SetPenSeries(const wxPen &pen)
    {
      m_penSeries = pen;
    }

  protected:
    wxString m_content;  //!< string holding the coordinates to be drawn.
    unsigned int m_labelType;
    unsigned int m_timeConv;
    wxCoord m_mouseX;
    wxCoord m_mouseY;
    wxBitmap* m_coord_bmp;
    wxRect m_oldDim;
    bool m_series_coord;
    wxPen m_penSeries;

  DECLARE_DYNAMIC_CLASS(mpInfoCoords)
};

/** @class mpInfoLegend
 @brief Implements the legend to be added to the plot
 This layer allows you to add a legend to describe the plots in the window. The legend uses
 the layer name as a label, and displays only layers of type mpLAYER_PLOT. */
class WXDLLIMPEXP_MATHPLOT mpInfoLegend: public mpInfoLayer
{
  public:
    /** Default constructor */
    mpInfoLegend();

    /** Complete constructor, setting initial rectangle and background brush.
     @param rect The initial bounding rectangle.
     @param brush The wxBrush to be used for box background: default is transparent
     @param location to place in the margin or free
     @sa mpInfoLayer::mpInfoLayer */
    mpInfoLegend(wxRect rect, const wxBrush &brush = *wxWHITE_BRUSH, mpLocation location = mpMarginNone);

    /**  Default destructor */
    ~mpInfoLegend();

    /** Plot method.
     @param dc the device content where to plot
     @param w the window to plot
     @sa mpLayer::Plot */
    virtual void DoPlot(wxDC &dc, mpWindow &w);

    /** Swith item mode, which is the element on the left of text representing the plot line.
     * @param mode The item draw mode: mpLEGEND_LINE or mpLEGEND_SQUARE. */
    void SetItemMode(mpLegendStyle mode)
    {
      m_item_mode = mode;
      m_need_update = true;
    }

    mpLegendStyle GetItemMode() const
    {
      return m_item_mode;
    }

    void SetItemDirection(mpLegendDirection mode)
    {
      m_item_direction = mode;
      m_need_update = true;
    }

    mpLegendDirection GetItemDirection() const
    {
      return m_item_direction;
    }

    void SetNeedUpdate()
    {
      m_need_update = true;
    }

    /** Specifies that this is an InfoLegend box layer.
     @return always \a TRUE
     @sa mpLayer::IsInfoLegend */
    virtual bool IsInfo(mpInfoType *info)
    {
      *info = mpiLegend;
      return true;
    }

  protected:
    wxBitmap* m_legend_bmp;
    mpLegendStyle m_item_mode;
    mpLegendDirection m_item_direction;

  private:
    bool m_need_update;
    void UpdateBitmap(wxDC &dc, mpWindow &w);

  DECLARE_DYNAMIC_CLASS(mpInfoLegend)
};

//-----------------------------------------------------------------------------
// mpLayer implementations - functions
//-----------------------------------------------------------------------------

/*@}*/

/** @name mpLayer implementations - functions
 @{*/

/** Plot layer implementing an abstract function class.
 */
class WXDLLIMPEXP_MATHPLOT mpFunction: public mpLayer
{
  public:
    /** Full constructor.
     */
    mpFunction(const wxString &name = wxEmptyString, bool useY2Axis = false);

    /** Set the 'continuity' property of the layer (true: draws a continuous line, false: draws separate points (default)).
     * @sa GetContinuity
     */
    void SetContinuity(bool continuity)
    {
      m_continuous = continuity;
    }

    /** Gets the 'continuity' property of the layer.
     * @sa SetContinuity
     */
    bool GetContinuity() const
    {
      return m_continuous;
    }

    /** Set step for plot.
     @param step */
    void SetStep(unsigned int step)
    {
      m_step = step;
    }

    /** Get step for plot.
     @return step*/
    unsigned int GetStep() const
    {
      return m_step;
    }

    /** Set symbol.
     @param symbol (choose between mps...) */
    void SetSymbol(mpSymbol symbol)
    {
      m_symbol = symbol;
    }

    /** Get symbol.
     @return symbol */
    mpSymbol GetSymbol() const
    {
      return m_symbol;
    }

    /** Set symbol size.
     @param size  */
    void SetSymbolSize(int size)
    {
      m_symbolSize = size;
      m_symbolSize2 = size / 2;
    }

    /** Get symbol size.
     @return size  */
    int GetSymbolSize() const
    {
      return m_symbolSize;
    }

    /** Draw a symbol in place of point.
     @return void
     */
    virtual void DrawSymbol(wxDC &dc, wxCoord x, wxCoord y);

    /** Set use of second Y axis
     @param _useY2 for the scaling
     */
    void SetY2Axis(bool _useY2)
    {
      m_UseY2Axis = _useY2;
    }

    /** Get use of second Y axis
     @return UseY2Axis
     */
    bool GetY2Axis() const
    {
      return m_UseY2Axis;
    }

  protected:
    bool m_continuous;          //!< Specify if the layer will be plotted as a continuous line or a set of points. Default false
    mpSymbol m_symbol;          //!< A symbol for the plot in place of point. Default mpNone
    int m_symbolSize;           //!< Size of the symbol. Default 6
    int m_symbolSize2;          //!< Size of the symbol div 2.
    unsigned int m_step;        //!< Step to get point to be draw. Default : 1
    /**
     * Use Y2 axis
     * This second axis must exist
     */
    bool m_UseY2Axis;

  DECLARE_DYNAMIC_CLASS(mpFunction)
};

/** Abstract class providing an horizontal line.
 */
class WXDLLIMPEXP_MATHPLOT mpHorizontalLine: public mpFunction
{
  public:
    mpHorizontalLine(double yvalue, const wxPen &pen = *wxGREEN_PEN, bool useY2Axis = false);

    // We don't want to include horizontal line in BBox computation
    virtual bool HasBBox() override
    {
      return false;
    }

    virtual void DoPlot(wxDC &dc, mpWindow &w);

    /** Set y
     @param yvalue
     */
    void SetYValue(double yvalue)
    {
      m_yvalue = yvalue;
    }

  protected:
    double m_yvalue;

    DECLARE_DYNAMIC_CLASS(mpHorizontalLine)
};

/** Abstract class providing an vertical line.
 */
class WXDLLIMPEXP_MATHPLOT mpVerticalLine: public mpFunction
{
  public:
    mpVerticalLine(double xvalue, const wxPen &pen = *wxGREEN_PEN);

    // We don't want to include vertical line in BBox computation
    virtual bool HasBBox() override
    {
      return false;
    }

    virtual void DoPlot(wxDC &dc, mpWindow &w);

    /** Set x
     @param xvalue
     */
    void SetXValue(double xvalue)
    {
      m_xvalue = xvalue;
    }

  protected:
    double m_xvalue;

    DECLARE_DYNAMIC_CLASS(mpVerticalLine)
};

/** Abstract base class providing plot and labeling functionality for functions F:X->Y.
 Override mpFX::GetY to implement a function.
 Override mpFX::GetMinY and mpFX::GetMaxY to provide min and max Y range
 Optionally implement a constructor and pass a name (label) and a label alignment
 to the constructor mpFX::mpFX. If the layer name is empty, no label will be plotted.
 */
class WXDLLIMPEXP_MATHPLOT mpFX: public mpFunction
{
  public:
    /** @param name  Label
     @param flags Label alignment, pass one of #mpALIGN_RIGHT, #mpALIGN_CENTER, #mpALIGN_LEFT.
     */
    mpFX(const wxString &name = wxEmptyString, int flags = mpALIGN_RIGHT, bool useY2Axis = false);

    /** Get function value for argument.
     Override this function in your implementation.
     @param x Argument
     @return Function value
     */
    virtual double GetY(double x) = 0;

    /**
     * Get function value with log test
     */
    double DoGetY(double x);

    /** Layer plot handler.
     This implementation will plot the function in the visible area and
     put a label according to the aligment specified.
     */
    virtual void DoPlot(wxDC &dc, mpWindow &w);

    /** Specifies that this is a FX layer.
     @return always \a TRUE
     @sa mpLayer::IsFonction*/
    virtual bool IsFunction(mpFunctionType *function)
    {
      *function = mpfFX;
      return true;
    }

  protected:

  DECLARE_DYNAMIC_CLASS(mpFX)
};

/** Abstract base class providing plot and labeling functionality for functions F:Y->X.
 Override mpFY::GetX to implement a function.
 Override mpFY::GetMinX and mpFY::GetMaxX to provide min and max X range
 Optionally implement a constructor and pass a name (label) and a label alignment
 to the constructor mpFY::mpFY. If the layer name is empty, no label will be plotted.
 */
class WXDLLIMPEXP_MATHPLOT mpFY: public mpFunction
{
  public:
    /** @param name  Label
     @param flags Label alignment, pass one of #mpALIGN_BOTTOM, #mpALIGN_CENTER, #mpALIGN_TOP.
     */
    mpFY(const wxString &name = wxEmptyString, int flags = mpALIGN_TOP, bool useY2Axis = false);

    /** Get function value for argument.
     Override this function in your implementation.
     @param y Argument
     @return Function value
     */
    virtual double GetX(double y) = 0;

    /**
     * Get function value with log test
     */
    double DoGetX(double y);

    /** Layer plot handler.
     This implementation will plot the function in the visible area and
     put a label according to the aligment specified.
     */
    virtual void DoPlot(wxDC &dc, mpWindow &w);

    /** Specifies that this is a FY layer.
     @return always \a TRUE
     @sa mpLayer::IsFunction */
    virtual bool IsFunction(mpFunctionType *function)
    {
      *function = mpfFY;
      return true;
    }

  protected:

  DECLARE_DYNAMIC_CLASS(mpFY)
};

/** Abstract base class providing plot and labeling functionality for a locus plot F:N->X,Y.
 Locus argument N is assumed to be in range 0 .. MAX_N, and implicitly derived by enumerating
 all locus values.
 Override mpFXY::Rewind and mpFXY::GetNextXY to implement a locus.
 Override mpFXY::GetMinX and mpFXY::GetMaxX to provide min and max X range
 Override mpFXY::GetMinY and mpFXY::GetMaxY to provide min and max Y range
 Optionally implement a constructor and pass a name (label) and a label alignment
 to the constructor mpFXY::mpFXY. If the layer name is empty, no label will be plotted.
 */
class WXDLLIMPEXP_MATHPLOT mpFXY: public mpFunction
{
  public:
    /** @param name  Label
     @param flags Label alignment, pass one of #mpALIGN_NE, #mpALIGN_NW, #mpALIGN_SW, #mpALIGN_SE.
     */
    mpFXY(const wxString &name = wxEmptyString, int flags = mpALIGN_NE, bool viewAsBar = false, bool useY2Axis = false);

    /** Rewind value enumeration with mpFXY::GetNextXY.
     Override this function in your implementation.
     */
    virtual void Rewind() = 0;

    /** Clears all the data, leaving the layer empty.
     * @sa SetData
     */
    virtual void Clear()
    {
      // Nothing to do here
    }

    /** Get locus value for next N.
     Override this function in your implementation.
     @param x Returns X value
     @param y Returns Y value
     */
    virtual bool GetNextXY(double *x, double *y) = 0;

    /**
     * Get function value with log test
     */
    bool DoGetNextXY(double *x, double *y);

    /** Layer plot handler.
     This implementation will plot the locus in the visible area and
     put a label according to the alignment specified.
     */
    virtual void DoPlot(wxDC &dc, mpWindow &w);

    /** Specifies that this is a FXY layer.
     @return always \a TRUE
     @sa mpLayer::IsFunction */
    virtual bool IsFunction(mpFunctionType *function)
    {
      *function = mpfFXY;
      return true;
    }

    /**
     * If true, XY series is plotted as bar
     */
    void SetViewMode(bool asBar);

    /**
     * return the width of the bar
     */
    int GetBarWidth(void) const
    {
      return m_BarWidth;
    }

  protected:

    // Data to calculate label positioning
    wxCoord maxDrawX, minDrawX, maxDrawY, minDrawY;

    // Min delta between 2 x coordinate (used for view as bar)
    double m_deltaX, m_deltaY;

    // The width of a bar
    int m_BarWidth;

    // Plot data as bar graph
    bool m_ViewAsBar = false;

    // Is the serie can be deleted
    bool m_CanDelete = true;

    /** Update label positioning data
     @param xnew New x coordinate
     @param ynew New y coordinate
     */
    void UpdateViewBoundary(wxCoord xnew, wxCoord ynew);

  DECLARE_DYNAMIC_CLASS(mpFXY)
};

//-----------------------------------------------------------------------------
// mpFXYVector - provided by Jose Luis Blanco
//-----------------------------------------------------------------------------

/** A class providing graphs functionality for a 2D plot (either continuous or a set of points), from vectors of data.
 This class can be used directly, the user does not need to derive any new class. Simply pass the data as two vectors
 with the same length containing the X and Y coordinates to the method SetData.

 To generate a graph with a set of points, call
 \code
 layerVar->SetContinuity(false)
 \endcode

 or

 \code
 layerVar->SetContinuity(true)
 \endcode

 to render the sequence of coordinates as a continuous line.

 (Added: Jose Luis Blanco, AGO-2007)
 */
class WXDLLIMPEXP_MATHPLOT mpFXYVector: public mpFXY
{
  public:
    /** @param name  Label
     @param flags Label alignment, pass one of #mpALIGN_NE, #mpALIGN_NW, #mpALIGN_SW, #mpALIGN_SE.
     */
    mpFXYVector(const wxString &name = wxEmptyString, int flags = mpALIGN_NE, bool viewAsBar = false, bool useY2Axis = false);

    /** destrutor
     */
    virtual ~mpFXYVector()
    {
      Clear();
    }

    /** Changes the internal data: the set of points to draw.
     Both vectors MUST be of the same length. This method DOES NOT refresh the mpWindow; do it manually.
     * @sa Clear
     */
    void SetData(const std::vector<double> &xs, const std::vector<double> &ys);

    /** Clears all the data, leaving the layer empty.
     * @sa SetData
     */
    void Clear();

    /** Add data to the internal vector. This method DOES NOT refresh the mpWindow unless updatePlot = true
     * and the added point is in bound; do it manually by calling UpdateAll() or just Fit() if we want to adjust plot
     * @param x
     * @param y
     * @param updatePlot. boolean, set true to update plot. This speed the rendering because just new point is drawing.
     * @return true if limits are changed (and may some refresh)
     */
    bool AddData(const double x, const double y, bool updatePlot);

    /** Set memory reserved for m_xs and m_ys
     * Note :
     * this does not modify the size of m_xs and m_ys, this is not a resize. Call Clear() to resize to 0.
     * if capacity is already superior at reserve, do nothing
     */
    void SetReserve(int reserve)
    {
      m_reserveXY = reserve;
      m_xs.reserve(m_reserveXY);
      m_ys.reserve(m_reserveXY);
    }

    /** Get memory reserved for m_xs and m_ys
     */
    int GetReserve() const
    {
      return m_reserveXY;
    }

    /** Specifies that this is a FXYVector layer.
     @return always \a TRUE
     @sa mpLayer::IsFunction */
    virtual bool IsFunction(mpFunctionType *function)
    {
      if (m_ViewAsBar)
        *function = mpfBar;
      else
        *function = mpfFXYVector;
      return true;
    }

  protected:
    /** The internal copy of the set of data to draw.
     */
    std::vector<double> m_xs, m_ys;

    /** Memory reserved for m_xs and m_ys. Default 1000
     */
    int m_reserveXY;

    /** The internal counter for the "GetNextXY" interface
     */
    size_t m_index;

    /** Loaded at SetData
     */
    double m_minX, m_maxX, m_minY, m_maxY, m_lastX, m_lastY;

    /** Rewind value enumeration with mpFXY::GetNextXY.
     Overridden in this implementation.
     */
    inline void Rewind()
    {
      m_index = 0;
    }

    /** Get locus value for next N.
     Overridden in this implementation.
     @param x Returns X value
     @param y Returns Y value
     */
    virtual bool GetNextXY(double *x, double *y);

    /** Draw the point added if there is in bound
     */
    void DrawAddedPoint(double x, double y);

    /** Returns the actual minimum X data (loaded in SetData).
     */
    virtual double GetMinX()
    {
      return m_minX;
    }

    /** Returns the actual minimum Y data (loaded in SetData).
     */
    virtual double GetMinY()
    {
      return m_minY;
    }

    /** Returns the actual maximum X data (loaded in SetData).
     */
    virtual double GetMaxX()
    {
      return m_maxX;
    }

    /** Returns the actual maximum Y data (loaded in SetData).
     */
    virtual double GetMaxY()
    {
      return m_maxY;
    }

  DECLARE_DYNAMIC_CLASS(mpFXYVector)
};

/** Abstract base class providing plot and labeling functionality for functions F:Y->X.
 Override mpProfile::GetX to implement a function.
 This class is similar to mpFY, but the Plot method is different. The plot is in fact represented by lines
 instead of points, which gives best rendering of rapidly-varying functions, and in general,
 data which are not so close one to another.
 Optionally implement a constructor and pass a name (label) and a label alignment
 to the constructor mpProfile::mpProfile. If the layer name is empty, no label will be plotted.
 */
class WXDLLIMPEXP_MATHPLOT mpProfile: public mpFunction
{
  public:
    /** @param name  Label
     @param flags Label alignment, pass one of #mpALIGN_BOTTOM, #mpALIGN_CENTER, #mpALIGN_TOP.
     */
    mpProfile(const wxString &name = wxEmptyString, int flags = mpALIGN_TOP);

    /** Get function value for argument.
     Override this function in your implementation.
     @param x Argument
     @return Function value
     */
    virtual double GetY(double x) = 0;

    /** Layer plot handler.
     This implementation will plot the function in the visible area and
     put a label according to the aligment specified.
     */
    virtual void DoPlot(wxDC &dc, mpWindow &w);

  protected:

  DECLARE_DYNAMIC_CLASS(mpProfile)
};

/*@}*/

//-----------------------------------------------------------------------------
// mpLayer implementations - furniture (scales, ...)
//-----------------------------------------------------------------------------
/** @name mpLayer implementations - furniture (scales, ...)
 @{*/

/** Plot layer implementing an abstract scale ruler.
 The scale numbering automatically adjusts to view and zoom factor.
 */
class WXDLLIMPEXP_MATHPLOT mpScale: public mpLayer
{
  public:
    /** Full constructor.
     @param name Label to plot by the ruler
     @param flags Set the position of the scale with respect to the window.
     @param grids Show grid or not. Give false (default) for not drawing the grid. */
    mpScale(const wxString &name, int flags, bool grids);

    /** Check whether this layer has a bounding box.
     This implementation returns \a FALSE thus making the ruler invisible
     to the plot layer bounding box calculation by mpWindow. */
    virtual bool HasBBox()
    {
      return false;
    }

    /** Set axis ticks
     @param ticks TRUE to plot axis ticks. */
    void ShowTicks(bool ticks)
    {
      m_ticks = ticks;
    }

    /** Get axis ticks
     @return true if ticks are shown */
    bool GetShowTicks() const
    {
      return m_ticks;
    }

    /** Set axis grids
     @param grids TRUE to plot axis grids. */
    void ShowGrids(bool grids)
    {
      m_grids = grids;
    }

    /** Get axis grids
     @return true if grids are shown */
    bool GetShowGrids() const
    {
      return m_grids;
    }

    /** Set axis Label format (used for mpX_NORMAL draw mode).
     @param format The format string */
    virtual void SetLabelFormat(const wxString &format)
    {
      m_labelFormat = format;
    }

    /** Get axis Label format (used for mpX_NORMAL draw mode).
     @return The format string */
    const wxString& GetLabelFormat() const
    {
      return m_labelFormat;
    }

    /** Set grid pen
     @param pen Pen, will be copied to internal class member
     */
    void SetGridPen(const wxPen &pen)
    {
      m_gridpen = pen;
    }

    /** Get pen set for this axis.
     @return Pen
     */
    const wxPen& GetGridPen() const
    {
      return m_gridpen;
    }

    /** Set auto scaling
     @param _auto for the scaling
     */
    void SetAuto(bool _auto)
    {
      m_auto = _auto;
    }

    /** Get auto property for this axis.
     @return auto
     */
    bool GetAuto() const
    {
      return m_auto;
    }

    void SetMinScale(double min)
    {
      m_min = min;
    }

    double GetMinScale() const
    {
      return m_min;
    }

    void SetMaxScale(double max)
    {
      m_max = max;
    }

    double GetMaxScale() const
    {
      return m_max;
    }

    virtual bool IsLogAxis() = 0;
    virtual void SetLogAxis(bool log) = 0;

  protected:
    wxPen m_gridpen;         //!< Grid's pen. Default Colour = LIGHT_GREY, width = 1, style = wxPENSTYLE_DOT
    bool m_ticks;            //!< Flag to show ticks. Default true
    bool m_grids;            //!< Flag to show grids. Default false
    bool m_auto;             //!< Flag to autosize grids. Default true
    double m_min, m_max;     //!< Min and max scale when autosize is false
    wxString m_labelFormat;  //!< Format string used to print labels

    virtual int GetOrigin(mpWindow &w) = 0;
    double GetStep(double scale);
    virtual void DrawScaleName(wxDC &dc, mpWindow &w, int origin, int labelSize) = 0;

    wxString FormatLogValue(double n);

  DECLARE_DYNAMIC_CLASS(mpScale)
};

/** Set label for X axis in normal mode */
#define mpX_NORMAL  0x00
/** Set label for X axis in time mode: the value is represented as minutes:seconds.milliseconds if time
 * is less than 2 minutes, hours:minutes:seconds otherwise. */
#define mpX_TIME  0x01
/** Set label for X axis in hours mode: the value is always represented as hours:minutes:seconds. */
#define mpX_HOURS 0x02
/** Set label for X axis in date mode: the value is always represented as yyyy-mm-dd. */
#define mpX_DATE 0x03
/** Set label for X axis in datetime mode: the value is always represented as yyyy-mm-ddThh:mm:ss. */
#define mpX_DATETIME 0x04
/** Set label user defined */
#define mpX_USER 0x05

/** Plot layer implementing a x-scale ruler.
 The ruler is fixed at Y=0 in the coordinate system. A label is plotted at
 the bottom-right hand of the ruler. The scale numbering automatically
 adjusts to view and zoom factor.
 */
class WXDLLIMPEXP_MATHPLOT mpScaleX: public mpScale
{
  public:
    /** Full constructor.
     @param name Label to plot by the ruler
     @param flags Set the position of the scale with respect to the window.
     @param grids Show grid or not. Give false (default) for not drawing the grid.
     @param type mpX_NORMAL for normal labels, mpX_TIME for time axis in hours, minutes, seconds. */
    mpScaleX(const wxString &name = _T("X"), int flags = mpALIGN_CENTERX, bool grids = false, unsigned int type = mpX_NORMAL) :
        mpScale(name, flags, grids)
    {
      m_labelType = type;
      m_timeConv = mpX_RAWTIME;
    }

    /** Layer plot handler.
     This implementation will plot the ruler adjusted to the visible area. */
    virtual void DoPlot(wxDC &dc, mpWindow &w);

    virtual void SetLabelFormat(const wxString &format)
    {
      mpScale::SetLabelFormat(format);
      m_labelType = mpX_USER;
    }

    /** Get X axis label view mode.
     @return mpX_NORMAL for normal labels, mpX_TIME for time axis in hours, minutes, seconds. */
    unsigned int GetLabelMode() const
    {
      return m_labelType;
    }

    /** Set X axis label view mode.
     @param mode mpX_NORMAL for normal labels, mpX_TIME for time axis in hours, minutes, seconds. */
    void SetLabelMode(unsigned int mode, unsigned int time_conv = mpX_RAWTIME)
    {
      m_labelType = mode;
      m_timeConv = time_conv;
    }

    /** Specifies that this is a ScaleX layer.
     @return always \a TRUE
     @sa mpLayer::IsScale */
    virtual bool IsScale(mpScaleType *scale)
    {
      *scale = mpsScaleX;
      return true;
    }

    /**
     * Logarithmic X axis
     */
    virtual bool IsLogAxis();
    virtual void SetLogAxis(bool log);

  protected:
    unsigned int m_labelType;  //!< Select labels mode: mpX_NORMAL for normal labels, mpX_TIME for time axis in hours, minutes, seconds
    unsigned int m_timeConv;   //!< Selects if time has to be converted to local time or not.

    virtual int GetOrigin(mpWindow &w);
    virtual void DrawScaleName(wxDC &dc, mpWindow &w, int origin, int labelSize);
    wxString FormatValue(const wxString &fmt, double n);

  DECLARE_DYNAMIC_CLASS(mpScaleX)
};

/** Plot layer implementing a y-scale ruler.
 If align is set to mpALIGN_CENTER, the ruler is fixed at X=0 in the coordinate system.
 If the align is set to mpALIGN_TOP or mpALIGN_BOTTOM, the axis is always drawn respectively at
 top or bottom of the window. A label is plotted at the top-right hand of the ruler.
 The scale numbering automatically adjusts to view and zoom factor.
 */
class WXDLLIMPEXP_MATHPLOT mpScaleY: public mpScale
{
  public:
    /** Full constructor.
     @param name Label to plot by the ruler
     @param flags Set the position of the scale with respect to the window.
     @param grids Show grid or not. Give false (default) for not drawing the grid.
     @param Y2Axis to another scale for the axis. default = false*/
    mpScaleY(const wxString &name = _T("Y"), int flags = mpALIGN_CENTERY, bool grids = false, bool Y2Axis = false) :
        mpScale(name, flags, grids)
    {
      m_isY2Axis = Y2Axis;
    }

    /** Layer plot handler.
     This implementation will plot the ruler adjusted to the visible area. */
    virtual void DoPlot(wxDC &dc, mpWindow &w);

    /** Specifies that this is a ScaleY layer.
     @return always \a TRUE
     @sa mpLayer::IsScale */
    virtual bool IsScale(mpScaleType *scale)
    {
      *scale = mpsScaleY;
      return true;
    }

    /**
     * Logarithmic Y axis
     */
    virtual bool IsLogAxis();
    virtual void SetLogAxis(bool log);

    virtual bool IsY2Axis()
    {
      return m_isY2Axis;
    }
    virtual void SetY2Axis(bool y2Axis);

  protected:
    bool m_isY2Axis;

    virtual int GetOrigin(mpWindow &w);
    virtual void DrawScaleName(wxDC &dc, mpWindow &w, int origin, int labelSize);

  DECLARE_DYNAMIC_CLASS(mpScaleY)
};

//-----------------------------------------------------------------------------
// mpWindow
//-----------------------------------------------------------------------------

/** @name Constants defining mouse modes for mpWindow
 @{*/

/** Mouse panning drags the view. Mouse mode for mpWindow. */
#define mpMOUSEMODE_DRAG    0
/** Mouse panning creates a zoom box. Mouse mode for mpWindow. */
#define mpMOUSEMODE_ZOOMBOX 1

/*@}*/
/** Define the type for the list of layers inside mpWindow */
//WX_DECLARE_HASH_MAP( int, mpLayer*, wxIntegerHash, wxIntegerEqual, wxLayerList );
typedef std::deque<mpLayer*> wxLayerList;

/**
 * Define an event for when we delete a layer
 * Use like this :
 *  your_plot->SetOnDeleteLayer([this](void *Sender, const wxString &classname, bool &cancel)
 {  your_event_function(Sender, classname, cancel);});
 */
typedef std::function<void(void *Sender, const wxString &classname, bool &cancel)> wxOnDeleteLayer;

/**
 * Class for drawing mouse magnetization
 */
class mpMagnet
{
  public:
    mpMagnet()
    {
      m_IsDrawn = false;
      m_rightClick = false;
      m_IsWasDrawn = false;
    }
    ~mpMagnet()
    {
      ;
    }
    void UpdateBox(wxCoord left, wxCoord top, wxCoord width, wxCoord height)
    {
      m_domain = wxRect(left, top, width, height);
      m_plot_size = wxRect(left, top, width + left, height + top);
    }
    void Plot(wxClientDC &dc, const wxPoint &mousePos);
    void ClearPlot(wxClientDC &dc);
    void UpdatePlot(wxClientDC &dc, const wxPoint &mousePos);
    void SaveDrawState(void)
    {
      m_IsWasDrawn = m_IsDrawn;
      // In any cases, set to false because we erase and repaint all the plot
      m_IsDrawn = false;
    }

    void SetRightClick(void)
    {
      m_rightClick = true;
    }

  private:
    wxRect m_domain;           //!< The area delimited by axis (m_margin.left, m_margin.top, m_plotWidth, m_plotHeight)
    wxRect m_plot_size;        //!< The coordinates for the cross (xmin, xmax), (ymin,ymax)
    wxPoint m_mousePosition;   //!< The last position of the mouse
    bool m_IsDrawn;            //!< Is that the cross is drawn ?
    bool m_IsWasDrawn;         //!< Is that the cross was drawn before the OnPaint event ?
    bool m_rightClick;         //!< Is the mouse right click ?
    void DrawCross(wxClientDC &dc);
};

/** Canvas for plotting mpLayer implementations.

 This class defines a zoomable and moveable 2D plot canvas. Any number
 of mpLayer implementations (scale rulers, function plots, ...) can be
 attached using mpWindow::AddLayer.

 The canvas window provides a context menu with actions for navigating the view.
 The context menu can be retrieved with mpWindow::GetPopupMenu, e.g. for extending it
 externally.

 Since wxMathPlot version 0.03, the mpWindow incorporates the following features:
 - DoubleBuffering (Default=disabled): Can be set with EnableDoubleBuffer
 - Mouse based pan/zoom (Default=enabled): Can be set with EnableMousePanZoom.

 The mouse commands can be visualized by the user through the popup menu, and are:
 - Mouse Move+CTRL: Pan (Move)
 - Mouse Wheel UP: Zoom in
 - Mouse Wheel DOWN: Zoom out
 - Mouse Wheel+CTRL: Vertical scroll
 - Mouse Wheel+SHIFT: Horizontal scroll
 */
class WXDLLIMPEXP_MATHPLOT mpWindow: public wxWindow
{
  public:
    mpWindow()
    {
      InitParameters();
    }

    mpWindow(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
        long flags = 0);

    ~mpWindow();

    /** Get reference to context menu of the plot canvas.
     @return Pointer to menu. The menu can be modified.
     */
    wxMenu* GetPopupMenu()
    {
      return &m_popmenu;
    }

    /** Add a plot layer to the canvas.
     @param layer Pointer to layer. The mpLayer object will get under control of mpWindow,
     i.e. it will be delete'd on mpWindow destruction
     @param refreshDisplay States whether to refresh the display (UpdateAll) after adding the layer.
     @retval TRUE Success
     @retval FALSE Failure due to out of memory.
     */
    bool AddLayer(mpLayer *layer, bool refreshDisplay = true);

    /** Remove a plot layer from the canvas.
     @param layer Pointer to layer. The mpLayer object will be destructed using delete.
     @param alsoDeleteObject If set to true, the mpLayer object will be also "deleted", not just removed from the internal list.
     @param refreshDisplay States whether to refresh the display (UpdateAll) after removing the layer.
     @return true if layer is deleted correctly

     N.B. Only the layer reference in the mpWindow is deleted, the layer object still exists!
     */
    bool DelLayer(mpLayer *layer, bool alsoDeleteObject = false, bool refreshDisplay = true);

    /** Remove all layers from the plot.
     @param alsoDeleteObject If set to true, the mpLayer objects will be also "deleted", not just removed from the internal list.
     @param refreshDisplay States whether to refresh the display (UpdateAll) after removing the layers.
     */
    void DelAllLayers(bool alsoDeleteObject, bool refreshDisplay = true);

    /** Remove all plot layers.
     @param alsoDeleteObject If set to true, the mpLayer objects will be also "deleted", not just removed from the internal list.
     @param func Select type of plot
     @param refreshDisplay States whether to refresh the display (UpdateAll) after removing the layers.
     */
    void DelAllPlot(bool alsoDeleteObject, mpFunctionType func = mpfAllType, bool refreshDisplay = true);

    /*! Get the layer in list position indicated.
     N.B. You <i>must</i> know the index of the layer inside the list!
     @param position position of the layer in the layers list
     @return pointer to mpLayer
     */
    mpLayer* GetLayer(int position);

    /*! Get the layer plot in list position indicated.
     N.B. You <i>must</i> know the index of the layer plot inside the list of plot!
     @param position position of the layer plot in the layers plot list
     @return pointer to mpLayer
     */
    mpLayer* GetLayerPlot(int position, mpFunctionType func = mpfAllType);

    /*! Get the layer axis in list position indicated.
     */
    mpLayer* GetLayerAxis(int position, mpScaleType scale = mpsAllType);

    /*!
     * Return the serie n
     * If the serie not exist then create it
     */
    mpFXYVector* GetXYSeries(unsigned int n, const wxString &name = _T("Serie "), bool create = true);

    /*!
     * Search the point of the layer plot nearest a point
     */
    mpLayer* GetClosestPlot(wxCoord ix, wxCoord iy, double *xnear, double *ynear, bool *isY2Axis);

    /*! Get the layer by its name (case sensitive).
     @param name The name of the layer to retrieve
     @return A pointer to the mpLayer object, or NULL if not found.
     */
    mpLayer* GetLayerByName(const wxString &name);

    /*! Get the layer by its class name (case sensitive).
     @param name The class name of the layer to retrieve
     @return A pointer to the mpLayer object, or NULL if not found.
     */
    mpLayer* GetLayerByClassName(const wxString &name);

    /*! Get the first scale X layer (X axis).
     @return A pointer to the mpScaleX object, or NULL if not found.
     */
    mpScaleX* GetLayerXAxis();

    /*! Get the first scale Y layer (Y axis).
     @return A pointer to the mpScaleY object, or NULL if not found.
     */
    mpScaleY* GetLayerYAxis();

    /*! Get the first scale Y2 layer (Y2 axis).
     @return A pointer to the mpScaleY object, or NULL if not found.
     */
    mpScaleY* GetLayerY2Axis();

    /** Set current view's X scale and refresh display.
     @param scaleX New scale, must not be 0.
     */
    void SetScaleX(const double scaleX)
    {
      if (ISNOTNULL(scaleX))
        m_scaleX = scaleX;
      UpdateAll();
    }

    /** Get current view's X scale.
     See @ref mpLayer::Plot "rules for coordinate transformation"
     @return Scale
     */
    double GetScaleX(void) const
    {
      return m_scaleX;
    }

    /** Set current view's Y scale and refresh display.
     @param scaleY New scale, must not be 0.
     */
    void SetScaleY(const double scaleY)
    {
      if (ISNOTNULL(scaleY))
        m_scaleY = scaleY;
      UpdateAll();
    }

    /** Get current view's Y scale.
     See @ref mpLayer::Plot "rules for coordinate transformation"
     @return Scale
     */
    double GetScaleY(bool Y2 = false) const
    {
      if (Y2)
        return m_scaleY2;
      else
        return m_scaleY;
    } // Schaling's method: maybe another method exists with the same name

    /**
     * Update bound for mpFX and mpFY when axis is scaled
     */
    void SetBound();

    /** Get plot bound.
     @return Bound
     */
    mpFloatRect Get_Bound(void) const
    {
      return m_bound;
    }

    /** Set current view's X position and refresh display.
     @param posX New position that corresponds to the center point of the view.
     */
    void SetPosX(const double posX)
    {
      m_posX = posX;
      UpdateAll();
    }

    /** Get current view's X position.
     See @ref mpLayer::Plot "rules for coordinate transformation"
     @return X Position in layer coordinate system, that corresponds to the center point of the view.
     */
    double GetPosX(void) const
    {
      return m_posX;
    }

    /** Set current view's Y position and refresh display.
     @param posY New position that corresponds to the center point of the view.
     */
    void SetPosY(const double posY, const double posY2)
    {
      m_posY = posY;
      m_posY2 = posY2;
      UpdateAll();
    }

    /** Get current view's Y position.
     See @ref mpLayer::Plot "rules for coordinate transformation"
     @return Y Position in layer coordinate system, that corresponds to the center point of the view.
     */
    double GetPosY(bool Y2 = false) const
    {
      if (Y2)
        return m_posY2;
      else
        return m_posY;
    }

    /** Set current view's dimensions in device context units.
     Needed by plotting functions. It doesn't refresh display.
     @param scrX New position that corresponds to the center point of the view.
     @param scrY New position that corresponds to the center point of the view.
     */
    void SetScreen(const int scrX, const int scrY)
    {
      m_scrX = scrX;
      m_scrY = scrY;
      m_plotWidth = m_scrX - (m_margin.left + m_margin.right);
      m_plotHeight = m_scrY - (m_margin.top + m_margin.bottom);

      m_plotBondaries.endPx = m_scrX;
      m_plotBondariesMargin.endPx = m_scrX - m_margin.right;
      m_plotBondaries.endPy = m_scrY;
      m_plotBondariesMargin.endPy = m_scrY - m_margin.bottom;

      m_magnet.UpdateBox(m_margin.left - EXTRA_MARGIN, m_margin.top - EXTRA_MARGIN,
          m_plotWidth + 2*EXTRA_MARGIN, m_plotHeight + 2*EXTRA_MARGIN);
    }

    /** Get current view's X dimension in device context units.
     Usually this is equal to wxDC::GetSize, but it might differ thus mpLayer
     implementations should rely on the value returned by the function.
     See @ref mpLayer::Plot "rules for coordinate transformation"
     @return X dimension.
     */
    int GetScreenX(void) const
    {
      return m_scrX;
    }

    /** Get current view's Y dimension in device context units.
     Usually this is equal to wxDC::GetSize, but it might differ thus mpLayer
     implementations should rely on the value returned by the function.
     See @ref mpLayer::Plot "rules for coordinate transformation"
     @return Y dimension.
     */
    int GetScreenY(void) const
    {
      return m_scrY;
    }

    /** Set current view's X and Y position and refresh display.
     @param posX New position that corresponds to the center point of the view.
     @param posY New position that corresponds to the center point of the view.
     */
    void SetPos(const double posX, const double posY, const double posY2)
    {
      m_posX = posX;
      m_posY = posY;
      m_posY2 = posY2;
      UpdateAll();
    }

    /** Converts mpWindow (screen) pixel coordinates into graph (floating point) coordinates,
     * using current mpWindow position and scale.
     * @sa p2y,x2p,y2p */
    inline double p2x(const wxCoord pixelCoordX) const
    {
      return m_posX + pixelCoordX / m_scaleX;
    }

    /** Converts mpWindow (screen) pixel coordinates into graph (floating point) coordinates,
     * using current mpWindow position and scale.
     * @sa p2x,x2p,y2p */
    inline double p2y(const wxCoord pixelCoordY, bool Y2 = false) const
    {
      if (Y2)
        return m_posY2 - pixelCoordY / m_scaleY2;
      else
        return m_posY - pixelCoordY / m_scaleY;
    }

    /** Converts graph (floating point) coordinates into mpWindow (screen) pixel coordinates,
     * using current mpWindow position and scale.
     * @sa p2x,p2y,y2p */
    inline wxCoord x2p(const double x) const
    {
      return (wxCoord)((x - m_posX) * m_scaleX);
    }

    /** Converts graph (floating point) coordinates into mpWindow (screen) pixel coordinates,
     * using current mpWindow position and scale.
     * @sa p2x,p2y,x2p */
    inline wxCoord y2p(const double y, bool Y2 = false) const
    {
      if (Y2)
        return (wxCoord)((m_posY2 - y) * m_scaleY2);
      else
        return (wxCoord)((m_posY - y) * m_scaleY);
    }

    /** Enable/disable the double-buffering of the window, eliminating the flicker (default=enabled).
     */
    void EnableDoubleBuffer(const bool enabled)
    {
      m_enableDoubleBuffer = enabled;
    }

    /** Enable/disable the feature of pan/zoom with the mouse (default=enabled)
     */
    void EnableMousePanZoom(const bool enabled)
    {
      m_enableMouseNavigation = enabled;
    }

    /** Enable or disable X/Y scale aspect locking for the view.
     @note Explicit calls to mpWindow::SetScaleX and mpWindow::SetScaleY will set
     an unlocked aspect, but any other action changing the view scale will
     lock the aspect again.
     */
    void LockAspect(bool enable = true);

    /** Checks whether the X/Y scale aspect is locked.
     @retval TRUE Locked
     @retval FALSE Unlocked
     */
    inline bool IsAspectLocked() const
    {
      return m_lockaspect;
    }

    /** Checks if we are repainting.
     @retval TRUE
     @retval FALSE
     */
    inline bool IsRepainting() const
    {
      return m_repainting;
    }

    /** Set view to fit global bounding box of all plot layers and refresh display.
     Scale and position will be set to show all attached mpLayers.
     The X/Y scale aspect lock is taken into account.
     */
    void Fit();

    /** Set view to fit a given bounding box and refresh display.
     The X/Y scale aspect lock is taken into account.
     If provided, the parameters printSizeX and printSizeY are taken as the DC size, and the
     pixel scales are computed accordingly. Also, in this case the passed borders are not saved
     as the "desired borders", since this use will be invoked only when printing.
     */
    void Fit(const mpFloatRect &rect, wxCoord *printSizeX = NULL, wxCoord *printSizeY = NULL);

    /** Zoom into current view and refresh display
     * @param centerPoint The point (pixel coordinates) that will stay in the same position on the screen after
     * the zoom (by default, the center of the mpWindow).
     */
    void ZoomIn(const wxPoint &centerPoint = wxDefaultPosition);

    /** Zoom out current view and refresh display
     * @param centerPoint The point (pixel coordinates) that will stay in the same position on the screen after
     * the zoom (by default, the center of the mpWindow).
     */
    void ZoomOut(const wxPoint &centerPoint = wxDefaultPosition);

    /** Zoom in current view along X and refresh display */
    void ZoomInX();

    /** Zoom out current view along X and refresh display */
    void ZoomOutX();

    /** Zoom in current view along Y and refresh display */
    void ZoomInY();

    /** Zoom out current view along Y and refresh display */
    void ZoomOutY();

    /** Zoom view fitting given coordinates to the window (p0 and p1 do not need to be in any specific order) */
    void ZoomRect(wxPoint p0, wxPoint p1);

    /** Refresh display */
    void UpdateAll();

    // Added methods by Davide Rondini

    /** Counts the number of plot layers, including axes: this is to count only the layers which have a bounding box.
     \return The number of profiles plotted.
     */
    unsigned int CountLayers();

    /** Counts the number of plot layers, whether or not they have a bounding box.
     \return The number of layers in the mpWindow. */
    unsigned int CountAllLayers()
    {
      return (unsigned int)m_layers.size();
    }

    /** Counts the number of plot layers: this is to count only the layers which have a plot.
     \return The number of profiles plotted.
     */
    unsigned int CountLayersType(mpLayerType type);
    unsigned int CountLayersFXYPlot();

    /** Draws the mpWindow on a page for printing
     \param print the mpPrintout where to print the graph */
    //void PrintGraph(mpPrintout *print);
    /** Returns the left-border layer coordinate that the user wants the mpWindow to show (it may be not exactly the actual shown coordinate in the case of locked aspect ratio).
     * @sa Fit
     */
    double GetDesiredXmin() const
    {
      return m_desired.Xmin;
    }

    /** Returns the right-border layer coordinate that the user wants the mpWindow to show (it may be not exactly the actual shown coordinate in the case of locked aspect ratio).
     * @sa Fit
     */
    double GetDesiredXmax() const
    {
      return m_desired.Xmax;
    }

    /** Returns the bottom-border layer coordinate that the user wants the mpWindow to show (it may be
     * not exactly the actual shown coordinate in the case of locked aspect ratio).
     * @sa Fit
     */
    double GetDesiredYmin() const
    {
      return m_desired.Ymin;
    }

    /** Returns the top layer-border coordinate that the user wants the mpWindow to show (it may be
     * not exactly the actual shown coordinate in the case of locked aspect ratio).
     * @sa Fit
     */
    double GetDesiredYmax() const
    {
      return m_desired.Ymax;
    }

    /** Returns the bounding box coordinates
     @param bbox Pointer to a 6-element double array where to store bounding box coordinates. */
    void GetBoundingBox(double *bbox);
    mpFloatRect *GetBoundingBox(void)
    {
      return &m_bound;
    }

    /** Enable/disable scrollbars
     @param status Set to true to show scrollbars */
    void SetMPScrollbars(bool status);

    /** Get scrollbars status.
     @return true if scrollbars are visible */
    bool GetMPScrollbars() const
    {
      return m_enableScrollBars;
    }

    /** Draw the window on a wxBitmap, then save it to a file.
     @param filename File name where to save the screenshot
     @param type image type to be saved: see wxImage output file types for flags
     @param imageSize Set a size for the output image. Default is the same as the screen size
     @param fit Decide whether to fit the plot into the size*/
    bool SaveScreenshot(const wxString &filename, int type = wxBITMAP_TYPE_BMP, wxSize imageSize = wxDefaultSize, bool fit = false);

    /**
     * Get a screen shot of the window plot
     */
    wxBitmap* BitmapScreenshot(wxSize imageSize = wxDefaultSize, bool fit = false);

    /**
     * Send the screen shot to the Clipboard
     */
    void ClipboardScreenshot(wxSize imageSize = wxDefaultSize, bool fit = false);

    /**
     * Load file
     */
    bool LoadFile(const wxString &filename);

    /** This value sets the zoom steps whenever the user clicks "Zoom in/out" or performs zoom with the mouse wheel.
     *  It must be a number above unity. This number is used for zoom in, and its inverse for zoom out. Set to 1.5 by default. */
    static double zoomIncrementalFactor;

    /** Set window margins, creating a blank area where some kinds of layers cannot draw.
     * This is useful for example to draw axes outside the area where the plots are drawn.
     @param top Top border
     @param right Right border
     @param bottom Bottom border
     @param left Left border */
    void SetMargins(int top, int right, int bottom, int left);

    /** Set the top margin. @param top Top Margin */
    void SetMarginTop(int top)
    {
      SetMargins(top, m_margin.right, m_margin.bottom, m_margin.left);
    }

    /** Get the top margin. @param top Top Margin */
    int GetMarginTop() const
    {
      return m_margin.top;
    }

    /** Set the right margin. @param right Right Margin */
    void SetMarginRight(int right)
    {
      SetMargins(m_margin.top, right, m_margin.bottom, m_margin.left);
    }

    /** Get the right margin. @param right Right Margin */
    int GetMarginRight() const
    {
      return m_margin.right;
    }

    /** Set the bottom margin. @param bottom Bottom Margin */
    void SetMarginBottom(int bottom)
    {
      SetMargins(m_margin.top, m_margin.right, bottom, m_margin.left);
    }

    /** Get the bottom margin. @param bottom Bottom Margin */
    int GetMarginBottom() const
    {
      return m_margin.bottom;
    }

    /** Set the left margin. @param left Left Margin */
    void SetMarginLeft(int left)
    {
      SetMargins(m_margin.top, m_margin.right, m_margin.bottom, left);
    }

    /** Get the left margin. @param left Left Margin */
    int GetMarginLeft() const
    {
      return m_margin.left;
    }

    /** Get the width of the plot. */
    int GetPlotWidth() const
    {
      return m_plotWidth;
    }

    /** Get the height of the plot. */
    int GetPlotHeight() const
    {
      return m_plotHeight;
    }

    /** Get the bondaries of the plot. */
    mpRect GetPlotBondaries(bool with_margin) const
    {
      mpRect bond;
      if (with_margin)
        bond = m_plotBondariesMargin;
      else
        bond = m_plotBondaries;
      bond.startPx -= EXTRA_MARGIN;
      bond.endPx += EXTRA_MARGIN;
      bond.startPy -= EXTRA_MARGIN;
      bond.endPy += EXTRA_MARGIN;
      return bond;
    }

    /** Set the draw of the box around the plot. */
    void SetDrawBox(bool drawbox)
    {
      m_drawBox = drawbox;
    }

    /** Get the draw of the box around the plot. */
    bool GetDrawBox() const
    {
      return m_drawBox;
    }

    /** Check if a given point is inside the area of a mpInfoLayer and eventually returns its pointer.
     @param point The position to be checked
     @return If an info layer is found, returns its pointer, NULL otherwise */
    mpInfoLayer* IsInsideInfoLayer(const wxPoint &point);

    /** Sets the visibility of a layer by its name.
     @param name The layer name to set visibility
     @param viewable the view status to be set */
    void SetLayerVisible(const wxString &name, bool viewable);

    /** Check whether a layer with given name is visible
     @param name The layer name
     @return layer visibility status */
    bool IsLayerVisible(const wxString &name);

    /** Check whether the layer at given position is visible
     @param position The layer position in layer list
     @return layer visibility status */
    bool IsLayerVisible(const unsigned int position);

    /** Sets the visibility of a layer by its position in layer list.
     @param position The layer position in layer list
     @param viewable the view status to be set */
    void SetLayerVisible(const unsigned int position, bool viewable);

    /** Set Color theme. Provide colours to set a new colour theme.
     @param bgColour Background colour
     @param drawColour The colour used to draw all elements in foreground, axes excluded
     @param axesColour The colour used to draw axes (but not their labels) */
    void SetColourTheme(const wxColour &bgColour, const wxColour &drawColour, const wxColour &axesColour);

    /** Get axes draw colour
     @return reference to axis colour used in theme */
    const wxColour& GetAxesColour() const
    {
      return m_axColour;
    }

    const wxColour& GetbgColour() const
    {
      return m_bgColour;
    }

    void SetbgColour(const wxColour &colour)
    {
      m_bgColour = colour;
    }

    /** On delete layer event
     @return reference to event */
    void SetOnDeleteLayer(wxOnDeleteLayer event)
    {
      m_OnDeleteLayer = event;
    }

    /**
     * Log axis control.
     * It is an axis property but as we need to control the bound and the scale,
     * it is easiest to declare this property here
     */
    bool IsLogXaxis()
    {
      return m_LogXaxis;
    }

    bool IsLogYaxis()
    {
      return m_LogYaxis;
    }

    void SetLogXaxis(bool log)
    {
      m_LogXaxis = log;
    }

    void SetLogYaxis(bool log)
    {
      m_LogYaxis = log;
    }

    bool GetMagnetize() const
    {
      return m_magnetize;
    }

    void SetMagnetize(bool mag)
    {
      m_magnetize = mag;
    }

    void RefreshConfigWindow();

    void Update_CountY2Axis(bool Y2Axis);

    bool Y2AxisExist(void)
    {
      return m_countY2Axis > 0;
    }

  protected:
    void OnPaint(wxPaintEvent &event);                    //!< Paint handler, will plot all attached layers
    void OnSize(wxSizeEvent &event);                      //!< Size handler, will update scroll bar sizes
    void OnShowPopupMenu(wxMouseEvent &event);            //!< Mouse handler, will show context menu
    void OnCenter(wxCommandEvent &event);                 //!< Context menu handler
    void OnFit(wxCommandEvent &event);                    //!< Context menu handler
    void OnToggleGrids(wxCommandEvent &event);            //!< Context menu handler
    void OnToggleCoords(wxCommandEvent &event);           //!< Context menu handler
    void OnScreenShot(wxCommandEvent &event);             //!< Context menu handler
    void OnFullScreen(wxCommandEvent &event);             //!< Context menu handler
    void OnConfiguration(wxCommandEvent &event);          //!< Context menu handler
    void OnLoadFile(wxCommandEvent &event);               //!< Context menu handler
    void OnZoomIn(wxCommandEvent &event);                 //!< Context menu handler
    void OnZoomOut(wxCommandEvent &event);                //!< Context menu handler
    void OnLockAspect(wxCommandEvent &event);             //!< Context menu handler
    void OnMouseHelp(wxCommandEvent &event);              //!< Context menu handler
    void OnMouseLeftDown(wxMouseEvent &event);            //!< Mouse left click (for rect zoom)
    void OnMouseRightDown(wxMouseEvent &event); //!< Mouse handler, for detecting when the user drags with the right button or just "clicks" for the menu
    void OnMouseMove(wxMouseEvent &event);                //!< Mouse handler for mouse motion (for pan)
    void OnMouseLeftRelease(wxMouseEvent &event);         //!< Mouse left click (for rect zoom)
    void OnMouseWheel(wxMouseEvent &event);               //!< Mouse handler for the wheel
    void OnMouseLeave(wxMouseEvent &event);               //!< Mouse handler for mouse motion (for pan)
    void OnScrollThumbTrack(wxScrollWinEvent &event);     //!< Scroll thumb on scroll bar moving
    void OnScrollPageUp(wxScrollWinEvent &event);         //!< Scroll page up
    void OnScrollPageDown(wxScrollWinEvent &event);       //!< Scroll page down
    void OnScrollLineUp(wxScrollWinEvent &event);         //!< Scroll line up
    void OnScrollLineDown(wxScrollWinEvent &event);       //!< Scroll line down
    void OnScrollTop(wxScrollWinEvent &event);            //!< Scroll to top
    void OnScrollBottom(wxScrollWinEvent &event);         //!< Scroll to bottom

    void DoScrollCalc(const int position, const int orientation);

    void DoZoomInXCalc(const int staticXpixel);
    void DoZoomInYCalc(const int staticYpixel);
    void DoZoomOutXCalc(const int staticXpixel);
    void DoZoomOutYCalc(const int staticYpixel);

    void Zoom(bool zoomIn, const wxPoint &centerPoint);

    /** Recalculate global layer bounding box, and save it in m_minX,...
     * \return true if there is any valid BBox information.
     */
    virtual bool UpdateBBox();

    void InitParameters();

    wxTopLevelWindow* m_parent;
    bool m_fullscreen;

    wxLayerList m_layers;   //!< List of attached plot layers
    mpScaleX* m_XAxis;      //!< Pointer to the X axis layer
    mpScaleY* m_YAxis;      //!< Pointer to the Y axis layer
    mpScaleY* m_Y2Axis;     //!< Pointer to the Y2 axis layer

    wxMenu m_popmenu;       //!< Canvas' context menu
    bool m_lockaspect;      //!< Scale aspect is locked or not
    wxColour m_bgColour;    //!< Background Colour
    wxColour m_fgColour;    //!< Foreground Colour
    wxColour m_axColour;    //!< Axes Colour
    bool m_drawBox;         //!< Draw box of the plot bound. Default true

    mpFloatRect m_bound;    //!< Global layer bounding box, left, right, top and bottom border incl.
    double m_scaleX;        //!< Current view's X scale
    double m_scaleY;        //!< Current view's Y scale
    double m_scaleY2;       //!< Current view's Y scale
    double m_posX;          //!< Current view's X position
    double m_posY;          //!< Current view's Y position
    double m_posY2;         //!< Current view's Y position
    int m_scrX;             //!< Current view's X dimension
    int m_scrY;             //!< Current view's Y dimension
    int m_clickedX;         //!< Last mouse click X position, for centering and zooming the view
    int m_clickedY;         //!< Last mouse click Y position, for centering and zooming the view

    /** These are updated in Fit() only, and may be different from the real borders (layer coordinates)
     * only if lock aspect ratio is true.
     */
    mpFloatRect m_desired;

    mpRect m_margin;                    //!< Margin around the plot. Default 50
    wxCoord m_plotWidth;                //!< Width of the plot = m_scrX - (m_margin.left + m_margin.right)
    wxCoord m_plotHeight;               //!< Height of the plot = m_scrY - (m_margin.top + m_margin.bottom)

    mpRect m_plotBondaries;             //!< The full size of the plot. Calculated
    mpRect m_plotBondariesMargin;       //!< The size of the plot with the margins. Calculated

    bool m_repainting;
    int m_last_lx, m_last_ly;           //!< For double buffering
    wxBitmap* m_buff_bmp;               //!< For double buffering
    bool m_enableDoubleBuffer;          //!< For double buffering. Default enabled
    bool m_enableMouseNavigation;       //!< For pan/zoom with the mouse.
    bool m_mouseMovedAfterRightClick;
    wxPoint m_mouseRClick;              //!< For the right button "drag" feature
    wxPoint m_mouseLClick;              //!< Starting coords for rectangular zoom selection
    bool m_enableScrollBars;
    int m_scrollX, m_scrollY;
    mpInfoLayer* m_movingInfoLayer;     //!< For moving info layers over the window area
    mpInfoCoords* m_InfoCoords;         //!< Shortcut to info coords layer

    wxBitmap* m_zoom_bmp;               //!< For zoom selection
    wxRect m_zoom_dim;
    wxRect m_zoom_oldDim;

    bool m_magnetize;                   //!< For mouse magnetization
    mpMagnet m_magnet;                  //!< For mouse magnetization

    bool m_LogXaxis = false;            //!< For logarithmic X axis
    bool m_LogYaxis = false;            //!< For logarithmic X axis

    wxBitmap* m_Screenshot_bmp;         //!< For clipboard, save and print

    MathPlotConfigDialog* m_configWindow = NULL;   //!< For the config dialog

    wxOnDeleteLayer m_OnDeleteLayer = NULL;        //!< Event when we delete a layer

  private:
    int m_countY2Axis = 0;
    void FillI18NString();

  DECLARE_DYNAMIC_CLASS(mpWindow)DECLARE_EVENT_TABLE()

    // To have direct access to m_Screenshot_dc
    friend mpPrintout;
};

//-----------------------------------------------------------------------------
// mpText - provided by Val Greene
//-----------------------------------------------------------------------------

/** Plot layer implementing a text string.
 The text is plotted in two ways :
 first : using a percentage system 0-100%, so the actual
 coordinates for the location are not required, and the text stays on the plot
 reguardless of the other layers location and scaling factors.
 second : center of a margin
 */
class WXDLLIMPEXP_MATHPLOT mpText: public mpLayer
{
  public:
    /** Default constructor.
     */
    mpText(const wxString &name = wxEmptyString)
    {
      m_type = mpLAYER_INFO;
      SetName(name);
      m_offsetx = 5;
      m_offsety = 50;
      m_location = mpMarginNone;
      m_ZIndex = mpZIndex_TEXT;
    }

    /** @param name text to be drawn in the plot
     @param offsetx holds offset for the X location in percentage (0-100)
     @param offsety holds offset for the Y location in percentage (0-100) */
    mpText(const wxString &name, int offsetx, int offsety);

    /** @param name text to be displayed
     @param location in the margin
     */
    mpText(const wxString &name, mpLocation marginLocation);

    /** Text Layer plot handler.
     This implementation will plot text adjusted to the visible area. */
    virtual void DoPlot(wxDC &dc, mpWindow &w);

    /** Text Layer has not bounding box. @sa mpLayer::HasBBox
     @return always \a FALSE */
    virtual bool HasBBox()
    {
      return false;
    }

    /** Specifies that this is a Text layer.
     @return always \a TRUE
     @sa mpLayer::IsText */
    virtual bool IsText()
    {
      return true;
    }

    /** Set the location of the box
     */
    void SetLocation(mpLocation location)
    {
      m_location = location;
    }

    /** Returns the location of the box
     @return location */
    mpLocation GetLocation() const
    {
      return m_location;
    }

    /** Set offset
     */
    void SetOffset(int offX, int offY)
    {
      m_offsetx = offX;
      m_offsety = offY;
    }

    /** Get the offset
     @return void */
    void GetOffset(int *offX, int *offY)
    {
      *offX = m_offsetx;
      *offY = m_offsety;
    }

  protected:
    int m_offsetx;  //!< Holds offset for X in percentage
    int m_offsety;  //!< Holds offset for Y in percentage
    mpLocation m_location;

  DECLARE_DYNAMIC_CLASS(mpText)
};

/** Plot layer implementing a simple title.
 * Title is placed in center top margin
 */
class WXDLLIMPEXP_MATHPLOT mpTitle: public mpText
{
  public:
    /** Default constructor.
     */
    mpTitle();

    /** @param name text to be drawn for the title of the plot
     */
    mpTitle(const wxString &name) :
        mpText(name, mpMarginTopCenter)
    {
      SetPen(*wxWHITE_PEN);
      SetBrush(*wxWHITE_BRUSH);
    }

    /** Specifies that this is a Title layer.
     @return always \a TRUE
     @sa mpLayer::IsTitle */
    virtual bool IsTitle()
    {
      return true;
    }

  protected:

  DECLARE_DYNAMIC_CLASS(mpTitle)
};

//-----------------------------------------------------------------------------
// mpPrintout - provided by Davide Rondini
//-----------------------------------------------------------------------------

/** Printout class used by mpWindow to draw in the objects to be printed.
 The object itself can then used by the default wxWidgets printing system
 to print mppWindow objects.
 */
class WXDLLIMPEXP_MATHPLOT mpPrintout: public wxPrintout
{
  public:
    mpPrintout()
    {
      plotWindow = NULL;
      drawn = false;
      stretch_factor = 2;
    }

    mpPrintout(mpWindow *drawWindow, const wxString &title = _T("wxMathPlot print output"), int factor = 2);
    virtual ~mpPrintout()
    {
      ;
    }

    void SetDrawState(bool drawState)
    {
      drawn = drawState;
    }

    bool OnPrintPage(int page);
    bool HasPage(int page);

    /** @param factor to reduce the size of the area of the plot
     */
    void SetFactor(int factor)
    {
      stretch_factor = factor;
    }

  private:
    bool drawn;
    mpWindow* plotWindow;
    int stretch_factor;  // To reduce the size of plot

  protected:

  DECLARE_DYNAMIC_CLASS(mpPrintout)
};

//-----------------------------------------------------------------------------
// mpMovableObject  - provided by Jose Luis Blanco
//-----------------------------------------------------------------------------
/** This virtual class represents objects that can be moved to an arbitrary 2D location+rotation.
 *  The current transformation is set through SetCoordinateBase.
 *  To ease the implementation of descendent classes, mpMovableObject will
 *  be in charge of Bounding Box computation and layer rendering, assuming that
 *  the object updates its shape in m_shape_xs & m_shape_ys.
 *  We derive from mpFunction to have symbol and continuity properties
 */
class WXDLLIMPEXP_MATHPLOT mpMovableObject: public mpFunction
{
  public:
    /** Default constructor (sets location and rotation to (0,0,0))
     */
    mpMovableObject() :
        m_reference_x(0), m_reference_y(0), m_reference_phi(0), m_shape_xs(0), m_shape_ys(0)
    {
      m_type = mpLAYER_PLOT;
      m_bbox_min_x = m_bbox_max_x = 0;
      m_bbox_min_y = m_bbox_max_y = 0;
    }

    virtual ~mpMovableObject()
    {
      ;
    }

    /** Get the current coordinate transformation.
     */
    void GetCoordinateBase(double &x, double &y, double &phi) const
    {
      x = m_reference_x;
      y = m_reference_y;
      phi = m_reference_phi;
    }

    /** Set the coordinate transformation (phi in radians, 0 means no rotation).
     */
    void SetCoordinateBase(double x, double y, double phi = 0)
    {
      m_reference_x = x;
      m_reference_y = y;
      m_reference_phi = phi;
      m_flags = mpALIGN_NE;
      ShapeUpdated();
    }

    virtual bool HasBBox()
    {
      return m_trans_shape_xs.size() != 0;
    }

    /** Get inclusive left border of bounding box.
     */
    virtual double GetMinX()
    {
      return m_bbox_min_x;
    }

    /** Get inclusive right border of bounding box.
     */
    virtual double GetMaxX()
    {
      return m_bbox_max_x;
    }

    /** Get inclusive bottom border of bounding box.
     */
    virtual double GetMinY()
    {
      return m_bbox_min_y;
    }

    /** Get inclusive top border of bounding box.
     */
    virtual double GetMaxY()
    {
      return m_bbox_max_y;
    }

    virtual void DoPlot(wxDC &dc, mpWindow &w);

    /** Specifies that this is a FX layer.
     @return always \a TRUE
     @sa mpLayer::IsFonction*/
    virtual bool IsFunction(mpFunctionType *function)
    {
      *function = mpfMovable;
      return true;
    }

  protected:

    /** The coordinates of the object (orientation "phi" is in radians).
     */
    double m_reference_x, m_reference_y, m_reference_phi;

    /** A method for 2D translation and rotation, using the current transformation stored in m_reference_x,m_reference_y,m_reference_phi.
     */
    void TranslatePoint(double x, double y, double &out_x, double &out_y);

    /** This contains the object points, in local coordinates (to be transformed by the current transformation).
     */
    std::vector<double> m_shape_xs, m_shape_ys;

    /** The buffer for the translated & rotated points (to avoid recomputing them with each mpWindow refresh).
     *
     */
    std::vector<double> m_trans_shape_xs, m_trans_shape_ys;

    /** The precomputed bounding box:
     * @sa ShapeUpdated
     */
    double m_bbox_min_x, m_bbox_max_x, m_bbox_min_y, m_bbox_max_y;

    /** Must be called by the descendent class after updating the shape (m_shape_xs/ys), or when the transformation changes.
     *  This method updates the buffers m_trans_shape_xs/ys, and the precomputed bounding box.
     */
    void ShapeUpdated();

  DECLARE_DYNAMIC_CLASS(mpMovableObject)
};

//-----------------------------------------------------------------------------
// mpCovarianceEllipse  - provided by Jose Luis Blanco
//-----------------------------------------------------------------------------
/** A 2D ellipse, described by a 2x2 covariance matrix.
 *  The relation between the multivariate Gaussian confidence interval and
 *   the "quantiles" in this class is:
 *     - 1 : 68.27% confidence interval
 *     - 2 : 95.45%
 *     - 3 : 99.73%
 *     - 4 : 99.994%
 * For example, see http://en.wikipedia.org/wiki/Normal_distribution#Standard_deviation_and_confidence_intervals
 *
 * The ellipse will be always centered at the origin. Use mpMovableObject::SetCoordinateBase to move it.
 */
class WXDLLIMPEXP_MATHPLOT mpCovarianceEllipse: public mpMovableObject
{
  public:
    /** Default constructor.
     * Initializes to a unity diagonal covariance matrix, a 95% confidence interval (2 sigmas), 32 segments, and a continuous plot (m_continuous=true).
     */
    mpCovarianceEllipse(double cov_00 = 1, double cov_11 = 1, double cov_01 = 0, double quantiles = 2, int segments = 32,
        const wxString &layerName = _T("")) :
        m_cov_00(cov_00), m_cov_11(cov_11), m_cov_01(cov_01), m_quantiles(quantiles), m_segments(segments)
    {
      m_type = mpLAYER_PLOT;
      m_continuous = true;
      m_name = layerName;
      RecalculateShape();
    }

    virtual ~mpCovarianceEllipse()
    {
      ;
    }

    double GetQuantiles() const
    {
      return m_quantiles;
    }

    /** Set how many "quantiles" to draw, that is, the confidence interval of the ellipse (see above).
     */
    void SetQuantiles(double q)
    {
      m_quantiles = q;
      RecalculateShape();
    }

    void SetSegments(int segments)
    {
      m_segments = segments;
    }

    int GetSegments() const
    {
      return m_segments;
    }

    /** Returns the elements of the current covariance matrix:
     */
    void GetCovarianceMatrix(double &cov_00, double &cov_01, double &cov_11) const
    {
      cov_00 = m_cov_00;
      cov_01 = m_cov_01;
      cov_11 = m_cov_11;
    }

    /** Changes the covariance matrix:
     */
    void SetCovarianceMatrix(double cov_00, double cov_01, double cov_11)
    {
      m_cov_00 = cov_00;
      m_cov_01 = cov_01;
      m_cov_11 = cov_11;
      RecalculateShape();
    }

  protected:
    /** The elements of the matrix (only 3 since cov(0,1)=cov(1,0) in any positive definite matrix).
     */
    double m_cov_00, m_cov_11, m_cov_01;
    double m_quantiles;

    /** The number of line segments that build up the ellipse.
     */
    int m_segments;

    /** Called to update the m_shape_xs, m_shape_ys vectors, whenever a parameter changes.
     */
    void RecalculateShape();

  DECLARE_DYNAMIC_CLASS(mpCovarianceEllipse)
};

//-----------------------------------------------------------------------------
// mpPolygon - provided by Jose Luis Blanco
//-----------------------------------------------------------------------------
/** An arbitrary polygon, descendant of mpMovableObject.
 *  Use "setPoints" to set the list of N points. This class also can draw non-closed polygons by
 *   passing the appropriate parameters to "setPoints". To draw a point-cloud, call "SetContinuity(false)".
 */
class WXDLLIMPEXP_MATHPLOT mpPolygon: public mpMovableObject
{
  public:
    /** Default constructor.
     */
    mpPolygon(const wxString &layerName = _T(""))
    {
      m_continuous = true;
      m_name = layerName;
    }

    virtual ~mpPolygon()
    {
      ;
    }

    /** Set the points in the polygon.
     * @param points_xs  The X coordinates of the points.
     * @param points_ys  The Y coordinates of the points.
     * @param closedShape If set to true, an additional segment will be added from the last to the first point.
     */
    void setPoints(const std::vector<double> &points_xs, const std::vector<double> &points_ys, bool closedShape = true);

  protected:

  DECLARE_DYNAMIC_CLASS(mpPolygon)
};

//-----------------------------------------------------------------------------
// mpBitmapLayer - provided by Jose Luis Blanco
//-----------------------------------------------------------------------------
/** A layer that allows you to have a bitmap image printed in the mpWindow.
 Use SetBitmap() to load the image in the right place.
 You can retrieve the image from the layer at anytime you want with getBitmapCopy()
 */
class WXDLLIMPEXP_MATHPLOT mpBitmapLayer: public mpLayer
{
  public:
    /** Default constructor.
     */
    mpBitmapLayer()
    {
      m_type = mpLAYER_BITMAP;
      m_min_x = m_max_x = 0;
      m_min_y = m_max_y = 0;
      m_validImg = false;
      m_bitmapChanged = false;
      m_scaledBitmap_offset_x = m_scaledBitmap_offset_y = 0;
    }

    virtual ~mpBitmapLayer()
    {
      ;
    }

    /** Returns a copy of the current bitmap assigned to the layer.
     */
    void GetBitmapCopy(wxImage &outBmp) const;

    /** Change the bitmap associated with the layer (to update the screen, refresh the mpWindow).
     *  @param inBmp The bitmap to associate. A copy is made, thus it can be released after calling this.
     *  @param x The left corner X coordinate (in plot units).
     *  @param y The top corner Y coordinate (in plot units).
     *  @param lx The width in plot units.
     *  @param ly The height in plot units.
     */
    void SetBitmap(const wxImage &inBmp, double x, double y, double lx, double ly);

    /** Get inclusive left border of bounding box.
     */
    virtual double GetMinX()
    {
      return m_min_x;
    }

    /** Get inclusive right border of bounding box.
     */
    virtual double GetMaxX()
    {
      return m_max_x;
    }

    /** Get inclusive bottom border of bounding box.
     */
    virtual double GetMinY()
    {
      return m_min_y;
    }

    /** Get inclusive top border of bounding box.
     */
    virtual double GetMaxY()
    {
      return m_max_y;
    }

    virtual void DoPlot(wxDC &dc, mpWindow &w);

  protected:

    /** The internal copy of the Bitmap:
     */
    wxImage m_bitmap;
    wxBitmap m_scaledBitmap;
    wxCoord m_scaledBitmap_offset_x, m_scaledBitmap_offset_y;
    bool m_validImg;
    bool m_bitmapChanged;

    /** The shape of the bitmap:
     */
    double m_min_x, m_max_x, m_min_y, m_max_y;

  DECLARE_DYNAMIC_CLASS(mpBitmapLayer)
};

// utilitary class

// Enumeration of classic colour
typedef enum __mp_Colour
{
  mpBlue,
  mpRed,
  mpGreen,
  mpPurple,
  mpYellow,
  mpFuchsia,
  mpLime,
  mpAqua,
  mpOlive
} mpColour;

/**
 * Create a wxColour
 * id is the number of the colour : blue, red, green, ...
 */
class WXDLLIMPEXP_MATHPLOT wxIndexColour: public wxColour
{
  public:
    wxIndexColour(unsigned int id)
    {
      switch (id)
      {
        case 0:
          this->Set(0, 0, 255);
          break;  // Blue
        case 1:
          this->Set(255, 0, 0);
          break;  // Red
        case 2:
          this->Set(0, 128, 0);
          break;  // Green
        case 3:
          this->Set(128, 0, 128);
          break;  // Purple
        case 4:
          this->Set(255, 255, 0);
          break;  // Yellow
        case 5:
          this->Set(255, 0, 255);
          break;  // Fuchsia
        case 6:
          this->Set(0, 255, 0);
          break;  // Lime
        case 7:
          this->Set(0, 255, 255);
          break;  // Aqua/Cyan
        case 8:
          this->Set(128, 128, 0);
          break;  // Olive
        default:
          this->Set((ChannelType)((rand() * 255) / RAND_MAX), (ChannelType)((rand() * 255) / RAND_MAX),
              (ChannelType)((rand() * 255) / RAND_MAX));
      }
    }
};

/*@}*/

// ---------------------------------------------------------------------
}// namespace MathPlot
using namespace MathPlot;

#endif // _MP_MATHPLOT_H_
