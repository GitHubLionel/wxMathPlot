/***************************************************************
 * Name:      MathPlotDemoMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Lionel ()
 * Created:   2020-11-26
 * Copyright: Lionel ()
 * License:
 **************************************************************/

#include "MathPlotDemoMain.h"
#include <wx/wx.h>
#include <wx/msgdlg.h>

//(*InternalHeaders(MathPlotDemoFrame)
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/overlay.h>
#include <wx/tipwin.h>
#include "Sample.h"

//helper functions
enum wxBuildInfoFormat { short_f, long_f };

wxString wxBuildInfo(wxBuildInfoFormat format)
{
    wxString wxbuild(wxVERSION_STRING);
    if (sizeof(int*) == 8) wxbuild << " 64-bit ";
    if (format == long_f) {
    #if defined(__WXMSW__)
        wxbuild << _T(" Windows");
    #elif defined(__UNIX__)
        wxbuild << _T(" Linux");
    #endif
    #if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
    #else
        wxbuild << _T("-ANSI build");
    #endif // wxUSE_UNICODE
    }
    return wxbuild;
}

//(*IdInit(MathPlotDemoFrame)
const wxWindowID MathPlotDemoFrame::ID_PANEL1 = wxNewId();
const wxWindowID MathPlotDemoFrame::ID_MATHPLOT1 = wxNewId();
const wxWindowID MathPlotDemoFrame::ID_PANEL2 = wxNewId();
const wxWindowID MathPlotDemoFrame::idMenuPreview = wxNewId();
const wxWindowID MathPlotDemoFrame::idMenuPrint = wxNewId();
const wxWindowID MathPlotDemoFrame::idMenuExit = wxNewId();
const wxWindowID MathPlotDemoFrame::idMenuAbout = wxNewId();
const wxWindowID MathPlotDemoFrame::ID_TIMER = wxNewId();
//*)

BEGIN_EVENT_TABLE(MathPlotDemoFrame,wxFrame)
    //(*EventTable(MathPlotDemoFrame)
    //*)
END_EVENT_TABLE()

MathPlotDemoFrame::MathPlotDemoFrame(wxWindow* parent,wxWindowID id)
{
    (void) id; // Compiler happy
    //(*Initialize(MathPlotDemoFrame)
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer2;
    wxMenu* Menu1;
    wxMenu* Menu3;
    wxMenuBar* MenuBar1;
    wxMenuItem* miAbout;
    wxMenuItem* miPreview;
    wxMenuItem* miQuit;

    Create(parent, wxID_ANY, _("MathPlot Demo"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(800,400));
    AuiManager1 = new wxAuiManager(this, wxAUI_MGR_ALLOW_ACTIVE_PANE|wxAUI_MGR_DEFAULT);
    pLog = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    pLog->SetMinSize(wxSize(120,-1));
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    bDraw = new wxButton(pLog, wxID_ANY, _("Draw sinus"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    BoxSizer2->Add(bDraw, 0, wxALL|wxEXPAND, 10);
    bSample = new wxButton(pLog, wxID_ANY, _("Draw Sample"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    BoxSizer2->Add(bSample, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    bBar = new wxButton(pLog, wxID_ANY, _("Draw Bar"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    BoxSizer2->Add(bBar, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    bLog = new wxButton(pLog, wxID_ANY, _("Log Y sample"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    BoxSizer2->Add(bLog, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    bLogXY = new wxButton(pLog, wxID_ANY, _("Log XY sample"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    BoxSizer2->Add(bLogXY, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    bBarChart = new wxButton(pLog, wxID_ANY, _("Draw BarChart"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    BoxSizer2->Add(bBarChart, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    bImage = new wxButton(pLog, wxID_ANY, _("Draw Image"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    BoxSizer2->Add(bImage, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    bMultiYAxis = new wxButton(pLog, wxID_ANY, _("Multi Y-Axis"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    BoxSizer2->Add(bMultiYAxis, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    bMovingObject = new wxButton(pLog, wxID_ANY, _("Moving object"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    BoxSizer2->Add(bMovingObject, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    cbFreeLine = new wxCheckBox(pLog, wxID_ANY, _("Free line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator);
    cbFreeLine->SetValue(false);
    cbFreeLine->SetToolTip(_("Free drawing on the plot area. Left click and move the mouse. Illustration of OnUserMouseAction"));
    BoxSizer2->Add(cbFreeLine, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    pLog->SetSizer(BoxSizer2);
    AuiManager1->AddPane(pLog, wxAuiPaneInfo().Name(_T("PaneName0")).DefaultPane().Caption(_("Log")).CaptionVisible().CloseButton(false).Left().Floatable(false).MinSize(wxSize(120,-1)).Movable(false));
    pPlot = new wxPanel(this, ID_PANEL2, wxPoint(227,228), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    mPlot = new mpWindow(pPlot, ID_MATHPLOT1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    mPlot->UpdateAll();
    mPlot->Fit();
    BoxSizer1->Add(mPlot, 1, wxALL|wxEXPAND, 5);
    pPlot->SetSizer(BoxSizer1);
    AuiManager1->AddPane(pPlot, wxAuiPaneInfo().Name(_T("PaneName1")).DefaultPane().Caption(_("Plot")).CaptionVisible().MaximizeButton().CloseButton(false).Center());
    AuiManager1->Update();
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    miPreview = new wxMenuItem(Menu1, idMenuPreview, _("Print Preview"), wxEmptyString, wxITEM_NORMAL);
    miPreview->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_FULL_SCREEN")),wxART_MENU));
    Menu1->Append(miPreview);
    miPrint = new wxMenuItem(Menu1, idMenuPrint, _("Print"), wxEmptyString, wxITEM_NORMAL);
    miPrint->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_PRINT")),wxART_MENU));
    Menu1->Append(miPrint);
    miQuit = new wxMenuItem(Menu1, idMenuExit, _("Exit\tAlt-X"), wxEmptyString, wxITEM_NORMAL);
    miQuit->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_QUIT")),wxART_MENU));
    Menu1->Append(miQuit);
    MenuBar1->Append(Menu1, _("File"));
    Menu3 = new wxMenu();
    miAbout = new wxMenuItem(Menu3, idMenuAbout, _("About\tF1"), wxEmptyString, wxITEM_NORMAL);
    miAbout->SetBitmap(wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("wxART_INFORMATION")),wxART_MENU));
    Menu3->Append(miAbout);
    MenuBar1->Append(Menu3, _("Help"));
    SetMenuBar(MenuBar1);
    Timer.SetOwner(this, ID_TIMER);
    Timer.Start(25, false);

    bDraw->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbDrawClick, this);
    bSample->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbSampleClick, this);
    bBar->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbBarClick, this);
    bLog->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbLogClick, this);
    bLogXY->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbLogXYClick, this);
    bBarChart->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbBarChartClick, this);
    bImage->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbImageClick, this);
    bMultiYAxis->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbMultiYAxisClick, this);
    bMovingObject->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbMovingObjectClick, this);
    cbFreeLine->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MathPlotDemoFrame::OncbFreeLineClick, this);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MathPlotDemoFrame::OnmiPreviewSelected, this, idMenuPreview);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MathPlotDemoFrame::OnmiPrintSelected, this, idMenuPrint);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MathPlotDemoFrame::OnmiQuitSelected, this, idMenuExit);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MathPlotDemoFrame::OnmiAboutSelected, this, idMenuAbout);
    Bind(wxEVT_TIMER, &MathPlotDemoFrame::OnTimerTrigger, this, ID_TIMER);
    //*)

    InitializePlot();
}

MathPlotDemoFrame::~MathPlotDemoFrame()
{
  //(*Destroy(MathPlotDemoFrame)
  //*)
  mPlot->DelAllLayers(mpForceDelete, false);
  AuiManager1->UnInit(); // Nadler bugfix: wxSmith-generated code fails to uninitialize AUI manager and its bindings
  delete AuiManager1;    // Nadler bugfix: wxSmith-generated code fails to delete AUI manager and leaks memory
}

void MathPlotDemoFrame::InitializePlot(void)
{
  mPlot->EnableDoubleBuffer(true);
  mPlot->SetMargins(50, 20, 80, 80);

  // We always have a bottom axis (X axis) and a left axis (Y axis)
  bottomAxis = new mpScaleX(wxT("X"), mpALIGN_CENTERX, true, mpX_NORMAL);
  bottomAxis->SetCanDelete(false); // We can not delete this axis in IHM
  bottomAxis->SetLabelFormat("%g");
  leftAxis = new mpScaleY(wxT("Y"), mpALIGN_CENTERY, true);
  leftAxis->SetCanDelete(false); // We can not delete this axis in IHM
  leftAxis->SetLabelFormat("%g");

  wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
  wxPen axispen(*wxRED, 2, wxPENSTYLE_SOLID);
  bottomAxis->SetFont(graphFont);
  leftAxis->SetFont(graphFont);
  bottomAxis->SetPen(axispen);
  leftAxis->SetPen(axispen);

  mPlot->AddLayer(bottomAxis, false, false);
  mPlot->AddLayer(leftAxis, false, false);

  // Add a title layer
  mpTitle* plotTitle;
  mPlot->AddLayer(plotTitle = new mpTitle(_("Demo MathPlot")), false, false);

  wxFont titleFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
  plotTitle->SetFont(titleFont);

  // Add a coordinate info layer
  mpInfoCoords* info;
  mPlot->AddLayer(info = new mpInfoCoords(), false, false);
  info->SetVisible(true);

  // Add a legend info layer
  mpInfoLegend* legend;
  mPlot->AddLayer(legend = new mpInfoLegend(), false);
  legend->SetItemDirection(mpHorizontal); // Note: Comment out this line to test mpVertical
  legend->SetVisible(true);

  mPlot->Fit();
}

void MathPlotDemoFrame::CleanPlot(void)
{
  // Stop timer for moving object
  Timer.Stop();
  // Remove all the plot (all functions mpFX, mpFY, mpFXY)
  mPlot->DelAllPlot(mpForceDelete);
  mPlot->SetMouseLeftDownAction(mpMouseBoxZoom);
  mPlot->SetMarginLeft(50);
  // Restore X and Y axis
  bottomAxis->SetAlign(mpALIGN_CENTERX);
  bottomAxis->SetLogAxis(false);
  bottomAxis->SetAuto(true);
  bottomAxis->SetPen(wxPen(*wxRED, 2, wxPENSTYLE_SOLID));
  leftAxis->SetAlign(mpALIGN_CENTERY);
  leftAxis->SetLogAxis(false);
  leftAxis->SetAuto(true);
  leftAxis->SetVisible(true);
  // Remove Bar chart if present
  mPlot->DelLayer(mPlot->GetLayerByName(_T("BarChart")), mpForceDelete);
  // Remove Bitmap if present
  mPlot->DelLayer(mPlot->GetLayerByClassName("mpBitmapLayer"), mpForceDelete);
  // Remove all extra Y axis if present
  mPlot->DelAllYAxisAfterID(mpForceDelete);
}

void MathPlotDemoFrame::OnbDrawClick(wxCommandEvent &WXUNUSED(event))
{
  CleanPlot();
  // add a simple sinus series
  mpFXYVector* series = mPlot->GetXYSeries(0);
  for (int i = 0; i <= 100; i++)
    series->AddData(i / 10.0, sin(i / 10.0), false);
  mPlot->Fit();
}

void MathPlotDemoFrame::OnbSampleClick(wxCommandEvent &WXUNUSED(event))
{
  CleanPlot();
  // Sample from the original wxMathPlot widget
  mPlot->AddLayer(new MyFunction(), false, false);
  mPlot->AddLayer(new MySIN(10.0, 220.0), false, false);
  mPlot->AddLayer(new MyCOSinverse(10.0, 100.0), false, false);
  mPlot->AddLayer(new MyLissajoux(125.0), false);
  mPlot->Fit();
}

void MathPlotDemoFrame::OnbBarClick(wxCommandEvent &WXUNUSED(event))
{
  CleanPlot();

  mpFXYVector* vectorLayer = new mpFXYVector(_T("Bar X²"), mpALIGN_NE, true);
  vectorLayer->SetBrush(*wxGREEN);
  // Create two vectors for x,y and fill them with data
  std::vector<double> vectorx, vectory;
  double xcoord;
  for (unsigned int p = 0; p <= 20; p++)
  {
    xcoord = ((double)p - 10.0) * 5.0;
    vectorx.push_back(xcoord);
    vectory.push_back(0.001 * pow(xcoord, 2));
  }
  vectorLayer->SetData(vectorx, vectory);
  mPlot->AddLayer(vectorLayer);
  //  There is no need to call Fit() here since AddLayer() does it.
}

void MathPlotDemoFrame::OnbLogClick(wxCommandEvent &WXUNUSED(event))
{
  CleanPlot();

  bottomAxis->SetAuto(false);
  bottomAxis->SetScale(0, 10);
  leftAxis->SetLogAxis(true);
  mPlot->AddLayer(new MyPower());
  //  There is no need to call Fit() here since AddLayer() does it.
}

void MathPlotDemoFrame::OnbLogXYClick(wxCommandEvent &WXUNUSED(event))
{
  CleanPlot();

  bottomAxis->SetAlign(mpALIGN_BOTTOM);
  bottomAxis->SetLogAxis(true);
  leftAxis->SetAlign(mpALIGN_LEFT);
  leftAxis->SetLogAxis(true);

  std::vector<double> vectorX, vectorY;
  for (int i = 2; i <= 128; i *= 2)
  {
    vectorX.push_back(i);
    vectorY.push_back(i);
  }

  mpFXYVector* Power2 = new mpFXYVector(_("Power of 2"), mpALIGN_NE, false);
  Power2->SetData(vectorX, vectorY);
  Power2->SetContinuity(true);
  wxPen s1pen(*wxGREEN, 2, wxPENSTYLE_SOLID);
  Power2->SetPen(s1pen);
  Power2->SetBrush(*wxGREEN);
  Power2->SetSymbol(mpsCircle);

  mPlot->AddLayer(Power2);
  //  There is no need to call Fit() here since AddLayer() does it.
}

void MathPlotDemoFrame::OnbBarChartClick(wxCommandEvent &WXUNUSED(event))
{
  CleanPlot();
  mpBarChart* barChart = new mpBarChart(_T("BarChart"));
  // Create vector for y and fill it with data
  std::vector<double> vectory;
  double ycoord;
  for (unsigned int p = 0; p < 5; p++)
  {
    ycoord = 1.5 * ((double)p) + 1.0;
    vectory.push_back(ycoord);
  }
  barChart->SetChartValues(vectory);
  std::vector < std::string > labels;
  std::string label;
  label.assign("Red");
  labels.push_back(label);
  label.assign("Green");
  labels.push_back(label);
  label.assign("Blue");
  labels.push_back(label);
  label.assign("Black");
  labels.push_back(label);
  label.assign("Yellow");
  labels.push_back(label);
  barChart->SetChartLabels(labels);
  barChart->SetBarColour(wxColour(125, 200, 255));
  barChart->SetBarLabelPosition(mpBAR_TOP);
  mPlot->AddLayer(barChart);
  //  There is no need to call Fit() here since AddLayer() does it.
}

void MathPlotDemoFrame::OncbFreeLineClick(wxCommandEvent &WXUNUSED(event))
{
  CleanPlot();
  if (cbFreeLine->GetValue())
  {
    mPlot->SetOnUserMouseAction([this](void *Sender, wxMouseEvent &event, bool &cancel)
    { OnUserMouseAction(Sender, event, cancel);});
  }
  else
    mPlot->UnSetOnUserMouseAction();
}

// Function to interpolate between two colors
wxColour InterpolateColor(const wxColour& start, const wxColour& end, double factor)
{
  return wxColour(
      start.Red()   + factor * (end.Red() - start.Red()),
      start.Green() + factor * (end.Green() - start.Green()),
      start.Blue()  + factor * (end.Blue() - start.Blue()),
      start.Alpha() + factor * (end.Alpha() - start.Alpha())
  );
}

// Create a wxImage with a rainbow gradient
wxImage CreateRainbowImage(int width, int height, bool withAlpha = false)
{
  wxImage image(width, height);

  if (withAlpha) {
      image.InitAlpha(); // Initialize the alpha channel
  }

  // Define colors for gradient
  wxColour startColor(255, 0, 0);   // Red
  wxColour endColor(0, 0, 255);     // Blue

  for (int x = 0; x < width; ++x) {
      double factor = static_cast<double>(x) / (width - 1);
      wxColour currentColor = InterpolateColor(startColor, endColor, factor);

      for (int y = 0; y < height; ++y) {
          image.SetRGB(x, y, currentColor.Red(), currentColor.Green(), currentColor.Blue());
          if (withAlpha) {
              image.SetAlpha(x, y, 255 - static_cast<unsigned char>(factor * 255)); // Optional transparency
          }
      }
  }

  return image;
}

void MathPlotDemoFrame::OnbImageClick(wxCommandEvent &WXUNUSED(event))
{
  CleanPlot();

  // Create rainbow image
  wxImage rainbowImage = CreateRainbowImage(512, 512, true);
  mpBitmapLayer* bitmapLayer = new mpBitmapLayer();
  bitmapLayer->SetBitmap(rainbowImage, 0, 0, 512, 512);

  mPlot->AddLayer(bitmapLayer);
  //  There is no need to call Fit() here since AddLayer() does it.
}

void MathPlotDemoFrame::OnUserMouseAction(void *Sender, wxMouseEvent &event, bool &cancel)
{
  static wxOverlay m_overlay;
  #if wxCHECK_VERSION(3, 3, 0)
  m_overlay.setOpacity(-1);
  #endif

  // Get the mouse position relative to the mpWindow
  wxPoint mousePosition = event.GetPosition();

  // Cast Sender to mpWindow and convert the coordinates
  mpWindow* plotWindow = (mpWindow*)Sender;

  static wxPoint currentPoint;
  static wxPoint lastPoint;
  double plotX, plotY;
  int x = mousePosition.x;
  int y = mousePosition.y;

  plotX = plotWindow->p2x(mousePosition.x);
  plotY = plotWindow->p2y(mousePosition.y, 0);  // Use 1st y-axis

  cancel = false;
  // Left mouse button down
  if (event.LeftDown())
  {
    // Start dragging, add the initial point
    isDragging = true;

    CurrentPolyline = new mpFXYVector("New polyline");
    wxColour random_color = wxIndexColour(rand() * 20 / RAND_MAX);
    CurrentPolyline->SetPen(wxPen(random_color, 2));
    CurrentPolyline->SetContinuity(true);

    // Add new Polyline but not plot it
    plotWindow->AddLayer(CurrentPolyline, false, false);
    // Add point to Polyline but not plot it
    CurrentPolyline->AddData(plotX, plotY, false);

    lastPoint = wxPoint(x, y);
    cancel = true;
  }
  // Mouse dragging with left button held down
  else
    if (event.Dragging() && event.LeftIsDown())
    {
      if (isDragging)
      {
        wxClientDC dc(plotWindow);
        PrepareDC(dc);
        wxDCOverlay overlay(m_overlay, &dc);

        // Only draw the last segment
        currentPoint = wxPoint(x, y);
        dc.SetPen(wxPen(*wxLIGHT_GREY, 2));
        dc.DrawLine(lastPoint, currentPoint);
        lastPoint = currentPoint;

        // Add point to Polyline but not plot it
        CurrentPolyline->AddData(plotX, plotY, false);
        cancel = true;
      }
    }
    // Left mouse button released
    else
      if (event.LeftUp())
      {
        if (isDragging)
        {
          // Finalize the polyline by adding the last point
          isDragging = false;
          m_overlay.Reset();

          // Prevent the simple click with no dragging
          if (CurrentPolyline->GetSize() == 1)
          {
            plotWindow->DelLayer(CurrentPolyline, mpYesDelete, false);
          }
          else
          {
            CurrentPolyline->AddData(plotX, plotY, false); // Last point
            plotWindow->Refresh();  // then refresh the plot
          }
          cancel = true;
        }
      }
}

void MathPlotDemoFrame::OnmiQuitSelected(wxCommandEvent &WXUNUSED(event))
{
  Close(true);
}

void MathPlotDemoFrame::OnmiAboutSelected(wxCommandEvent &WXUNUSED(event))
{
  wxMessageBox(wxString::Format("Welcome to MathPlot Demo!\n"
      "Demonstrates available plot types...\n"
      "Build Info: %s\n"
      "Lionel github version, running under %s.", wxBuildInfo(long_f), wxGetOsDescription()), "About wxWidgets MathPlot demo",
  wxOK | wxICON_INFORMATION, this);
}

void MathPlotDemoFrame::OnmiPreviewSelected(wxCommandEvent &WXUNUSED(event))
{
  // Pass two printout objects: for preview, and possible printing.
  mpPrintout* plotPrint = new mpPrintout(mPlot);
  mpPrintout* plotPrintPreview = new mpPrintout(mPlot);
  wxPrintPreview* preview = new wxPrintPreview(plotPrintPreview, plotPrint);
  wxPreviewFrame* frame = new wxPreviewFrame(preview, this, wxT("Print Plot"), wxPoint(100, 100), wxSize(600, 650));
  frame->Centre(wxBOTH);
  frame->Initialize();
  frame->Show(true);
}

void MathPlotDemoFrame::OnmiPrintSelected(wxCommandEvent &WXUNUSED(event))
{
  wxPrinter printer;
  mpPrintout printout(mPlot, wxT("Plot print"));
  printer.Print(this, &printout, true);
}

void MathPlotDemoFrame::OnbMultiYAxisClick(wxCommandEvent &WXUNUSED(event))
{
  CleanPlot();

  wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
  wxPen axispen(*wxBLACK, 1, wxPENSTYLE_SOLID);

  // Use same color palette as Matlab
  const std::vector<wxColour> plotColors =
  {
    wxColour(0, 114, 189),
    wxColour(217, 83, 25),
    wxColour(237, 177, 32),
    wxColour(126, 47, 142),
  };

  mPlot->SetMouseLeftDownAction(mpMouseDragZoom);
  mPlot->SetMarginLeft(0);
  leftAxis->SetVisible(false);    // Easiest to disable default Y-axis temporarily
  bottomAxis->SetAlign(mpALIGN_BOTTOM);
  bottomAxis->SetPen(axispen);

  MyLissajoux* f1 = new MyLissajoux(125.0);
  f1->SetDrawOutsideMargins(false);
  f1->SetPen(wxPen(plotColors[0], 2));
  mPlot->AddLayer(f1, false, false);

  mpScaleY *axis1 = new mpScaleY(f1->GetName(), mpALIGN_LEFT, false);
  axis1->SetCanDelete(false); // We can not delete this axis in IHM
  axis1->SetLabelFormat("%g");
  axis1->SetFont(graphFont);
  axis1->SetPen(axispen);
  axis1->SetFontColour(plotColors[0]);
  mPlot->AddLayer(axis1, false, false);
  // This axis is dedicated for f1
  f1->SetYAxisID(axis1->GetAxisID());

  MySIN* f2 = new MySIN(10.0, 220.0);
  f2->SetDrawOutsideMargins(false);
  f2->SetPen(wxPen(plotColors[1], 2));
  f2->SetContinuity(true);
  mPlot->AddLayer(f2, false, false);

  mpScaleY *axis2 = new mpScaleY(f2->GetName(), mpALIGN_LEFT, false);
  axis2->SetCanDelete(false); // We can not delete this axis in IHM
  axis2->SetLabelFormat("%g");
  axis2->SetFont(graphFont);
  axis2->SetPen(axispen);
  axis2->SetFontColour(plotColors[1]);
  mPlot->AddLayer(axis2, false, false);
  // This axis is dedicated for f2
  f2->SetYAxisID(axis2->GetAxisID());

  MyFunction* f3 = new MyFunction();
  f3->SetDrawOutsideMargins(false);
  f3->SetPen(wxPen(plotColors[2], 2));
  f3->SetContinuity(true);
  mPlot->AddLayer(f3, false, false);

  mpScaleY *axis3 = new mpScaleY(f3->GetName(), mpALIGN_LEFT, false);
  axis3->SetCanDelete(false); // We can not delete this axis in IHM
  axis3->SetLabelFormat("%g");
  axis3->SetFont(graphFont);
  axis3->SetPen(axispen);
  axis3->SetFontColour(plotColors[2]);
  mPlot->AddLayer(axis3, false, false);
  // This axis is dedicated for f3
  f3->SetYAxisID(axis3->GetAxisID());

  MyCOSinverse* f4 = new MyCOSinverse(10.0, 100.0);
  f4->SetDrawOutsideMargins(false);
  f4->SetPen(wxPen(plotColors[3], 2));
  f4->SetContinuity(true);
  mPlot->AddLayer(f4, false, false);

  mpScaleY *axis4 = new mpScaleY(f4->GetName(), mpALIGN_LEFT, false);
  axis4->SetCanDelete(false); // We can not delete this axis in IHM
  axis4->SetLabelFormat("%g");
  axis4->SetFont(graphFont);
  axis4->SetPen(axispen);
  axis4->SetFontColour(plotColors[3]);
  axis4->SetAuto(false);
  axis4->SetScale(-300, 300);
  mPlot->AddLayer(axis4, false);
  // This axis is dedicated for f4
  f4->SetYAxisID(axis4->GetAxisID());

  // Fit all and refresh display
  mPlot->Fit();

  new wxTipWindow(this,
                  "Interact with individual axis by keeping mouse inside a specific axis and:\n"
                  " - Pan by holding right mouse button and moving up/down\n"
                  " - Zoom by holding left mouse button and moving up/down\n"
                  " - Zoom by scrolling mouse wheel\n"
                  "If keeping mouse inside plot area, same action applies but affect all axes",
                  400);
}

void MathPlotDemoFrame::OnbMovingObjectClick(wxCommandEvent &WXUNUSED(event))
{
  CleanPlot();

  mpBitmapLayer* bmpLayer;
  mPlot->AddLayer(bmpLayer = new mpBitmapLayer(), false);

  wxImage bmp;
  ::wxInitAllImageHandlers();
  bmp.LoadFile(wxT("./gridmap.png"), wxBITMAP_TYPE_PNG);
  bmp.SetMaskColour(0, 0, 0);
  bmpLayer->SetBitmap(bmp, -40, -40, 120, 120);

  mPlot->AddLayer(new mpCovarianceEllipse(0.4, 0.4, 0.2, 2, 32, wxT("Cov1")), false);
  mPlot->AddLayer(new mpCovarianceEllipse(0.2, 0.2, -0.1, 2, 32, wxT("Cov2")), false);

  // Car shape:
  std::vector<double> car_xs(20), car_ys(20);
  int i = 0;
  car_xs[i] = -0.5;
  car_ys[i++] = -0.5;
  car_xs[i] = -0.2;
  car_ys[i++] = -0.5;
  car_xs[i] = -0.2;
  car_ys[i++] = -0.6;
  car_xs[i] = 0;
  car_ys[i++] = -0.6;
  car_xs[i] = 0;
  car_ys[i++] = -0.5;
  car_xs[i] = 0.6;
  car_ys[i++] = -0.5;
  car_xs[i] = 0.6;
  car_ys[i++] = -0.6;
  car_xs[i] = 0.8;
  car_ys[i++] = -0.6;
  car_xs[i] = 0.8;
  car_ys[i++] = -0.5;
  car_xs[i] = 1.0;
  car_ys[i++] = -0.5;
  car_xs[i] = 1.0;
  car_ys[i++] = 0.5;
  car_xs[i] = 0.8;
  car_ys[i++] = 0.5;
  car_xs[i] = 0.8;
  car_ys[i++] = 0.6;
  car_xs[i] = 0.6;
  car_ys[i++] = 0.6;
  car_xs[i] = 0.6;
  car_ys[i++] = 0.5;
  car_xs[i] = 0;
  car_ys[i++] = 0.5;
  car_xs[i] = 0;
  car_ys[i++] = 0.6;
  car_xs[i] = -0.2;
  car_ys[i++] = 0.6;
  car_xs[i] = -0.2;
  car_ys[i++] = 0.5;
  car_xs[i] = -0.5;
  car_ys[i++] = 0.5;

  mpPolygon* lCar;
  mPlot->AddLayer(lCar = new mpPolygon(wxT("car")), false);
  lCar->SetPen(wxPen(*wxBLACK, 3, wxPENSTYLE_SOLID));
  lCar->setPoints(car_xs, car_ys, true);

  mPlot->GetLayerByName(wxT("Cov1"))->SetPen(wxPen(*wxRED, 2, wxPENSTYLE_SOLID));
  mPlot->GetLayerByName(wxT("Cov2"))->SetPen(wxPen(*wxBLUE, 2, wxPENSTYLE_SOLID));

  mpMovableObject* obj;
  obj = (mpMovableObject*)mPlot->GetLayerByName(wxT("Cov1"));
  obj->SetCoordinateBase(-4, -4, 1);
  obj = (mpMovableObject*)mPlot->GetLayerByName(wxT("Cov2"));
  obj->SetCoordinateBase(12, 7, 0);

  mPlot->Fit();

  Timer.Start(25);
}

void MathPlotDemoFrame::OnTimerTrigger(wxTimerEvent &WXUNUSED(event))
{
  mpMovableObject* obj = (mpMovableObject*)mPlot->GetLayerByName(wxT("car"));
  if (obj)
  {
    double x, y, phi, v, w, At = Timer.GetInterval() * 0.001;
    obj->GetCoordinateBase(x, y, phi);

    if (x <= 5 && x >= 0)
    {
      v = 5;
      w = 0;
    }
    else
    {
      v = 4;
      w = 1;
    }

    x += cos(phi) * v * At;
    y += sin(phi) * v * At;
    phi += w * At;

    obj->SetCoordinateBase(x, y, phi);
    mPlot->UpdateAll();
  }
}

