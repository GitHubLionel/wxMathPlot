/***************************************************************
 * Name:      MathPlotConfig.h
 * Purpose:   Defines Application Frame
 * Author:    Lionel ()
 * Created:   2021-01-24
 * Copyright: Lionel ()
 * License:
 **************************************************************/

#ifndef MATHPLOTCONFIGMAIN_H
#define MATHPLOTCONFIGMAIN_H

#include "mathplot.h"

//(*Headers(MathPlotConfigDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include <wx/colordlg.h>
#include <wx/fontdlg.h>
#include <wx/valnum.h>

// MathPlot namespace
namespace MathPlot
{

class WXDLLIMPEXP_MATHPLOT mpWindow;
class WXDLLIMPEXP_MATHPLOT mpLayer;
class WXDLLIMPEXP_MATHPLOT mpInfoCoords;
class WXDLLIMPEXP_MATHPLOT mpInfoLegend;
class WXDLLIMPEXP_MATHPLOT mpScale;
class WXDLLIMPEXP_MATHPLOT mpText;

class MathPlotConfigDialog: public wxDialog
{
    public:

        MathPlotConfigDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~MathPlotConfigDialog();

        void Initialize();

    private:

        mpWindow *m_plot;
        mpText *CurrentTitle;
        mpInfoLegend * CurrentLegend;
        mpInfoCoords * CurrentCoords;
        mpScale *CurrentScale;
        mpLayer *CurrentSerie;
        wxButton* colourButton;
        wxChoice* CurrentChoice;
        bool fontTitleChanged;
        bool fontLegendChanged;
        bool fontAxisChanged;
        unsigned int int_top, int_bottom, int_left, int_right;
        int scale_offset;
        double scale_min, scale_max;
        bool CheckBar;

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
        //*)

        //(*Identifiers(MathPlotConfigDialog)
        static const long ID_STATICTEXT11;
        static const long ID_TEXTCTRL3;
        static const long ID_BUTTON5;
        static const long ID_CHECKBOX8;
        static const long ID_STATICTEXT15;
        static const long ID_TEXTCTRL4;
        static const long ID_TEXTCTRL5;
        static const long ID_TEXTCTRL6;
        static const long ID_TEXTCTRL7;
        static const long ID_BUTTON7;
        static const long ID_CHECKBOX5;
        static const long ID_STATICTEXT18;
        static const long ID_CHOICE11;
        static const long ID_CHECKBOX10;
        static const long ID_CHECKBOX13;
        static const long ID_CHECKBOX16;
        static const long ID_STATICTEXT22;
        static const long ID_BUTTON10;
        static const long ID_STATICTEXT23;
        static const long ID_CHOICE13;
        static const long ID_PANEL17;
        static const long ID_STATICTEXT16;
        static const long ID_CHOICE10;
        static const long ID_STATICTEXT24;
        static const long ID_CHOICE14;
        static const long ID_STATICTEXT25;
        static const long ID_CHOICE15;
        static const long ID_CHECKBOX9;
        static const long ID_BUTTON12;
        static const long ID_STATICTEXT20;
        static const long ID_BUTTON9;
        static const long ID_STATICTEXT21;
        static const long ID_CHOICE12;
        static const long ID_PANEL15;
        static const long ID_CHOICE7;
        static const long ID_STATICTEXT8;
        static const long ID_TEXTCTRL2;
        static const long ID_BUTTON8;
        static const long ID_STATICTEXT12;
        static const long ID_BUTTON6;
        static const long ID_STATICTEXT13;
        static const long ID_CHOICE8;
        static const long ID_STATICTEXT14;
        static const long ID_CHOICE9;
        static const long ID_CHECKBOX11;
        static const long ID_STATICTEXT27;
        static const long ID_TEXTCTRL9;
        static const long ID_STATICTEXT28;
        static const long ID_TEXTCTRL10;
        static const long ID_STATICTEXT17;
        static const long ID_CHOICE6;
        static const long ID_CHECKBOX4;
        static const long ID_CHECKBOX7;
        static const long ID_STATICTEXT29;
        static const long ID_CHOICE16;
        static const long ID_STATICTEXT26;
        static const long ID_TEXTCTRL8;
        static const long ID_CHECKBOX15;
        static const long ID_PANEL1;
        static const long ID_STATICTEXT1;
        static const long ID_CHOICE1;
        static const long ID_BUTTON11;
        static const long ID_STATICTEXT2;
        static const long ID_TEXTCTRL1;
        static const long ID_STATICTEXT3;
        static const long ID_BUTTON1;
        static const long ID_STATICTEXT4;
        static const long ID_CHOICE2;
        static const long ID_STATICTEXT5;
        static const long ID_CHOICE3;
        static const long ID_CHECKBOX1;
        static const long ID_CHECKBOX2;
        static const long ID_CHECKBOX3;
        static const long ID_CHECKBOX6;
        static const long ID_CHECKBOX14;
        static const long ID_STATICTEXT6;
        static const long ID_BUTTON2;
        static const long ID_STATICTEXT7;
        static const long ID_CHOICE4;
        static const long ID_STATICTEXT9;
        static const long ID_CHOICE5;
        static const long ID_STATICTEXT10;
        static const long ID_SPINCTRL2;
        static const long ID_STATICTEXT19;
        static const long ID_SPINCTRL1;
        static const long ID_CHECKBOX12;
        static const long ID_PANEL4;
        static const long ID_NOTEBOOK1;
        static const long ID_BUTTON3;
        static const long ID_BUTTON4;
        //*)

        //(*Declarations(MathPlotConfigDialog)
        wxBoxSizer* BoxSizer2;
        wxBoxSizer* sizerMain;
        wxButton* bApply;
        wxButton* bAxisPenColor;
        wxButton* bBGColor;
        wxButton* bClose;
        wxButton* bCoordBrushColor;
        wxButton* bDelSeries;
        wxButton* bFontAxis;
        wxButton* bFontLegend;
        wxButton* bFontTitle;
        wxButton* bLegendBrushColor;
        wxButton* bSeriesBrushColor;
        wxButton* bSeriesPenColor;
        wxCheckBox* cbAutoScale;
        wxCheckBox* cbAxisOutside;
        wxCheckBox* cbAxisVisible;
        wxCheckBox* cbBar;
        wxCheckBox* cbCoordVisible;
        wxCheckBox* cbCoordinates;
        wxCheckBox* cbDrawBox;
        wxCheckBox* cbLegendVisible;
        wxCheckBox* cbLogAxis;
        wxCheckBox* cbMagnetize;
        wxCheckBox* cbSeriesContinuity;
        wxCheckBox* cbSeriesOutside;
        wxCheckBox* cbSeriesShowName;
        wxCheckBox* cbSeriesVisible;
        wxCheckBox* cbTitleVisible;
        wxCheckBox* cbTractable;
        wxChoice* ChoiceAxis;
        wxChoice* ChoiceSeries;
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
        wxChoice* cbSeriesBrushStyle;
        wxChoice* cbSeriesPenStyle;
        wxChoice* cbSeriesPenWidth;
        wxChoice* cbSeriesSymbolType;
        wxNotebook* nbConfig;
        wxPanel* Panel1;
        wxPanel* Panel2;
        wxPanel* Panel3;
        wxPanel* Panel4;
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
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText6;
        wxStaticText* StaticText7;
        wxStaticText* StaticText8;
        wxStaticText* StaticText9;
        wxTextCtrl* edAxisName;
        wxTextCtrl* edFormat;
        wxTextCtrl* edMarginBottom;
        wxTextCtrl* edMarginLeft;
        wxTextCtrl* edMarginRight;
        wxTextCtrl* edMarginTop;
        wxTextCtrl* edScaleMax;
        wxTextCtrl* edScaleMin;
        wxTextCtrl* edSeriesName;
        wxTextCtrl* edTitle;
        //*)

        void UpdateSelectedSerie(void);
        void UpdateAxis(void);

        void DoApplyColour(const wxColour& colour);
        void UpdateFont(mpLayer *layer, wxButton *button, bool get_set);
        void SetFontChildren(wxButton* p, const wxFontData& fontdata);

        wxBrushStyle IdToBrushStyle(int id);
        int BrushStyleToId(wxBrushStyle style);

        DECLARE_EVENT_TABLE()
};

} // MathPlot namespace

#endif // MATHPLOTCONFIGMAIN_H
