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
#include <wx/string.h>
//*)

//(*IdInit(MathPlotConfigDialog)
const long MathPlotConfigDialog::ID_STATICTEXT11 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL3 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON5 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX8 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL2 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT15 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL4 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL5 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL6 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL7 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON7 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX5 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL11 = wxNewId();
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
const long MathPlotConfigDialog::ID_PANEL17 = wxNewId();
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
const long MathPlotConfigDialog::ID_STATICTEXT26 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL8 = wxNewId();
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
const long MathPlotConfigDialog::ID_BUTTON11 = wxNewId();
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

MathPlotConfigDialog::MathPlotConfigDialog(wxWindow *parent, wxWindowID WXUNUSED(id))
{
	//(*Initialize(MathPlotConfigDialog)
	wxBoxSizer* BoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;

	Create(parent, wxID_ANY, _T("MathPlot Configuration"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	SetClientSize(wxSize(390,356));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	nbConfig = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxSize(383,400), 0, _T("ID_NOTEBOOK1"));
	Panel1 = new wxPanel(nbConfig, ID_PANEL17, wxPoint(239,4), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL17"));
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _T("Title "));
	pTitle = new wxPanel(Panel1, ID_PANEL2, wxDefaultPosition, wxSize(376,64), wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	StaticText11 = new wxStaticText(pTitle, ID_STATICTEXT11, _T("Title :"), wxPoint(8,10), wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	edTitle = new wxTextCtrl(pTitle, ID_TEXTCTRL3, wxEmptyString, wxPoint(64,6), wxSize(216,23), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	bFontTitle = new wxButton(pTitle, ID_BUTTON5, _T("Font"), wxPoint(288,6), wxSize(64,23), 0, wxDefaultValidator, _T("ID_BUTTON5"));
	bFontTitle->Disable();
	cbTitleVisible = new wxCheckBox(pTitle, ID_CHECKBOX8, _T("Visible"), wxPoint(8,42), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX8"));
	cbTitleVisible->SetValue(false);
	StaticBoxSizer1->Add(pTitle, 1, wxALL|wxEXPAND, 0);
	BoxSizer3->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _T("Margins "));
	pMargins = new wxPanel(Panel1, ID_PANEL11, wxDefaultPosition, wxSize(376,64), wxTAB_TRAVERSAL, _T("ID_PANEL11"));
	StaticText15 = new wxStaticText(pMargins, ID_STATICTEXT15, _T("Margins :"), wxPoint(8,10), wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	edMarginTop = new wxTextCtrl(pMargins, ID_TEXTCTRL4, _T("50"), wxPoint(64,6), wxSize(40,23), wxTE_RIGHT, wxIntegerValidator<unsigned int> (&int_top), _T("ID_TEXTCTRL4"));
	edMarginTop->SetToolTip(_T("Top"));
	edMarginBottom = new wxTextCtrl(pMargins, ID_TEXTCTRL5, _T("50"), wxPoint(112,6), wxSize(40,23), wxTE_RIGHT, wxIntegerValidator<unsigned int> (&int_bottom), _T("ID_TEXTCTRL5"));
	edMarginBottom->SetToolTip(_T("Bottom"));
	edMarginLeft = new wxTextCtrl(pMargins, ID_TEXTCTRL6, _T("50"), wxPoint(160,6), wxSize(40,23), wxTE_RIGHT, wxIntegerValidator<unsigned int> (&int_left), _T("ID_TEXTCTRL6"));
	edMarginLeft->SetToolTip(_T("Left"));
	edMarginRight = new wxTextCtrl(pMargins, ID_TEXTCTRL7, _T("50"), wxPoint(208,6), wxSize(40,23), wxTE_RIGHT, wxIntegerValidator<unsigned int> (&int_right), _T("ID_TEXTCTRL7"));
	edMarginRight->SetToolTip(_T("Right"));
	bBGColor = new wxButton(pMargins, ID_BUTTON7, _T("bg color"), wxPoint(272,6), wxSize(72,23), 0, wxDefaultValidator, _T("ID_BUTTON7"));
	cbDrawBox = new wxCheckBox(pMargins, ID_CHECKBOX5, _T("Draw box"), wxPoint(8,42), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
	cbDrawBox->SetValue(false);
	StaticBoxSizer2->Add(pMargins, 1, wxALL|wxEXPAND, 0);
	BoxSizer3->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _T("Mouse coordinates "));
	pCoord = new wxPanel(Panel1, ID_PANEL12, wxDefaultPosition, wxSize(376,100), wxTAB_TRAVERSAL, _T("ID_PANEL12"));
	StaticText18 = new wxStaticText(pCoord, ID_STATICTEXT18, _T("Position :"), wxPoint(8,10), wxDefaultSize, 0, _T("ID_STATICTEXT18"));
	cbCoord = new wxChoice(pCoord, ID_CHOICE11, wxPoint(64,6), wxSize(112,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE11"));
	cbCoord->Append(_T("Left center"));
	cbCoord->Append(_T("Top left"));
	cbCoord->Append(_T("Top center"));
	cbCoord->Append(_T("Top right"));
	cbCoord->Append(_T("Right center"));
	cbCoord->Append(_T("Bottom left"));
	cbCoord->Append(_T("Bottom center"));
	cbCoord->Append(_T("Bottom right"));
	cbCoord->Append(_T("Default position"));
	cbCoord->Append(_T("Cursor position"));
	cbCoordVisible = new wxCheckBox(pCoord, ID_CHECKBOX10, _T("Visible"), wxPoint(8,42), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX10"));
	cbCoordVisible->SetValue(false);
	pCoordBrush = new wxPanel(pCoord, ID_PANEL14, wxPoint(192,0), wxSize(160,84), wxTAB_TRAVERSAL, _T("ID_PANEL14"));
	sbCoordBrush = new wxStaticBox(pCoordBrush, ID_STATICBOX10, _T("Brush "), wxPoint(0,0), wxSize(160,78), 0, _T("ID_STATICBOX10"));
	StaticText22 = new wxStaticText(pCoordBrush, ID_STATICTEXT22, _T("Color :"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT22"));
	StaticText23 = new wxStaticText(pCoordBrush, ID_STATICTEXT23, _T("Style :"), wxPoint(16,48), wxDefaultSize, 0, _T("ID_STATICTEXT23"));
	bCoordBrushColor = new wxButton(pCoordBrush, ID_BUTTON10, wxEmptyString, wxPoint(62,20), wxSize(80,23), 0, wxDefaultValidator, _T("ID_BUTTON10"));
	cbCoordBrushStyle = new wxChoice(pCoordBrush, ID_CHOICE13, wxPoint(62,44), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE13"));
	cbCoordBrushStyle->Append(_T("Solid"));
	cbCoordBrushStyle->Append(_T("Transparent"));
	StaticBoxSizer3->Add(pCoord, 0, wxALL|wxEXPAND, 0);
	BoxSizer3->Add(StaticBoxSizer3, 0, wxALL|wxEXPAND, 2);
	Panel1->SetSizer(BoxSizer3);
	BoxSizer3->Fit(Panel1);
	BoxSizer3->SetSizeHints(Panel1);
	Panel2 = new wxPanel(nbConfig, ID_PANEL15, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL15"));
	pLegend = new wxPanel(Panel2, ID_PANEL16, wxPoint(0,0), wxSize(368,184), wxTAB_TRAVERSAL, _T("ID_PANEL16"));
	StaticText16 = new wxStaticText(pLegend, ID_STATICTEXT16, _T("Position :"), wxPoint(8,24), wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	cbLegendPosition = new wxChoice(pLegend, ID_CHOICE10, wxPoint(68,20), wxSize(108,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE10"));
	cbLegendPosition->Append(_T("Left center"));
	cbLegendPosition->Append(_T("Top left"));
	cbLegendPosition->Append(_T("Top center"));
	cbLegendPosition->Append(_T("Top right"));
	cbLegendPosition->Append(_T("Right center"));
	cbLegendPosition->Append(_T("Bottom left"));
	cbLegendPosition->Append(_T("Bottom center"));
	cbLegendPosition->Append(_T("Bottom right"));
	cbLegendPosition->Append(_T("Default position"));
	cbLegendVisible = new wxCheckBox(pLegend, ID_CHECKBOX9, _T("Visible"), wxPoint(8,120), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX9"));
	cbLegendVisible->SetValue(false);
	pLegendBrush = new wxPanel(pLegend, ID_PANEL13, wxPoint(192,12), wxSize(160,84), wxTAB_TRAVERSAL, _T("ID_PANEL13"));
	sbLegendBrush = new wxStaticBox(pLegendBrush, ID_STATICBOX9, _T("Brush "), wxPoint(0,0), wxSize(160,80), 0, _T("ID_STATICBOX9"));
	StaticText20 = new wxStaticText(pLegendBrush, ID_STATICTEXT20, _T("Color :"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT20"));
	StaticText21 = new wxStaticText(pLegendBrush, ID_STATICTEXT21, _T("Style :"), wxPoint(16,48), wxDefaultSize, 0, _T("ID_STATICTEXT21"));
	bLegendBrushColor = new wxButton(pLegendBrush, ID_BUTTON9, wxEmptyString, wxPoint(62,20), wxSize(80,23), 0, wxDefaultValidator, _T("ID_BUTTON9"));
	cbLegendBrushStyle = new wxChoice(pLegendBrush, ID_CHOICE12, wxPoint(62,44), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE12"));
	cbLegendBrushStyle->Append(_T("Solid"));
	cbLegendBrushStyle->Append(_T("Transparent"));
	StaticText24 = new wxStaticText(pLegend, ID_STATICTEXT24, _T("Style :"), wxPoint(8,56), wxDefaultSize, 0, _T("ID_STATICTEXT24"));
	cbLegendStyle = new wxChoice(pLegend, ID_CHOICE14, wxPoint(68,52), wxSize(108,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE14"));
	cbLegendStyle->Append(_T("Line"));
	cbLegendStyle->Append(_T("Square"));
	StaticText25 = new wxStaticText(pLegend, ID_STATICTEXT25, _T("Direction :"), wxPoint(8,88), wxDefaultSize, 0, _T("ID_STATICTEXT25"));
	cbLegendDirection = new wxChoice(pLegend, ID_CHOICE15, wxPoint(68,84), wxSize(108,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE15"));
	cbLegendDirection->Append(_T("Vertical"));
	cbLegendDirection->Append(_T("Horizontal"));
	Panel3 = new wxPanel(nbConfig, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	pAxis = new wxPanel(Panel3, ID_PANEL9, wxPoint(0,0), wxSize(368,248), wxTAB_TRAVERSAL, _T("ID_PANEL9"));
	ChoiceAxis = new wxChoice(pAxis, ID_CHOICE7, wxPoint(8,16), wxSize(160,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE7"));
	ChoiceAxis->Append(_T("X axis"));
	ChoiceAxis->Append(_T("Y axis"));
	StaticText8 = new wxStaticText(pAxis, ID_STATICTEXT8, _T("Name :"), wxPoint(8,56), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	edAxisName = new wxTextCtrl(pAxis, ID_TEXTCTRL2, wxEmptyString, wxPoint(56,52), wxSize(232,23), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	pAxisPen = new wxPanel(pAxis, ID_PANEL10, wxPoint(8,88), wxSize(168,112), wxTAB_TRAVERSAL, _T("ID_PANEL10"));
	sbAxisPen = new wxStaticBox(pAxisPen, ID_STATICBOX6, _T("Pen "), wxPoint(0,0), wxSize(160,104), 0, _T("ID_STATICBOX6"));
	StaticText12 = new wxStaticText(pAxisPen, ID_STATICTEXT12, _T("Color :"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	StaticText13 = new wxStaticText(pAxisPen, ID_STATICTEXT13, _T("Width :"), wxPoint(16,48), wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	StaticText14 = new wxStaticText(pAxisPen, ID_STATICTEXT14, _T("Style :"), wxPoint(16,72), wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	bAxisPenColor = new wxButton(pAxisPen, ID_BUTTON6, wxEmptyString, wxPoint(62,20), wxSize(80,23), 0, wxDefaultValidator, _T("ID_BUTTON6"));
	cbAxisPenWidth = new wxChoice(pAxisPen, ID_CHOICE8, wxPoint(62,44), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE8"));
	cbAxisPenWidth->Append(_T("1"));
	cbAxisPenWidth->Append(_T("2"));
	cbAxisPenWidth->Append(_T("3"));
	cbAxisPenWidth->Append(_T("4"));
	cbAxisPenWidth->Append(_T("5"));
	cbAxisPenWidth->Append(_T("6"));
	cbAxisPenWidth->Append(_T("7"));
	cbAxisPenWidth->Append(_T("8"));
	cbAxisPenWidth->Append(_T("9"));
	cbAxisPenWidth->Append(_T("10"));
	cbAxisPenStyle = new wxChoice(pAxisPen, ID_CHOICE9, wxPoint(62,68), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE9"));
	cbAxisPenStyle->Append(_T("Solid"));
	cbAxisPenStyle->Append(_T("Dot"));
	cbAxisPenStyle->Append(_T("Long Dash"));
	cbAxisPenStyle->Append(_T("Short Dash"));
	cbAxisPenStyle->Append(_T("Dot Dash"));
	cbAxisVisible = new wxCheckBox(pAxis, ID_CHECKBOX4, _T("Visible"), wxPoint(192,152), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
	cbAxisVisible->SetValue(false);
	bFontAxis = new wxButton(pAxis, ID_BUTTON8, _T("Font"), wxPoint(296,52), wxSize(64,23), 0, wxDefaultValidator, _T("ID_BUTTON8"));
	bFontAxis->Disable();
	StaticText17 = new wxStaticText(pAxis, ID_STATICTEXT17, _T("Position :"), wxPoint(192,88), wxDefaultSize, 0, _T("ID_STATICTEXT17"));
	cbAxisPosition = new wxChoice(pAxis, ID_CHOICE6, wxPoint(192,112), wxSize(136,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE6"));
	cbAxisOutside = new wxCheckBox(pAxis, ID_CHECKBOX7, _T("Draw Outside Margins"), wxPoint(192,176), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX7"));
	cbAxisOutside->SetValue(false);
	StaticText26 = new wxStaticText(pAxis, ID_STATICTEXT26, _T("Label Format :"), wxPoint(8,216), wxDefaultSize, 0, _T("ID_STATICTEXT26"));
	edFormat = new wxTextCtrl(pAxis, ID_TEXTCTRL8, wxEmptyString, wxPoint(96,212), wxSize(80,23), 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
	edFormat->SetToolTip(_T("Format of the label for the axis. Should be like c++ format."));
	Panel4 = new wxPanel(nbConfig, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	StaticText1 = new wxStaticText(Panel4, ID_STATICTEXT1, _T("Series name :"), wxPoint(8,16), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	ChoiceSeries = new wxChoice(Panel4, ID_CHOICE1, wxPoint(112,12), wxSize(136,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	ChoiceSeries->Append(_T("0"));
	ChoiceSeries->Append(_T("1"));
	cbSeriesVisible = new wxCheckBox(Panel4, ID_CHECKBOX1, _T("Visible"), wxPoint(8,192), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	cbSeriesVisible->SetValue(false);
	StaticText2 = new wxStaticText(Panel4, ID_STATICTEXT2, _T("Name :"), wxPoint(8,48), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	edSeriesName = new wxTextCtrl(Panel4, ID_TEXTCTRL1, wxEmptyString, wxPoint(64,44), wxSize(296,23), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	pSeriesPen = new wxPanel(Panel4, ID_PANEL5, wxPoint(8,76), wxSize(160,108), wxTAB_TRAVERSAL, _T("ID_PANEL5"));
	sbSeriesPen = new wxStaticBox(pSeriesPen, ID_STATICBOX1, _T("Pen "), wxPoint(0,0), wxSize(160,104), 0, _T("ID_STATICBOX1"));
	StaticText3 = new wxStaticText(pSeriesPen, ID_STATICTEXT3, _T("Color :"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText4 = new wxStaticText(pSeriesPen, ID_STATICTEXT4, _T("Width :"), wxPoint(16,48), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText5 = new wxStaticText(pSeriesPen, ID_STATICTEXT5, _T("Style :"), wxPoint(16,72), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	bSeriesPenColor = new wxButton(pSeriesPen, ID_BUTTON1, wxEmptyString, wxPoint(62,20), wxSize(80,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	cbSeriesPenWidth = new wxChoice(pSeriesPen, ID_CHOICE2, wxPoint(62,44), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
	cbSeriesPenWidth->Append(_T("1"));
	cbSeriesPenWidth->Append(_T("2"));
	cbSeriesPenWidth->Append(_T("3"));
	cbSeriesPenWidth->Append(_T("4"));
	cbSeriesPenWidth->Append(_T("5"));
	cbSeriesPenWidth->Append(_T("6"));
	cbSeriesPenWidth->Append(_T("7"));
	cbSeriesPenWidth->Append(_T("8"));
	cbSeriesPenWidth->Append(_T("9"));
	cbSeriesPenWidth->Append(_T("10"));
	cbSeriesPenStyle = new wxChoice(pSeriesPen, ID_CHOICE3, wxPoint(62,68), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE3"));
	cbSeriesPenStyle->Append(_T("Solid"));
	cbSeriesPenStyle->Append(_T("Dot"));
	cbSeriesPenStyle->Append(_T("Long Dash"));
	cbSeriesPenStyle->Append(_T("Short Dash"));
	cbSeriesPenStyle->Append(_T("Dot Dash"));
	pSeriesBrush = new wxPanel(Panel4, ID_PANEL6, wxPoint(200,76), wxSize(160,84), wxTAB_TRAVERSAL, _T("ID_PANEL6"));
	sbSeriesBrush = new wxStaticBox(pSeriesBrush, ID_STATICBOX2, _T("Brush "), wxPoint(0,0), wxSize(160,80), 0, _T("ID_STATICBOX2"));
	StaticText6 = new wxStaticText(pSeriesBrush, ID_STATICTEXT6, _T("Color :"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	StaticText7 = new wxStaticText(pSeriesBrush, ID_STATICTEXT7, _T("Style :"), wxPoint(16,48), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	bSeriesBrushColor = new wxButton(pSeriesBrush, ID_BUTTON2, wxEmptyString, wxPoint(62,20), wxSize(80,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	cbSeriesBrushStyle = new wxChoice(pSeriesBrush, ID_CHOICE4, wxPoint(62,44), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE4"));
	cbSeriesBrushStyle->Append(_T("Solid"));
	cbSeriesBrushStyle->Append(_T("Transparent"));
	cbSeriesBrushStyle->Append(_T("BDiagonal"));
	cbSeriesBrushStyle->Append(_T("CrossDiagonal"));
	cbSeriesBrushStyle->Append(_T("FDiagonal"));
	cbSeriesBrushStyle->Append(_T("Cross"));
	cbSeriesBrushStyle->Append(_T("Horizontal"));
	cbSeriesBrushStyle->Append(_T("Vertical"));
	pSeriesSymbol = new wxPanel(Panel4, ID_PANEL7, wxPoint(200,164), wxSize(160,84), wxTAB_TRAVERSAL, _T("ID_PANEL7"));
	sbSeriesSymbol = new wxStaticBox(pSeriesSymbol, ID_STATICBOX4, _T("Symbol "), wxPoint(0,0), wxSize(160,80), 0, _T("ID_STATICBOX4"));
	StaticText9 = new wxStaticText(pSeriesSymbol, ID_STATICTEXT9, _T("Type :"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	StaticText10 = new wxStaticText(pSeriesSymbol, ID_STATICTEXT10, _T("Size :"), wxPoint(16,48), wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	cbSeriesSymbolType = new wxChoice(pSeriesSymbol, ID_CHOICE5, wxPoint(62,20), wxSize(80,23), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE5"));
	cbSeriesSymbolType->Append(_T("None"));
	cbSeriesSymbolType->Append(_T("Circle"));
	cbSeriesSymbolType->Append(_T("Square"));
	cbSeriesSymbolType->Append(_T("UpTriangle"));
	cbSeriesSymbolType->Append(_T("DownTriangle"));
	cbSeriesSymbolType->Append(_T("Cross"));
	cbSeriesSymbolType->Append(_T("Plus"));
	cbSeriesSymbolSize = new wxSpinCtrl(pSeriesSymbol, ID_SPINCTRL2, _T("4"), wxPoint(96,48), wxSize(44,23), 0, 4, 100, 4, _T("ID_SPINCTRL2"));
	cbSeriesSymbolSize->SetValue(_T("4"));
	cbSeriesContinuity = new wxCheckBox(Panel4, ID_CHECKBOX2, _T("Continuity"), wxPoint(8,216), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	cbSeriesContinuity->SetValue(false);
	cbSeriesOutside = new wxCheckBox(Panel4, ID_CHECKBOX3, _T("Draw Outside Margins"), wxPoint(8,240), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	cbSeriesOutside->SetValue(false);
	cbSeriesShowName = new wxCheckBox(Panel4, ID_CHECKBOX6, _T("Show name"), wxPoint(8,264), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX6"));
	cbSeriesShowName->SetValue(false);
	StaticText19 = new wxStaticText(Panel4, ID_STATICTEXT19, _T("Skip point over :"), wxPoint(204,262), wxDefaultSize, 0, _T("ID_STATICTEXT19"));
	cbSeriesStep = new wxSpinCtrl(Panel4, ID_SPINCTRL1, _T("1"), wxPoint(296,260), wxSize(46,23), 0, 1, 100, 1, _T("ID_SPINCTRL1"));
	cbSeriesStep->SetValue(_T("1"));
	bDelSeries = new wxButton(Panel4, ID_BUTTON11, _T("Delete"), wxPoint(280,12), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON11"));
	bDelSeries->Disable();
	nbConfig->AddPage(Panel1, _T("General"), false);
	nbConfig->AddPage(Panel2, _T("Legend"), false);
	nbConfig->AddPage(Panel3, _T("Axis"), false);
	nbConfig->AddPage(Panel4, _T("Series"), false);
	BoxSizer1->Add(nbConfig, 1, wxALL|wxEXPAND, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	bApply = new wxButton(this, ID_BUTTON3, _T("Apply"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	BoxSizer2->Add(bApply, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	bClose = new wxButton(this, ID_BUTTON4, _T("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
	BoxSizer2->Add(bClose, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	BoxSizer1->Add(BoxSizer2, 0, wxEXPAND, 4);
	SetSizer(BoxSizer1);
	SetSizer(BoxSizer1);
	Layout();

	Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbFontClick);
	Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbColorClick);
	Connect(ID_BUTTON10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbColorClick);
	Connect(ID_BUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbColorClick);
	Connect(ID_CHOICE7,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&MathPlotConfigDialog::OnAxisSelect);
	Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbColorClick);
	Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbFontClick);
	Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&MathPlotConfigDialog::OnChoiceSeries);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbColorClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbColorClick);
	Connect(ID_BUTTON11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbDelSeriesClick);
	Connect(ID_NOTEBOOK1,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&MathPlotConfigDialog::OnnbConfigPageChanged);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbApplyClick);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnQuit);
	//*)

	colourButton = NULL;
	CurrentTitle = NULL;
	CurrentChoice = NULL;
	CurrentSerie = NULL;
	CurrentLegend = NULL;
	CurrentCoords = NULL;
	// The plot window
	m_plot = wxDynamicCast(parent, mpWindow);
	CurrentScale = NULL;
	fontTitleChanged = false;
	fontAxisChanged = false;
//    Initialize();
}

MathPlotConfigDialog::~MathPlotConfigDialog()
{
	//(*Destroy(MathPlotConfigDialog)
	//*)
}

void MathPlotConfigDialog::Initialize()
{
	CurrentTitle = (mpText*) m_plot->GetLayerByClassName(_T("mpTitle"));
	if (CurrentTitle)
	{
		edTitle->SetValue(CurrentTitle->GetName());
		cbTitleVisible->SetValue(CurrentTitle->IsVisible());
		bFontTitle->SetFont(CurrentTitle->GetFont());
		bFontTitle->SetForegroundColour(CurrentTitle->GetFontColour());
		bFontTitle->Enable();
	}

	int_top = m_plot->GetMarginTop();
	int_bottom = m_plot->GetMarginBottom();
	int_left = m_plot->GetMarginLeft();
	int_right = m_plot->GetMarginRight();

	cbDrawBox->SetValue(m_plot->GetDrawBox());
	bBGColor->SetBackgroundColour(m_plot->GetbgColour());

	CurrentLegend = (mpInfoLegend*) m_plot->GetLayerByClassName(_T("mpInfoLegend"));
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

	CurrentCoords = (mpInfoCoords*) m_plot->GetLayerByClassName(_T("mpInfoCoords"));
	if (CurrentCoords)
	{
		cbCoord->SetSelection(CurrentCoords->GetLocation());
		cbCoordVisible->SetValue(CurrentCoords->IsVisible());
		// Brush config
		bCoordBrushColor->SetBackgroundColour(CurrentCoords->GetBrush().GetColour());
		cbCoordBrushStyle->SetSelection(BrushStyleToId(CurrentCoords->GetBrush().GetStyle()));
	}

	// X axis
	ChoiceAxis->SetSelection(0);
	UpdateAxis();

	// Fill series counter
	ChoiceSeries->Clear();
	for (unsigned int i = 0; i < m_plot->CountLayersPlot(); i++)
	{
		ChoiceSeries->Append(((mpLayer*) m_plot->GetLayerPlot(i))->GetName());
	}
	bDelSeries->Enable(ChoiceSeries->GetCount() > 0);

	// Select the first serie
	if (ChoiceSeries->GetCount() > 0)
	{
		ChoiceSeries->SetSelection(0);
		UpdateSelectedSerie();
	}

	CurrentChoice = NULL;
}

void MathPlotConfigDialog::OnQuit(wxCommandEvent&WXUNUSED(event))
{
	Close();
}

void MathPlotConfigDialog::OnbColorClick(wxCommandEvent &event)
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
	else
		// Restore initial color
		DoApplyColour(m_clrData.GetColour());
}

void MathPlotConfigDialog::OnColourChanged(wxColourDialogEvent &event)
{
	DoApplyColour(event.GetColour());
}

void MathPlotConfigDialog::DoApplyColour(const wxColour &colour)
{
	if (colour == colourButton->GetBackgroundColour())
		return;

	colourButton->SetBackgroundColour(colour);
	colourButton->ClearBackground();
	colourButton->Refresh();
}

void MathPlotConfigDialog::OnbFontClick(wxCommandEvent &event)
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

void MathPlotConfigDialog::SetFontChildren(wxButton *p, const wxFontData &fontdata)
{
	wxFont font(fontdata.GetChosenFont());

	p->SetFont(font);
	p->SetForegroundColour(fontdata.GetColour());
}

void MathPlotConfigDialog::OnnbConfigPageChanged(wxNotebookEvent &event)
{
	const int idx = event.GetSelection();
	switch (idx)
	{
		case 0:
			break;
		case 1:
			break;
		case 2:
			CurrentChoice = ChoiceAxis;
			break;
		case 3:
			CurrentChoice = ChoiceSeries;
			break;
	}
}

void MathPlotConfigDialog::UpdateAxis(void)
{
	if (ChoiceAxis->GetSelection() == 0)
	{
		CurrentScale = (mpScale*) m_plot->GetLayerByClassName(_T("mpScaleX"));
		cbAxisPosition->Clear();
		for (int i = 0; i <= mpALIGN_BORDER_TOP; i++)
			cbAxisPosition->Append(XAxis_Align[i]);
	}
	else
	{
		CurrentScale = (mpScale*) m_plot->GetLayerByClassName(_T("mpScaleY"));
		cbAxisPosition->Clear();
		for (int i = 0; i <= mpALIGN_BORDER_RIGHT; i++)
			cbAxisPosition->Append(YAxis_Align[i]);
	}

	if (CurrentScale)
	{
		edAxisName->SetValue(CurrentScale->GetName());
		// Pen config
		bAxisPenColor->SetBackgroundColour(CurrentScale->GetPen().GetColour());
		cbAxisPenWidth->SetSelection(CurrentScale->GetPen().GetWidth() - 1);
		cbAxisPenStyle->SetSelection(CurrentScale->GetPen().GetStyle() - wxPENSTYLE_SOLID);
		cbAxisVisible->SetValue(CurrentScale->IsVisible());
		cbAxisPosition->SetSelection(CurrentScale->GetAlign());
		edFormat->SetValue(CurrentScale->GetLabelFormat());

		cbAxisOutside->SetValue(CurrentScale->GetDrawOutsideMargins());

		bFontAxis->SetFont(CurrentScale->GetFont());
		bFontAxis->SetForegroundColour(CurrentScale->GetFontColour());
		bFontAxis->Enable();
	}
}

void MathPlotConfigDialog::OnAxisSelect(wxCommandEvent&WXUNUSED(event))
{
	UpdateAxis();
	CurrentChoice = ChoiceAxis;
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
			return (wxBrushStyle) (wxBRUSHSTYLE_BDIAGONAL_HATCH + (id - 2));
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

void MathPlotConfigDialog::UpdateSelectedSerie(void)
{
	CurrentSerie = (mpLayer*) m_plot->GetLayerPlot(ChoiceSeries->GetSelection());

	if (CurrentSerie)
	{
		CurrentChoice = ChoiceSeries;

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
	else
		CurrentChoice = NULL;
}

void MathPlotConfigDialog::OnChoiceSeries(wxCommandEvent&WXUNUSED(event))
{
	UpdateSelectedSerie();
}

void MathPlotConfigDialog::OnbDelSeriesClick(wxCommandEvent&WXUNUSED(event))
{
	if (CurrentSerie && CurrentSerie->GetCanDelete())
	{
		if (wxMessageDialog(this, _("Delete the serie ?"), _("Confirmation"), wxYES_NO | wxCENTRE).ShowModal() == wxID_YES)
		{
			m_plot->DelLayer(CurrentSerie, true, true);
			if (CurrentLegend)
				CurrentLegend->SetNeedUpdate();
			m_plot->Fit();
			CurrentSerie = NULL;
			Initialize();
		}
	}
}

void MathPlotConfigDialog::OnbApplyClick(wxCommandEvent&WXUNUSED(event))
{
	switch (nbConfig->GetSelection())
	{
		case 0: // General
		{
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
				CurrentCoords->SetLocation((mpLocation) cbCoord->GetSelection());
				CurrentCoords->SetVisible(cbCoordVisible->GetValue());
				// Brush config
				wxBrush brush(bCoordBrushColor->GetBackgroundColour(), IdToBrushStyle(cbCoordBrushStyle->GetSelection()));
				CurrentCoords->SetBrush(brush);
			}

			m_plot->Refresh();
			break;
		}
		case 1: // Legend page
			if (CurrentLegend)
			{
				CurrentLegend->SetLocation((mpLocation) cbLegendPosition->GetSelection());
				CurrentLegend->SetVisible(cbLegendVisible->GetValue());
				CurrentLegend->SetItemMode((mpLegendStyle) cbLegendStyle->GetSelection());
				CurrentLegend->SetItemDirection((mpLegendDirection) cbLegendDirection->GetSelection());
				// Brush config
				wxBrush brush(bLegendBrushColor->GetBackgroundColour(), IdToBrushStyle(cbLegendBrushStyle->GetSelection()));
				CurrentLegend->SetBrush(brush);

				m_plot->UpdateAll();
			}
			break;

		case 2: // Axis page
			if ((CurrentChoice == ChoiceAxis) && (CurrentScale != NULL))
			{
				CurrentScale->SetName(edAxisName->GetValue());
				// Pen config
				wxPen pen(bAxisPenColor->GetBackgroundColour(), cbAxisPenWidth->GetSelection() + 1,
						(wxPenStyle) (cbAxisPenStyle->GetSelection() + wxPENSTYLE_SOLID));
				CurrentScale->SetPen(pen);
				CurrentScale->SetVisible(cbAxisVisible->GetValue());
				CurrentScale->SetAlign(cbAxisPosition->GetSelection());
				CurrentScale->SetDrawOutsideMargins(cbAxisOutside->GetValue());
				CurrentScale->SetLabelFormat(edFormat->GetValue());

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
			if ((CurrentChoice == ChoiceSeries) && (CurrentSerie != NULL))
			{
				CurrentSerie->SetName(edSeriesName->GetValue());
				// Pen config
				wxPen pen(bSeriesPenColor->GetBackgroundColour(), cbSeriesPenWidth->GetSelection() + 1,
						(wxPenStyle) (cbSeriesPenStyle->GetSelection() + wxPENSTYLE_SOLID));
				CurrentSerie->SetPen(pen);

				// Brush config
				wxBrush brush(bSeriesBrushColor->GetBackgroundColour(), IdToBrushStyle(cbSeriesBrushStyle->GetSelection()));
				CurrentSerie->SetBrush(brush);

				// Symbol config
				CurrentSerie->SetSymbol((mpSymbol) cbSeriesSymbolType->GetSelection());
				CurrentSerie->SetSymbolSize(cbSeriesSymbolSize->GetValue());

				CurrentSerie->SetVisible(cbSeriesVisible->GetValue());
				CurrentSerie->SetContinuity(cbSeriesContinuity->GetValue());
				CurrentSerie->SetDrawOutsideMargins(cbSeriesOutside->GetValue());
				CurrentSerie->SetShowName(cbSeriesShowName->GetValue());

				CurrentSerie->SetStep(cbSeriesStep->GetValue());

				if (CurrentLegend)
					CurrentLegend->SetNeedUpdate();

				m_plot->Refresh();
			}
			break;
		default:
			;
	}
}

