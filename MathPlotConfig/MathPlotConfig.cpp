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
const long MathPlotConfigDialog::ID_STATICTEXT15 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL4 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL5 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL6 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL7 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON7 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX5 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT18 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE11 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX10 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT22 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON10 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT23 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE13 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL17 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT16 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE10 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT24 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE14 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT25 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE15 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX9 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT20 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON9 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT21 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE12 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL15 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE7 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT8 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL2 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON8 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT12 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON6 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT13 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE8 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT14 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE9 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX11 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT27 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL9 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT28 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL10 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT17 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE6 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX4 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX7 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT26 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL8 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL1 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT1 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE1 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON11 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT2 = wxNewId();
const long MathPlotConfigDialog::ID_TEXTCTRL1 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT3 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON1 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT4 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE2 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT5 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE3 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX1 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX2 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX3 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX6 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT6 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON2 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT7 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE4 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT9 = wxNewId();
const long MathPlotConfigDialog::ID_CHOICE5 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT10 = wxNewId();
const long MathPlotConfigDialog::ID_SPINCTRL2 = wxNewId();
const long MathPlotConfigDialog::ID_STATICTEXT19 = wxNewId();
const long MathPlotConfigDialog::ID_SPINCTRL1 = wxNewId();
const long MathPlotConfigDialog::ID_CHECKBOX12 = wxNewId();
const long MathPlotConfigDialog::ID_PANEL4 = wxNewId();
const long MathPlotConfigDialog::ID_NOTEBOOK1 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON3 = wxNewId();
const long MathPlotConfigDialog::ID_BUTTON4 = wxNewId();
//*)

const wxString XAxis_Align[] = {_T("Bottom border"), _T("Bottom"), _T("Center"), _T("Top"), _T("Top border")};
const wxString YAxis_Align[] = {_T("Left border"), _T("Left"), _T("Center"), _T("Right"), _T("Right border")};

BEGIN_EVENT_TABLE(MathPlotConfigDialog,wxDialog)
//(*EventTable(MathPlotConfigDialog)
//*)
END_EVENT_TABLE()

MathPlotConfigDialog::MathPlotConfigDialog(wxWindow *parent, wxWindowID WXUNUSED(id))
{
	// Choices list
	// Use this method to have correct sizing under Linux
  const wxString cbCoord_choices[] = {
	  _T("Left center"),
	  _T("Top left"),
	  _T("Top center"),
	  _T("Top right"),
	  _T("Right center"),
	  _T("Bottom left"),
	  _T("Bottom center"),
	  _T("Bottom right"),
	  _T("Default position"),
	  _T("Cursor position")
  };

  const wxString cbBrushStyle_choices[] = {
	  _T("Solid"),
	  _T("Transparent")
  };

  const wxString cbLegendPosition_choices[] = {
	  _T("Left center"),
	  _T("Top left"),
	  _T("Top center"),
	  _T("Top right"),
	  _T("Right center"),
	  _T("Bottom left"),
	  _T("Bottom center"),
	  _T("Bottom right"),
	  _T("Default position")
  };

  const wxString cbLegendStyle_choices[] = {
	  _T("Line"),
	  _T("Square")
  };

  const wxString cbLegendDirection_choices[] = {
	  _T("Vertical"),
	  _T("Horizontal")
  };

  const wxString ChoiceAxis_choices[] = {
	  _T("X axis"),
	  _T("Y axis")
  };

  const wxString cbPenWidth_choices[] = {
	  _T("1"),
	  _T("2"),
	  _T("3"),
	  _T("4"),
	  _T("5"),
	  _T("6"),
	  _T("7"),
	  _T("8"),
	  _T("9"),
	  _T("10")
  };

  const wxString cbPenStyle_choices[] = {
	  _T("Solid"),
	  _T("Dot"),
	  _T("Long Dash"),
	  _T("Short Dash"),
	  _T("Dot Dash")
  };

  const wxString cbSeriesBrushStyle_choices[] = {
	  _T("Solid"),
	  _T("Transparent"),
	  _T("BDiagonal"),
	  _T("CrossDiagonal"),
	  _T("FDiagonal"),
	  _T("Cross"),
	  _T("Horizontal"),
	  _T("Vertical")
  };

  const wxString cbSeriesSymbolType_choices[] = {
	  _T("None"),
	  _T("Circle"),
	  _T("Square"),
	  _T("UpTriangle"),
	  _T("DownTriangle"),
	  _T("Cross"),
	  _T("Plus")
  };

	//(*Initialize(MathPlotConfigDialog)
	wxBoxSizer* BoxSizer10;
	wxBoxSizer* BoxSizer11;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxBoxSizer* BoxSizer7;
	wxBoxSizer* BoxSizer8;
	wxBoxSizer* BoxSizer9;
	wxFlexGridSizer* FlexGridSizer10;
	wxFlexGridSizer* FlexGridSizer11;
	wxFlexGridSizer* FlexGridSizer12;
	wxFlexGridSizer* FlexGridSizer13;
	wxFlexGridSizer* FlexGridSizer14;
	wxFlexGridSizer* FlexGridSizer15;
	wxFlexGridSizer* FlexGridSizer16;
	wxFlexGridSizer* FlexGridSizer17;
	wxFlexGridSizer* FlexGridSizer18;
	wxFlexGridSizer* FlexGridSizer19;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer7;
	wxFlexGridSizer* FlexGridSizer8;
	wxStaticBoxSizer* StaticBoxSizer10;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer5;
	wxStaticBoxSizer* StaticBoxSizer6;
	wxStaticBoxSizer* StaticBoxSizer7;
	wxStaticBoxSizer* StaticBoxSizer8;
	wxStaticBoxSizer* StaticBoxSizer9;

	Create(parent, wxID_ANY, _T("MathPlot Configuration"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	sizerMain = new wxBoxSizer(wxVERTICAL);
	nbConfig = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_NOTEBOOK1"));
	Panel1 = new wxPanel(nbConfig, ID_PANEL17, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL17"));
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, Panel1, _T("Title "));
	FlexGridSizer1 = new wxFlexGridSizer(2, 3, 0, 0);
	FlexGridSizer1->AddGrowableCol(1);
	StaticText11 = new wxStaticText(Panel1, ID_STATICTEXT11, _T("Title :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer1->Add(StaticText11, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	edTitle = new wxTextCtrl(Panel1, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	FlexGridSizer1->Add(edTitle, 1, wxALL|wxEXPAND, 5);
	bFontTitle = new wxButton(Panel1, ID_BUTTON5, _T("Font"), wxDefaultPosition, wxSize(64,-1), 0, wxDefaultValidator, _T("ID_BUTTON5"));
	bFontTitle->Disable();
	FlexGridSizer1->Add(bFontTitle, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	cbTitleVisible = new wxCheckBox(Panel1, ID_CHECKBOX8, _T("Visible"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX8"));
	cbTitleVisible->SetValue(false);
	FlexGridSizer1->Add(cbTitleVisible, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(FlexGridSizer1, 0, wxALL|wxEXPAND, 5);
	BoxSizer3->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, Panel1, _T("Margins "));
	FlexGridSizer2 = new wxFlexGridSizer(2, 6, 0, 0);
	StaticText15 = new wxStaticText(Panel1, ID_STATICTEXT15, _T("Margins :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	FlexGridSizer2->Add(StaticText15, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	edMarginTop = new wxTextCtrl(Panel1, ID_TEXTCTRL4, _T("50"), wxDefaultPosition, wxSize(40,-1), wxTE_RIGHT, wxIntegerValidator<unsigned int> (&int_top), _T("ID_TEXTCTRL4"));
	edMarginTop->SetToolTip(_T("Top"));
	FlexGridSizer2->Add(edMarginTop, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	edMarginBottom = new wxTextCtrl(Panel1, ID_TEXTCTRL5, _T("50"), wxDefaultPosition, wxSize(40,-1), wxTE_RIGHT, wxIntegerValidator<unsigned int> (&int_bottom), _T("ID_TEXTCTRL5"));
	edMarginBottom->SetToolTip(_T("Bottom"));
	FlexGridSizer2->Add(edMarginBottom, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	edMarginLeft = new wxTextCtrl(Panel1, ID_TEXTCTRL6, _T("50"), wxDefaultPosition, wxSize(40,-1), wxTE_RIGHT, wxIntegerValidator<unsigned int> (&int_left), _T("ID_TEXTCTRL6"));
	edMarginLeft->SetToolTip(_T("Left"));
	FlexGridSizer2->Add(edMarginLeft, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	edMarginRight = new wxTextCtrl(Panel1, ID_TEXTCTRL7, _T("50"), wxDefaultPosition, wxSize(40,-1), wxTE_RIGHT, wxIntegerValidator<unsigned int> (&int_right), _T("ID_TEXTCTRL7"));
	edMarginRight->SetToolTip(_T("Right"));
	FlexGridSizer2->Add(edMarginRight, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	bBGColor = new wxButton(Panel1, ID_BUTTON7, _T("bg color"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON7"));
	FlexGridSizer2->Add(bBGColor, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	cbDrawBox = new wxCheckBox(Panel1, ID_CHECKBOX5, _T("Draw box"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
	cbDrawBox->SetValue(false);
	FlexGridSizer2->Add(cbDrawBox, 1, wxALL|wxEXPAND, 5);
	StaticBoxSizer2->Add(FlexGridSizer2, 0, wxALL|wxEXPAND, 5);
	BoxSizer3->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _T("Mouse coordinates "));
	FlexGridSizer3 = new wxFlexGridSizer(2, 2, 0, 0);
	StaticText18 = new wxStaticText(Panel1, ID_STATICTEXT18, _T("Position :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT18"));
	FlexGridSizer3->Add(StaticText18, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbCoord = new wxChoice(Panel1, ID_CHOICE11, wxDefaultPosition, wxDefaultSize, 10, cbCoord_choices, 0, wxDefaultValidator, _T("ID_CHOICE11"));
	FlexGridSizer3->Add(cbCoord, 1, wxALL|wxEXPAND, 5);
	cbCoordVisible = new wxCheckBox(Panel1, ID_CHECKBOX10, _T("Visible"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX10"));
	cbCoordVisible->SetValue(false);
	FlexGridSizer3->Add(cbCoordVisible, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3->Add(FlexGridSizer3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _T("Brush "));
	FlexGridSizer4 = new wxFlexGridSizer(2, 2, 0, 0);
	StaticText22 = new wxStaticText(Panel1, ID_STATICTEXT22, _T("Color :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT22"));
	FlexGridSizer4->Add(StaticText22, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	bCoordBrushColor = new wxButton(Panel1, ID_BUTTON10, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON10"));
	FlexGridSizer4->Add(bCoordBrushColor, 1, wxALL|wxEXPAND, 2);
	StaticText23 = new wxStaticText(Panel1, ID_STATICTEXT23, _T("Style :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT23"));
	FlexGridSizer4->Add(StaticText23, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbCoordBrushStyle = new wxChoice(Panel1, ID_CHOICE13, wxDefaultPosition, wxDefaultSize, 2, cbBrushStyle_choices, 0, wxDefaultValidator, _T("ID_CHOICE13"));
	FlexGridSizer4->Add(cbCoordBrushStyle, 1, wxALL|wxEXPAND, 2);
	StaticBoxSizer4->Add(FlexGridSizer4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	StaticBoxSizer3->Add(StaticBoxSizer4, 0, wxALL|wxALIGN_TOP, 5);
	BoxSizer3->Add(StaticBoxSizer3, 0, wxALL|wxEXPAND, 2);
	Panel1->SetSizer(BoxSizer3);
	BoxSizer3->Fit(Panel1);
	BoxSizer3->SetSizeHints(Panel1);
	Panel2 = new wxPanel(nbConfig, ID_PANEL15, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL15"));
	FlexGridSizer5 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer6 = new wxFlexGridSizer(4, 2, 0, 0);
	StaticText16 = new wxStaticText(Panel2, ID_STATICTEXT16, _T("Position :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	FlexGridSizer6->Add(StaticText16, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbLegendPosition = new wxChoice(Panel2, ID_CHOICE10, wxDefaultPosition, wxDefaultSize, 9, cbLegendPosition_choices, 0, wxDefaultValidator, _T("ID_CHOICE10"));
	FlexGridSizer6->Add(cbLegendPosition, 1, wxALL|wxEXPAND, 2);
	StaticText24 = new wxStaticText(Panel2, ID_STATICTEXT24, _T("Style :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT24"));
	FlexGridSizer6->Add(StaticText24, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbLegendStyle = new wxChoice(Panel2, ID_CHOICE14, wxDefaultPosition, wxDefaultSize, 2, cbLegendStyle_choices, 0, wxDefaultValidator, _T("ID_CHOICE14"));
	FlexGridSizer6->Add(cbLegendStyle, 1, wxALL|wxEXPAND, 2);
	StaticText25 = new wxStaticText(Panel2, ID_STATICTEXT25, _T("Direction :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT25"));
	FlexGridSizer6->Add(StaticText25, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbLegendDirection = new wxChoice(Panel2, ID_CHOICE15, wxDefaultPosition, wxDefaultSize, 2, cbLegendDirection_choices, 0, wxDefaultValidator, _T("ID_CHOICE15"));
	FlexGridSizer6->Add(cbLegendDirection, 1, wxALL|wxEXPAND, 2);
	cbLegendVisible = new wxCheckBox(Panel2, ID_CHECKBOX9, _T("Visible"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX9"));
	cbLegendVisible->SetValue(false);
	FlexGridSizer6->Add(cbLegendVisible, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(FlexGridSizer6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer5 = new wxStaticBoxSizer(wxHORIZONTAL, Panel2, _T("Brush "));
	FlexGridSizer7 = new wxFlexGridSizer(2, 2, 0, 0);
	StaticText20 = new wxStaticText(Panel2, ID_STATICTEXT20, _T("Color :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT20"));
	FlexGridSizer7->Add(StaticText20, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	bLegendBrushColor = new wxButton(Panel2, ID_BUTTON9, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON9"));
	FlexGridSizer7->Add(bLegendBrushColor, 1, wxALL|wxEXPAND, 2);
	StaticText21 = new wxStaticText(Panel2, ID_STATICTEXT21, _T("Style :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT21"));
	FlexGridSizer7->Add(StaticText21, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbLegendBrushStyle = new wxChoice(Panel2, ID_CHOICE12, wxDefaultPosition, wxDefaultSize, 2, cbBrushStyle_choices, 0, wxDefaultValidator, _T("ID_CHOICE12"));
	FlexGridSizer7->Add(cbLegendBrushStyle, 1, wxALL|wxEXPAND, 2);
	StaticBoxSizer5->Add(FlexGridSizer7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	FlexGridSizer5->Add(StaticBoxSizer5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	Panel2->SetSizer(FlexGridSizer5);
	FlexGridSizer5->Fit(Panel2);
	FlexGridSizer5->SetSizeHints(Panel2);
	Panel3 = new wxPanel(nbConfig, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	BoxSizer4 = new wxBoxSizer(wxVERTICAL);
	ChoiceAxis = new wxChoice(Panel3, ID_CHOICE7, wxDefaultPosition, wxDefaultSize, 2, ChoiceAxis_choices, 0, wxDefaultValidator, _T("ID_CHOICE7"));
	BoxSizer4->Add(ChoiceAxis, 0, wxALL, 5);
	FlexGridSizer8 = new wxFlexGridSizer(1, 3, 0, 0);
	FlexGridSizer8->AddGrowableCol(1);
	StaticText8 = new wxStaticText(Panel3, ID_STATICTEXT8, _T("Name :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer8->Add(StaticText8, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	edAxisName = new wxTextCtrl(Panel3, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	FlexGridSizer8->Add(edAxisName, 0, wxALL|wxEXPAND, 5);
	bFontAxis = new wxButton(Panel3, ID_BUTTON8, _T("Font"), wxDefaultPosition, wxSize(64,-1), 0, wxDefaultValidator, _T("ID_BUTTON8"));
	bFontAxis->Disable();
	FlexGridSizer8->Add(bFontAxis, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4->Add(FlexGridSizer8, 0, wxALL|wxEXPAND, 2);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer7 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer6 = new wxStaticBoxSizer(wxHORIZONTAL, Panel3, _T("Pen "));
	FlexGridSizer10 = new wxFlexGridSizer(3, 2, 0, 0);
	StaticText12 = new wxStaticText(Panel3, ID_STATICTEXT12, _T("Color :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	FlexGridSizer10->Add(StaticText12, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	bAxisPenColor = new wxButton(Panel3, ID_BUTTON6, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
	FlexGridSizer10->Add(bAxisPenColor, 1, wxALL|wxEXPAND, 2);
	StaticText13 = new wxStaticText(Panel3, ID_STATICTEXT13, _T("Width :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	FlexGridSizer10->Add(StaticText13, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbAxisPenWidth = new wxChoice(Panel3, ID_CHOICE8, wxDefaultPosition, wxDefaultSize, 10, cbPenWidth_choices, 0, wxDefaultValidator, _T("ID_CHOICE8"));
	FlexGridSizer10->Add(cbAxisPenWidth, 1, wxALL|wxEXPAND, 2);
	StaticText14 = new wxStaticText(Panel3, ID_STATICTEXT14, _T("Style :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	FlexGridSizer10->Add(StaticText14, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbAxisPenStyle = new wxChoice(Panel3, ID_CHOICE9, wxDefaultPosition, wxDefaultSize, 5, cbPenStyle_choices, 0, wxDefaultValidator, _T("ID_CHOICE9"));
	FlexGridSizer10->Add(cbAxisPenStyle, 1, wxALL|wxEXPAND, 2);
	StaticBoxSizer6->Add(FlexGridSizer10, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer7->Add(StaticBoxSizer6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer7 = new wxStaticBoxSizer(wxHORIZONTAL, Panel3, _T("Scale "));
	FlexGridSizer12 = new wxFlexGridSizer(3, 2, 0, 0);
	cbAutoScale = new wxCheckBox(Panel3, ID_CHECKBOX11, _T("Auto"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX11"));
	cbAutoScale->SetValue(true);
	FlexGridSizer12->Add(cbAutoScale, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer12->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText27 = new wxStaticText(Panel3, ID_STATICTEXT27, _T("Min :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT27"));
	FlexGridSizer12->Add(StaticText27, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	edScaleMin = new wxTextCtrl(Panel3, ID_TEXTCTRL9, _T("-1"), wxDefaultPosition, wxSize(64,-1), wxTE_RIGHT, wxFloatingPointValidator<double> (2, &scale_min), _T("ID_TEXTCTRL9"));
	edScaleMin->Disable();
	FlexGridSizer12->Add(edScaleMin, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText28 = new wxStaticText(Panel3, ID_STATICTEXT28, _T("Max :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT28"));
	FlexGridSizer12->Add(StaticText28, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	edScaleMax = new wxTextCtrl(Panel3, ID_TEXTCTRL10, _T("1"), wxDefaultPosition, wxSize(62,-1), wxTE_RIGHT, wxFloatingPointValidator<double> (2, &scale_max), _T("ID_TEXTCTRL10"));
	edScaleMax->Disable();
	FlexGridSizer12->Add(edScaleMax, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer7->Add(FlexGridSizer12, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer7->Add(StaticBoxSizer7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer6->Add(BoxSizer7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer5 = new wxBoxSizer(wxVERTICAL);
	StaticText17 = new wxStaticText(Panel3, ID_STATICTEXT17, _T("Position :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT17"));
	BoxSizer5->Add(StaticText17, 0, wxALL|wxALIGN_LEFT, 5);
	cbAxisPosition = new wxChoice(Panel3, ID_CHOICE6, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE6"));
	BoxSizer5->Add(cbAxisPosition, 0, wxALL|wxALIGN_LEFT, 5);
	cbAxisVisible = new wxCheckBox(Panel3, ID_CHECKBOX4, _T("Visible"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
	cbAxisVisible->SetValue(false);
	BoxSizer5->Add(cbAxisVisible, 0, wxALL|wxALIGN_LEFT, 5);
	cbAxisOutside = new wxCheckBox(Panel3, ID_CHECKBOX7, _T("Draw Outside Margins"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX7"));
	cbAxisOutside->SetValue(false);
	BoxSizer5->Add(cbAxisOutside, 0, wxALL|wxALIGN_LEFT, 5);
	FlexGridSizer11 = new wxFlexGridSizer(1, 2, 0, 0);
	StaticText26 = new wxStaticText(Panel3, ID_STATICTEXT26, _T("Label Format :"), wxPoint(192,200), wxDefaultSize, 0, _T("ID_STATICTEXT26"));
	FlexGridSizer11->Add(StaticText26, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	edFormat = new wxTextCtrl(Panel3, ID_TEXTCTRL8, wxEmptyString, wxPoint(280,196), wxSize(56,23), 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
	edFormat->SetToolTip(_T("Format of the label for the axis. Should be like c++ format."));
	FlexGridSizer11->Add(edFormat, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer5->Add(FlexGridSizer11, 1, wxALL|wxALIGN_LEFT, 5);
	BoxSizer6->Add(BoxSizer5, 0, wxALL|wxALIGN_TOP, 2);
	BoxSizer4->Add(BoxSizer6, 0, wxALL|wxEXPAND, 0);
	Panel3->SetSizer(BoxSizer4);
	BoxSizer4->Fit(Panel3);
	BoxSizer4->SetSizeHints(Panel3);
	Panel4 = new wxPanel(nbConfig, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	FlexGridSizer13 = new wxFlexGridSizer(1, 3, 0, 0);
	FlexGridSizer13->AddGrowableCol(1);
	StaticText1 = new wxStaticText(Panel4, ID_STATICTEXT1, _T("Series name :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer13->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	ChoiceSeries = new wxChoice(Panel4, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	FlexGridSizer13->Add(ChoiceSeries, 1, wxALL|wxEXPAND, 5);
	bDelSeries = new wxButton(Panel4, ID_BUTTON11, _T("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON11"));
	bDelSeries->Disable();
	FlexGridSizer13->Add(bDelSeries, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer8->Add(FlexGridSizer13, 0, wxALL|wxEXPAND, 2);
	FlexGridSizer14 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer14->AddGrowableCol(1);
	StaticText2 = new wxStaticText(Panel4, ID_STATICTEXT2, _T("Name :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer14->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	edSeriesName = new wxTextCtrl(Panel4, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	FlexGridSizer14->Add(edSeriesName, 1, wxALL|wxEXPAND, 5);
	BoxSizer8->Add(FlexGridSizer14, 0, wxALL|wxEXPAND, 2);
	FlexGridSizer15 = new wxFlexGridSizer(1, 2, 0, 0);
	BoxSizer9 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer8 = new wxStaticBoxSizer(wxHORIZONTAL, Panel4, _T("Pen "));
	FlexGridSizer17 = new wxFlexGridSizer(3, 2, 0, 0);
	StaticText3 = new wxStaticText(Panel4, ID_STATICTEXT3, _T("Color :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer17->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	bSeriesPenColor = new wxButton(Panel4, ID_BUTTON1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	FlexGridSizer17->Add(bSeriesPenColor, 1, wxALL|wxEXPAND, 2);
	StaticText4 = new wxStaticText(Panel4, ID_STATICTEXT4, _T("Width :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer17->Add(StaticText4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbSeriesPenWidth = new wxChoice(Panel4, ID_CHOICE2, wxDefaultPosition, wxDefaultSize, 10, cbPenWidth_choices, 0, wxDefaultValidator, _T("ID_CHOICE2"));
	FlexGridSizer17->Add(cbSeriesPenWidth, 1, wxALL|wxEXPAND, 2);
	StaticText5 = new wxStaticText(Panel4, ID_STATICTEXT5, _T("Style :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer17->Add(StaticText5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbSeriesPenStyle = new wxChoice(Panel4, ID_CHOICE3, wxDefaultPosition, wxDefaultSize, 5, cbPenStyle_choices, 0, wxDefaultValidator, _T("ID_CHOICE3"));
	FlexGridSizer17->Add(cbSeriesPenStyle, 1, wxALL|wxEXPAND, 2);
	StaticBoxSizer8->Add(FlexGridSizer17, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer9->Add(StaticBoxSizer8, 0, wxALL|wxALIGN_LEFT, 2);
	BoxSizer11 = new wxBoxSizer(wxVERTICAL);
	cbSeriesVisible = new wxCheckBox(Panel4, ID_CHECKBOX1, _T("Visible"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	cbSeriesVisible->SetValue(false);
	BoxSizer11->Add(cbSeriesVisible, 1, wxALL|wxALIGN_LEFT, 5);
	cbSeriesContinuity = new wxCheckBox(Panel4, ID_CHECKBOX2, _T("Continuity"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	cbSeriesContinuity->SetValue(false);
	BoxSizer11->Add(cbSeriesContinuity, 1, wxALL|wxALIGN_LEFT, 5);
	cbSeriesOutside = new wxCheckBox(Panel4, ID_CHECKBOX3, _T("Draw Outside Margins"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	cbSeriesOutside->SetValue(false);
	BoxSizer11->Add(cbSeriesOutside, 1, wxALL|wxALIGN_LEFT, 5);
	cbSeriesShowName = new wxCheckBox(Panel4, ID_CHECKBOX6, _T("Show name"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX6"));
	cbSeriesShowName->SetValue(false);
	BoxSizer11->Add(cbSeriesShowName, 1, wxALL|wxALIGN_LEFT, 5);
	BoxSizer9->Add(BoxSizer11, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer15->Add(BoxSizer9, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer10 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer9 = new wxStaticBoxSizer(wxHORIZONTAL, Panel4, _T("Brush "));
	FlexGridSizer16 = new wxFlexGridSizer(2, 2, 0, 0);
	StaticText6 = new wxStaticText(Panel4, ID_STATICTEXT6, _T("Color :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer16->Add(StaticText6, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	bSeriesBrushColor = new wxButton(Panel4, ID_BUTTON2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	FlexGridSizer16->Add(bSeriesBrushColor, 1, wxALL|wxEXPAND, 2);
	StaticText7 = new wxStaticText(Panel4, ID_STATICTEXT7, _T("Style :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer16->Add(StaticText7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbSeriesBrushStyle = new wxChoice(Panel4, ID_CHOICE4, wxDefaultPosition, wxDefaultSize, 8, cbSeriesBrushStyle_choices, 0, wxDefaultValidator, _T("ID_CHOICE4"));
	FlexGridSizer16->Add(cbSeriesBrushStyle, 1, wxALL|wxEXPAND, 2);
	StaticBoxSizer9->Add(FlexGridSizer16, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer10->Add(StaticBoxSizer9, 0, wxALL|wxALIGN_LEFT, 2);
	StaticBoxSizer10 = new wxStaticBoxSizer(wxHORIZONTAL, Panel4, _T("Symbol "));
	FlexGridSizer18 = new wxFlexGridSizer(2, 2, 0, 0);
	StaticText9 = new wxStaticText(Panel4, ID_STATICTEXT9, _T("Type :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer18->Add(StaticText9, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbSeriesSymbolType = new wxChoice(Panel4, ID_CHOICE5, wxDefaultPosition, wxDefaultSize, 7, cbSeriesSymbolType_choices, 0, wxDefaultValidator, _T("ID_CHOICE5"));
	FlexGridSizer18->Add(cbSeriesSymbolType, 1, wxALL|wxEXPAND, 2);
	StaticText10 = new wxStaticText(Panel4, ID_STATICTEXT10, _T("Size :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer18->Add(StaticText10, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbSeriesSymbolSize = new wxSpinCtrl(Panel4, ID_SPINCTRL2, _T("4"), wxDefaultPosition, wxDefaultSize, 0, 4, 100, 4, _T("ID_SPINCTRL2"));
	cbSeriesSymbolSize->SetValue(_T("4"));
	FlexGridSizer18->Add(cbSeriesSymbolSize, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer10->Add(FlexGridSizer18, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
	BoxSizer10->Add(StaticBoxSizer10, 0, wxALL, 2);
	FlexGridSizer19 = new wxFlexGridSizer(1, 2, 0, 0);
	StaticText19 = new wxStaticText(Panel4, ID_STATICTEXT19, _T("Skip point over :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT19"));
	FlexGridSizer19->Add(StaticText19, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	cbSeriesStep = new wxSpinCtrl(Panel4, ID_SPINCTRL1, _T("1"), wxDefaultPosition, wxDefaultSize, 0, 1, 100, 1, _T("ID_SPINCTRL1"));
	cbSeriesStep->SetValue(_T("1"));
	FlexGridSizer19->Add(cbSeriesStep, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer10->Add(FlexGridSizer19, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	cbBar = new wxCheckBox(Panel4, ID_CHECKBOX12, _T("View as bar"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX12"));
	cbBar->SetValue(false);
	cbBar->Disable();
	BoxSizer10->Add(cbBar, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer15->Add(BoxSizer10, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer8->Add(FlexGridSizer15, 0, wxALL|wxEXPAND, 2);
	Panel4->SetSizer(BoxSizer8);
	BoxSizer8->Fit(Panel4);
	BoxSizer8->SetSizeHints(Panel4);
	nbConfig->AddPage(Panel1, _T("General"), false);
	nbConfig->AddPage(Panel2, _T("Legend"), false);
	nbConfig->AddPage(Panel3, _T("Axis"), false);
	nbConfig->AddPage(Panel4, _T("Series"), false);
	sizerMain->Add(nbConfig, 1, wxALL|wxEXPAND, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	bApply = new wxButton(this, ID_BUTTON3, _T("Apply"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	BoxSizer2->Add(bApply, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	bClose = new wxButton(this, ID_BUTTON4, _T("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
	BoxSizer2->Add(bClose, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
	sizerMain->Add(BoxSizer2, 0, wxEXPAND, 4);
	SetSizer(sizerMain);
	sizerMain->Fit(this);
	sizerMain->SetSizeHints(this);

	Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbFontClick);
	Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbColorClick);
	Connect(ID_BUTTON10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbColorClick);
	Connect(ID_BUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbColorClick);
	Connect(ID_CHOICE7,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&MathPlotConfigDialog::OnAxisSelect);
	Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbFontClick);
	Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbColorClick);
	Connect(ID_CHECKBOX11,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OncbAutoScaleClick);
	Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&MathPlotConfigDialog::OnChoiceSeries);
	Connect(ID_BUTTON11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbDelSeriesClick);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbColorClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbColorClick);
	Connect(ID_NOTEBOOK1,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&MathPlotConfigDialog::OnnbConfigPageChanged);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnbApplyClick);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotConfigDialog::OnQuit);
	//*)

#ifdef _WIN32
#else
  cbLegendBrushStyle->SetToolTip(_T("Transparent not work on Linux"));
#endif // _WIN32

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

	scale_min = -1;
	scale_max = 1;
	CheckBar = false;
//    Initialize();
}

MathPlotConfigDialog::~MathPlotConfigDialog()
{
	//(*Destroy(MathPlotConfigDialog)
	//*)
}

void MathPlotConfigDialog::Initialize()
{
	CurrentTitle = (mpText*) m_plot->GetLayerByClassName(_("mpTitle"));
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
	edMarginTop->GetValidator()->TransferToWindow();
	edMarginBottom->GetValidator()->TransferToWindow();
	edMarginLeft->GetValidator()->TransferToWindow();
	edMarginRight->GetValidator()->TransferToWindow();

	cbDrawBox->SetValue(m_plot->GetDrawBox());
	bBGColor->SetBackgroundColour(m_plot->GetbgColour());

	CurrentLegend = (mpInfoLegend*) m_plot->GetLayerByClassName(_("mpInfoLegend"));
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

	CurrentCoords = (mpInfoCoords*) m_plot->GetLayerByClassName(_("mpInfoCoords"));
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

	switch (nbConfig->GetSelection())
	{
		case 2:
			CurrentChoice = ChoiceAxis;
			break;
		case 3:
			CurrentChoice = ChoiceSeries;
			break;
		default : CurrentChoice = NULL;
	}
}

void MathPlotConfigDialog::OnQuit(wxCommandEvent& WXUNUSED(event))
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

	ColourDialog.SetTitle(_T("Please choose the background colour"));
	if (ColourDialog.ShowModal() == wxID_OK)
	{
		m_clrData = ColourDialog.GetColourData();
		DoApplyColour(m_clrData.GetColour());
	}
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
	mpFloatRect BoundScale = m_plot->Get_Bound();

	if (ChoiceAxis->GetSelection() == 0)
	{
		CurrentScale = (mpScale*) m_plot->GetLayerByClassName(_("mpScaleX"));
		cbAxisPosition->Clear();
		for (int i = 0; i <= mpALIGN_BORDER_TOP; i++)
			cbAxisPosition->Append(XAxis_Align[i]);
	}
	else
	{
		CurrentScale = (mpScale*) m_plot->GetLayerByClassName(_("mpScaleY"));
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

		cbAutoScale->SetValue(CurrentScale->GetAuto());
		edScaleMin->Enable(!CurrentScale->GetAuto());
		edScaleMax->Enable(!CurrentScale->GetAuto());
		if (CurrentScale->GetAuto())
		{
			if (ChoiceAxis->GetSelection() == 0)
			{
				scale_min = BoundScale.Xmin;
				scale_max = BoundScale.Xmax;
			}
			else
			{
				scale_min = BoundScale.Ymin;
				scale_max = BoundScale.Ymax;
			}
		}
		else
		{
			scale_min = CurrentScale->GetMinScale();
			scale_max = CurrentScale->GetMaxScale();
		}
		edScaleMin->GetValidator()->TransferToWindow();
		edScaleMax->GetValidator()->TransferToWindow();
	}
}

void MathPlotConfigDialog::OnAxisSelect(wxCommandEvent& WXUNUSED(event))
{
	UpdateAxis();
	CurrentChoice = ChoiceAxis;
}

void MathPlotConfigDialog::OncbAutoScaleClick(wxCommandEvent& WXUNUSED(event))
{
	edScaleMin->Enable(!cbAutoScale->GetValue());
	edScaleMax->Enable(!cbAutoScale->GetValue());
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

		mpFunctionType func;
		CheckBar = (CurrentSerie->IsFunction(&func) && ((func == mpfFXYVector) || (func == mpfBar)));

		if (CheckBar)
		{
			cbBar->Enable();
			cbBar->SetValue(func == mpfBar);
		}
		else
		{
			cbBar->Disable();
			cbBar->SetValue(false);
		}

	}
	else
		CurrentChoice = NULL;
}

void MathPlotConfigDialog::OnChoiceSeries(wxCommandEvent& WXUNUSED(event))
{
	UpdateSelectedSerie();
}

void MathPlotConfigDialog::OnbDelSeriesClick(wxCommandEvent& WXUNUSED(event))
{
	if (CurrentSerie && CurrentSerie->GetCanDelete())
	{
		if (wxMessageDialog(this, _T("Delete the serie ?"), _T("Confirmation"), wxYES_NO | wxCENTRE).ShowModal() == wxID_YES)
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

void MathPlotConfigDialog::OnbApplyClick(wxCommandEvent& WXUNUSED(event))
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

			edMarginTop->GetValidator()->TransferFromWindow();
			edMarginBottom->GetValidator()->TransferFromWindow();
			edMarginLeft->GetValidator()->TransferFromWindow();
			edMarginRight->GetValidator()->TransferFromWindow();
			m_plot->SetMargins(int_top, int_right, int_bottom, int_left);

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

				CurrentScale->SetAuto(cbAutoScale->GetValue());
				edScaleMin->GetValidator()->TransferFromWindow();
				edScaleMax->GetValidator()->TransferFromWindow();
				CurrentScale->SetMinScale(scale_min);
				CurrentScale->SetMaxScale(scale_max);

				if (fontAxisChanged)
				{
					CurrentScale->SetFont(bFontAxis->GetFont());
					CurrentScale->SetFontColour(bFontAxis->GetForegroundColour());
					fontAxisChanged = false;
				}

				if (!CurrentScale->GetAuto())
				{
					mpFloatRect BoundScale = m_plot->Get_Bound();
					if (ChoiceAxis->GetSelection() == 0)
					{
						BoundScale.Xmin = scale_min;
						BoundScale.Xmax = scale_max;

						// Get bound of the other axis
						mpScale *axis = (mpScale*) m_plot->GetLayerByClassName(_("mpScaleY"));
						if (!axis->GetAuto())
						{
							BoundScale.Ymin = axis->GetMinScale();
							BoundScale.Ymax = axis->GetMaxScale();
						}
					}
					else
					{
						BoundScale.Ymin = scale_min;
						BoundScale.Ymax = scale_max;

						// Get bound of the other axis
						mpScale *axis = (mpScale*) m_plot->GetLayerByClassName(_("mpScaleX"));
						if (!axis->GetAuto())
						{
							BoundScale.Xmin = axis->GetMinScale();
							BoundScale.Xmax = axis->GetMaxScale();
						}
					}
					m_plot->Fit(BoundScale);
				}
				else
			  	m_plot->Fit();

				// Refresh page
				UpdateAxis();
			}
			break;

		case 3: // Series page
			if ((CurrentChoice == ChoiceSeries) && (CurrentSerie != NULL))
			{
				CurrentSerie->SetName(edSeriesName->GetValue());
				ChoiceSeries->SetString(ChoiceSeries->GetSelection(), edSeriesName->GetValue());

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

				if (CheckBar)
				{
					((mpFXYVector *)CurrentSerie)->SetViewMode(cbBar->GetValue());
					if (cbBar->GetValue())
						cbSeriesContinuity->SetValue(false);
				}

				if (CurrentLegend)
					CurrentLegend->SetNeedUpdate();

				m_plot->Refresh();
			}
			break;
		default:
			;
	}
}

