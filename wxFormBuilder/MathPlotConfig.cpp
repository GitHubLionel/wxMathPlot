#ifdef ENABLE_MP_CONFIG

#include "MathPlotConfig.h"
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/intl.h>
#include <wx/string.h>

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
  win = win->FindWindow("notebook");
  if (win)
    DoRecursiveSearch(true, win);
}

void MathPlotConfigSettings::GetSettings(wxWindow* win)
{
  if (!win)
    return;

  DoPosition(false, win);

  // Go directly to the notebook
  win = win->FindWindow("notebook");
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
  SetPath("/Position");
  if (set)
  {
    int posX, posY;
    if (Read("PosX", &posX) && Read("PosY", &posY))
      win->Move(wxPoint(posX, posY), wxSIZE_FORCE);
  }
  else // get
  {
    wxPoint pos = win->GetScreenPosition();
    Write("PosX", pos.x);
    Write("PosY", pos.y);
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
  if ((win->GetName()).IsSameAs("panel"))
  {
    if (level > 2)
      return;
  }

  // wxTextCtrl: edit widget
  if (win->IsKindOf(wxCLASSINFO(wxTextCtrl)))
  {
    wxTextCtrl* text = (wxTextCtrl*)win;
    SetPath(path);
    wxString key = "Text" + wxString::Format("%d", level);
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
    wxString key = "Choice" + wxString::Format("%d", level);
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
    wxString key = "Check" + wxString::Format("%d", level);
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
    if ((button->GetLabel()).IsSameAs("Font"))
    {
      wxString key1 = "ButtonFont" + wxString::Format("%d", level);
      wxString key2 = "ButtonForeground" + wxString::Format("%d", level);
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
      wxString key = "ButtonBackground" + wxString::Format("%d", level);
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
  wxString winPath = path + "/" + win->GetName();
  if (level != 0)
    winPath += wxString::Format("%d", level);
  int i = 1;
#if wxCHECK_VERSION(3, 0, 0)
  for (wxWindow* child : children)
  {
    wxWindow* current = child;
    if (!current->IsKindOf(wxCLASSINFO(wxStaticText))) // Skip static text
    {
      DoRecursiveSearch(set, current, winPath, i);
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

MathPlotConfigDialog::MathPlotConfigDialog( wxWindow* parent ): MathPlotConfigDialogBuilder( parent )
{
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
  int i = 0;
  for (const auto& axisDataY : m_plot->GetAxisDataYList())
  {
    if (axisDataY.second.axis)
    {
      // axisDataY.second.Axis->GetAxisID() if we want ID
      wxString yAxisName = wxString::Format(_T("Y%d axis - %s"), i, axisDataY.second.axis->GetName());
      yChoice->Append(yAxisName, axisDataY.second.axis);
      i++;
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
  const wxString XAxis_Align[] = {_("Bottom border"), _("Bottom"), _("Center"), _("Top"), _("Top border")};
  const wxString YAxis_Align[] = {_("Left border"), _("Left"), _("Center"), _("Right"), _("Right border")};

  CurrentScale = (mpScale*)ChoiceAxis->GetClientData(ChoiceAxis->GetSelection());
  if (!CurrentScale)
    return;
  wxString classname = CurrentScale->GetClassInfo()->GetClassName();

  if (classname.IsSameAs(_T("mpScaleX")))
  {
    cbAxisPosition->Clear();
    scale_offset = mpALIGN_BORDER_BOTTOM;
    for (int i = scale_offset; i <= mpALIGN_BORDER_TOP; i++)
      cbAxisPosition->Append(XAxis_Align[i - scale_offset]);
    cbFormat->Enable();
    cbFormat->SetSelection(((mpScaleX*)CurrentScale)->GetLabelMode());
    edFormat->Enable(cbFormat->GetSelection() == 5);
  }
  else
  {
    cbAxisPosition->Clear();
    scale_offset = mpALIGN_BORDER_LEFT;
    for (int i = scale_offset; i <= mpALIGN_BORDER_RIGHT; i++)
      cbAxisPosition->Append(YAxis_Align[i - scale_offset]);
    cbFormat->SetSelection(0);
    cbFormat->Enable(false);
    edFormat->Enable();
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
  edFormat->Enable(cbFormat->GetSelection() == 5);
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

        wxString newName = _T("");
        wxString classname = CurrentScale->GetClassInfo()->GetClassName();
        if (classname.IsSameAs(_T("mpScaleX")))
        {
          newName = _T("X axis - ");
          ((mpScaleX*)CurrentScale)->SetLabelMode(cbFormat->GetSelection());
          // Update InfoCoords if present
          if (CurrentCoords)
          {
            CurrentCoords->SetLabelMode(cbFormat->GetSelection());
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
          mpRange BoundScaleX;
          std::vector<mpRange> BoundScaleY;
          for (const auto& axisDataY : m_plot->GetAxisDataYList())
          {
            BoundScaleY.push_back(axisDataY.second.bound);
          }
          if (classname.IsSameAs(_T("mpScaleX"))) // X axis
          {
            BoundScaleX.Set(scale_min, scale_max);

            // Get bound of the other axis
            int i = 0;
            for (const auto& axisDataY : m_plot->GetAxisDataYList())
            {
              if (axisDataY.second.axis && (!axisDataY.second.axis->GetAuto()))
              {
                BoundScaleY[i] = axisDataY.second.axis->GetScale();
              }
              i++;
            }
          }
          else if (classname.IsSameAs(_T("mpScaleY")))
          {
            mpScaleY* yAxis = static_cast<mpScaleY*>(CurrentScale);
            int yAxisID = yAxis->GetAxisID();
            int i = 0;
            for (const auto& axisDataY : m_plot->GetAxisDataYList())
            {
              if (axisDataY.first == yAxisID)
              {
                BoundScaleY[i].Set(scale_min, scale_max);
              }
              i++;
            }

            // Get bound of the other axis
            mpScale* axis = (mpScale*)m_plot->GetLayerXAxis();
            if (axis && (!axis->GetAuto()))
            {
              BoundScaleX = axis->GetScale();
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
