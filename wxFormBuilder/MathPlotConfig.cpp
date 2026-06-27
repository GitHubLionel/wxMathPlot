#if defined(MP_ENABLE_CONFIG) || defined(ENABLE_MP_CONFIG)

#include "MathPlotConfig.h"
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/arrstr.h>
#include <wx/choicdlg.h>
#include <wx/intl.h>
#include <wx/string.h>

#if defined(MP_ENABLE_NAMESPACE) || defined(ENABLE_MP_NAMESPACE) // MathPlot namespace
  namespace MathPlot {
#endif // MP_ENABLE_NAMESPACE

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
 * wxMultiTextCtrlDialog class
 ***************************************************/
wxBEGIN_EVENT_TABLE(wxMultiTextCtrlDialog, wxDialog)
    EVT_BUTTON(wxID_OK, wxMultiTextCtrlDialog::OnOK)
    EVT_BUTTON(wxID_CANCEL, wxMultiTextCtrlDialog::OnCancel)
wxEND_EVENT_TABLE()

bool wxMultiTextCtrlDialog::Create(wxWindow* parent, const wxString& title,
    const wxString& message, long size, const wxString prompt[], double *values,
    const wxPoint& pos)
{
  if (!wxDialog::Create(GetParentForModalDialog(parent, 0), wxID_ANY, title, pos, wxDefaultSize))
  {
    return false;
  }

  // Top vertical sizer
  wxBoxSizer* topsizer = new wxBoxSizer(wxVERTICAL);

  // add text message
  topsizer->Add(CreateTextSizer(message), wxSizerFlags().DoubleBorder());

  // add prompt and text ctrl to a grid sizer
  wxFlexGridSizer* inputsizer = new wxFlexGridSizer(size, 2, 0, 0);
  for (int i = 0; i < size; i++)
  {
    // prompt if any
    if (!prompt[i].empty())
      inputsizer->Add(new wxStaticText(this, wxID_ANY, prompt[i]), wxSizerFlags().Center().DoubleBorder(wxLEFT));
    else
      inputsizer->Add(0, 0, 1);

    // text ctrl
    wxTextCtrl* textctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, wxDefaultCoord), 0, wxFloatingPointValidator<double> (4, &values[i]));
    textctrl->GetValidator()->TransferToWindow();
    m_ctrls.push_back(textctrl);

    inputsizer->Add(textctrl, wxSizerFlags(1).Center().DoubleBorder(wxLEFT | wxRIGHT));
  }
  topsizer->Add(inputsizer, wxSizerFlags().Expand().Border(wxLEFT | wxRIGHT));

  // add buttons
  wxSizer* buttonSizer = CreateSeparatedButtonSizer(wxOK | wxCANCEL);
  if (buttonSizer)
  {
    topsizer->Add(buttonSizer, wxSizerFlags().Expand().DoubleBorder());
  }

  SetSizer(topsizer);
  topsizer->SetSizeHints(this);

  Centre(wxBOTH);

  return true;
}

void wxMultiTextCtrlDialog::OnOK(wxCommandEvent&WXUNUSED(event))
{
  for (auto ctrl: m_ctrls)
  {
    ctrl->GetValidator()->TransferFromWindow();
  }

  EndModal(wxID_OK);
}

void wxMultiTextCtrlDialog::OnCancel(wxCommandEvent&WXUNUSED(event))
{
  EndModal(wxID_CANCEL);
}

/***************************************************
 * MathPlotConfigDialog class
 ***************************************************/

// List of string message used
wxString MESS_TRANSPARENT = _T("");
wxString MESS_COLOUR = _T("");
wxString MESS_AXIS_DELETE = _T("");
wxString MESS_DELETE = _T("");
wxString MESS_LINES_DELETE = _T("");
wxString MESS_CONFIRM = _T("");

// Axis position
wxString XAxis_Align[5] = {_T("")};
wxString YAxis_Align[5] = {_T("")};

MathPlotConfigDialog::MathPlotConfigDialog( wxWindow* parent ): MathPlotConfigDialogBuilder( parent )
{
  // Defined here for I18N translation
  MESS_TRANSPARENT  = _("Transparent not work on Linux");
  MESS_COLOUR       = _("Please choose the background colour");
  MESS_AXIS_DELETE  = _("Delete the selected axis ?");
  MESS_DELETE       = _("Delete the serie ?");
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

  // Missing initialization of wxFormBuilder
  CompleteInitialization();

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
  if (m_settings)
  {
    m_settings->GetSettings(this);
    m_settings->Flush();
    delete m_settings;
    m_settings = NULL;
  }
}

void MathPlotConfigDialog::CompleteInitialization(void)
{
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

  // Choices
  cbAxisPenWidth->Set(WXSIZEOF(PenWidth_choices), PenWidth_choices);
  cbAxisPenWidth->SetSelection(0);
  cbGridPenWidth->Set(WXSIZEOF(PenWidth_choices), PenWidth_choices);
  cbGridPenWidth->SetSelection(0);
  cbSeriesPenWidth->Set(WXSIZEOF(PenWidth_choices), PenWidth_choices);
  cbLinesPenWidth->Set(WXSIZEOF(PenWidth_choices), PenWidth_choices);
  cbAxisPenStyle->Set(WXSIZEOF(PenStyle_choices), PenStyle_choices);
  cbGridPenStyle->Set(WXSIZEOF(PenStyle_choices), PenStyle_choices);
  cbSeriesPenStyle->Set(WXSIZEOF(PenStyle_choices), PenStyle_choices);
  cbLinesPenStyle->Set(WXSIZEOF(PenStyle_choices), PenStyle_choices);
  cbCoordBrushStyle->Set(WXSIZEOF(InfoBrushStyle_choices), InfoBrushStyle_choices);
  cbCoordBrushStyle->SetSelection(0);
  cbLegendBrushStyle->Set(WXSIZEOF(InfoBrushStyle_choices), InfoBrushStyle_choices);
  cbLegendBrushStyle->SetSelection(0);
  cbCoord->Set(WXSIZEOF(InfoPosition_choices), InfoPosition_choices);
  cbCoord->SetSelection(7);
  cbLegendPosition->Set(WXSIZEOF(InfoPosition_choices), InfoPosition_choices);\
  cbLegendPosition->SetSelection(6);

  // Delete the last item who is nonsense for Legend
  cbLegendPosition->Delete(WXSIZEOF(InfoPosition_choices) - 1);

  // Validators
  edMarginTop->SetValidator(wxIntegerValidator<unsigned int> (&int_top));
  edMarginBottom->SetValidator(wxIntegerValidator<unsigned int> (&int_bottom));
  edMarginLeft->SetValidator(wxIntegerValidator<unsigned int> (&int_left));
  edMarginRight->SetValidator(wxIntegerValidator<unsigned int> (&int_right));
  edExtraMargin->SetValidator(wxIntegerValidator<unsigned int> (&int_extra));
  edScaleMin->SetValidator(wxFloatingPointValidator<double> (2, &scale_min));
  edScaleMax->SetValidator(wxFloatingPointValidator<double> (2, &scale_max));
  edLinesValue->SetValidator(wxFloatingPointValidator<double> (2, &line_value));

  #ifdef _WIN32
  #else
  cbMagnetize->Show(false); // wxINVERT not work on Linux GTK
  cbLegendBrushStyle->SetToolTip(MESS_TRANSPARENT);
  #endif // _WIN32

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

  // Mouse coordinates
  CurrentCoords = (mpInfoCoords*)m_plot->GetLayerByClassName(_T("mpInfoCoords"));
  if (CurrentCoords)
  {
    cbCoord->SetSelection(CurrentCoords->GetLocation());
    cbCoordVisible->SetValue(CurrentCoords->IsVisible());
    cbCoordOutside->SetValue(CurrentCoords->GetDrawOutsideMargins());
    cbCoordinates->SetValue(CurrentCoords->IsSeriesCoord());
    cbCoordDefaultVisibility->SetValue(m_plot->m_DefaultCoordIsAlwaysVisible);
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
    cbLegendDefaultVisibility->SetValue(m_plot->m_DefaultLegendIsAlwaysVisible);
  }

  // ** Axis page **
  // Fill Axis list. Group X axis first then Y axis
  ChoiceAxis->Clear();
  for (unsigned int i = 0; i < m_plot->CountLayersType(mpLAYER_AXIS); i++)
  {
    mpScale* axis = m_plot->GetLayerAxis(i);
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
  if ((colourButton = wxDynamicCast(event.GetEventObject(), wxButton)) != NULL)
  {
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
}

void MathPlotConfigDialog::FillYAxisList(wxChoice* yChoice, bool clearChoice)
{
  if (clearChoice)
    yChoice->Clear();
  for (const MP_LOOP_ITER : m_plot->GetAxisDataYList())
  {
    if (m_yData.axis)
    {
      // axisDataY.second.Axis->GetAxisID() if we want ID
      wxString yAxisName = wxString::Format(_T("Y%d axis - %s"), m_yID, m_yData.axis->GetName());
      yChoice->Append(yAxisName, m_yData.axis);
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
  if (wxButton* const fontButton = wxDynamicCast(event.GetEventObject(), wxButton))
  {
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
  // I don't know why but I have an event over nbPenAxisGrid, so this workaround !
  if (wxNotebook* const notebook = wxDynamicCast(event.GetEventObject(), wxNotebook))
  {
    if (notebook == nbPenAxisGrid)
    {
      event.Skip();
      return;
    }
  }

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
  if (ChoiceAxis->GetSelection() == -1)
    return;
  CurrentScale = (mpScale*)ChoiceAxis->GetClientData(ChoiceAxis->GetSelection());
  if (!CurrentScale)
    return;
  wxString classname = CurrentScale->GetClassInfo()->GetClassName();
  cbAxisPosition->Clear();
  cbFormat->SetSelection((int)(CurrentScale)->GetLabelMode());
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
  // Pen config: Axis and Grid
  DoButtonColour(bAxisPenColor, CurrentScale->GetPen().GetColour());
  cbAxisPenWidth->SetSelection(CurrentScale->GetPen().GetWidth() - 1);
  cbAxisPenStyle->SetSelection(CurrentScale->GetPen().GetStyle() - wxPENSTYLE_SOLID);
  DoButtonColour(bGridPenColor, CurrentScale->GetGridPen().GetColour());
  cbGridPenWidth->SetSelection(CurrentScale->GetGridPen().GetWidth() - 1);
  cbGridPenStyle->SetSelection(CurrentScale->GetGridPen().GetStyle() - wxPENSTYLE_SOLID);

  cbAxisVisible->SetValue(CurrentScale->IsVisible());
  cbGridVisible->SetValue(CurrentScale->GetShowGrids());
  cbAxisPosition->SetSelection(CurrentScale->GetAlign() - scale_offset);
  edFormat->SetValue(CurrentScale->GetLabelFormat());
  cbLogAxis->SetValue(CurrentScale->IsLogAxis());
  cbMouseCoordVisible->SetValue(CurrentScale->GetCoordIsAlwaysVisible());

  cbAxisOutside->SetValue(CurrentScale->GetDrawOutsideMargins());

  UpdateFont(CurrentScale, bFontAxis, true);
  bFontAxis->Enable();

  // Scale
  mpRange<double> BoundScale;
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
  if (wxButton* const bt = wxDynamicCast(event.GetEventObject(), wxButton))
  {
    mpScale* newScale = NULL;
    if (bt == bAddXAxis)
      newScale = (mpScale*)new mpScaleX(wxT("New X"), mpALIGN_BOTTOM, true, mpLabel_AUTO);
    else
      newScale = (mpScale*)new mpScaleY(wxT("New Y"), mpALIGN_LEFT, true);

    if (newScale && (m_plot->AddLayer(newScale, true, false)))
    {
      // Search the first X or Y scale present to retreive its font
      // since we probably want the same font for all the axis.
      mpScale* firstScale = NULL;
      if (bt == bAddXAxis)
        firstScale = m_plot->GetLayerAxis(0, mpsScaleX);
      else
        firstScale = m_plot->GetLayerAxis(0, mpsScaleY);
      if (firstScale)
        newScale->SetFont(firstScale->GetFont());

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
}

void MathPlotConfigDialog::OnbDelAxisClick(wxCommandEvent& WXUNUSED(event))
{
  if (CurrentScale)
  {
    if (wxMessageDialog(this, MESS_AXIS_DELETE, MESS_CONFIRM, wxYES_NO | wxCENTRE).ShowModal() == wxID_YES)
    {
      m_plot->DelLayer(CurrentScale, mpYesDelete, true, false); // Should we also delete the object ?
      m_plot->UpdateAll();
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
  cbSeriesNamePosition->SetSelection(CurrentSerie->GetAlign() - mpALIGN_NW);
  cbSeriesNamePosition->Enable(CurrentSerie->GetShowName());
  cbTractable->SetValue(CurrentSerie->IsTractable());
  cbSeriesLegend->SetValue(CurrentSerie->GetLegendIsAlwaysVisible());

  cbAutoStep->SetValue(CurrentSerie->GetAutoStep());
  UpdateSeriesStep();

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

void MathPlotConfigDialog::OncbSeriesShowNameClick(wxCommandEvent& WXUNUSED(event))
{
  cbSeriesNamePosition->Enable(cbSeriesShowName->GetValue());
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
      m_plot->UpdateAll();
      CurrentSerie = NULL;
      edSeriesName->SetValue(_T(""));
      Initialize(mpcpiSeries);
    }
  }
}

void MathPlotConfigDialog::OncbAutoStepClick(wxCommandEvent& WXUNUSED(event))
{
  UpdateSeriesStep();
}

void MathPlotConfigDialog::UpdateSeriesStep(void)
{
  if (cbAutoStep->IsChecked())
  {
    // Auto step uses max nOf points instead of fixed step
    stStepChoice->SetLabel(_("Max nOf points :"));
    stStepChoice->SetToolTip(_("Sets how many points to max show in the plot at the same time. Decrease for faster plot"));
    spinSeriesStep->SetRange(1, 50000);
    if (CurrentSerie)
      spinSeriesStep->SetValue(CurrentSerie->GetMaxNOfPoints());
  }
  else
  {
    stStepChoice->SetLabel(_("Step size :"));
    stStepChoice->SetToolTip(_("Set step size, e.g. 1 to show all points, 2 to show every other step and so on"));
    spinSeriesStep->SetRange(1, 100);
    if (CurrentSerie)
      spinSeriesStep->SetValue(CurrentSerie->GetStep());
  }
  sizerSeriesStep->Layout();
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
  // String for all operations on lines
  wxArrayString Choice_string;
  wxArrayString Line_string;

  // First dialog
  Choice_string.Add(_("Horizontal Line"));
  Choice_string.Add(_("Vertical Line"));
  Choice_string.Add(_("Gaussian distribution"));
  Choice_string.Add(_("Normal distribution"));

  Line_string.Add(_("Selection of the line function"));
  Line_string.Add(_("Select:"));

  // Line dialog
  Line_string.Add(_("Enter value:"));
  Line_string.Add(_("Axis value"));

  // Gaussian dialog
  Line_string.Add(_("Enter parameters:"));
  Line_string.Add(_("mean"));
  Line_string.Add(_("Sigma"));

  // Dialog window
  wxSingleChoiceDialog dialog(this, Line_string[1], Line_string[0], Choice_string);


  if (dialog.ShowModal() == wxID_CANCEL)
    return;

  mpFunction* newLine = NULL;
  int selection = dialog.GetSelection();
  if (selection < 2)
  {
    double values[1] = {1.0};
    wxString *prompts[1] = {&Line_string[3]};
    wxMultiTextCtrlDialog dlg(this, Choice_string[selection], Line_string[2], 1, prompts[0], &values[0]);
    if (dlg.ShowModal() == wxID_OK)
    {
      if (selection == 0)
        newLine = (mpFunction*)new mpHorizontalLine(values[0]);
      else
        newLine = (mpFunction*)new mpVerticalLine(values[0]);
    }

    if (newLine && (m_plot->AddLayer(newLine, true, false)))
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
  else
  {
    double values[2] = {0.0, 1.0};
    wxString *prompts[2] = {&Line_string[5], &Line_string[6]};
    wxMultiTextCtrlDialog dlg(this, Choice_string[selection], Line_string[4], 2, prompts[0], &values[0]);
    if (dlg.ShowModal() == wxID_OK)
    {
      if (selection == 2)
        newLine = (mpFunction*)new mpGaussian(values[0], values[1]);
      else
        newLine = (mpFunction*)new mpNormal(values[0], values[1]);
    }

    if (newLine && (m_plot->AddLayer(newLine, true, false)))
    {
      Initialize(mpcpiSeries);
      ChoiceSeries->SetSelection(ChoiceSeries->GetCount() - 1);
      UpdateSelectedSerie();
    }
  }
}

void MathPlotConfigDialog::OnbDelLinesClick(wxCommandEvent& WXUNUSED(event))
{
  if (CurrentLine)
  {
    if (wxMessageDialog(this, MESS_LINES_DELETE, MESS_CONFIRM, wxYES_NO | wxCENTRE).ShowModal() == wxID_YES)
    {
      m_plot->DelLayer(CurrentLine, mpYesDelete, true, false);
      m_plot->UpdateAll();
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

void MathPlotConfigDialog::OnbApplyAndFitClick(wxCommandEvent& WXUNUSED(event))
{
  Apply(nbConfig->GetSelection());
  m_plot->Fit();
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
        if(CurrentCoords->GetLocation() != (mpLocation)cbCoord->GetSelection())
          CurrentCoords->SetLocation((mpLocation)cbCoord->GetSelection());
        CurrentCoords->SetVisible(cbCoordVisible->GetValue());
        CurrentCoords->SetDrawOutsideMargins(cbCoordOutside->GetValue());
        CurrentCoords->SetSeriesCoord(cbCoordinates->GetValue());
        m_plot->m_DefaultCoordIsAlwaysVisible = cbCoordDefaultVisibility->GetValue();
        // Brush config
        CurrentCoords->SetBrush(bCoordBrushColor->GetBackgroundColour(), IdToBrushStyle(cbCoordBrushStyle->GetSelection()));
      }

      m_plot->SetMouseLeftDownAction((mpMouseButtonAction)ChoiceLeftMouseAction->GetSelection());

      m_plot->UpdateAll();
      break;
    }
    case mpcpiLegend: // Legend page
      if (CurrentLegend)
      {
        if(CurrentLegend->GetLocation() != (mpLocation)cbLegendPosition->GetSelection())
          CurrentLegend->SetLocation((mpLocation)cbLegendPosition->GetSelection());
        CurrentLegend->SetVisible(cbLegendVisible->GetValue());
        CurrentLegend->SetItemMode((mpLegendStyle)cbLegendStyle->GetSelection());
        CurrentLegend->SetItemDirection((mpLegendDirection)cbLegendDirection->GetSelection());
        // Brush config
        CurrentLegend->SetBrush(bLegendBrushColor->GetBackgroundColour(), IdToBrushStyle(cbLegendBrushStyle->GetSelection()));

        if (fontLegendChanged || updateFont)
        {
          UpdateFont(CurrentLegend, bFontLegend, false);
          fontLegendChanged = false;
        }
        m_plot->m_DefaultLegendIsAlwaysVisible = cbLegendDefaultVisibility->GetValue();
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
        // Pen config: Axis and Grid
        wxPen penAxis(bAxisPenColor->GetBackgroundColour(), cbAxisPenWidth->GetSelection() + 1,
            (wxPenStyle)(cbAxisPenStyle->GetSelection() + wxPENSTYLE_SOLID));
        CurrentScale->SetPen(penAxis);
        wxPen penGrid(bGridPenColor->GetBackgroundColour(), cbGridPenWidth->GetSelection() + 1,
            (wxPenStyle)(cbGridPenStyle->GetSelection() + wxPENSTYLE_SOLID));
        CurrentScale->SetGridPen(penGrid);

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
        CurrentScale->SetCoordIsAlwaysVisible(cbMouseCoordVisible->GetValue());

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
          mpRange<double> BoundScaleX = m_plot->GetBoundX();
          std::unordered_map<int, mpRange<double>> BoundScaleY = m_plot->GetAllBoundY();

          if (classname.IsSameAs(_T("mpScaleX"))) // X axis
          {
            BoundScaleX.Set(scale_min, scale_max);

            // Get bound of the other axis
            for (const MP_LOOP_ITER : m_plot->GetAxisDataYList())
            {
              if (m_yData.axis && !m_yData.axis->GetAuto())
              {
                BoundScaleY[m_yID] = m_yData.axis->GetScale();
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
        {
          m_plot->UpdateAll();
        }

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
        CurrentSerie->SetBrush(bSeriesBrushColor->GetBackgroundColour(), IdToBrushStyle(cbSeriesBrushStyle->GetSelection()));

        // Symbol config
        CurrentSerie->SetSymbol((mpSymbol)cbSeriesSymbolType->GetSelection());
        CurrentSerie->SetSymbolSize(cbSeriesSymbolSize->GetValue());

        CurrentSerie->SetVisible(cbSeriesVisible->GetValue());
        CurrentSerie->SetContinuity(cbSeriesContinuity->GetValue());
        CurrentSerie->SetDrawOutsideMargins(cbSeriesOutside->GetValue());
        CurrentSerie->SetShowName(cbSeriesShowName->GetValue());
        CurrentSerie->SetAlign(cbSeriesNamePosition->GetSelection() + mpALIGN_NW);
        CurrentSerie->SetTractable(cbTractable->GetValue());
        CurrentSerie->SetLegendIsAlwaysVisible(cbSeriesLegend->GetValue());

        bool yAxisChange = false;
        if (ChoiceSeriesYAxis->GetSelection() != wxNOT_FOUND)
        {
          mpScaleY* yAxis = static_cast<mpScaleY*>(ChoiceSeriesYAxis->GetClientData(ChoiceSeriesYAxis->GetSelection()));
          yAxisChange = (CurrentSerie->GetYAxisID() != yAxis->GetAxisID());
          CurrentSerie->SetYAxisID(yAxis->GetAxisID());
        }

        if (cbAutoStep->GetValue())
        {
          CurrentSerie->SetAutoStep(true);
          CurrentSerie->SetMaxNOfPoints(spinSeriesStep->GetValue());
        }
        else
        {
          CurrentSerie->SetAutoStep(false);
          CurrentSerie->SetStep(spinSeriesStep->GetValue());
        }

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
          SerieVisibleChange = cbSeriesVisible->GetValue();

        m_plot->UpdateAll();
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
        m_plot->UpdateAll();
      }
      break;
    default:
      ;
  }
}

#if defined(MP_ENABLE_NAMESPACE) || defined(ENABLE_MP_NAMESPACE) // MathPlot namespace
  } // namespace MathPlot
#endif // MP_ENABLE_NAMESPACE

#endif // MP_ENABLE_CONFIG
