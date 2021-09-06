/***************************************************************
 * Name:      MathPlotConfig.cpp
 * Purpose:   Code for Application Frame
 * Author:    Lionel ()
 * Created:   2021-01-24
 * Copyright: Lionel ()
 * License:
 **************************************************************/

#include "MathPlotConfig.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(MathPlotConfigDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(MathPlotConfigDialog)
const long MathPlotConfigDialog::ID_STATICBOX3 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT11 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL3 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON5 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX8 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL2 = wxNewId();
const long MathPlotConfigDialog::ID_STATICBOX5 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT15 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL4 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL5 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL6 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL7 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON7 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX5 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL11 = wxNewId();
const long MathPlotConfigDialog::ID_STATICBOX8 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT18 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE11 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX10 = wxNewId();
const long MathPlotConfigDialog::ID_STATICBOX10 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT22 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT23 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON10 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE13 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL14 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL12 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL8 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL3 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT16 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE10 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX9 = wxNewId();
const long MathPlotConfigDialog::ID_STATICBOX9 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT20 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT21 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON9 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE12 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL13 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT24 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE14 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT25 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE15 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL16 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL15 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE7 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT8 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL2 = wxNewId();
const long MathPlotConfigDialog::ID_STATICBOX6 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT12 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT13 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT14 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON6 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE8 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE9 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL10 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX4 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON8 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT17 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE6 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX7 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL9 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL1 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT1 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE1 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX1 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT2 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL1 = wxNewId();
const long MathPlotConfigDialog::ID_STATICBOX1 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT3 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT4 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT5 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON1 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE2 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE3 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL5 = wxNewId();
const long MathPlotConfigDialog::ID_STATICBOX2 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT6 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT7 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON2 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE4 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL6 = wxNewId();
const long MathPlotConfigDialog::ID_STATICBOX4 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT9 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT10 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE5 = wxNewId();
const long MathPlotConfigDialog::ID_SPINCTRL2 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL7 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX2 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX3 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX6 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT19 = wxNewId();
const long MathPlotConfigDialog::ID_SPINCTRL1 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL4 = wxNewId();
const long MathPlotConfigDialog::ID_NOTEBOOK1 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON3 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON4 = wxNewId();
//*)

const wxString XAxis_Align[] = {"Bottom border", "Bottom", "Center", "Top", "Top border"};
const wxString YAxis_Align[] = {"Left border", "Left", "Center", "Right", "Right border"};

BEGIN_EVENT_TABLE(MathPlotConfigDialog,wxDialog)
    //(*EventTable(MathPlotConfigDialog)
    //*)
END_EVENT_TABLE()

MathPlotConfigDialog::MathPlotConfigDialog(wxWindow* parent,wxWindowID WXUNUSED(id))
{
    //(*Initialize(MathPlotConfigDialog)
    Create(parent, wxID_ANY, _("MathPlot Configuration"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(390,356));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    nbConfig = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxSize(383,400), 0, _T("ID_NOTEBOOK1"));
    Panel3 = new wxPanel(nbConfig, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    pGeneral = new wxPanel(Panel3, ID_PANEL8, wxPoint(0,0), wxSize(376,304), wxTAB_TRAVERSAL, _T("ID_PANEL8"));
    pTitle = new wxPanel(pGeneral, ID_PANEL2, wxPoint(0,0), wxSize(376,80), wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    StaticBox1 = new wxStaticBox(pTitle, ID_STATICBOX3, _("Title "), wxPoint(0,0), wxSize(364,80), 0, _T("ID_STATICBOX3"));
    StaticText11 = new wxStaticText(pTitle, ID_STATICTEXT11, _("Title :"), wxPoint(8,24), wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    edTitle = new wxTextCtrl(pTitle, ID_TEXTCTRL3, wxEmptyString, wxPoint(64,20), wxSize(220,23), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    bFontTitle = new wxButton(pTitle, ID_BUTTON5, _("Font"), wxPoint(292,20), wxSize(64,23), 0, wxDefaultValidator, _T("ID_BUTTON5"));
    bFontTitle->Disable();
    cbTitleVisible = new wxCheckBox(pTitle, ID_CHECKBOX8, _("Visible"), wxPoint(8,56), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX8"));
    cbTitleVisible->SetValue(false);
    pMargins = new wxPanel(pGeneral, ID_PANEL11, wxPoint(0,88), wxSize(376,80), wxTAB_TRAVERSAL, _T("ID_PANEL11"));
    StaticBox2 = new wxStaticBox(pMargins, ID_STATICBOX5, _("Margins "), wxPoint(0,0), wxSize(364,80), 0, _T("ID_STATICBOX5"));
    StaticText15 = new wxStaticText(pMargins, ID_STATICTEXT15, _("Margins :"), wxPoint(8,24), wxDefaultSize, 0, _T("ID_STATICTEXT15"));
    edMarginTop = new wxTextCtrl(pMargins, ID_TEXTCTRL4, _("50"), wxPoint(64,20), wxSize(40,23), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL4"));
    edMarginTop->SetToolTip(_("Top"));
    edMarginBottom = new wxTextCtrl(pMargins, ID_TEXTCTRL5, _("50"), wxPoint(112,20), wxSize(40,23), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL5"));
    edMarginBottom->SetToolTip(_("Bottom"));
    edMarginLeft = new wxTextCtrl(pMargins, ID_TEXTCTRL6, _("50"), wxPoint(160,20), wxSize(40,23), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL6"));
    edMarginLeft->SetToolTip(_("Left"));
    edMarginRight = new wxTextCtrl(pMargins, ID_TEXTCTRL7, _("50"), wxPoint(208,20), wxSize(40,23), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL7"));
    edMarginRight->SetToolTip(_("Right"));
    bBGColor = new wxButton(pMargins, ID_BUTTON7, _("bg color"), wxPoint(272,20), wxSize(72,23), 0, wxDefaultValidator, _T("ID_BUTTON7"));
    cbDrawBox = new wxCheckBox(pMargins, ID_CHECKBOX5, _("Draw box"), wxPoint(8,56), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
    cbDrawBox->SetValue(false);
    pCoord = new wxPanel(pGeneral, ID_PANEL12, wxPoint(0,176), wxSize(376,120), wxTAB_TRAVERSAL, _T("ID_PANEL12"));
    StaticBox4 = new wxStaticBox(pCoord, ID_STATICBOX8, _("Coord "), wxPoint(0,0), wxSize(360,104), 0, _T("ID_STATICBOX8"));
    StaticText18 = new wxStaticText(pCoord, ID_STATICTEXT18, _("Position :"), wxPoint(8,24), wxDefaultSize, 0, _T("ID_STATICTEXT18"));
    cbCoord = new wxChoice(pCoord, ID_CHOICE11, wxPoint(64,20), wxSize(112,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE11"));
    cbCoord->Append(_("Left center"));
    cbCoord->Append(_("Top left"));
    cbCoord->Append(_("Top center"));
    cbCoord->Append(_("Top right"));
    cbCoord->Append(_("Right center"));
    cbCoord->Append(_("Bottom left"));
    cbCoord->Append(_("Bottom center"));
    cbCoord->Append(_("Bottom right"));
    cbCoord->Append(_("Default position"));
    cbCoord->Append(_("Cursor position"));
    cbCoordVisible = new wxCheckBox(pCoord, ID_CHECKBOX10, _("Visible"), wxPoint(8,56), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX10"));
    cbCoordVisible->SetValue(false);
    pCoordBrush = new wxPanel(pCoord, ID_PANEL14, wxPoint(192,12), wxSize(160,84), wxTAB_TRAVERSAL, _T("ID_PANEL14"));
    sbCoordBrush = new wxStaticBox(pCoordBrush, ID_STATICBOX10, _("Brush "), wxPoint(0,0), wxSize(160,80), 0, _T("ID_STATICBOX10"));
    StaticText22 = new wxStaticText(pCoordBrush, ID_STATICTEXT22, _("Color :"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT22"));
    StaticText23 = new wxStaticText(pCoordBrush, ID_STATICTEXT23, _("Style :"), wxPoint(16,48), wxDefaultSize, 0, _T("ID_STATICTEXT23"));
    bCoordBrushColor = new wxButton(pCoordBrush, ID_BUTTON10, wxEmptyString, wxPoint(62,20), wxSize(80,23), 0, wxDefaultValidator, _T("ID_BUTTON10"));
    cbCoordBrushStyle = new wxChoice(pCoordBrush, ID_CHOICE13, wxPoint(62,44), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE13"));
    cbCoordBrushStyle->Append(_("Solid"));
    cbCoordBrushStyle->Append(_("Transparent"));
    Panel2 = new wxPanel(nbConfig, ID_PANEL15, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL15"));
    pLegend = new wxPanel(Panel2, ID_PANEL16, wxPoint(0,0), wxSize(368,184), wxTAB_TRAVERSAL, _T("ID_PANEL16"));
    StaticText16 = new wxStaticText(pLegend, ID_STATICTEXT16, _("Position :"), wxPoint(8,24), wxDefaultSize, 0, _T("ID_STATICTEXT16"));
    cbLegendPosition = new wxChoice(pLegend, ID_CHOICE10, wxPoint(68,20), wxSize(108,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE10"));
    cbLegendPosition->Append(_("Left center"));
    cbLegendPosition->Append(_("Top left"));
    cbLegendPosition->Append(_("Top center"));
    cbLegendPosition->Append(_("Top right"));
    cbLegendPosition->Append(_("Right center"));
    cbLegendPosition->Append(_("Bottom left"));
    cbLegendPosition->Append(_("Bottom center"));
    cbLegendPosition->Append(_("Bottom right"));
    cbLegendPosition->Append(_("Default position"));
    cbLegendVisible = new wxCheckBox(pLegend, ID_CHECKBOX9, _("Visible"), wxPoint(8,120), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX9"));
    cbLegendVisible->SetValue(false);
    pLegendBrush = new wxPanel(pLegend, ID_PANEL13, wxPoint(192,12), wxSize(160,84), wxTAB_TRAVERSAL, _T("ID_PANEL13"));
    sbLegendBrush = new wxStaticBox(pLegendBrush, ID_STATICBOX9, _("Brush "), wxPoint(0,0), wxSize(160,80), 0, _T("ID_STATICBOX9"));
    StaticText20 = new wxStaticText(pLegendBrush, ID_STATICTEXT20, _("Color :"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT20"));
    StaticText21 = new wxStaticText(pLegendBrush, ID_STATICTEXT21, _("Style :"), wxPoint(16,48), wxDefaultSize, 0, _T("ID_STATICTEXT21"));
    bLegendBrushColor = new wxButton(pLegendBrush, ID_BUTTON9, wxEmptyString, wxPoint(62,20), wxSize(80,23), 0, wxDefaultValidator, _T("ID_BUTTON9"));
    cbLegendBrushStyle = new wxChoice(pLegendBrush, ID_CHOICE12, wxPoint(62,44), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE12"));
    cbLegendBrushStyle->Append(_("Solid"));
    cbLegendBrushStyle->Append(_("Transparent"));
    StaticText24 = new wxStaticText(pLegend, ID_STATICTEXT24, _("Style :"), wxPoint(8,56), wxDefaultSize, 0, _T("ID_STATICTEXT24"));
    cbLegendStyle = new wxChoice(pLegend, ID_CHOICE14, wxPoint(68,52), wxSize(108,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE14"));
    cbLegendStyle->Append(_("Line"));
    cbLegendStyle->Append(_("Square"));
    StaticText25 = new wxStaticText(pLegend, ID_STATICTEXT25, _("Direction :"), wxPoint(8,88), wxDefaultSize, 0, _T("ID_STATICTEXT25"));
    cbLegendDirection = new wxChoice(pLegend, ID_CHOICE15, wxPoint(68,84), wxSize(108,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE15"));
    cbLegendDirection->Append(_("Vertical"));
    cbLegendDirection->Append(_("Horizontal"));
    Panel1 = new wxPanel(nbConfig, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    pAxis = new wxPanel(Panel1, ID_PANEL9, wxPoint(0,0), wxSize(368,208), wxTAB_TRAVERSAL, _T("ID_PANEL9"));
    ChoiceAxis = new wxChoice(pAxis, ID_CHOICE7, wxPoint(8,16), wxSize(160,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE7"));
    ChoiceAxis->Append(_("X axis"));
    ChoiceAxis->Append(_("Y axis"));
    StaticText8 = new wxStaticText(pAxis, ID_STATICTEXT8, _("Name :"), wxPoint(8,56), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    edAxisName = new wxTextCtrl(pAxis, ID_TEXTCTRL2, wxEmptyString, wxPoint(56,52), wxSize(232,23), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    pAxisPen = new wxPanel(pAxis, ID_PANEL10, wxPoint(8,88), wxSize(168,112), wxTAB_TRAVERSAL, _T("ID_PANEL10"));
    sbAxisPen = new wxStaticBox(pAxisPen, ID_STATICBOX6, _("Pen "), wxPoint(0,0), wxSize(160,104), 0, _T("ID_STATICBOX6"));
    StaticText12 = new wxStaticText(pAxisPen, ID_STATICTEXT12, _("Color :"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    StaticText13 = new wxStaticText(pAxisPen, ID_STATICTEXT13, _("Width :"), wxPoint(16,48), wxDefaultSize, 0, _T("ID_STATICTEXT13"));
    StaticText14 = new wxStaticText(pAxisPen, ID_STATICTEXT14, _("Style :"), wxPoint(16,72), wxDefaultSize, 0, _T("ID_STATICTEXT14"));
    bAxisPenColor = new wxButton(pAxisPen, ID_BUTTON6, wxEmptyString, wxPoint(62,20), wxSize(80,23), 0, wxDefaultValidator, _T("ID_BUTTON6"));
    cbAxisPenWidth = new wxChoice(pAxisPen, ID_CHOICE8, wxPoint(62,44), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE8"));
    cbAxisPenWidth->Append(_("1"));
    cbAxisPenWidth->Append(_("2"));
    cbAxisPenWidth->Append(_("3"));
    cbAxisPenWidth->Append(_("4"));
    cbAxisPenWidth->Append(_("5"));
    cbAxisPenWidth->Append(_("6"));
    cbAxisPenWidth->Append(_("7"));
    cbAxisPenWidth->Append(_("8"));
    cbAxisPenWidth->Append(_("9"));
    cbAxisPenWidth->Append(_("10"));
    cbAxisPenStyle = new wxChoice(pAxisPen, ID_CHOICE9, wxPoint(62,68), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE9"));
    cbAxisPenStyle->Append(_("Solid"));
    cbAxisPenStyle->Append(_("Dot"));
    cbAxisPenStyle->Append(_("Long Dash"));
    cbAxisPenStyle->Append(_("Short Dash"));
    cbAxisPenStyle->Append(_("Dot Dash"));
    cbAxisVisible = new wxCheckBox(pAxis, ID_CHECKBOX4, _("Visible"), wxPoint(192,152), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
    cbAxisVisible->SetValue(false);
    bFontAxis = new wxButton(pAxis, ID_BUTTON8, _("Font"), wxPoint(296,52), wxSize(64,23), 0, wxDefaultValidator, _T("ID_BUTTON8"));
    bFontAxis->Disable();
    StaticText17 = new wxStaticText(pAxis, ID_STATICTEXT17, _("Position :"), wxPoint(192,88), wxDefaultSize, 0, _T("ID_STATICTEXT17"));
    cbAxisPosition = new wxChoice(pAxis, ID_CHOICE6, wxPoint(192,112), wxSize(136,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE6"));
    cbAxisOutside = new wxCheckBox(pAxis, ID_CHECKBOX7, _("Draw Outside Margins"), wxPoint(192,176), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX7"));
    cbAxisOutside->SetValue(false);
    Panel4 = new wxPanel(nbConfig, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    StaticText1 = new wxStaticText(Panel4, ID_STATICTEXT1, _("Series number :"), wxPoint(8,16), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    ChoiceSeries = new wxChoice(Panel4, ID_CHOICE1, wxPoint(112,12), wxSize(56,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    ChoiceSeries->Append(_("0"));
    ChoiceSeries->Append(_("1"));
    cbSeriesVisible = new wxCheckBox(Panel4, ID_CHECKBOX1, _("Visible"), wxPoint(8,192), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    cbSeriesVisible->SetValue(false);
    StaticText2 = new wxStaticText(Panel4, ID_STATICTEXT2, _("Name :"), wxPoint(8,48), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    edSeriesName = new wxTextCtrl(Panel4, ID_TEXTCTRL1, wxEmptyString, wxPoint(64,44), wxSize(296,23), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    pSeriesPen = new wxPanel(Panel4, ID_PANEL5, wxPoint(8,76), wxSize(160,108), wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    sbSeriesPen = new wxStaticBox(pSeriesPen, ID_STATICBOX1, _("Pen "), wxPoint(0,0), wxSize(160,104), 0, _T("ID_STATICBOX1"));
    StaticText3 = new wxStaticText(pSeriesPen, ID_STATICTEXT3, _("Color :"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    StaticText4 = new wxStaticText(pSeriesPen, ID_STATICTEXT4, _("Width :"), wxPoint(16,48), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    StaticText5 = new wxStaticText(pSeriesPen, ID_STATICTEXT5, _("Style :"), wxPoint(16,72), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    bSeriesPenColor = new wxButton(pSeriesPen, ID_BUTTON1, wxEmptyString, wxPoint(62,20), wxSize(80,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
    cbSeriesPenWidth = new wxChoice(pSeriesPen, ID_CHOICE2, wxPoint(62,44), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
    cbSeriesPenWidth->Append(_("1"));
    cbSeriesPenWidth->Append(_("2"));
    cbSeriesPenWidth->Append(_("3"));
    cbSeriesPenWidth->Append(_("4"));
    cbSeriesPenWidth->Append(_("5"));
    cbSeriesPenWidth->Append(_("6"));
    cbSeriesPenWidth->Append(_("7"));
    cbSeriesPenWidth->Append(_("8"));
    cbSeriesPenWidth->Append(_("9"));
    cbSeriesPenWidth->Append(_("10"));
    cbSeriesPenStyle = new wxChoice(pSeriesPen, ID_CHOICE3, wxPoint(62,68), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE3"));
    cbSeriesPenStyle->Append(_("Solid"));
    cbSeriesPenStyle->Append(_("Dot"));
    cbSeriesPenStyle->Append(_("Long Dash"));
    cbSeriesPenStyle->Append(_("Short Dash"));
    cbSeriesPenStyle->Append(_("Dot Dash"));
    pSeriesBrush = new wxPanel(Panel4, ID_PANEL6, wxPoint(200,76), wxSize(160,84), wxTAB_TRAVERSAL, _T("ID_PANEL6"));
    sbSeriesBrush = new wxStaticBox(pSeriesBrush, ID_STATICBOX2, _("Brush "), wxPoint(0,0), wxSize(160,80), 0, _T("ID_STATICBOX2"));
    StaticText6 = new wxStaticText(pSeriesBrush, ID_STATICTEXT6, _("Color :"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    StaticText7 = new wxStaticText(pSeriesBrush, ID_STATICTEXT7, _("Style :"), wxPoint(16,48), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    bSeriesBrushColor = new wxButton(pSeriesBrush, ID_BUTTON2, wxEmptyString, wxPoint(62,20), wxSize(80,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    cbSeriesBrushStyle = new wxChoice(pSeriesBrush, ID_CHOICE4, wxPoint(62,44), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE4"));
    cbSeriesBrushStyle->Append(_("Solid"));
    cbSeriesBrushStyle->Append(_("Transparent"));
    cbSeriesBrushStyle->Append(_("BDiagonal"));
    cbSeriesBrushStyle->Append(_("CrossDiagonal"));
    cbSeriesBrushStyle->Append(_("FDiagonal"));
    cbSeriesBrushStyle->Append(_("Cross"));
    cbSeriesBrushStyle->Append(_("Horizontal"));
    cbSeriesBrushStyle->Append(_("Vertical"));
    pSeriesSymbol = new wxPanel(Panel4, ID_PANEL7, wxPoint(200,164), wxSize(160,84), wxTAB_TRAVERSAL, _T("ID_PANEL7"));
    sbSeriesSymbol = new wxStaticBox(pSeriesSymbol, ID_STATICBOX4, _("Symbol "), wxPoint(0,0), wxSize(160,80), 0, _T("ID_STATICBOX4"));
    StaticText9 = new wxStaticText(pSeriesSymbol, ID_STATICTEXT9, _("Type :"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    StaticText10 = new wxStaticText(pSeriesSymbol, ID_STATICTEXT10, _("Size :"), wxPoint(16,48), wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    cbSeriesSymbolType = new wxChoice(pSeriesSymbol, ID_CHOICE5, wxPoint(62,20), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE5"));
    cbSeriesSymbolType->Append(_("None"));
    cbSeriesSymbolType->Append(_("Circle"));
    cbSeriesSymbolType->Append(_("Square"));
    cbSeriesSymbolType->Append(_("UpTriangle"));
    cbSeriesSymbolType->Append(_("DownTriangle"));
    cbSeriesSymbolType->Append(_("Cross"));
    cbSeriesSymbolType->Append(_("Plus"));
    cbSeriesSymbolSize = new wxSpinCtrl(pSeriesSymbol, ID_SPINCTRL2, _T("4"), wxPoint(96,48), wxSize(44,23), 0, 4, 100, 4, _T("ID_SPINCTRL2"));
    cbSeriesSymbolSize->SetValue(_T("4"));
    cbSeriesContinuity = new wxCheckBox(Panel4, ID_CHECKBOX2, _("Continuity"), wxPoint(8,216), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    cbSeriesContinuity->SetValue(false);
    cbSeriesOutside = new wxCheckBox(Panel4, ID_CHECKBOX3, _("Draw Outside Margins"), wxPoint(8,240), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
    cbSeriesOutside->SetValue(false);
    cbSeriesShowName = new wxCheckBox(Panel4, ID_CHECKBOX6, _("Show name"), wxPoint(8,264), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX6"));
    cbSeriesShowName->SetValue(false);
    StaticText19 = new wxStaticText(Panel4, ID_STATICTEXT19, _("Step :"), wxPoint(208,264), wxDefaultSize, 0, _T("ID_STATICTEXT19"));
    cbSeriesStep = new wxSpinCtrl(Panel4, ID_SPINCTRL1, _T("1"), wxPoint(296,260), wxSize(46,23), 0, 1, 100, 1, _T("ID_SPINCTRL1"));
    cbSeriesStep->SetValue(_T("1"));
    nbConfig->AddPage(Panel3, _("General"), false);
    nbConfig->AddPage(Panel2, _("Legend"), false);
    nbConfig->AddPage(Panel1, _("Axis"), false);
    nbConfig->AddPage(Panel4, _("Series"), false);
    BoxSizer1->Add(nbConfig, 1, wxALL|wxEXPAND, 5);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    bApply = new wxButton(this, ID_BUTTON3, _("Apply"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    BoxSizer2->Add(bApply, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    bClose = new wxButton(this, ID_BUTTON4, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    BoxSizer2->Add(bClose, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    BoxSizer1->Add(BoxSizer2, 0, wxEXPAND, 4);
    SetSizer(BoxSizer1);
    ColourDialog = new wxColourDialog(this);
    	wxFontData fontData_1;
    	fontData_1.SetInitialFont(*wxNORMAL_FONT);
    	fontData_1.SetAllowSymbols(true);
    FontDialog = new wxFontDialog(this, fontData_1);
    SetSizer(BoxSizer1);
    Layout();

    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MathPlotConfigDialog::OnbFontClick));
    Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MathPlotConfigDialog::OnbColorClick));
    Connect(ID_BUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MathPlotConfigDialog::OnbColorClick));
    Connect(ID_BUTTON10,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MathPlotConfigDialog::OnbColorClick));
    Connect(ID_CHOICE7,wxEVT_COMMAND_CHOICE_SELECTED,wxCommandEventHandler(MathPlotConfigDialog::OnAxisSelect));
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MathPlotConfigDialog::OnbColorClick));
    Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MathPlotConfigDialog::OnbFontClick));
    Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,wxCommandEventHandler(MathPlotConfigDialog::OnChoiceSeries));
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MathPlotConfigDialog::OnbColorClick));
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MathPlotConfigDialog::OnbColorClick));
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MathPlotConfigDialog::OnbApplyClick));
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(MathPlotConfigDialog::OnQuit));
    //*)

    colourButton = NULL;
    CurrentTitle = NULL;
    CurrentChoice = NULL;
    CurrentSerie = NULL;
    // The plot window
//    m_plot = (mpWindow *)parent;
    m_plot = wxDynamicCast(parent, mpWindow);
    CurrentScale = NULL;
    fontTitleChanged = false;
    fontAxisChanged = false;
    Initialize();
}

MathPlotConfigDialog::~MathPlotConfigDialog()
{
    //(*Destroy(MathPlotConfigDialog)
    //*)
}

void MathPlotConfigDialog::Initialize()
{
	CurrentTitle = (mpText*)m_plot->GetLayerByClassName(_T("mpTitle"));
	if (CurrentTitle)
	{
		edTitle->SetValue(CurrentTitle->GetName());
		cbTitleVisible->SetValue(CurrentTitle->IsVisible());
		bFontTitle->SetFont(CurrentTitle->GetFont());
		bFontTitle->SetForegroundColour(CurrentTitle->GetFontColour());
		bFontTitle->Enable();
	}

	edMarginTop->SetValue(wxString::Format("%d", m_plot->GetMarginTop()));
	edMarginBottom->SetValue(wxString::Format("%d", m_plot->GetMarginBottom()));
	edMarginLeft->SetValue(wxString::Format("%d", m_plot->GetMarginLeft()));
	edMarginRight->SetValue(wxString::Format("%d", m_plot->GetMarginRight()));

	cbDrawBox->SetValue(m_plot->GetDrawBox());
	bBGColor->SetBackgroundColour(m_plot->GetbgColour());

	CurrentLegend = (mpInfoLegend*)m_plot->GetLayerByClassName(_T("mpInfoLegend"));
	if (CurrentLegend)
	{
		cbLegendPosition->SetSelection(CurrentLegend->GetLocation());
		cbLegendStyle->SetSelection(CurrentLegend->GetItemMode());
		cbLegendDirection->SetSelection(CurrentLegend->GetItemDirection());
		cbLegendVisible->SetValue(CurrentLegend->IsVisible());
		// Brush config
		bLegendBrushColor->SetBackgroundColour(CurrentLegend->GetBrush().GetColour());
		cbLegendBrushStyle->SetSelection(BrushStyleToId(CurrentLegend->GetBrush().GetStyle()));
	}

	CurrentCoords = (mpInfoCoords*)m_plot->GetLayerByClassName(_T("mpInfoCoords"));
	if (CurrentCoords)
	{
		cbCoord->SetSelection(CurrentCoords->GetLocation());
		cbCoordVisible->SetValue(CurrentCoords->IsVisible());
		// Brush config
		bCoordBrushColor->SetBackgroundColour(CurrentCoords->GetBrush().GetColour());
		cbCoordBrushStyle->SetSelection(BrushStyleToId(CurrentCoords->GetBrush().GetStyle()));
	}

	// Fill series counter
	ChoiceSeries->Clear();
	for (unsigned int i=0; i<m_plot->CountLayersPlot(); i++)
	{
	  ChoiceSeries->Append(wxString::Format(wxT("%i"), i));
	}
}

void MathPlotConfigDialog::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void MathPlotConfigDialog::OnbColorClick(wxCommandEvent& event)
{
	// Get the sender
	colourButton = wxDynamicCast(event.GetEventObject(), wxButton);

	wxColourData m_clrData;
	m_clrData.SetColour(colourButton->GetBackgroundColour());

	wxColourDialog ColourDialog(this, &m_clrData);

	ColourDialog.Bind(wxEVT_COLOUR_CHANGED, &MathPlotConfigDialog::OnColourChanged, this);
	ColourDialog.SetTitle("Please choose the background colour");
	if (ColourDialog.ShowModal() == wxID_OK)
	{
		m_clrData = ColourDialog.GetColourData();

		Refresh();
	}
	else  // Restore initial color
		DoApplyColour(m_clrData.GetColour());
}

void MathPlotConfigDialog::OnColourChanged(wxColourDialogEvent& event)
{
	DoApplyColour(event.GetColour());
}

void MathPlotConfigDialog::DoApplyColour(const wxColour& colour)
{
	if ( colour == colourButton->GetBackgroundColour() )
		return;

	colourButton->SetBackgroundColour(colour);
	colourButton->ClearBackground();
	colourButton->Refresh();
}

void MathPlotConfigDialog::OnbFontClick(wxCommandEvent& event)
{
	wxButton *fontButton = wxDynamicCast(event.GetEventObject(), wxButton);

	wxFontData retData;
  retData.SetInitialFont(fontButton->GetFont());
  retData.SetColour(fontButton->GetForegroundColour());

	wxFontDialog FontDialog(this, retData);

  if (FontDialog.ShowModal() == wxID_OK)
  {
  	retData = FontDialog.GetFontData();

  	SetFontChildren(fontButton, retData);
  	if (fontButton == bFontTitle)
  		fontTitleChanged = true;
  	if (fontButton == bFontAxis)
  		fontAxisChanged = true;
  }
}

void MathPlotConfigDialog::SetFontChildren(wxButton* p, const wxFontData& fontdata)
{
	wxFont font(fontdata.GetChosenFont());

	p->SetFont(font);
	p->SetForegroundColour(fontdata.GetColour());
}

void MathPlotConfigDialog::OnAxisSelect(wxCommandEvent& WXUNUSED(event))
{
	int i = 0;

	if (ChoiceAxis->GetSelection() == 0)
	{
		CurrentScale = (mpScale*)m_plot->GetLayerByClassName(_T("mpScaleX"));
		cbAxisPosition->Clear();
		for (i = 0; i <= mpALIGN_BORDER_TOP; i++)
			cbAxisPosition->Append(XAxis_Align[i]);
	}
	else
	{
		CurrentScale = (mpScale*)m_plot->GetLayerByClassName(_T("mpScaleY"));
		cbAxisPosition->Clear();
		for (i = 0; i <= mpALIGN_BORDER_RIGHT; i++)
			cbAxisPosition->Append(YAxis_Align[i]);
	}

	CurrentChoice = ChoiceAxis;

	if (CurrentScale)
	{
		edAxisName->SetValue(CurrentScale->GetName());
		// Pen config
		bAxisPenColor->SetBackgroundColour(CurrentScale->GetPen().GetColour());
		cbAxisPenWidth->SetSelection(CurrentScale->GetPen().GetWidth() - 1);
		cbAxisPenStyle->SetSelection(CurrentScale->GetPen().GetStyle() - wxPENSTYLE_SOLID);
		cbAxisVisible->SetValue(CurrentScale->IsVisible());
		cbAxisPosition->SetSelection(CurrentScale->GetAlign());

		cbAxisOutside->SetValue(CurrentScale->GetDrawOutsideMargins());

		bFontAxis->SetFont(CurrentScale->GetFont());
		bFontAxis->SetForegroundColour(CurrentScale->GetFontColour());
		bFontAxis->Enable();
	}
}

wxBrushStyle MathPlotConfigDialog::IdToBrushStyle(int id)
{
	if (id == 0)
		return wxBRUSHSTYLE_SOLID;
	else
		if (id == 1)
			return wxBRUSHSTYLE_TRANSPARENT;
		else
		{
			return (wxBrushStyle)(wxBRUSHSTYLE_BDIAGONAL_HATCH + (id - 2));
		}
}

int MathPlotConfigDialog::BrushStyleToId(wxBrushStyle style)
{
	if (style == wxBRUSHSTYLE_SOLID)
		return 0;
	else
		if (style == wxBRUSHSTYLE_TRANSPARENT)
			return 1;
		else
		{
			return style - wxBRUSHSTYLE_BDIAGONAL_HATCH + 2;
		}
}

void MathPlotConfigDialog::OnChoiceSeries(wxCommandEvent& WXUNUSED(event))
{
	CurrentSerie = (mpLayer*)m_plot->GetLayerPlot(ChoiceSeries->GetSelection());

	CurrentChoice = ChoiceSeries;

	if (CurrentSerie)
	{
		edSeriesName->SetValue(CurrentSerie->GetName());
		// Pen config
		bSeriesPenColor->SetBackgroundColour(CurrentSerie->GetPen().GetColour());
		cbSeriesPenWidth->SetSelection(CurrentSerie->GetPen().GetWidth() - 1);
		cbSeriesPenStyle->SetSelection(CurrentSerie->GetPen().GetStyle() - wxPENSTYLE_SOLID);
		// Brush config
		bSeriesBrushColor->SetBackgroundColour(CurrentSerie->GetBrush().GetColour());
		cbSeriesBrushStyle->SetSelection(BrushStyleToId(CurrentSerie->GetBrush().GetStyle()));
		// Symbol config
		cbSeriesSymbolType->SetSelection(CurrentSerie->GetSymbol());
		cbSeriesSymbolSize->SetValue(CurrentSerie->GetSymbolSize());

		cbSeriesVisible->SetValue(CurrentSerie->IsVisible());
		cbSeriesContinuity->SetValue(CurrentSerie->GetContinuity());
		cbSeriesOutside->SetValue(CurrentSerie->GetDrawOutsideMargins());
		cbSeriesShowName->SetValue(CurrentSerie->GetShowName());

		cbSeriesStep->SetValue(CurrentSerie->GetStep());
	}
}

void MathPlotConfigDialog::OnbApplyClick(wxCommandEvent& WXUNUSED(event))
{
	switch (nbConfig->GetSelection())
	{
		case 0 :
		{
			// General
			if (CurrentTitle)
			{
			  CurrentTitle->SetName(edTitle->GetValue());
			  CurrentTitle->SetVisible(cbTitleVisible->GetValue());
			  if (fontTitleChanged)
			  {
			  	CurrentTitle->SetFont(bFontTitle->GetFont());
			  	CurrentTitle->SetFontColour(bFontTitle->GetForegroundColour());
			  	fontTitleChanged = false;
			  }
			}

			long top, bottom, left, right;
			edMarginTop->GetValue().ToLong(&top);
			edMarginRight->GetValue().ToLong(&right);
			edMarginBottom->GetValue().ToLong(&bottom);
			edMarginLeft->GetValue().ToLong(&left);
			m_plot->SetMargins(top, right, bottom, left);

			m_plot->SetDrawBox(cbDrawBox->GetValue());
			m_plot->SetbgColour(bBGColor->GetBackgroundColour());

			if (CurrentCoords)
			{
				CurrentCoords->SetLocation((mpLocation)cbCoord->GetSelection());
				CurrentCoords->SetVisible(cbCoordVisible->GetValue());
				// Brush config
				wxBrush brush(bCoordBrushColor->GetBackgroundColour(), IdToBrushStyle(cbCoordBrushStyle->GetSelection()));
				CurrentCoords->SetBrush(brush);
			}

			m_plot->UpdateAll();
			break;
		}
		case 1 : // Legend page
			if (CurrentLegend)
			{
				CurrentLegend->SetLocation((mpLocation)cbLegendPosition->GetSelection());
				CurrentLegend->SetVisible(cbLegendVisible->GetValue());
				CurrentLegend->SetItemMode((mpLegendStyle)cbLegendStyle->GetSelection());
				CurrentLegend->SetItemDirection((mpLegendDirection)cbLegendDirection->GetSelection());
				// Brush config
				wxBrush brush(bLegendBrushColor->GetBackgroundColour(), IdToBrushStyle(cbLegendBrushStyle->GetSelection()));
				CurrentLegend->SetBrush(brush);

				m_plot->UpdateAll();
			}
			break;

		case 2 : // Axis page
			if (CurrentChoice == ChoiceAxis)
			{
				CurrentScale->SetName(edAxisName->GetValue());
				// Pen config
				wxPen pen(bAxisPenColor->GetBackgroundColour(), cbAxisPenWidth->GetSelection() + 1,
						(wxPenStyle)(cbAxisPenStyle->GetSelection() + wxPENSTYLE_SOLID));
				CurrentScale->SetPen(pen);
				CurrentScale->SetVisible(cbAxisVisible->GetValue());
				CurrentScale->SetAlign(cbAxisPosition->GetSelection());
				CurrentScale->SetDrawOutsideMargins(cbAxisOutside->GetValue());

			  if (fontAxisChanged)
			  {
			  	CurrentScale->SetFont(bFontAxis->GetFont());
			  	CurrentScale->SetFontColour(bFontAxis->GetForegroundColour());
			  	fontAxisChanged = false;
			  }

				m_plot->UpdateAll();
			}
			break;

		case 3: // Series page
			if (CurrentChoice == ChoiceSeries)
			{
				CurrentSerie->SetName(edSeriesName->GetValue());
				// Pen config
				wxPen pen(bSeriesPenColor->GetBackgroundColour(), cbSeriesPenWidth->GetSelection() + 1,
						(wxPenStyle)(cbSeriesPenStyle->GetSelection() + wxPENSTYLE_SOLID));
				CurrentSerie->SetPen(pen);

				// Brush config
				wxBrush brush(bSeriesBrushColor->GetBackgroundColour(), IdToBrushStyle(cbSeriesBrushStyle->GetSelection()));
				CurrentSerie->SetBrush(brush);

				// Symbol config
				CurrentSerie->SetSymbol((mpSymbol)cbSeriesSymbolType->GetSelection());
				CurrentSerie->SetSymbolSize(cbSeriesSymbolSize->GetValue());

				CurrentSerie->SetVisible(cbSeriesVisible->GetValue());
				CurrentSerie->SetContinuity(cbSeriesContinuity->GetValue());
				CurrentSerie->SetDrawOutsideMargins(cbSeriesOutside->GetValue());
				CurrentSerie->SetShowName(cbSeriesShowName->GetValue());

				CurrentSerie->SetStep(cbSeriesStep->GetValue());

				if (CurrentLegend)
					CurrentLegend->SetNeedUpdate();

				m_plot->Fit();
			}
			break;
		default : ;
	}
}

