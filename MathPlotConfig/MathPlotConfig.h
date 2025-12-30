/***************************************************************
 * Name:      MathPlotConfig.h (included from mathplot.h when ENABLE_MP_CONFIG defined)
 * Purpose:   Defines Application Frame
 * Author:    Lionel ()
 * Created:   2021-01-24
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
    MathPlotConfigSettings(const wxString& localFilename) :
        wxFileConfig(wxEmptyString, wxEmptyString, localFilename) {}
    virtual ~MathPlotConfigSettings() {}

    void SetSettings(wxWindow* win);
    void GetSettings(wxWindow* win);
  protected:
    void DoPosition(bool set, wxWindow *win);
    void DoRecursiveSearch(bool set, wxWindow *win, const wxString &path = wxEmptyString, int level = 0);
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

class MathPlotConfigDialog: public wxDialog
{
  public:

    MathPlotConfigDialog(wxWindow *parent, wxWindowID id = -1);
    virtual ~MathPlotConfigDialog();

    void Initialize(mpConfigPageId id = mpcpiNone);
    void SelectChoiceSerie(unsigned int serie);
    void CreateSettingsFile(const wxString& filename, const wxString& path = wxEmptyString, bool apply = false);
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
    unsigned int int_top, int_bottom, int_left, int_right, int_extra;
    int scale_offset;
    double scale_min, scale_max;
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
    wxCheckBox* cbLegendVisible;
    wxCheckBox* cbLinesOutside;
    wxCheckBox* cbLinesShowName;
    wxCheckBox* cbLinesVisible;
    wxCheckBox* cbLogAxis;
    wxCheckBox* cbMagnetize;
    wxCheckBox* cbSeriesContinuity;
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
    wxChoice* cbLegendBrushStyle;
    wxChoice* cbLegendDirection;
    wxChoice* cbLegendPosition;
    wxChoice* cbLegendStyle;
    wxChoice* cbLinesPenStyle;
    wxChoice* cbLinesPenWidth;
    wxChoice* cbSeriesBrushStyle;
    wxChoice* cbSeriesPenStyle;
    wxChoice* cbSeriesPenWidth;
    wxChoice* cbSeriesSymbolType;
    wxNotebook* nbConfig;
    wxPanel* Panel1;
    wxPanel* Panel2;
    wxPanel* Panel3;
    wxPanel* Panel4;
    wxPanel* Panel5;
    wxPanel* pLines;
    wxRadioButton* rbLinesDirection;
    wxSpinCtrl* cbSeriesStep;
    wxSpinCtrl* cbSeriesSymbolSize;
    wxStaticText* StaticText10;
    wxStaticText* StaticText11;
    wxStaticText* StaticText12;
    wxStaticText* StaticText13;
    wxStaticText* StaticText14;
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
