/***************************************************************
 * Name:      MathPlotConfig.h (included from mathplot.h when ENABLE_MP_CONFIG defined)
 * Purpose:   Defines Application Frame
 * Author:    Lionel ()
 * Created:   2021-01-24
 * Last edit: 2026-03-27
 * Copyright: Lionel ()
 * License:
 **************************************************************/

#ifndef MATHPLOTCONFIG_H_INCLUDED
#define MATHPLOTCONFIG_H_INCLUDED

//(*Headers(MathPlotConfigDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include <wx/colordlg.h>
#include <wx/fontdlg.h>
#include <wx/valnum.h>
#include <wx/fileconf.h>

#ifdef ENABLE_MP_NAMESPACE
  namespace MathPlot {
#endif // ENABLE_MP_NAMESPACE

class WXDLLIMPEXP_MATHPLOT mpWindow;
class WXDLLIMPEXP_MATHPLOT mpLayer;
class WXDLLIMPEXP_MATHPLOT mpFunction;
class WXDLLIMPEXP_MATHPLOT mpLine;
class WXDLLIMPEXP_MATHPLOT mpInfoCoords;
class WXDLLIMPEXP_MATHPLOT mpInfoLegend;
class WXDLLIMPEXP_MATHPLOT mpScale;
class WXDLLIMPEXP_MATHPLOT mpText;

/**
 * Helper class to save/restore configuration
 */
class MathPlotConfigSettings : public wxFileConfig
{
  public:
    /**
     * Manages the backup of configuration window data
     * @param localFilename the name of the file in which the data would be saved
     */
    MathPlotConfigSettings(const wxString& localFilename) :
        wxFileConfig(wxEmptyString, wxEmptyString, localFilename) {}
    virtual ~MathPlotConfigSettings() {}

    /**
     * Set the settings to the plot window
     * @param win the plot window handler
     */
    void SetSettings(wxWindow* win);

    /**
     * Get the settings from the plot window
     * @param win the plot window handler
     */
    void GetSettings(wxWindow* win);

  protected:
    /**
     * Get/set the position of the config window in the screen
     * @param set : set if true else get
     * @param win : the config window
     */
    void DoPosition(bool set, wxWindow *win);

    /**
     * Search recursively all the widgets then get/set parameters
     * @param set : set if true else get
     * @param win : the window (container)
     * @param path : the complete path (created with the name (class) of each window) to reach the window
     * @param level : the depth in the window (container) hierarchy
     */
    void DoRecursiveSearch(bool set, wxWindow *win, const wxString &path = wxEmptyString, int level = 0);
};

/**
 * wxMultiTextCtrlDialog class
 * A simple text control dialog window that support several text control
 */
class wxMultiTextCtrlDialog: public wxDialog
{
  public:
    /**
     * Constructor
     * @param parent handle of parent window
     * @param title title of the dialog window
     * @param message message to display before all the text control
     * @param size number of input text
     * @param prompt array of text control label
     * @param values array of values (double formated with 4 digits)
     * @param pos position of the window
     */
    wxMultiTextCtrlDialog(wxWindow* parent, const wxString& title,
        const wxString& message, long size, const wxString prompt[], double *values,
        const wxPoint& pos = wxDefaultPosition)
    {
      Create(parent, title, message, size, prompt, values, pos);
    }

    /**
     * Create the dialog window.
     * @sa wxMultiTextCtrlDialog
     */
    bool Create(wxWindow* parent, const wxString& title,
        const wxString& message, long size, const wxString prompt[], double *values,
        const wxPoint& pos = wxDefaultPosition);

  protected:
    void OnOK(wxCommandEvent& event);      //!< Ok handler. Data are transfered to values array
    void OnCancel(wxCommandEvent& event);  //!< Cancel handler.

  private:
    std::vector<wxTextCtrl *> m_ctrls;    //!< list of wxTextCtrl
    wxDECLARE_EVENT_TABLE();
};


/**
 * The list of index of the page of the config window
 */
typedef enum __ConfigPageIndex
{
  mpcpiNone = -1,
  mpcpiGeneral = 0,
  mpcpiLegend,
  mpcpiAxis,
  mpcpiSeries,
  mpcpiLines
} mpConfigPageId;

/**
 * Dialog box for configuring the plot's layer objects
 * In this dialog, you can configure:
 * - mpTitle layer (the title of the plot)
 * - mpInfoCoords layer (mouse coordinate tracking)
 * - mpInfoLegend layer (the legend of all plots)
 * - mpScale layer (X and Y axis)
 * - mpFunction layer (All plotting functions: mpFX, mpFY, mpFXY, mpHorizontalLine, mpVerticalLine)
 */
class MathPlotConfigDialog: public wxDialog
{
  public:

    /**
     * Create a dialog window to configure the plot's layer objects
     * @param parent the handle of the plot window
     * @param id an ID for the dialog window (default -1)
     */
    MathPlotConfigDialog(wxWindow *parent, wxWindowID id = -1);
    virtual ~MathPlotConfigDialog();

    /**
     * Initialize the dialog box by retrieving the layer properties.
     * @param id the page to be open in the dialog window (default first page)
     */
    void Initialize(mpConfigPageId id = mpcpiNone);

    /**
     * Select the series by number
     * @param serie the number of the series in the list
     */
    void SelectChoiceSerie(unsigned int serie);

    /**
     * Create a file to store the properties of mpTitle, mpInfoCoords and mpInfoLegend layers
     * @param filename the name of the configuration file
     * @param path the path for the file (if empty, the directory of the program is used)
     * @param apply if true then load and apply the settings immediately.
     */
    void CreateSettingsFile(const wxString& filename, const wxString& path = wxEmptyString, bool apply = false);

    /**
     * Apply the settings loaded in the configuration file
     */
    void ApplySettings(void);

    /**
     * Give access to m_settings if we want to save some others parameters
     */
    MathPlotConfigSettings* GetSettingsHandle(void)
    {
      return m_settings;
    }

  private:

    mpWindow* m_plot;
    MathPlotConfigSettings* m_settings;
    wxButton* colourButton;
    mpText* CurrentTitle;
    mpInfoLegend* CurrentLegend;
    mpInfoCoords* CurrentCoords;
    mpScale* CurrentScale;
    mpFunction* CurrentSerie;
    mpLine* CurrentLine;
    wxChoice* CurrentChoice;
    bool fontTitleChanged;
    bool fontLegendChanged;
    bool fontAxisChanged;
    // Margin validator
    unsigned int int_top, int_bottom, int_left, int_right, int_extra;
    int scale_offset;
    // Scale validator
    double scale_min, scale_max;
    // Line position validator
    double line_value;
    bool CheckBar;
    bool SerieVisibleChange;

    //(*Handlers(MathPlotConfigDialog)
    void OnQuit(wxCommandEvent& event);
    void OnnbConfigPageChanged(wxNotebookEvent& event);
    void OnbColorClick(wxCommandEvent& event);
    void OnChoiceSeries(wxCommandEvent& event);
    void OnAxisSelect(wxCommandEvent& event);
    void OncbFormatSelect(wxCommandEvent& event);
    void OncbAutoScaleClick(wxCommandEvent& event);
    void OnbApplyClick(wxCommandEvent& event);
    void OnbFontClick(wxCommandEvent& event);
    void OnbDelSeriesClick(wxCommandEvent& event);
    void OnbAddAxisClick(wxCommandEvent& event);
    void OnChoiceLinesSelect(wxCommandEvent& event);
    void OnbAddLinesClick(wxCommandEvent& event);
    void OnbDelLinesClick(wxCommandEvent& event);
    void OnbDelAxisClick(wxCommandEvent& event);
    void OncbSeriesShowNameClick(wxCommandEvent& event);
    //*)

    //(*Identifiers(MathPlotConfigDialog)
    //*)

    //(*Declarations(MathPlotConfigDialog)
    wxBoxSizer* sizerAxis;
    wxBoxSizer* sizerLines;
    wxBoxSizer* sizerMain;
    wxButton* bAddLines;
    wxButton* bAddXAxis;
    wxButton* bAddYAxis;
    wxButton* bApply;
    wxButton* bAxisPenColor;
    wxButton* bBGColor;
    wxButton* bClose;
    wxButton* bCoordBrushColor;
    wxButton* bDelAxis;
    wxButton* bDelLines;
    wxButton* bDelSeries;
    wxButton* bFontAxis;
    wxButton* bFontLegend;
    wxButton* bFontTitle;
    wxButton* bGridPenColor;
    wxButton* bLegendBrushColor;
    wxButton* bLinesPenColor;
    wxButton* bSeriesBrushColor;
    wxButton* bSeriesPenColor;
    wxCheckBox* cbAutoScale;
    wxCheckBox* cbAxisOutside;
    wxCheckBox* cbAxisVisible;
    wxCheckBox* cbBar;
    wxCheckBox* cbCoordOutside;
    wxCheckBox* cbCoordVisible;
    wxCheckBox* cbCoordinates;
    wxCheckBox* cbDrawBox;
    wxCheckBox* cbGridVisible;
    wxCheckBox* cbLegendDefaultVisibility;
    wxCheckBox* cbLegendVisible;
    wxCheckBox* cbLinesOutside;
    wxCheckBox* cbLinesShowName;
    wxCheckBox* cbLinesVisible;
    wxCheckBox* cbLogAxis;
    wxCheckBox* cbMagnetize;
    wxCheckBox* cbSeriesContinuity;
    wxCheckBox* cbSeriesLegend;
    wxCheckBox* cbSeriesOutside;
    wxCheckBox* cbSeriesShowName;
    wxCheckBox* cbSeriesVisible;
    wxCheckBox* cbTitleVisible;
    wxCheckBox* cbTractable;
    wxChoice* ChoiceAxis;
    wxChoice* ChoiceLeftMouseAction;
    wxChoice* ChoiceLines;
    wxChoice* ChoiceLinesYAxis;
    wxChoice* ChoiceSeries;
    wxChoice* ChoiceSeriesYAxis;
    wxChoice* cbAxisPenStyle;
    wxChoice* cbAxisPenWidth;
    wxChoice* cbAxisPosition;
    wxChoice* cbCoord;
    wxChoice* cbCoordBrushStyle;
    wxChoice* cbFormat;
    wxChoice* cbGridPenStyle;
    wxChoice* cbGridPenWidth;
    wxChoice* cbLegendBrushStyle;
    wxChoice* cbLegendDirection;
    wxChoice* cbLegendPosition;
    wxChoice* cbLegendStyle;
    wxChoice* cbLinesPenStyle;
    wxChoice* cbLinesPenWidth;
    wxChoice* cbSeriesBrushStyle;
    wxChoice* cbSeriesNamePosition;
    wxChoice* cbSeriesPenStyle;
    wxChoice* cbSeriesPenWidth;
    wxChoice* cbSeriesSymbolType;
    wxNotebook* nbConfig;
    wxNotebook* nbPenAxisGrid;
    wxPanel* Panel1;
    wxPanel* Panel2;
    wxPanel* Panel3;
    wxPanel* Panel4;
    wxPanel* Panel5;
    wxPanel* Panel6;
    wxPanel* Panel7;
    wxPanel* pLines;
    wxRadioButton* rbLinesDirection;
    wxSpinCtrl* cbSeriesStep;
    wxSpinCtrl* cbSeriesSymbolSize;
    wxStaticText* StaticText10;
    wxStaticText* StaticText11;
    wxStaticText* StaticText12;
    wxStaticText* StaticText13;
    wxStaticText* StaticText14;
    wxStaticText* StaticText15;
    wxStaticText* StaticText16;
    wxStaticText* StaticText17;
    wxStaticText* StaticText18;
    wxStaticText* StaticText19;
    wxStaticText* StaticText1;
    wxStaticText* StaticText20;
    wxStaticText* StaticText21;
    wxStaticText* StaticText22;
    wxStaticText* StaticText23;
    wxStaticText* StaticText24;
    wxStaticText* StaticText25;
    wxStaticText* StaticText26;
    wxStaticText* StaticText27;
    wxStaticText* StaticText28;
    wxStaticText* StaticText29;
    wxStaticText* StaticText2;
    wxStaticText* StaticText30;
    wxStaticText* StaticText31;
    wxStaticText* StaticText32;
    wxStaticText* StaticText33;
    wxStaticText* StaticText34;
    wxStaticText* StaticText35;
    wxStaticText* StaticText36;
    wxStaticText* StaticText37;
    wxStaticText* StaticText38;
    wxStaticText* StaticText39;
    wxStaticText* StaticText3;
    wxStaticText* StaticText40;
    wxStaticText* StaticText41;
    wxStaticText* StaticText42;
    wxStaticText* StaticText43;
    wxStaticText* StaticText44;
    wxStaticText* StaticText4;
    wxStaticText* StaticText5;
    wxStaticText* StaticText6;
    wxStaticText* StaticText7;
    wxStaticText* StaticText8;
    wxStaticText* StaticText9;
    wxStaticText* stLinesYIndexLabel;
    wxTextCtrl* edAxisName;
    wxTextCtrl* edExtraMargin;
    wxTextCtrl* edFormat;
    wxTextCtrl* edLinesName;
    wxTextCtrl* edLinesValue;
    wxTextCtrl* edMarginBottom;
    wxTextCtrl* edMarginLeft;
    wxTextCtrl* edMarginRight;
    wxTextCtrl* edMarginTop;
    wxTextCtrl* edScaleMax;
    wxTextCtrl* edScaleMin;
    wxTextCtrl* edSeriesName;
    wxTextCtrl* edTitle;
    //*)

    void Apply(int pageIndex, bool updateFont = false);
    void UpdateSelectedSerie(void);
    void UpdateSelectedLine(void);
    void UpdateAxis(void);
    void FillYAxisList(wxChoice* yChoice, bool clearChoice = true);

    void DoButtonColour(wxButton* button, const wxColour& colour);
    void DoApplyColour(const wxColour& colour);
    void UpdateFont(mpLayer* layer, wxButton* button, bool get_set);
    void SetFontChildren(wxButton* p, const wxFontData& fontdata);

    wxBrushStyle IdToBrushStyle(int id);
    int BrushStyleToId(wxBrushStyle style);

  DECLARE_EVENT_TABLE()
};

#ifdef ENABLE_MP_NAMESPACE
  }  // namespace MathPlot
#endif // ENABLE_MP_NAMESPACE

#endif // MATHPLOTCONFIG_H_INCLUDED
