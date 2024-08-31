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
const long MathPlotDemoFrame::ID_BUTTON1 = wxNewId();
const long MathPlotDemoFrame::ID_BUTTON2 = wxNewId();
const long MathPlotDemoFrame::ID_BUTTON3 = wxNewId();
const long MathPlotDemoFrame::ID_BUTTON4 = wxNewId();
const long MathPlotDemoFrame::ID_BUTTON5 = wxNewId();
const long MathPlotDemoFrame::ID_BUTTON6 = wxNewId();
const long MathPlotDemoFrame::ID_CHECKBOX1 = wxNewId();
const long MathPlotDemoFrame::ID_PANEL1 = wxNewId();
const long MathPlotDemoFrame::ID_MATHPLOT1 = wxNewId();
const long MathPlotDemoFrame::ID_PANEL2 = wxNewId();
const long MathPlotDemoFrame::idMenuPreview = wxNewId();
const long MathPlotDemoFrame::idMenuPrint = wxNewId();
const long MathPlotDemoFrame::idMenuExit = wxNewId();
const long MathPlotDemoFrame::idMenuAbout = wxNewId();
//*)

BEGIN_EVENT_TABLE(MathPlotDemoFrame,wxFrame)
    //(*EventTable(MathPlotDemoFrame)
    //*)
END_EVENT_TABLE()

MathPlotDemoFrame::MathPlotDemoFrame(wxWindow* parent,wxWindowID id)
{
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
    bDraw = new wxButton(pLog, ID_BUTTON1, _("Draw sinus"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(bDraw, 0, wxALL|wxEXPAND, 10);
    bSample = new wxButton(pLog, ID_BUTTON2, _("Draw Sample"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer2->Add(bSample, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    bBar = new wxButton(pLog, ID_BUTTON3, _("Draw Bar"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    BoxSizer2->Add(bBar, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    bLog = new wxButton(pLog, ID_BUTTON4, _("Log Y sample"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    BoxSizer2->Add(bLog, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    bLogXY = new wxButton(pLog, ID_BUTTON5, _("Log XY sample"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    BoxSizer2->Add(bLogXY, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    bBarChart = new wxButton(pLog, ID_BUTTON6, _("Draw BarChart"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
    BoxSizer2->Add(bBarChart, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    cbFreeLine = new wxCheckBox(pLog, ID_CHECKBOX1, _("Free line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    cbFreeLine->SetValue(false);
    cbFreeLine->SetToolTip(_("Free drawing on the plot area. Left click and move the mouse. Illustration of OnUserMouseAction"));
    BoxSizer2->Add(cbFreeLine, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 10);
    pLog->SetSizer(BoxSizer2);
    BoxSizer2->Fit(pLog);
    BoxSizer2->SetSizeHints(pLog);
    AuiManager1->AddPane(pLog, wxAuiPaneInfo().Name(_T("PaneName0")).DefaultPane().Caption(_("Log")).CaptionVisible().CloseButton(false).Left().Floatable(false).MinSize(wxSize(120,-1)).Movable(false));
    pPlot = new wxPanel(this, ID_PANEL2, wxPoint(227,228), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    mPlot = new mpWindow(pPlot, ID_MATHPLOT1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    mPlot->UpdateAll();
    mPlot->Fit();
    BoxSizer1->Add(mPlot, 1, wxALL|wxEXPAND, 5);
    pPlot->SetSizer(BoxSizer1);
    BoxSizer1->Fit(pPlot);
    BoxSizer1->SetSizeHints(pPlot);
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

    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbDrawClick, this, ID_BUTTON1);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbSampleClick, this, ID_BUTTON2);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbBarClick, this, ID_BUTTON3);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbLogClick, this, ID_BUTTON4);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbLogXYClick, this, ID_BUTTON5);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MathPlotDemoFrame::OnbBarChartClick, this, ID_BUTTON6);
    Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &MathPlotDemoFrame::OncbFreeLineClick, this, ID_CHECKBOX1);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MathPlotDemoFrame::OnmiPreviewSelected, this, idMenuPreview);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MathPlotDemoFrame::OnmiPrintSelected, this, idMenuPrint);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MathPlotDemoFrame::OnmiQuitSelected, this, idMenuExit);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MathPlotDemoFrame::OnmiAboutSelected, this, idMenuAbout);
    //*)

    InitializePlot();
}

MathPlotDemoFrame::~MathPlotDemoFrame()
{
  //(*Destroy(MathPlotDemoFrame)
  //*)
  AuiManager1->UnInit();
}

void MathPlotDemoFrame::InitializePlot(void)
{
  mPlot->EnableDoubleBuffer(true);
  mPlot->SetMargins(50, 20, 80, 80);

  bottomAxis = new mpScaleX(wxT("X"), mpALIGN_CENTERX, true, mpX_NORMAL);
  bottomAxis->SetLabelFormat("%g");
  leftAxis = new mpScaleY(wxT("Y"), mpALIGN_CENTERY, true);
  leftAxis->SetLabelFormat("%g");

  wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
  wxPen axispen(*wxRED, 2, wxPENSTYLE_SOLID);
  bottomAxis->SetFont(graphFont);
  leftAxis->SetFont(graphFont);
  bottomAxis->SetPen(axispen);
  leftAxis->SetPen(axispen);

  mPlot->AddLayer(bottomAxis);
  mPlot->AddLayer(leftAxis);
  mpTitle* plotTitle;
  mPlot->AddLayer(plotTitle = new mpTitle(_("Demo MathPlot")));

  wxFont titleFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
  plotTitle->SetFont(titleFont);

  mpInfoCoords* info;
  mPlot->AddLayer(info = new mpInfoCoords());
  info->SetVisible(true);

  mpInfoLegend* legend;
  mPlot->AddLayer(legend = new mpInfoLegend());
  legend->SetItemDirection(mpHorizontal);
  legend->SetVisible(true);

  mPlot->Fit();
}

void MathPlotDemoFrame::CleanPlot(void)
{
  mPlot->DelAllPlot(true);
  bottomAxis->SetAlign(mpALIGN_CENTERX);
  bottomAxis->SetLogAxis(false);
  leftAxis->SetAlign(mpALIGN_CENTERY);
  leftAxis->SetLogAxis(false);
  bottomAxis->SetAuto(true);
  mPlot->DelLayer(mPlot->GetLayerByName(_T("BarChart")), true);
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
  mPlot->AddLayer(new MyFunction());
  mPlot->AddLayer(new MySIN(10.0, 220.0));
  mPlot->AddLayer(new MyCOSinverse(10.0, 100.0));
  mPlot->AddLayer(new MyLissajoux(125.0));
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
  mPlot->Fit();
}

void MathPlotDemoFrame::OnbLogClick(wxCommandEvent &WXUNUSED(event))
{
  CleanPlot();

  bottomAxis->SetAuto(false);
  bottomAxis->SetMinScale(0);
  bottomAxis->SetMaxScale(10);
  leftAxis->SetLogAxis(true);

  mPlot->AddLayer(new MyPower());
  mPlot->Fit();
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

  mpFXYVector* Power2 = new mpFXYVector(_("Power of 2"), mpALIGN_NE);
  Power2->SetData(vectorX, vectorY);
  Power2->SetContinuity(true);
  wxPen s1pen(*wxGREEN, 2, wxPENSTYLE_SOLID);
  Power2->SetPen(s1pen);
  Power2->SetBrush(*wxGREEN);
  Power2->SetSymbol(mpsCircle);

  mPlot->AddLayer(Power2);
  mPlot->Fit();
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
  mPlot->Fit();
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

void MathPlotDemoFrame::OnUserMouseAction(void *Sender, wxMouseEvent &event, bool &cancel)
{
  static wxOverlay m_overlay;

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
  plotY = plotWindow->p2y(mousePosition.y);

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
    plotWindow->AddLayer(CurrentPolyline, false);
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
            plotWindow->DelLayer(CurrentPolyline, true, false);
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
