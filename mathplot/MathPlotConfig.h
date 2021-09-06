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

//(*Headers(MathPlotConfigDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/colordlg.h>
#include <wx/dialog.h>
#include <wx/fontdlg.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

#include "mathplot.h"

// MathPlot namespace
namespace MathPlot
{


class MathPlotConfigDialog: public wxDialog
{
    public:

        MathPlotConfigDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~MathPlotConfigDialog();

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
        bool fontAxisChanged;

        //(*Handlers(MathPlotConfigDialog)
        void OnQuit(wxCommandEvent& event);
        void OnbColorClick(wxCommandEvent& event);
        void OnChoiceSeries(wxCommandEvent& event);
        void OnAxisSelect(wxCommandEvent& event);
        void OnbApplyClick(wxCommandEvent& event);
        void OnbFontClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(MathPlotConfigDialog)
        static const long ID_STATICBOX3;
        static const long ID_STATICTEXT11;
        static const long ID_TEXTCTRL3;
        static const long ID_BUTTON5;
        static const long ID_CHECKBOX8;
        static const long ID_PANEL2;
        static const long ID_STATICBOX5;
        static const long ID_STATICTEXT15;
        static const long ID_TEXTCTRL4;
        static const long ID_TEXTCTRL5;
        static const long ID_TEXTCTRL6;
        static const long ID_TEXTCTRL7;
        static const long ID_BUTTON7;
        static const long ID_CHECKBOX5;
        static const long ID_PANEL11;
        static const long ID_STATICBOX8;
        static const long ID_STATICTEXT18;
        static const long ID_CHOICE11;
        static const long ID_CHECKBOX10;
        static const long ID_STATICBOX10;
        static const long ID_STATICTEXT22;
        static const long ID_STATICTEXT23;
        static const long ID_BUTTON10;
        static const long ID_CHOICE13;
        static const long ID_PANEL14;
        static const long ID_PANEL12;
        static const long ID_PANEL8;
        static const long ID_PANEL3;
        static const long ID_STATICTEXT16;
        static const long ID_CHOICE10;
        static const long ID_CHECKBOX9;
        static const long ID_STATICBOX9;
        static const long ID_STATICTEXT20;
        static const long ID_STATICTEXT21;
        static const long ID_BUTTON9;
        static const long ID_CHOICE12;
        static const long ID_PANEL13;
        static const long ID_STATICTEXT24;
        static const long ID_CHOICE14;
        static const long ID_STATICTEXT25;
        static const long ID_CHOICE15;
        static const long ID_PANEL16;
        static const long ID_PANEL15;
        static const long ID_CHOICE7;
        static const long ID_STATICTEXT8;
        static const long ID_TEXTCTRL2;
        static const long ID_STATICBOX6;
        static const long ID_STATICTEXT12;
        static const long ID_STATICTEXT13;
        static const long ID_STATICTEXT14;
        static const long ID_BUTTON6;
        static const long ID_CHOICE8;
        static const long ID_CHOICE9;
        static const long ID_PANEL10;
        static const long ID_CHECKBOX4;
        static const long ID_BUTTON8;
        static const long ID_STATICTEXT17;
        static const long ID_CHOICE6;
        static const long ID_CHECKBOX7;
        static const long ID_PANEL9;
        static const long ID_PANEL1;
        static const long ID_STATICTEXT1;
        static const long ID_CHOICE1;
        static const long ID_CHECKBOX1;
        static const long ID_STATICTEXT2;
        static const long ID_TEXTCTRL1;
        static const long ID_STATICBOX1;
        static const long ID_STATICTEXT3;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT5;
        static const long ID_BUTTON1;
        static const long ID_CHOICE2;
        static const long ID_CHOICE3;
        static const long ID_PANEL5;
        static const long ID_STATICBOX2;
        static const long ID_STATICTEXT6;
        static const long ID_STATICTEXT7;
        static const long ID_BUTTON2;
        static const long ID_CHOICE4;
        static const long ID_PANEL6;
        static const long ID_STATICBOX4;
        static const long ID_STATICTEXT9;
        static const long ID_STATICTEXT10;
        static const long ID_CHOICE5;
        static const long ID_SPINCTRL2;
        static const long ID_PANEL7;
        static const long ID_CHECKBOX2;
        static const long ID_CHECKBOX3;
        static const long ID_CHECKBOX6;
        static const long ID_STATICTEXT19;
        static const long ID_SPINCTRL1;
        static const long ID_PANEL4;
        static const long ID_NOTEBOOK1;
        static const long ID_BUTTON3;
        static const long ID_BUTTON4;
        //*)

        //(*Declarations(MathPlotConfigDialog)
        wxBoxSizer* BoxSizer1;
        wxBoxSizer* BoxSizer2;
        wxButton* bApply;
        wxButton* bAxisPenColor;
        wxButton* bBGColor;
        wxButton* bClose;
        wxButton* bCoordBrushColor;
        wxButton* bFontAxis;
        wxButton* bFontTitle;
        wxButton* bLegendBrushColor;
        wxButton* bSeriesBrushColor;
        wxButton* bSeriesPenColor;
        wxCheckBox* cbAxisOutside;
        wxCheckBox* cbAxisVisible;
        wxCheckBox* cbCoordVisible;
        wxCheckBox* cbDrawBox;
        wxCheckBox* cbLegendVisible;
        wxCheckBox* cbSeriesContinuity;
        wxCheckBox* cbSeriesOutside;
        wxCheckBox* cbSeriesShowName;
        wxCheckBox* cbSeriesVisible;
        wxCheckBox* cbTitleVisible;
        wxChoice* ChoiceAxis;
        wxChoice* ChoiceSeries;
        wxChoice* cbAxisPenStyle;
        wxChoice* cbAxisPenWidth;
        wxChoice* cbAxisPosition;
        wxChoice* cbCoord;
        wxChoice* cbCoordBrushStyle;
        wxChoice* cbLegendBrushStyle;
        wxChoice* cbLegendDirection;
        wxChoice* cbLegendPosition;
        wxChoice* cbLegendStyle;
        wxChoice* cbSeriesBrushStyle;
        wxChoice* cbSeriesPenStyle;
        wxChoice* cbSeriesPenWidth;
        wxChoice* cbSeriesSymbolType;
        wxColourDialog* ColourDialog;
        wxFontDialog* FontDialog;
        wxNotebook* nbConfig;
        wxPanel* Panel1;
        wxPanel* Panel2;
        wxPanel* Panel3;
        wxPanel* Panel4;
        wxPanel* pAxis;
        wxPanel* pAxisPen;
        wxPanel* pCoord;
        wxPanel* pCoordBrush;
        wxPanel* pGeneral;
        wxPanel* pLegend;
        wxPanel* pLegendBrush;
        wxPanel* pMargins;
        wxPanel* pSeriesBrush;
        wxPanel* pSeriesPen;
        wxPanel* pSeriesSymbol;
        wxPanel* pTitle;
        wxSpinCtrl* cbSeriesStep;
        wxSpinCtrl* cbSeriesSymbolSize;
        wxStaticBox* StaticBox1;
        wxStaticBox* StaticBox2;
        wxStaticBox* StaticBox4;
        wxStaticBox* sbAxisPen;
        wxStaticBox* sbCoordBrush;
        wxStaticBox* sbLegendBrush;
        wxStaticBox* sbSeriesBrush;
        wxStaticBox* sbSeriesPen;
        wxStaticBox* sbSeriesSymbol;
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
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText6;
        wxStaticText* StaticText7;
        wxStaticText* StaticText8;
        wxStaticText* StaticText9;
        wxTextCtrl* edAxisName;
        wxTextCtrl* edMarginBottom;
        wxTextCtrl* edMarginLeft;
        wxTextCtrl* edMarginRight;
        wxTextCtrl* edMarginTop;
        wxTextCtrl* edSeriesName;
        wxTextCtrl* edTitle;
        //*)

        void DoApplyColour(const wxColour& colour);
        void OnColourChanged(wxColourDialogEvent& event);
        void SetFontChildren(wxButton* p, const wxFontData& fontdata);
        void Initialize();

        wxBrushStyle IdToBrushStyle(int id);
        int BrushStyleToId(wxBrushStyle style);

        DECLARE_EVENT_TABLE()
};

}  // namespace MathPlot

#endif // MATHPLOTCONFIGMAIN_H
