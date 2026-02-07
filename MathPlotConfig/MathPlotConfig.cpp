/***************************************************************
 * Name:      MathPlotConfig.cpp
 * Purpose:   Code for Application Frame
 * Author:    Lionel ()
 * Created:   2021-01-24
 * Copyright: Lionel ()
 * License:
 **************************************************************/

#ifdef ENABLE_MP_CONFIG

#include "mathplot.h" // includes MathPlotConfig.h when ENABLE_MP_CONFIG defined
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

//(*InternalHeaders(MathPlotConfigDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(MathPlotConfigDialog)
//*)

#ifdef ENABLE_MP_NAMESPACE // MathPlot namespace
  namespace MathPlot {
#endif // ENABLE_MP_NAMESPACE

/***************************************************
 * Helper class to save/restore configuration
 ***************************************************/

void MathPlotConfigSettings::SetSettings(wxWindow* win)
{
  if (!win)
    return;

  DoPosition(true, win);

  // Go directly to the notebook
  win = win->FindWindow(_T("notebook"));
  if (win)
    DoRecursiveSearch(true, win);
}

void MathPlotConfigSettings::GetSettings(wxWindow* win)
{
  if (!win)
    return;

  DoPosition(false, win);

  // Go directly to the notebook
  win = win->FindWindow(_T("notebook"));
  if (win)
    DoRecursiveSearch(false, win);
}

/**
 * Get/set the position of the config window in the screen
 * @param set : set if true else get
 * @param win : the config window
 */
void MathPlotConfigSettings::DoPosition(bool set, wxWindow* win)
{
  SetPath(_T("/Position"));
  if (set)
  {
    int posX, posY;
    if (Read(_T("PosX"), &posX) && Read(_T("PosY"), &posY))
      win->Move(wxPoint(posX, posY), wxSIZE_FORCE);
  }
  else // get
  {
    wxPoint pos = win->GetScreenPosition();
    Write(_T("PosX"), pos.x);
    Write(_T("PosY"), pos.y);
  }
}

/**
 * Search recursively all the widgets then get/set parameters
 * @param set : set if true else get
 * @param win : the window (container)
 * @param path : the complete path (created with the name (class) of each window) to reach the window
 * @param level : the depth in the window (container) hierarchy
 */
void MathPlotConfigSettings::DoRecursiveSearch(bool set, wxWindow* win, const wxString& path, int level)
{
  // We save only General (panel 1) and Legend (panel 2)
  if ((win->GetName()).IsSameAs(_T("panel")))
  {
    if (level > 2)
      return;
  }

  // wxTextCtrl: edit widget
  if (win->IsKindOf(wxCLASSINFO(wxTextCtrl)))
  {
    wxTextCtrl* text = (wxTextCtrl*)win;
    SetPath(path);
    wxString key = _T("Text") + wxString::Format(_T("%d"), level);
    if (set)
    {
      wxString value;
      if (Read(key, &value))
        text->SetValue(value);
    }
    else // get
    {
      Write(key, text->GetValue());
    }
    return;
  }

  // wxChoice: choice list widget
  if (win->IsKindOf(wxCLASSINFO(wxChoice)))
  {
    wxChoice* choice = (wxChoice*)win;
    SetPath(path);
    wxString key = _T("Choice") + wxString::Format(_T("%d"), level);
    if (set)
    {
      int value;
      if (Read(key, &value))
        choice->SetSelection(value);
    }
    else // get
    {
      Write(key, choice->GetSelection());
    }
    return;
  }

  // wxCheckBox: check box widget
  if (win->IsKindOf(wxCLASSINFO(wxCheckBox)))
  {
    wxCheckBox* check = (wxCheckBox*)win;
    SetPath(path);
    wxString key = _T("Check") + wxString::Format(_T("%d"), level);
    if (set)
    {
      bool value;
      if (Read(key, &value))
        check->SetValue(value);
    }
    else // get
    {
      Write(key, check->GetValue());
    }
    return;
  }

  // wxButton: button widget
  if (win->IsKindOf(wxCLASSINFO(wxButton)))
  {
    wxButton* button = (wxButton*)win;
    SetPath(path);
    // It is a button for font configuration
    if ((button->GetLabel()).IsSameAs(_T("Font")))
    {
      wxString key1 = _T("ButtonFont") + wxString::Format(_T("%d"), level);
      wxString key2 = _T("ButtonForeground") + wxString::Format(_T("%d"), level);
      if (set)
      {
        wxFont font;
        if (Read(key1, &font))
          button->SetFont(font);
        wxColour value;
        if (Read(key2, &value))
          button->SetForegroundColour(value);
      }
      else // get
      {
        Write(key1, button->GetFont());
        Write(key2, button->GetForegroundColour());
      }
    }
    else // It is a button for background colour configuration
    {
      wxString key = _T("ButtonBackground") + wxString::Format(_T("%d"), level);
      if (set)
      {
        wxColour value;
        if (Read(key, &value))
          button->SetBackgroundColour(value);
      }
      else // get
      {
        Write(key, button->GetBackgroundColour());
      }
    }
    return;
  }

  wxWindowList& children = win->GetChildren();
  wxString winPath = path + _T("/") + win->GetName();
  if (level != 0)
    winPath += wxString::Format(_T("%d"), level);
  int i = 1;
#if wxCHECK_VERSION(3, 0, 0)
  for (wxWindow* child : children)
  {
    if (!child->IsKindOf(wxCLASSINFO(wxStaticText))) // Skip static text
    {
      DoRecursiveSearch(set, child, winPath, i);
      i++;
    }
  }
#else
  for (wxWindowList::Node* node = children.GetFirst(); node; node = node->GetNext())
  {
    wxWindow* current = (wxWindow*)node->GetData();
    if (!current->IsKindOf(wxCLASSINFO(wxStaticText))) // Skip static text
    {
      DoRecursiveSearch(set, current, winPath, i);
      i++;
    }
  }
#endif
}

/***************************************************
 * MathPlotConfigDialog class
 ***************************************************/

// List of string message used
wxString MESS_TRANSPARENT = _T("");
wxString MESS_COLOUR = _T("");
wxString MESS_AXIS_DELETE = _T("");
wxString MESS_DELETE = _T("");
wxString MESS_LINES_ADD = _T("");
wxString MESS_LINES_DELETE = _T("");
wxString MESS_CONFIRM = _T("");

// Axis position
wxString XAxis_Align[5] = {_T("")};
wxString YAxis_Align[5] = {_T("")};

BEGIN_EVENT_TABLE(MathPlotConfigDialog,wxDialog)
//(*EventTable(MathPlotConfigDialog)
//*)
END_EVENT_TABLE()

MathPlotConfigDialog::MathPlotConfigDialog(wxWindow *parent, wxWindowID WXUNUSED(id))
{
  // Defined here for I18N translation
  MESS_TRANSPARENT  = _("Transparent not work on Linux");
  MESS_COLOUR       = _("Please choose the background colour");
  MESS_AXIS_DELETE  = _("Delete the selected axis ?");
  MESS_DELETE       = _("Delete the serie ?");
  MESS_LINES_ADD    = _("Add horizontal line (vertical if No) ?");
  MESS_LINES_DELETE = _("Delete the line ?");
  MESS_CONFIRM      = _("Confirmation");
  XAxis_Align[0] = _("Bottom border");
  XAxis_Align[1] = _("Bottom");
  XAxis_Align[2] = _("Center");
  XAxis_Align[3] = _("Top");
  XAxis_Align[4] = _("Top border");
  YAxis_Align[0] = _("Left border");
  YAxis_Align[1] = _("Left");
  YAxis_Align[2] = _("Center");
  YAxis_Align[3] = _("Right");
  YAxis_Align[4] = _("Right border");

  // Common choices
  const wxString PenWidth_choices[] = {
  _T("1"),
  _T("2"),
  _T("3"),
  _T("4"),
  _T("5"),
  _T("6"),
  _T("7"),
  _T("8"),
  _T("9"),
  _T("10"),
  };

  const wxString PenStyle_choices[] = {
  _("Solid"),
  _("Dot"),
  _("Long Dash"),
  _("Short Dash"),
  _("Dot Dash"),
  };

  const wxString InfoBrushStyle_choices[] = {
  _("Solid"),
  _("Transparent"),
  };

  const wxString InfoPosition_choices[] = {
  _("Left center"),
  _("Top left"),
  _("Top center"),
  _("Top right"),
  _("Right center"),
  _("Bottom left"),
  _("Bottom center"),
  _("Bottom right"),
  _("Default position"),
  _("Cursor position"),
  };

  //(*Initialize(MathPlotConfigDialog)
  wxBoxSizer* BoxSizer10;
  wxBoxSizer* BoxSizer11;
  wxBoxSizer* BoxSizer12;
  wxBoxSizer* BoxSizer13;
  wxBoxSizer* BoxSizer14;
  wxBoxSizer* BoxSizer15;
  wxBoxSizer* BoxSizer16;
  wxBoxSizer* BoxSizer17;
  wxBoxSizer* BoxSizer18;
  wxBoxSizer* BoxSizer19;
  wxBoxSizer* BoxSizer1;
  wxBoxSizer* BoxSizer20;
  wxBoxSizer* BoxSizer21;
  wxBoxSizer* BoxSizer22;
  wxBoxSizer* BoxSizer2;
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
  wxFlexGridSizer* FlexGridSizer20;
  wxFlexGridSizer* FlexGridSizer21;
  wxFlexGridSizer* FlexGridSizer3;
  wxFlexGridSizer* FlexGridSizer4;
  wxFlexGridSizer* FlexGridSizer5;
  wxFlexGridSizer* FlexGridSizer6;
  wxFlexGridSizer* FlexGridSizer7;
  wxFlexGridSizer* FlexGridSizer8;
  wxFlexGridSizer* FlexGridSizer9;
  wxStaticBoxSizer* StaticBoxSizer10;
  wxStaticBoxSizer* StaticBoxSizer11;
  wxStaticBoxSizer* StaticBoxSizer12;
  wxStaticBoxSizer* StaticBoxSizer1;
  wxStaticBoxSizer* StaticBoxSizer2;
  wxStaticBoxSizer* StaticBoxSizer3;
  wxStaticBoxSizer* StaticBoxSizer4;
  wxStaticBoxSizer* StaticBoxSizer5;
  wxStaticBoxSizer* StaticBoxSizer6;
  wxStaticBoxSizer* StaticBoxSizer7;
  wxStaticBoxSizer* StaticBoxSizer8;
  wxStaticBoxSizer* StaticBoxSizer9;

  Create(parent, wxID_ANY, _("MathPlot Configuration"), wxDefaultPosition, wxDefaultSize, wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
  sizerMain = new wxBoxSizer(wxVERTICAL);
  nbConfig = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
  Panel1 = new wxPanel(nbConfig, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  BoxSizer3 = new wxBoxSizer(wxVERTICAL);
  StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, Panel1, _("Title "));
  FlexGridSizer1 = new wxFlexGridSizer(2, 3, 0, 0);
  FlexGridSizer1->AddGrowableCol(1);
  StaticText11 = new wxStaticText(Panel1, wxID_ANY, _("Title :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer1->Add(StaticText11, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  edTitle = new wxTextCtrl(Panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  FlexGridSizer1->Add(edTitle, 1, wxALL|wxEXPAND, 5);
  bFontTitle = new wxButton(Panel1, wxID_ANY, _("Font"), wxDefaultPosition, wxSize(64,-1), 0, wxDefaultValidator);
  bFontTitle->Disable();
  FlexGridSizer1->Add(bFontTitle, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
  cbTitleVisible = new wxCheckBox(Panel1, wxID_ANY, _("Visible"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbTitleVisible->SetValue(false);
  FlexGridSizer1->Add(cbTitleVisible, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  StaticBoxSizer1->Add(FlexGridSizer1, 1, wxLEFT|wxRIGHT|wxEXPAND, 5);
  BoxSizer3->Add(StaticBoxSizer1, 0, wxALL|wxEXPAND, 2);
  StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, Panel1, _("Margins "));
  BoxSizer17 = new wxBoxSizer(wxVERTICAL);
  BoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
  StaticText41 = new wxStaticText(Panel1, wxID_ANY, _("Top :"), wxDefaultPosition, wxDefaultSize, 0);
  BoxSizer20->Add(StaticText41, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  edMarginTop = new wxTextCtrl(Panel1, wxID_ANY, _T("50"), wxDefaultPosition, wxSize(40,-1), wxTE_RIGHT, wxIntegerValidator<unsigned int> (&int_top));
  edMarginTop->SetToolTip(_("Top"));
  BoxSizer20->Add(edMarginTop, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  StaticText38 = new wxStaticText(Panel1, wxID_ANY, _("Bottom :"), wxDefaultPosition, wxDefaultSize, 0);
  BoxSizer20->Add(StaticText38, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  edMarginBottom = new wxTextCtrl(Panel1, wxID_ANY, _T("50"), wxDefaultPosition, wxSize(40,-1), wxTE_RIGHT, wxIntegerValidator<unsigned int> (&int_bottom));
  edMarginBottom->SetToolTip(_("Bottom"));
  BoxSizer20->Add(edMarginBottom, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  StaticText39 = new wxStaticText(Panel1, wxID_ANY, _("Left :"), wxDefaultPosition, wxDefaultSize, 0);
  BoxSizer20->Add(StaticText39, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  edMarginLeft = new wxTextCtrl(Panel1, wxID_ANY, _T("50"), wxDefaultPosition, wxSize(40,-1), wxTE_RIGHT, wxIntegerValidator<unsigned int> (&int_left));
  edMarginLeft->SetToolTip(_("Left"));
  BoxSizer20->Add(edMarginLeft, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  StaticText40 = new wxStaticText(Panel1, wxID_ANY, _("Right :"), wxDefaultPosition, wxDefaultSize, 0);
  BoxSizer20->Add(StaticText40, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  edMarginRight = new wxTextCtrl(Panel1, wxID_ANY, _T("50"), wxDefaultPosition, wxSize(40,-1), wxTE_RIGHT, wxIntegerValidator<unsigned int> (&int_right));
  edMarginRight->SetToolTip(_("Right"));
  BoxSizer20->Add(edMarginRight, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  BoxSizer17->Add(BoxSizer20, 1, wxLEFT|wxRIGHT|wxEXPAND, 5);
  BoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
  cbDrawBox = new wxCheckBox(Panel1, wxID_ANY, _("Draw box"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbDrawBox->SetValue(false);
  BoxSizer21->Add(cbDrawBox, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  StaticText37 = new wxStaticText(Panel1, wxID_ANY, _("Extra margin :"), wxDefaultPosition, wxDefaultSize, 0);
  BoxSizer21->Add(StaticText37, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  edExtraMargin = new wxTextCtrl(Panel1, wxID_ANY, _T("8"), wxDefaultPosition, wxSize(30,-1), wxTE_RIGHT, wxIntegerValidator<unsigned int> (&int_extra));
  edExtraMargin->SetToolTip(_("Extra margin for plot area"));
  BoxSizer21->Add(edExtraMargin, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  BoxSizer21->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  bBGColor = new wxButton(Panel1, wxID_ANY, _("bg color"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  bBGColor->SetToolTip(_("Color of the background of the plot area"));
  BoxSizer21->Add(bBGColor, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  BoxSizer17->Add(BoxSizer21, 1, wxLEFT|wxRIGHT|wxEXPAND, 5);
  StaticBoxSizer2->Add(BoxSizer17, 1, wxALL|wxEXPAND, 0);
  BoxSizer3->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND, 2);
  StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("Mouse coordinates "));
  BoxSizer1 = new wxBoxSizer(wxVERTICAL);
  BoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
  StaticText18 = new wxStaticText(Panel1, wxID_ANY, _("Position :"), wxDefaultPosition, wxDefaultSize, 0);
  BoxSizer12->Add(StaticText18, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  cbCoord = new wxChoice(Panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  cbCoord->Set(WXSIZEOF(InfoPosition_choices), InfoPosition_choices);
  cbCoord->SetSelection(7);
  BoxSizer12->Add(cbCoord, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  BoxSizer1->Add(BoxSizer12, 0, wxALL|wxALIGN_LEFT, 0);
  cbCoordVisible = new wxCheckBox(Panel1, wxID_ANY, _("Visible"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbCoordVisible->SetValue(false);
  BoxSizer1->Add(cbCoordVisible, 0, wxALL|wxEXPAND, 5);
  cbCoordOutside = new wxCheckBox(Panel1, wxID_ANY, _("Coordinates in Margins"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbCoordOutside->SetValue(false);
  cbCoordOutside->SetToolTip(_("Shows the coordinates when the mouse is in the margins"));
  BoxSizer1->Add(cbCoordOutside, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
  cbCoordinates = new wxCheckBox(Panel1, wxID_ANY, _("Series coordinates"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbCoordinates->SetValue(false);
  cbCoordinates->SetToolTip(_("Shows the coordinates of the series closest to the mouse position"));
  BoxSizer1->Add(cbCoordinates, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
  cbMagnetize = new wxCheckBox(Panel1, wxID_ANY, _("Magnetize"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbMagnetize->SetValue(false);
  cbMagnetize->SetToolTip(_("Follow the mouse by drawing a horizontal and vertical line"));
  BoxSizer1->Add(cbMagnetize, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
  StaticBoxSizer3->Add(BoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("Brush "));
  FlexGridSizer4 = new wxFlexGridSizer(2, 2, 0, 0);
  StaticText22 = new wxStaticText(Panel1, wxID_ANY, _("Color :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer4->Add(StaticText22, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  bCoordBrushColor = new wxButton(Panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  FlexGridSizer4->Add(bCoordBrushColor, 1, wxALL|wxEXPAND, 2);
  StaticText23 = new wxStaticText(Panel1, wxID_ANY, _("Style :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer4->Add(StaticText23, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  cbCoordBrushStyle = new wxChoice(Panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  cbCoordBrushStyle->Set(WXSIZEOF(InfoBrushStyle_choices), InfoBrushStyle_choices);
  cbCoordBrushStyle->SetSelection(0);
  FlexGridSizer4->Add(cbCoordBrushStyle, 1, wxALL|wxEXPAND, 2);
  StaticBoxSizer4->Add(FlexGridSizer4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
  StaticBoxSizer3->Add(StaticBoxSizer4, 0, wxALL|wxALIGN_TOP, 5);
  BoxSizer3->Add(StaticBoxSizer3, 0, wxALL|wxEXPAND, 2);
  StaticBoxSizer12 = new wxStaticBoxSizer(wxHORIZONTAL, Panel1, _("Mouse interaction"));
  BoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
  StaticText36 = new wxStaticText(Panel1, wxID_ANY, _("Left mouse action :"), wxDefaultPosition, wxDefaultSize, 0);
  BoxSizer22->Add(StaticText36, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  const wxString ChoiceLeftMouseAction_choices[] = {
  _("Box zoom"),
  _("Drag zoom"),
  };
  ChoiceLeftMouseAction = new wxChoice(Panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, ChoiceLeftMouseAction_choices, 0, wxDefaultValidator);
  BoxSizer22->Add(ChoiceLeftMouseAction, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  StaticBoxSizer12->Add(BoxSizer22, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  BoxSizer3->Add(StaticBoxSizer12, 1, wxALL|wxEXPAND, 2);
  Panel1->SetSizer(BoxSizer3);
  Panel2 = new wxPanel(nbConfig, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  BoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
  FlexGridSizer6 = new wxFlexGridSizer(4, 2, 0, 0);
  StaticText16 = new wxStaticText(Panel2, wxID_ANY, _("Position :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer6->Add(StaticText16, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  cbLegendPosition = new wxChoice(Panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  cbLegendPosition->Set(WXSIZEOF(InfoPosition_choices), InfoPosition_choices);\
  cbLegendPosition->SetSelection(6);
  FlexGridSizer6->Add(cbLegendPosition, 1, wxALL|wxEXPAND, 2);
  StaticText24 = new wxStaticText(Panel2, wxID_ANY, _("Style :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer6->Add(StaticText24, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  const wxString cbLegendStyle_choices[] = {
  _("Line"),
  _("Square"),
  _("Symbol"),
  };
  cbLegendStyle = new wxChoice(Panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, cbLegendStyle_choices, 0, wxDefaultValidator);
  cbLegendStyle->SetSelection(0);
  FlexGridSizer6->Add(cbLegendStyle, 1, wxALL|wxEXPAND, 2);
  StaticText25 = new wxStaticText(Panel2, wxID_ANY, _("Direction :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer6->Add(StaticText25, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  const wxString cbLegendDirection_choices[] = {
  _("Vertical"),
  _("Horizontal"),
  };
  cbLegendDirection = new wxChoice(Panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 2, cbLegendDirection_choices, 0, wxDefaultValidator);
  cbLegendDirection->SetSelection(0);
  FlexGridSizer6->Add(cbLegendDirection, 1, wxALL|wxEXPAND, 2);
  cbLegendVisible = new wxCheckBox(Panel2, wxID_ANY, _("Visible"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbLegendVisible->SetValue(false);
  FlexGridSizer6->Add(cbLegendVisible, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  BoxSizer16->Add(FlexGridSizer6, 1, wxALL|wxALIGN_TOP, 5);
  BoxSizer15 = new wxBoxSizer(wxVERTICAL);
  bFontLegend = new wxButton(Panel2, wxID_ANY, _("Font"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  bFontLegend->Disable();
  BoxSizer15->Add(bFontLegend, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
  StaticBoxSizer5 = new wxStaticBoxSizer(wxHORIZONTAL, Panel2, _("Brush "));
  FlexGridSizer7 = new wxFlexGridSizer(2, 2, 0, 0);
  StaticText20 = new wxStaticText(Panel2, wxID_ANY, _("Color :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer7->Add(StaticText20, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  bLegendBrushColor = new wxButton(Panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  FlexGridSizer7->Add(bLegendBrushColor, 1, wxALL|wxEXPAND, 2);
  StaticText21 = new wxStaticText(Panel2, wxID_ANY, _("Style :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer7->Add(StaticText21, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  cbLegendBrushStyle = new wxChoice(Panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  cbLegendBrushStyle->Set(WXSIZEOF(InfoBrushStyle_choices), InfoBrushStyle_choices);
  cbLegendBrushStyle->SetSelection(0);
  FlexGridSizer7->Add(cbLegendBrushStyle, 1, wxALL|wxEXPAND, 2);
  StaticBoxSizer5->Add(FlexGridSizer7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  BoxSizer15->Add(StaticBoxSizer5, 0, wxALL|wxEXPAND, 0);
  BoxSizer16->Add(BoxSizer15, 1, wxALL|wxALIGN_TOP, 5);
  Panel2->SetSizer(BoxSizer16);
  Panel3 = new wxPanel(nbConfig, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  sizerAxis = new wxBoxSizer(wxVERTICAL);
  FlexGridSizer3 = new wxFlexGridSizer(1, 4, 0, 0);
  FlexGridSizer3->AddGrowableCol(0);
  ChoiceAxis = new wxChoice(Panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  FlexGridSizer3->Add(ChoiceAxis, 1, wxALL|wxEXPAND, 5);
  bAddXAxis = new wxButton(Panel3, wxID_ANY, _("Add X axis"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  FlexGridSizer3->Add(bAddXAxis, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  bAddYAxis = new wxButton(Panel3, wxID_ANY, _("Add Y axis"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  FlexGridSizer3->Add(bAddYAxis, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  bDelAxis = new wxButton(Panel3, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  FlexGridSizer3->Add(bDelAxis, 1, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  sizerAxis->Add(FlexGridSizer3, 0, wxALL|wxEXPAND, 2);
  FlexGridSizer8 = new wxFlexGridSizer(1, 3, 0, 0);
  FlexGridSizer8->AddGrowableCol(1);
  StaticText8 = new wxStaticText(Panel3, wxID_ANY, _("Name :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer8->Add(StaticText8, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  edAxisName = new wxTextCtrl(Panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  FlexGridSizer8->Add(edAxisName, 0, wxALL|wxEXPAND, 5);
  bFontAxis = new wxButton(Panel3, wxID_ANY, _("Font"), wxDefaultPosition, wxSize(64,-1), 0, wxDefaultValidator);
  bFontAxis->Disable();
  FlexGridSizer8->Add(bFontAxis, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  sizerAxis->Add(FlexGridSizer8, 0, wxALL|wxEXPAND, 2);
  BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
  BoxSizer7 = new wxBoxSizer(wxVERTICAL);
  StaticBoxSizer6 = new wxStaticBoxSizer(wxHORIZONTAL, Panel3, _("Pen "));
  FlexGridSizer10 = new wxFlexGridSizer(3, 2, 0, 0);
  StaticText12 = new wxStaticText(Panel3, wxID_ANY, _("Color :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer10->Add(StaticText12, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  bAxisPenColor = new wxButton(Panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  FlexGridSizer10->Add(bAxisPenColor, 1, wxALL|wxEXPAND, 2);
  StaticText13 = new wxStaticText(Panel3, wxID_ANY, _("Width :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer10->Add(StaticText13, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  cbAxisPenWidth = new wxChoice(Panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  cbAxisPenWidth->Set(WXSIZEOF(PenWidth_choices), PenWidth_choices);
  cbAxisPenWidth->SetSelection(0);
  FlexGridSizer10->Add(cbAxisPenWidth, 1, wxALL|wxEXPAND, 2);
  StaticText14 = new wxStaticText(Panel3, wxID_ANY, _("Style :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer10->Add(StaticText14, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  cbAxisPenStyle = new wxChoice(Panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  cbAxisPenStyle->Set(WXSIZEOF(PenStyle_choices), PenStyle_choices);
  FlexGridSizer10->Add(cbAxisPenStyle, 1, wxALL|wxEXPAND, 2);
  StaticBoxSizer6->Add(FlexGridSizer10, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
  BoxSizer7->Add(StaticBoxSizer6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
  StaticBoxSizer7 = new wxStaticBoxSizer(wxHORIZONTAL, Panel3, _("Scale "));
  FlexGridSizer12 = new wxFlexGridSizer(3, 2, 0, 0);
  cbAutoScale = new wxCheckBox(Panel3, wxID_ANY, _("Auto"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbAutoScale->SetValue(true);
  FlexGridSizer12->Add(cbAutoScale, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  FlexGridSizer12->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  StaticText27 = new wxStaticText(Panel3, wxID_ANY, _("Min :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer12->Add(StaticText27, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
  edScaleMin = new wxTextCtrl(Panel3, wxID_ANY, _T("-1"), wxDefaultPosition, wxSize(64,-1), wxTE_RIGHT, wxFloatingPointValidator<double> (2, &scale_min));
  edScaleMin->Disable();
  FlexGridSizer12->Add(edScaleMin, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
  StaticText28 = new wxStaticText(Panel3, wxID_ANY, _("Max :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer12->Add(StaticText28, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
  edScaleMax = new wxTextCtrl(Panel3, wxID_ANY, _T("1"), wxDefaultPosition, wxSize(64,-1), wxTE_RIGHT, wxFloatingPointValidator<double> (2, &scale_max));
  edScaleMax->Disable();
  FlexGridSizer12->Add(edScaleMax, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
  StaticBoxSizer7->Add(FlexGridSizer12, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
  BoxSizer7->Add(StaticBoxSizer7, 0, wxALL|wxEXPAND, 2);
  BoxSizer6->Add(BoxSizer7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
  BoxSizer5 = new wxBoxSizer(wxVERTICAL);
  BoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
  StaticText17 = new wxStaticText(Panel3, wxID_ANY, _("Position :"), wxDefaultPosition, wxDefaultSize, 0);
  BoxSizer13->Add(StaticText17, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  cbAxisPosition = new wxChoice(Panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  BoxSizer13->Add(cbAxisPosition, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  BoxSizer5->Add(BoxSizer13, 0, wxALL|wxALIGN_LEFT, 0);
  cbAxisVisible = new wxCheckBox(Panel3, wxID_ANY, _("Visible"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbAxisVisible->SetValue(false);
  BoxSizer5->Add(cbAxisVisible, 0, wxALL|wxALIGN_LEFT, 5);
  cbGridVisible = new wxCheckBox(Panel3, wxID_ANY, _("Show grid"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbGridVisible->SetValue(false);
  BoxSizer5->Add(cbGridVisible, 0, wxALL|wxALIGN_LEFT, 5);
  cbAxisOutside = new wxCheckBox(Panel3, wxID_ANY, _("Draw Outside Margins"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbAxisOutside->SetValue(false);
  BoxSizer5->Add(cbAxisOutside, 0, wxALL|wxALIGN_LEFT, 5);
  BoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
  StaticText29 = new wxStaticText(Panel3, wxID_ANY, _("Format :"), wxDefaultPosition, wxDefaultSize, 0);
  BoxSizer14->Add(StaticText29, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  const wxString cbFormat_choices[] = {
  _("Auto"),
  _("Decimal"),
  _("Scientific"),
  _("Time"),
  _("Hours"),
  _("Date"),
  _("DateTime"),
  _("User"),
  _("None"),
  };
  cbFormat = new wxChoice(Panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 9, cbFormat_choices, 0, wxDefaultValidator);
  cbFormat->SetSelection(0);
  cbFormat->Set(WXSIZEOF(cbFormat_choices), cbFormat_choices);
  cbFormat->SetSelection(0);
  BoxSizer14->Add(cbFormat, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  BoxSizer5->Add(BoxSizer14, 0, wxALL|wxALIGN_LEFT, 0);
  FlexGridSizer11 = new wxFlexGridSizer(1, 2, 0, 0);
  StaticText26 = new wxStaticText(Panel3, wxID_ANY, _("User Label Format :"), wxPoint(192,200), wxDefaultSize, 0);
  FlexGridSizer11->Add(StaticText26, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  edFormat = new wxTextCtrl(Panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60,-1), 0, wxDefaultValidator);
  edFormat->Disable();
  edFormat->SetToolTip(_("Format of the label for the axis. Should be like c++ format."));
  FlexGridSizer11->Add(edFormat, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  BoxSizer5->Add(FlexGridSizer11, 1, wxALL|wxALIGN_LEFT, 0);
  cbLogAxis = new wxCheckBox(Panel3, wxID_ANY, _("Logarithmic axis"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbLogAxis->SetValue(false);
  BoxSizer5->Add(cbLogAxis, 0, wxALL|wxALIGN_LEFT, 5);
  BoxSizer6->Add(BoxSizer5, 0, wxALL|wxALIGN_TOP, 2);
  sizerAxis->Add(BoxSizer6, 0, wxALL|wxEXPAND, 0);
  Panel3->SetSizer(sizerAxis);
  Panel4 = new wxPanel(nbConfig, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  BoxSizer8 = new wxBoxSizer(wxVERTICAL);
  FlexGridSizer13 = new wxFlexGridSizer(1, 3, 0, 0);
  FlexGridSizer13->AddGrowableCol(1);
  StaticText1 = new wxStaticText(Panel4, wxID_ANY, _("Series name :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer13->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  ChoiceSeries = new wxChoice(Panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  FlexGridSizer13->Add(ChoiceSeries, 1, wxALL|wxEXPAND, 5);
  bDelSeries = new wxButton(Panel4, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  bDelSeries->Disable();
  FlexGridSizer13->Add(bDelSeries, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  BoxSizer8->Add(FlexGridSizer13, 0, wxALL|wxEXPAND, 2);
  FlexGridSizer14 = new wxFlexGridSizer(2, 2, 0, 0);
  FlexGridSizer14->AddGrowableCol(1);
  StaticText2 = new wxStaticText(Panel4, wxID_ANY, _("Name :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer14->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  edSeriesName = new wxTextCtrl(Panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  FlexGridSizer14->Add(edSeriesName, 1, wxALL|wxEXPAND, 5);
  StaticText35 = new wxStaticText(Panel4, wxID_ANY, _("Y axis :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer14->Add(StaticText35, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  ChoiceSeriesYAxis = new wxChoice(Panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  FlexGridSizer14->Add(ChoiceSeriesYAxis, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5);
  BoxSizer8->Add(FlexGridSizer14, 0, wxALL|wxEXPAND, 2);
  FlexGridSizer15 = new wxFlexGridSizer(1, 2, 0, 0);
  BoxSizer9 = new wxBoxSizer(wxVERTICAL);
  StaticBoxSizer8 = new wxStaticBoxSizer(wxHORIZONTAL, Panel4, _("Pen "));
  FlexGridSizer17 = new wxFlexGridSizer(3, 2, 0, 0);
  StaticText3 = new wxStaticText(Panel4, wxID_ANY, _("Color :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer17->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  bSeriesPenColor = new wxButton(Panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  FlexGridSizer17->Add(bSeriesPenColor, 1, wxALL|wxEXPAND, 2);
  StaticText4 = new wxStaticText(Panel4, wxID_ANY, _("Width :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer17->Add(StaticText4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  cbSeriesPenWidth = new wxChoice(Panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  cbSeriesPenWidth->Set(WXSIZEOF(PenWidth_choices), PenWidth_choices);
  FlexGridSizer17->Add(cbSeriesPenWidth, 1, wxALL|wxEXPAND, 2);
  StaticText5 = new wxStaticText(Panel4, wxID_ANY, _("Style :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer17->Add(StaticText5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  cbSeriesPenStyle = new wxChoice(Panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  cbSeriesPenStyle->Set(WXSIZEOF(PenStyle_choices), PenStyle_choices);
  FlexGridSizer17->Add(cbSeriesPenStyle, 1, wxALL|wxEXPAND, 2);
  StaticBoxSizer8->Add(FlexGridSizer17, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
  BoxSizer9->Add(StaticBoxSizer8, 0, wxALL|wxALIGN_LEFT, 2);
  BoxSizer11 = new wxBoxSizer(wxVERTICAL);
  cbSeriesVisible = new wxCheckBox(Panel4, wxID_ANY, _("Visible"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbSeriesVisible->SetValue(false);
  BoxSizer11->Add(cbSeriesVisible, 1, wxALL|wxALIGN_LEFT, 3);
  cbSeriesContinuity = new wxCheckBox(Panel4, wxID_ANY, _("Continuity"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbSeriesContinuity->SetValue(false);
  BoxSizer11->Add(cbSeriesContinuity, 1, wxALL|wxALIGN_LEFT, 3);
  cbSeriesOutside = new wxCheckBox(Panel4, wxID_ANY, _("Draw Outside Margins"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbSeriesOutside->SetValue(false);
  BoxSizer11->Add(cbSeriesOutside, 1, wxALL|wxALIGN_LEFT, 3);
  cbSeriesShowName = new wxCheckBox(Panel4, wxID_ANY, _("Show name"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbSeriesShowName->SetValue(false);
  BoxSizer11->Add(cbSeriesShowName, 1, wxALL|wxALIGN_LEFT, 3);
  cbTractable = new wxCheckBox(Panel4, wxID_ANY, _("Tractable"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbTractable->SetValue(false);
  cbTractable->SetToolTip(_("Allow mouse coordinates"));
  BoxSizer11->Add(cbTractable, 1, wxALL|wxALIGN_LEFT, 3);
  BoxSizer9->Add(BoxSizer11, 0, wxALL|wxALIGN_LEFT, 5);
  FlexGridSizer15->Add(BoxSizer9, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  BoxSizer10 = new wxBoxSizer(wxVERTICAL);
  StaticBoxSizer9 = new wxStaticBoxSizer(wxHORIZONTAL, Panel4, _("Brush "));
  FlexGridSizer16 = new wxFlexGridSizer(2, 2, 0, 0);
  StaticText6 = new wxStaticText(Panel4, wxID_ANY, _("Color :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer16->Add(StaticText6, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  bSeriesBrushColor = new wxButton(Panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  FlexGridSizer16->Add(bSeriesBrushColor, 1, wxALL|wxEXPAND, 2);
  StaticText7 = new wxStaticText(Panel4, wxID_ANY, _("Style :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer16->Add(StaticText7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  const wxString cbSeriesBrushStyle_choices[] = {
  _("Solid"),
  _("Transparent"),
  _("BDiagonal"),
  _("CrossDiagonal"),
  _("FDiagonal"),
  _("Cross"),
  _("Horizontal"),
  _("Vertical"),
  };
  cbSeriesBrushStyle = new wxChoice(Panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, 8, cbSeriesBrushStyle_choices, 0, wxDefaultValidator);
  cbSeriesBrushStyle->SetSelection(0);
  FlexGridSizer16->Add(cbSeriesBrushStyle, 1, wxALL|wxEXPAND, 2);
  StaticBoxSizer9->Add(FlexGridSizer16, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
  BoxSizer10->Add(StaticBoxSizer9, 0, wxALL|wxALIGN_LEFT, 2);
  StaticBoxSizer10 = new wxStaticBoxSizer(wxHORIZONTAL, Panel4, _("Symbol "));
  FlexGridSizer18 = new wxFlexGridSizer(2, 2, 0, 0);
  StaticText9 = new wxStaticText(Panel4, wxID_ANY, _("Type :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer18->Add(StaticText9, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  const wxString cbSeriesSymbolType_choices[] = {
  _("None"),
  _("Circle"),
  _("Square"),
  _("UpTriangle"),
  _("DownTriangle"),
  _("Cross"),
  _("Plus"),
  };
  cbSeriesSymbolType = new wxChoice(Panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, 7, cbSeriesSymbolType_choices, 0, wxDefaultValidator);
  cbSeriesSymbolType->SetSelection(0);
  FlexGridSizer18->Add(cbSeriesSymbolType, 1, wxALL|wxEXPAND, 2);
  StaticText10 = new wxStaticText(Panel4, wxID_ANY, _("Size :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer18->Add(StaticText10, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  cbSeriesSymbolSize = new wxSpinCtrl(Panel4, wxID_ANY, _T("4"), wxDefaultPosition, wxDefaultSize, 0, 4, 100, 4);
  cbSeriesSymbolSize->SetValue(_T("4"));
  FlexGridSizer18->Add(cbSeriesSymbolSize, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
  StaticBoxSizer10->Add(FlexGridSizer18, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
  BoxSizer10->Add(StaticBoxSizer10, 0, wxALL, 2);
  FlexGridSizer19 = new wxFlexGridSizer(1, 2, 0, 0);
  StaticText19 = new wxStaticText(Panel4, wxID_ANY, _("Skip point over :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer19->Add(StaticText19, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  cbSeriesStep = new wxSpinCtrl(Panel4, wxID_ANY, _T("1"), wxDefaultPosition, wxDefaultSize, 0, 1, 100, 1);
  cbSeriesStep->SetValue(_T("1"));
  FlexGridSizer19->Add(cbSeriesStep, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  BoxSizer10->Add(FlexGridSizer19, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  cbBar = new wxCheckBox(Panel4, wxID_ANY, _("View as bar"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbBar->SetValue(false);
  cbBar->Disable();
  BoxSizer10->Add(cbBar, 1, wxALL|wxEXPAND, 5);
  FlexGridSizer15->Add(BoxSizer10, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
  BoxSizer8->Add(FlexGridSizer15, 0, wxALL|wxEXPAND, 2);
  Panel4->SetSizer(BoxSizer8);
  Panel5 = new wxPanel(nbConfig, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  sizerLines = new wxBoxSizer(wxVERTICAL);
  FlexGridSizer5 = new wxFlexGridSizer(1, 5, 0, 0);
  FlexGridSizer5->AddGrowableCol(0);
  ChoiceLines = new wxChoice(Panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  FlexGridSizer5->Add(ChoiceLines, 1, wxALL|wxEXPAND, 5);
  bAddLines = new wxButton(Panel5, wxID_ANY, _("Add"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  FlexGridSizer5->Add(bAddLines, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  bDelLines = new wxButton(Panel5, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  bDelLines->Disable();
  FlexGridSizer5->Add(bDelLines, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  sizerLines->Add(FlexGridSizer5, 0, wxALL|wxEXPAND, 2);
  pLines = new wxPanel(Panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
  BoxSizer4 = new wxBoxSizer(wxVERTICAL);
  FlexGridSizer9 = new wxFlexGridSizer(3, 3, 0, 0);
  FlexGridSizer9->AddGrowableCol(1);
  StaticText31 = new wxStaticText(pLines, wxID_ANY, _("Name :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer9->Add(StaticText31, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  edLinesName = new wxTextCtrl(pLines, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  FlexGridSizer9->Add(edLinesName, 1, wxALL|wxEXPAND, 5);
  rbLinesDirection = new wxRadioButton(pLines, wxID_ANY, _("Horizontal"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  rbLinesDirection->SetValue(true);
  rbLinesDirection->Disable();
  FlexGridSizer9->Add(rbLinesDirection, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  StaticText30 = new wxStaticText(pLines, wxID_ANY, _("Value :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer9->Add(StaticText30, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  edLinesValue = new wxTextCtrl(pLines, wxID_ANY, _T("1"), wxDefaultPosition, wxSize(64,-1), wxTE_RIGHT, wxFloatingPointValidator<double> (2, &line_value));
  FlexGridSizer9->Add(edLinesValue, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  FlexGridSizer9->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  stLinesYIndexLabel = new wxStaticText(pLines, wxID_ANY, _("Y axis :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer9->Add(stLinesYIndexLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  ChoiceLinesYAxis = new wxChoice(pLines, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  FlexGridSizer9->Add(ChoiceLinesYAxis, 1, wxALL|wxEXPAND, 5);
  BoxSizer4->Add(FlexGridSizer9, 0, wxALL|wxEXPAND, 5);
  FlexGridSizer20 = new wxFlexGridSizer(1, 2, 0, 0);
  BoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
  StaticBoxSizer11 = new wxStaticBoxSizer(wxHORIZONTAL, pLines, _("Pen "));
  FlexGridSizer21 = new wxFlexGridSizer(3, 2, 0, 0);
  StaticText32 = new wxStaticText(pLines, wxID_ANY, _("Color :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer21->Add(StaticText32, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  bLinesPenColor = new wxButton(pLines, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  FlexGridSizer21->Add(bLinesPenColor, 1, wxALL|wxEXPAND, 2);
  StaticText33 = new wxStaticText(pLines, wxID_ANY, _("Width :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer21->Add(StaticText33, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  cbLinesPenWidth = new wxChoice(pLines, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  cbLinesPenWidth->Set(WXSIZEOF(PenWidth_choices), PenWidth_choices);
  FlexGridSizer21->Add(cbLinesPenWidth, 1, wxALL|wxEXPAND, 2);
  StaticText34 = new wxStaticText(pLines, wxID_ANY, _("Style :"), wxDefaultPosition, wxDefaultSize, 0);
  FlexGridSizer21->Add(StaticText34, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
  cbLinesPenStyle = new wxChoice(pLines, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator);
  cbLinesPenStyle->Set(WXSIZEOF(PenStyle_choices), PenStyle_choices);
  FlexGridSizer21->Add(cbLinesPenStyle, 1, wxALL|wxEXPAND, 2);
  StaticBoxSizer11->Add(FlexGridSizer21, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
  BoxSizer18->Add(StaticBoxSizer11, 0, wxALL, 2);
  BoxSizer19 = new wxBoxSizer(wxVERTICAL);
  cbLinesVisible = new wxCheckBox(pLines, wxID_ANY, _("Visible"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbLinesVisible->SetValue(false);
  BoxSizer19->Add(cbLinesVisible, 1, wxALL|wxALIGN_LEFT, 3);
  cbLinesOutside = new wxCheckBox(pLines, wxID_ANY, _("Draw Outside Margins"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbLinesOutside->SetValue(false);
  BoxSizer19->Add(cbLinesOutside, 1, wxALL|wxALIGN_LEFT, 3);
  cbLinesShowName = new wxCheckBox(pLines, wxID_ANY, _("Show name"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  cbLinesShowName->SetValue(false);
  BoxSizer19->Add(cbLinesShowName, 1, wxALL|wxALIGN_LEFT, 3);
  BoxSizer18->Add(BoxSizer19, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
  FlexGridSizer20->Add(BoxSizer18, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
  BoxSizer4->Add(FlexGridSizer20, 0, wxALL|wxEXPAND, 5);
  pLines->SetSizer(BoxSizer4);
  sizerLines->Add(pLines, 1, wxEXPAND, 5);
  Panel5->SetSizer(sizerLines);
  nbConfig->AddPage(Panel1, _("General"), false);
  nbConfig->AddPage(Panel2, _("Legend"), false);
  nbConfig->AddPage(Panel3, _("Axis"), false);
  nbConfig->AddPage(Panel4, _("Series"), false);
  nbConfig->AddPage(Panel5, _("Lines"), false);
  sizerMain->Add(nbConfig, 1, wxALL|wxEXPAND, 5);
  BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
  bApply = new wxButton(this, wxID_ANY, _("Apply"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  BoxSizer2->Add(bApply, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
  bClose = new wxButton(this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
  BoxSizer2->Add(bClose, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
  sizerMain->Add(BoxSizer2, 0, wxEXPAND, 4);
  SetSizer(sizerMain);
  sizerMain->SetSizeHints(this);

  bFontTitle->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbFontClick, this);
  bBGColor->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbColorClick, this);
  bCoordBrushColor->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbColorClick, this);
  bFontLegend->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbFontClick, this);
  bLegendBrushColor->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbColorClick, this);
  ChoiceAxis->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &MathPlotConfigDialog::OnAxisSelect, this);
  bAddXAxis->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbAddAxisClick, this);
  bAddYAxis->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbAddAxisClick, this);
  bDelAxis->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbDelAxisClick, this);
  bFontAxis->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbFontClick, this);
  bAxisPenColor->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbColorClick, this);
  cbAutoScale->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MathPlotConfigDialog::OncbAutoScaleClick, this);
  cbFormat->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &MathPlotConfigDialog::OncbFormatSelect, this);
  ChoiceSeries->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &MathPlotConfigDialog::OnChoiceSeries, this);
  bDelSeries->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbDelSeriesClick, this);
  bSeriesPenColor->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbColorClick, this);
  bSeriesBrushColor->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbColorClick, this);
  ChoiceLines->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &MathPlotConfigDialog::OnChoiceLinesSelect, this);
  bAddLines->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbAddLinesClick, this);
  bDelLines->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbDelLinesClick, this);
  bLinesPenColor->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbColorClick, this);
  nbConfig->Bind(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, &MathPlotConfigDialog::OnnbConfigPageChanged, this);
  bApply->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnbApplyClick, this);
  bClose->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotConfigDialog::OnQuit, this);
  //*)

#ifdef _WIN32
#else
  cbMagnetize->Show(false); // wxINVERT not work on Linux GTK
  cbLegendBrushStyle->SetToolTip(MESS_TRANSPARENT);
#endif // _WIN32

  // The plot window
  m_plot = wxDynamicCast(parent, mpWindow);
  // The settings for the config window
  m_settings = NULL;

  colourButton = NULL;
  CurrentTitle = NULL;
  CurrentLegend = NULL;
  CurrentCoords = NULL;
  CurrentScale = NULL;
  CurrentSerie = NULL;
  CurrentLine = NULL;
  CurrentChoice = NULL;
  fontTitleChanged = false;
  fontLegendChanged = false;
  fontAxisChanged = false;

  scale_offset = 0;
  scale_min = -1;
  scale_max = 1;
  line_value = 1;
  CheckBar = false;
  SerieVisibleChange = false;
//    Initialize();
}

MathPlotConfigDialog::~MathPlotConfigDialog()
{
  //(*Destroy(MathPlotConfigDialog)
  //*)
  if (m_settings)
  {
    m_settings->GetSettings(this);
    m_settings->Flush();
    delete m_settings;
    m_settings = NULL;
  }
}

void MathPlotConfigDialog::Initialize(mpConfigPageId id)
{
  // ** General page **
  CurrentTitle = (mpText*)m_plot->GetLayerByClassName(_T("mpTitle"));
  if (CurrentTitle)
  {
    edTitle->SetValue(CurrentTitle->GetName());
    cbTitleVisible->SetValue(CurrentTitle->IsVisible());
    UpdateFont(CurrentTitle, bFontTitle, true);
    bFontTitle->Enable();
  }

  int_top = m_plot->GetMarginTop();
  int_bottom = m_plot->GetMarginBottom();
  int_left = m_plot->GetMarginLeftOuter();
  int_right = m_plot->GetMarginRightOuter();
  int_extra = m_plot->GetExtraMargin();
  edMarginTop->GetValidator()->TransferToWindow();
  edMarginBottom->GetValidator()->TransferToWindow();
  edMarginLeft->GetValidator()->TransferToWindow();
  edMarginRight->GetValidator()->TransferToWindow();

  cbDrawBox->SetValue(m_plot->GetDrawBox());
  DoButtonColour(bBGColor, m_plot->GetbgColour());
  cbMagnetize->SetValue(m_plot->GetMagnetize());

  CurrentCoords = (mpInfoCoords*)m_plot->GetLayerByClassName(_T("mpInfoCoords"));
  if (CurrentCoords)
  {
    cbCoord->SetSelection(CurrentCoords->GetLocation());
    cbCoordVisible->SetValue(CurrentCoords->IsVisible());
    cbCoordOutside->SetValue(CurrentCoords->GetDrawOutsideMargins());
    cbCoordinates->SetValue(CurrentCoords->IsSeriesCoord());
    // Brush config
    DoButtonColour(bCoordBrushColor, CurrentCoords->GetBrush().GetColour());
    cbCoordBrushStyle->SetSelection(BrushStyleToId(CurrentCoords->GetBrush().GetStyle()));
  }

  ChoiceLeftMouseAction->SetSelection((int)m_plot->GetMouseLeftDownAction());

  // ** Legend page **
  CurrentLegend = (mpInfoLegend*)m_plot->GetLayerByClassName(_T("mpInfoLegend"));
  if (CurrentLegend)
  {
    cbLegendPosition->SetSelection(CurrentLegend->GetLocation());
    cbLegendStyle->SetSelection(CurrentLegend->GetItemMode());
    cbLegendDirection->SetSelection(CurrentLegend->GetItemDirection());
    cbLegendVisible->SetValue(CurrentLegend->IsVisible());
    // Brush config
    DoButtonColour(bLegendBrushColor, CurrentLegend->GetBrush().GetColour());
    cbLegendBrushStyle->SetSelection(BrushStyleToId(CurrentLegend->GetBrush().GetStyle()));

    UpdateFont(CurrentLegend, bFontLegend, true);
    bFontLegend->Enable();
  }

  // ** Axis page **
  // Fill Axis list. Group X axis first then Y axis
  ChoiceAxis->Clear();
  for (unsigned int i = 0; i < m_plot->CountLayersType(mpLAYER_AXIS); i++)
  {
    mpScale* axis = (mpScale*)m_plot->GetLayerAxis(i);
    wxString classname = axis->GetClassInfo()->GetClassName();
    // Only mpScaleX should be added to the list for the moment
    if (classname.IsSameAs(_T("mpScaleX")))
      ChoiceAxis->Append(_T("X axis - ") + axis->GetName(), axis);
  }
  FillYAxisList(ChoiceAxis, false);
  ChoiceAxis->SetSelection(0);
  UpdateAxis();

  // ** Series page **
  ChoiceSeries->Clear();
  for (unsigned int i = 0; i < m_plot->CountLayersType(mpLAYER_PLOT); i++)
  {
    ChoiceSeries->Append(((mpLayer*)m_plot->GetLayerPlot(i))->GetName());
  }
  bDelSeries->Enable(ChoiceSeries->GetCount() > 0);

  // Fill Y axis
  FillYAxisList(ChoiceSeriesYAxis);

  // Select the first serie
  SelectChoiceSerie(0);

  // ** Lines page **
  ChoiceLines->Clear();
  for (unsigned int i = 0; i < m_plot->CountLayersType(mpLAYER_LINE); i++)
  {
    ChoiceLines->Append(((mpLayer*)m_plot->GetLayersType(i, mpLAYER_LINE))->GetName());
  }
  bDelLines->Enable(ChoiceLines->GetCount() > 0);

  // Fill Y axis
  FillYAxisList(ChoiceLinesYAxis);

  if (ChoiceLines->GetCount() > 0)
  {
    ChoiceLines->SetSelection(0);
    UpdateSelectedLine();
  }
  else
  {
    CurrentLine = NULL;
    pLines->Show(false);
  }

  // Choice selection
  if (id != mpcpiNone)
    nbConfig->SetSelection(id);

  CurrentChoice = NULL;
  switch (nbConfig->GetSelection())
  {
    case mpcpiAxis:
      CurrentChoice = ChoiceAxis;
      break;
    case mpcpiSeries:
      CurrentChoice = ChoiceSeries;
      break;
    case mpcpiLines:
      CurrentChoice = ChoiceLines;
      break;
    default:
      ;
  }
}

void MathPlotConfigDialog::CreateSettingsFile(const wxString& filename, const wxString& path, bool apply)
{
  if (m_settings)
    delete m_settings;

  // Create wxFileName Class and assign complete exe path
  wxFileName f(wxStandardPaths::Get().GetExecutablePath());
  wxString filePath;

  if (path.IsEmpty()) // Default, save settings in exe directory
  {
    // Obtain only folder path of exe and add filename
    filePath = f.GetPath() + f.GetPathSeparators() + filename;
  }
  else
  {
    filePath = path + f.GetPathSeparators() + filename;
  }

  m_settings = new MathPlotConfigSettings(filePath);

  // If settings file exist, apply it
  if (wxFileExists(filePath) && apply)
  {
    ApplySettings();
  }
}

void MathPlotConfigDialog::ApplySettings(void)
{
  // Load and set Settings
  m_settings->SetSettings(this);
  // Apply new parameters to plot
  Apply(mpcpiGeneral, true);
  Apply(mpcpiLegend, true);
}

void MathPlotConfigDialog::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  if (m_settings)
  {
    m_settings->GetSettings(this);
    m_settings->Flush();
  }
  Close();
}

void MathPlotConfigDialog::OnbColorClick(wxCommandEvent& event)
{
  // Get the sender
  colourButton = wxDynamicCast(event.GetEventObject(), wxButton);

  wxColourData m_clrData;
  m_clrData.SetColour(colourButton->GetBackgroundColour());

  wxColourDialog ColourDialog(this, &m_clrData);
  ColourDialog.SetTitle(MESS_COLOUR);
  if (ColourDialog.ShowModal() == wxID_OK)
  {
    m_clrData = ColourDialog.GetColourData();
    DoApplyColour(m_clrData.GetColour());
  }
}

void MathPlotConfigDialog::FillYAxisList(wxChoice* yChoice, bool clearChoice)
{
  if (clearChoice)
    yChoice->Clear();
  for (auto& [yID, yData] : m_plot->GetSortedAxisDataYList())
  {
    if (yData.axis)
    {
      // axisDataY.second.Axis->GetAxisID() if we want ID
      wxString yAxisName = wxString::Format(_T("Y%d axis - %s"), yID, yData.axis->GetName());
      yChoice->Append(yAxisName, yData.axis);
    }
  }
}

void MathPlotConfigDialog::DoButtonColour(wxButton* button, const wxColour& colour)
{
  wxString RGB;
  RGB.Printf("%02X%02X%02X", colour.GetRed(), colour.GetGreen(), colour.GetBlue());
#ifdef _WIN32
  if (colour.GetRed() * 0.299 + colour.GetGreen() * 0.587 + colour.GetBlue() * 0.114 < 186)
    button->SetForegroundColour(*wxWHITE);
  else
    button->SetForegroundColour(*wxBLACK);
#else
  // On Linux, we have GTK warning with this.
#endif
  button->SetLabelText(RGB);
  button->SetBackgroundColour(colour);
}

void MathPlotConfigDialog::DoApplyColour(const wxColour& colour)
{
  if (colour == colourButton->GetBackgroundColour())
    return;
  DoButtonColour(colourButton, colour);
  colourButton->ClearBackground();
  colourButton->Refresh();
}

void MathPlotConfigDialog::OnbFontClick(wxCommandEvent& event)
{
  wxButton* fontButton = wxDynamicCast(event.GetEventObject(), wxButton);

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
    if (fontButton == bFontLegend)
      fontLegendChanged = true;
    if (fontButton == bFontAxis)
      fontAxisChanged = true;
  }
}

/**
 * if get_set then get font from layer and set to button
 * else get font from button and set to layer
 */
void MathPlotConfigDialog::UpdateFont(mpLayer* layer, wxButton* button, bool get_set)
{
  if (get_set)
  {
    button->SetFont(layer->GetFont());
    button->SetForegroundColour(layer->GetFontColour());
  }
  else
  {
    layer->SetFont(button->GetFont());
    layer->SetFontColour(button->GetForegroundColour());
  }
}

void MathPlotConfigDialog::SetFontChildren(wxButton* p, const wxFontData& fontdata)
{
  wxFont font(fontdata.GetChosenFont());

  p->SetFont(font);
  p->SetForegroundColour(fontdata.GetColour());
}

void MathPlotConfigDialog::OnnbConfigPageChanged(wxNotebookEvent& event)
{
  const int idx = event.GetSelection();
  CurrentChoice = NULL;
  switch (idx)
  {
    case mpcpiGeneral:
    case mpcpiLegend:
      break;
    case mpcpiAxis:
      CurrentChoice = ChoiceAxis;
      break;
    case mpcpiSeries:
      CurrentChoice = ChoiceSeries;
      break;
    case mpcpiLines:
      CurrentChoice = ChoiceLines;
      break;
    default:
      ;
  }
}

void MathPlotConfigDialog::UpdateAxis(void)
{
  CurrentScale = (mpScale*)ChoiceAxis->GetClientData(ChoiceAxis->GetSelection());
  if (!CurrentScale)
    return;
  wxString classname = CurrentScale->GetClassInfo()->GetClassName();
  cbAxisPosition->Clear();
  cbFormat->SetSelection((CurrentScale)->GetLabelMode());
  edFormat->Enable(cbFormat->GetSelection() == 7);

  if (classname.IsSameAs(_T("mpScaleX")))
  {
    scale_offset = mpALIGN_BORDER_BOTTOM;
    for (int i = scale_offset; i <= mpALIGN_BORDER_TOP; i++)
      cbAxisPosition->Append(XAxis_Align[i - scale_offset]);
  }
  else
  {
    scale_offset = mpALIGN_BORDER_LEFT;
    for (int i = scale_offset; i <= mpALIGN_BORDER_RIGHT; i++)
      cbAxisPosition->Append(YAxis_Align[i - scale_offset]);
  }

  edAxisName->SetValue(CurrentScale->GetName());
  // Pen config
  DoButtonColour(bAxisPenColor, CurrentScale->GetPen().GetColour());
  cbAxisPenWidth->SetSelection(CurrentScale->GetPen().GetWidth() - 1);
  cbAxisPenStyle->SetSelection(CurrentScale->GetPen().GetStyle() - wxPENSTYLE_SOLID);
  cbAxisVisible->SetValue(CurrentScale->IsVisible());
  cbGridVisible->SetValue(CurrentScale->GetShowGrids());
  cbAxisPosition->SetSelection(CurrentScale->GetAlign() - scale_offset);
  edFormat->SetValue(CurrentScale->GetLabelFormat());
  cbLogAxis->SetValue(CurrentScale->IsLogAxis());

  cbAxisOutside->SetValue(CurrentScale->GetDrawOutsideMargins());

  UpdateFont(CurrentScale, bFontAxis, true);
  bFontAxis->Enable();

  // Scale
  mpRange BoundScale;
  cbAutoScale->SetValue(CurrentScale->GetAuto());
  edScaleMin->Enable(!CurrentScale->GetAuto());
  edScaleMax->Enable(!CurrentScale->GetAuto());
  if (CurrentScale->GetAuto())
  {
    if (classname.IsSameAs(_T("mpScaleX")))
    {
      BoundScale = m_plot->GetBoundX();
      scale_min = BoundScale.min;
      scale_max = BoundScale.max;
    }
    else if (classname.IsSameAs(_T("mpScaleY")))
    {
      mpScaleY* yAxis = static_cast<mpScaleY*>(CurrentScale);
      BoundScale = m_plot->GetBoundY(yAxis->GetAxisID());
      scale_min = BoundScale.min;
      scale_max = BoundScale.max;
    }
  }
  else
  {
    CurrentScale->GetScale(&scale_min, &scale_max);
  }
  edScaleMin->GetValidator()->TransferToWindow();
  edScaleMax->GetValidator()->TransferToWindow();
}

void MathPlotConfigDialog::OnbAddAxisClick(wxCommandEvent& event)
{
  wxButton* bt = wxDynamicCast(event.GetEventObject(), wxButton);
  mpScale* newScale = NULL;
  if (bt == bAddXAxis)
    newScale = (mpScale*)new mpScaleX(wxT("New X"), mpALIGN_BOTTOM, true, mpLabel_AUTO);
  else
    newScale = (mpScale*)new mpScaleY(wxT("New Y"), mpALIGN_LEFT, true);

  if (newScale && (m_plot->AddLayer(newScale, true, false)))
  {
    Initialize(mpcpiAxis);
    // Find in the choice list the index of the new axis
    for (unsigned int i = 0; i < ChoiceAxis->GetCount(); i++)
    {
      if ((mpScale*)ChoiceAxis->GetClientData(i) == newScale)
      {
        ChoiceAxis->SetSelection(i);
        break;
      }
    }
    UpdateAxis();
  }
}

void MathPlotConfigDialog::OnbDelAxisClick(wxCommandEvent& WXUNUSED(event))
{
  if (CurrentScale)
  {
    if (wxMessageDialog(this, MESS_AXIS_DELETE, MESS_CONFIRM, wxYES_NO | wxCENTRE).ShowModal() == wxID_YES)
    {
      m_plot->DelLayer(CurrentScale, mpYesDelete, true, false); // Should we also delete the object ?
      m_plot->Fit();
      CurrentScale = NULL;
      Initialize(mpcpiAxis);
    }
  }
}

void MathPlotConfigDialog::OnAxisSelect(wxCommandEvent& WXUNUSED(event))
{
  UpdateAxis();
  CurrentChoice = ChoiceAxis;
}

void MathPlotConfigDialog::OncbFormatSelect(wxCommandEvent& WXUNUSED(event))
{
  edFormat->Enable(cbFormat->GetSelection() == 7);
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
  else if (id == 1)
    return wxBRUSHSTYLE_TRANSPARENT;
  else
    return (wxBrushStyle)(wxBRUSHSTYLE_BDIAGONAL_HATCH + (id - 2));
}

int MathPlotConfigDialog::BrushStyleToId(wxBrushStyle style)
{
  if (style == wxBRUSHSTYLE_SOLID)
    return 0;
  else if (style == wxBRUSHSTYLE_TRANSPARENT)
    return 1;
  else
    return style - wxBRUSHSTYLE_BDIAGONAL_HATCH + 2;
}

void MathPlotConfigDialog::SelectChoiceSerie(unsigned int serie)
{
  CurrentSerie = NULL;
  if ((ChoiceSeries->GetCount() > 0) && (serie < ChoiceSeries->GetCount()))
  {
    ChoiceSeries->SetSelection(serie);
    UpdateSelectedSerie();
  }
}

void MathPlotConfigDialog::UpdateSelectedSerie(void)
{
  CurrentSerie = (mpFunction*)m_plot->GetLayerPlot(ChoiceSeries->GetSelection());

  if (!CurrentSerie)
  {
    CurrentChoice = NULL;
    return;
  }

  CurrentChoice = ChoiceSeries;

  edSeriesName->SetValue(CurrentSerie->GetName());

  ChoiceSeriesYAxis->SetSelection(wxNOT_FOUND);
  for (unsigned int i = 0; i < ChoiceSeriesYAxis->GetCount(); i++)
  {
    mpScaleY* yAxis = static_cast<mpScaleY*>(ChoiceSeriesYAxis->GetClientData(i));
    if (yAxis->GetAxisID() == CurrentSerie->GetYAxisID())
    {
      ChoiceSeriesYAxis->SetSelection(i);
      break;
    }
  }

  // Pen config
  DoButtonColour(bSeriesPenColor, CurrentSerie->GetPen().GetColour());
  cbSeriesPenWidth->SetSelection(CurrentSerie->GetPen().GetWidth() - 1);
  cbSeriesPenStyle->SetSelection(CurrentSerie->GetPen().GetStyle() - wxPENSTYLE_SOLID);
  // Brush config
  DoButtonColour(bSeriesBrushColor, CurrentSerie->GetBrush().GetColour());
  cbSeriesBrushStyle->SetSelection(BrushStyleToId(CurrentSerie->GetBrush().GetStyle()));
  // Symbol config
  cbSeriesSymbolType->SetSelection(CurrentSerie->GetSymbol());
  cbSeriesSymbolSize->SetValue(CurrentSerie->GetSymbolSize());

  SerieVisibleChange = CurrentSerie->IsVisible();
  cbSeriesVisible->SetValue(SerieVisibleChange);
  cbSeriesContinuity->SetValue(CurrentSerie->GetContinuity());
  cbSeriesOutside->SetValue(CurrentSerie->GetDrawOutsideMargins());
  cbSeriesShowName->SetValue(CurrentSerie->GetShowName());
  cbTractable->SetValue(CurrentSerie->IsTractable());

  cbSeriesStep->SetValue(CurrentSerie->GetStep());

  CheckBar = (CurrentSerie->GetLayerSubType() == mpfFXYVector);
  if (CheckBar)
  {
    cbBar->Enable();
    cbBar->SetValue(((mpFXY*)CurrentSerie)->ViewAsBar());
  }
  else
  {
    cbBar->Disable();
    cbBar->SetValue(false);
  }
}

void MathPlotConfigDialog::OnChoiceSeries(wxCommandEvent& WXUNUSED(event))
{
  UpdateSelectedSerie();
}

void MathPlotConfigDialog::OnbDelSeriesClick(wxCommandEvent& WXUNUSED(event))
{
  if (CurrentSerie && CurrentSerie->GetCanDelete())
  {
    if (wxMessageDialog(this, MESS_DELETE, MESS_CONFIRM, wxYES_NO | wxCENTRE).ShowModal() == wxID_YES)
    {
      m_plot->DelLayer(CurrentSerie, mpYesDelete, true, false);
      if (CurrentLegend)
        CurrentLegend->SetNeedUpdate();
      m_plot->Fit();
      CurrentSerie = NULL;
      edSeriesName->SetValue(_T(""));
      Initialize(mpcpiSeries);
    }
  }
}

void MathPlotConfigDialog::UpdateSelectedLine(void)
{
  CurrentLine = (mpLine*)m_plot->GetLayersType(ChoiceLines->GetSelection(), mpLAYER_LINE);

  if (!CurrentLine)
  {
    CurrentChoice = NULL;
    return;
  }

  CurrentChoice = ChoiceLines;

  edLinesName->SetValue(CurrentLine->GetName());
  line_value = CurrentLine->GetValue();
  edLinesValue->GetValidator()->TransferToWindow();

  ChoiceLinesYAxis->SetSelection(wxNOT_FOUND);
  for (unsigned int i = 0; i < ChoiceSeriesYAxis->GetCount(); i++)
  {
    mpScaleY* yAxis = static_cast<mpScaleY*>(ChoiceLinesYAxis->GetClientData(i));
    if (yAxis->GetAxisID() == CurrentLine->GetYAxisID())
    {
      ChoiceLinesYAxis->SetSelection(i);
      break;
    }
  }

  // Pen config
  DoButtonColour(bLinesPenColor, CurrentLine->GetPen().GetColour());
  cbLinesPenWidth->SetSelection(CurrentLine->GetPen().GetWidth() - 1);
  cbLinesPenStyle->SetSelection(CurrentLine->GetPen().GetStyle() - wxPENSTYLE_SOLID);

  cbLinesVisible->SetValue(CurrentLine->IsVisible());
  cbLinesOutside->SetValue(CurrentLine->GetDrawOutsideMargins());
  cbLinesShowName->SetValue(CurrentLine->GetShowName());

  if (CurrentLine->IsHorizontal())
  {
    rbLinesDirection->SetLabel(_T("Horizontal"));
    stLinesYIndexLabel->Show(true);
    ChoiceLinesYAxis->Show(true);
  }
  else
  {
    rbLinesDirection->SetLabel(_T("Vertical"));
    stLinesYIndexLabel->Show(false);
    ChoiceLinesYAxis->Show(false);
  }
}

void MathPlotConfigDialog::OnChoiceLinesSelect(wxCommandEvent& WXUNUSED(event))
{
  UpdateSelectedLine();
}

void MathPlotConfigDialog::OnbAddLinesClick(wxCommandEvent& WXUNUSED(event))
{
  int answer = wxMessageDialog(this, MESS_LINES_ADD, MESS_CONFIRM, wxYES_NO | wxCANCEL | wxCENTRE).ShowModal();

  if (answer == wxID_CANCEL)
    return;

  mpLine* newLine = NULL;
  if (answer == wxID_YES)
    newLine = (mpLine*)new mpHorizontalLine(1.0);
  else
    newLine = (mpLine*)new mpVerticalLine(1.0);
  if (m_plot->AddLayer(newLine, true, false))
  {
    Initialize(mpcpiLines);
    ChoiceLines->SetSelection(ChoiceLines->GetCount() - 1);
    UpdateSelectedLine();
    pLines->Show(true);
#ifdef _WIN32
#else
    sizerLines->Layout();
#endif
  }
}

void MathPlotConfigDialog::OnbDelLinesClick(wxCommandEvent& WXUNUSED(event))
{
  if (CurrentLine)
  {
    if (wxMessageDialog(this, MESS_LINES_DELETE, MESS_CONFIRM, wxYES_NO | wxCENTRE).ShowModal() == wxID_YES)
    {
      m_plot->DelLayer(CurrentLine, mpYesDelete, true, false);
      m_plot->Fit();
      CurrentLine = NULL;
      edLinesName->SetValue(_T(""));
      Initialize(mpcpiLines);
    }
  }
}

void MathPlotConfigDialog::OnbApplyClick(wxCommandEvent& WXUNUSED(event))
{
  Apply(nbConfig->GetSelection());
}

void MathPlotConfigDialog::Apply(int pageIndex, bool updateFont)
{
  switch (pageIndex)
  {
    case mpcpiGeneral: // General
    {
      if (CurrentTitle)
      {
        CurrentTitle->SetName(edTitle->GetValue());
        CurrentTitle->SetVisible(cbTitleVisible->GetValue());
        if (fontTitleChanged || updateFont)
        {
          UpdateFont(CurrentTitle, bFontTitle, false);
          fontTitleChanged = false;
        }
      }

      edMarginTop->GetValidator()->TransferFromWindow();
      edMarginBottom->GetValidator()->TransferFromWindow();
      edMarginLeft->GetValidator()->TransferFromWindow();
      edMarginRight->GetValidator()->TransferFromWindow();
      edExtraMargin->GetValidator()->TransferFromWindow();
      m_plot->SetExtraMargin(int_extra);
      m_plot->SetMargins(int_top, int_right, int_bottom, int_left);

      m_plot->SetDrawBox(cbDrawBox->GetValue());
      m_plot->SetbgColour(bBGColor->GetBackgroundColour());
      m_plot->SetMagnetize(cbMagnetize->GetValue());

      if (CurrentCoords)
      {
        CurrentCoords->SetLocation((mpLocation)cbCoord->GetSelection());
        CurrentCoords->SetVisible(cbCoordVisible->GetValue());
        CurrentCoords->SetDrawOutsideMargins(cbCoordOutside->GetValue());
        CurrentCoords->SetSeriesCoord(cbCoordinates->GetValue());
        // Brush config
        wxBrush brush(bCoordBrushColor->GetBackgroundColour(), IdToBrushStyle(cbCoordBrushStyle->GetSelection()));
        CurrentCoords->SetBrush(brush);
      }

      m_plot->SetMouseLeftDownAction((mpMouseButtonAction)ChoiceLeftMouseAction->GetSelection());

      m_plot->Fit();
      break;
    }
    case mpcpiLegend: // Legend page
      if (CurrentLegend)
      {
        CurrentLegend->SetLocation((mpLocation)cbLegendPosition->GetSelection());
        CurrentLegend->SetVisible(cbLegendVisible->GetValue());
        CurrentLegend->SetItemMode((mpLegendStyle)cbLegendStyle->GetSelection());
        CurrentLegend->SetItemDirection((mpLegendDirection)cbLegendDirection->GetSelection());
        // Brush config
        wxBrush brush(bLegendBrushColor->GetBackgroundColour(), IdToBrushStyle(cbLegendBrushStyle->GetSelection()));
        CurrentLegend->SetBrush(brush);

        if (fontLegendChanged || updateFont)
        {
          UpdateFont(CurrentLegend, bFontLegend, false);
          fontLegendChanged = false;
        }

        CurrentLegend->SetNeedUpdate();
        m_plot->UpdateAll();
      }
      break;

    case mpcpiAxis: // Axis page
      if ((CurrentChoice == ChoiceAxis) && (CurrentScale != NULL))
      {
        bool NameChanged = CurrentScale->GetName() != edAxisName->GetValue();
        if (NameChanged)
          CurrentScale->SetName(edAxisName->GetValue());
        // Pen config
        wxPen pen(bAxisPenColor->GetBackgroundColour(), cbAxisPenWidth->GetSelection() + 1,
            (wxPenStyle)(cbAxisPenStyle->GetSelection() + wxPENSTYLE_SOLID));
        CurrentScale->SetPen(pen);
        CurrentScale->SetVisible(cbAxisVisible->GetValue());
        CurrentScale->ShowGrids(cbGridVisible->GetValue());
        CurrentScale->SetAlign(scale_offset + cbAxisPosition->GetSelection());
        CurrentScale->SetDrawOutsideMargins(cbAxisOutside->GetValue());
        CurrentScale->SetLabelFormat(edFormat->GetValue());
        CurrentScale->SetLabelMode((mpLabelType)cbFormat->GetSelection());

        wxString newName = _T("");
        wxString classname = CurrentScale->GetClassInfo()->GetClassName();
        if (classname.IsSameAs(_T("mpScaleX")))
        {
          newName = _T("X axis - ");
          // Update InfoCoords if present
          if (CurrentCoords)
          {
            CurrentCoords->SetLabelMode((mpLabelType)cbFormat->GetSelection());
          }
        }
        else if (classname.IsSameAs(_T("mpScaleY")))
        {
          mpScaleY* yAxis = static_cast<mpScaleY*>(CurrentScale);
          newName.Printf(_T("Y%d axis - "), (int)yAxis->GetAxisID());
        }
        // Update name in choice list
        ChoiceAxis->SetString(ChoiceAxis->GetCurrentSelection(), newName + edAxisName->GetValue());
        CurrentScale->SetLogAxis(cbLogAxis->GetValue());

        CurrentScale->SetAuto(cbAutoScale->GetValue());
        edScaleMin->GetValidator()->TransferFromWindow();
        edScaleMax->GetValidator()->TransferFromWindow();
        CurrentScale->SetScale(scale_min, scale_max);

        if (fontAxisChanged)
        {
          UpdateFont(CurrentScale, bFontAxis, false);
          fontAxisChanged = false;
        }

        if (!CurrentScale->GetAuto())
        {
          mpRange BoundScaleX = m_plot->GetBoundX();
          std::unordered_map<int, mpRange> BoundScaleY = m_plot->GetAllBoundY();

          if (classname.IsSameAs(_T("mpScaleX"))) // X axis
          {
            BoundScaleX.Set(scale_min, scale_max);

            // Get bound of the other axis
            for (auto& [yID, yData] : m_plot->GetAxisDataYList())
            {
              if (yData.axis && !yData.axis->GetAuto())
              {
                BoundScaleY[yID] = yData.axis->GetScale();
              }
            }
          }
          else if (classname.IsSameAs(_T("mpScaleY")))
          {
            mpScaleY* yAxis = static_cast<mpScaleY*>(CurrentScale);
            BoundScaleY[yAxis->GetAxisID()].Set(scale_min, scale_max);

            // Get bound of the other axis
            mpScale* xAxis = (mpScale*)m_plot->GetLayerXAxis();
            if (xAxis && (!xAxis->GetAuto()))
            {
              BoundScaleX = xAxis->GetScale();
            }
          }
          m_plot->Fit(BoundScaleX, BoundScaleY);
        }
        else
          m_plot->Fit();

        // Refresh page
        UpdateAxis();

        if (NameChanged)
        {
          // Series
          int id = ChoiceSeriesYAxis->GetSelection();
          FillYAxisList(ChoiceSeriesYAxis);
          ChoiceSeriesYAxis->SetSelection(id);
          // Line
          id = ChoiceLinesYAxis->GetSelection();
          FillYAxisList(ChoiceLinesYAxis);
          ChoiceLinesYAxis->SetSelection(id);
        }
      }
      break;

    case mpcpiSeries: // Series page
      if ((CurrentChoice == ChoiceSeries) && (CurrentSerie != NULL))
      {
        CurrentSerie->SetName(edSeriesName->GetValue());
        ChoiceSeries->SetString(ChoiceSeries->GetSelection(), edSeriesName->GetValue());

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
        CurrentSerie->SetTractable(cbTractable->GetValue());

        bool yAxisChange = false;
        if (ChoiceSeriesYAxis->GetSelection() != wxNOT_FOUND)
        {
          mpScaleY* yAxis = static_cast<mpScaleY*>(ChoiceSeriesYAxis->GetClientData(ChoiceSeriesYAxis->GetSelection()));
          yAxisChange = (CurrentSerie->GetYAxisID() != yAxis->GetAxisID());
          CurrentSerie->SetYAxisID(yAxis->GetAxisID());
        }

        CurrentSerie->SetStep(cbSeriesStep->GetValue());

        if (CheckBar)
        {
          ((mpFXYVector*)CurrentSerie)->SetViewMode(cbBar->GetValue());
          if (cbBar->GetValue())
            cbSeriesContinuity->SetValue(false);
        }

        if (CurrentLegend)
          CurrentLegend->SetNeedUpdate();

        // We need to fit if we change visibility or Y axis
        if (yAxisChange || (SerieVisibleChange != cbSeriesVisible->GetValue()))
        {
          SerieVisibleChange = cbSeriesVisible->GetValue();
          m_plot->Fit();
        }
        else
          m_plot->Refresh();
      }
      break;

    case mpcpiLines: // Line page
      if ((CurrentChoice == ChoiceLines) && (CurrentLine != NULL))
      {
        CurrentLine->SetName(edLinesName->GetValue());
        ChoiceLines->SetString(ChoiceLines->GetSelection(), edLinesName->GetValue());
        edLinesValue->GetValidator()->TransferFromWindow();
        CurrentLine->SetValue(line_value);
        if (ChoiceLinesYAxis->GetSelection() != wxNOT_FOUND)
        {
          mpScaleY* yAxis = static_cast<mpScaleY*>(ChoiceLinesYAxis->GetClientData(ChoiceLinesYAxis->GetSelection()));
          CurrentLine->SetYAxisID(yAxis->GetAxisID());
        }

        // Pen config
        wxPen pen(bLinesPenColor->GetBackgroundColour(), cbLinesPenWidth->GetSelection() + 1,
            (wxPenStyle)(cbLinesPenStyle->GetSelection() + wxPENSTYLE_SOLID));
        CurrentLine->SetPen(pen);

        CurrentLine->SetVisible(cbLinesVisible->GetValue());
        CurrentLine->SetDrawOutsideMargins(cbLinesOutside->GetValue());
        CurrentLine->SetShowName(cbLinesShowName->GetValue());
        m_plot->Refresh();
      }
      break;
    default:
      ;
  }
}

#ifdef ENABLE_MP_NAMESPACE // MathPlot namespace
  } // namespace MathPlot
#endif // ENABLE_MP_NAMESPACE

#endif // ENABLE_MP_CONFIG
