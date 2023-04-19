/***************************************************************
 * Name:      MathPlotDemoMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Lionel ()
 * Created:   2020-11-26
 * Copyright: Lionel ()
 * License:
 **************************************************************/

#include "MathPlotDemoMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(MathPlotDemoFrame)
#include <wx/string.h>
//*)

#include "Sample.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
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
const long MathPlotDemoFrame::ID_PANEL1 = wxNewId();
const long MathPlotDemoFrame::ID_MATHPLOT1 = wxNewId();
const long MathPlotDemoFrame::ID_PANEL2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(MathPlotDemoFrame,wxFrame)
    //(*EventTable(MathPlotDemoFrame)
    //*)
END_EVENT_TABLE()

MathPlotDemoFrame::MathPlotDemoFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(MathPlotDemoFrame)
    wxBoxSizer* BoxSizer1;

    Create(parent, wxID_ANY, _T("MathPlot Demo"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(800,400));
    AuiManager1 = new wxAuiManager(this, wxAUI_MGR_ALLOW_ACTIVE_PANE|wxAUI_MGR_DEFAULT);
    pLog = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    pLog->SetMinSize(wxSize(120,-1));
    bDraw = new wxButton(pLog, ID_BUTTON1, _T("Draw sinus"), wxPoint(16,24), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    bSample = new wxButton(pLog, ID_BUTTON2, _T("Draw Sample"), wxPoint(16,56), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    bBar = new wxButton(pLog, ID_BUTTON3, _T("Draw Bar"), wxPoint(16,88), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    bLog = new wxButton(pLog, ID_BUTTON4, _T("Draw Log"), wxPoint(16,120), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
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

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotDemoFrame::OnbDrawClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotDemoFrame::OnbSampleClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotDemoFrame::OnbBarClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MathPlotDemoFrame::OnbLogClick);
    //*)

    InitializePlot();
}

MathPlotDemoFrame::~MathPlotDemoFrame()
{
    //(*Destroy(MathPlotDemoFrame)
    //*)
}

void MathPlotDemoFrame::InitializePlot(void)
{
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
	mpTitle *plotTitle;
	mPlot->AddLayer(plotTitle = new mpTitle(wxT("Demo MathPlot")));

	wxFont titleFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	plotTitle->SetFont(titleFont);

	mpInfoCoords *info;
	mPlot->AddLayer(info = new mpInfoCoords());
	info->SetVisible(true);

	mpInfoLegend *legend;
	mPlot->AddLayer(legend = new mpInfoLegend());
	legend->SetItemDirection(mpHorizontal);
	legend->SetVisible(true);

	mPlot->Fit();
}

void MathPlotDemoFrame::OnbDrawClick(wxCommandEvent& event)
{
  mPlot->DelAllPlot(true);
  bottomAxis->SetAuto(true);
  leftAxis->SetLogAxis(false);

	// add a simple sinus serie
	mpFXYVector *serie = mPlot->GetXYSeries(0);
	for (int i = 0; i < 100; i++)
		serie->AddData(i / 10.0, sin(i / 10.0), true);
	mPlot->Fit();
}

void MathPlotDemoFrame::OnbSampleClick(wxCommandEvent& event)
{
  mPlot->DelAllPlot(true);
  bottomAxis->SetAuto(true);
  leftAxis->SetLogAxis(false);

	mPlot->AddLayer(new MyFunction());
	mPlot->AddLayer(new MySIN(10.0, 220.0));
	mPlot->AddLayer(new MyCOSinverse(10.0, 100.0));
  mPlot->AddLayer(new MyLissajoux(125.0));
	mPlot->Fit();
}

void MathPlotDemoFrame::OnbBarClick(wxCommandEvent& event)
{
  mPlot->DelAllPlot(true);
  bottomAxis->SetAuto(true);
  leftAxis->SetLogAxis(false);

	mpFXYVector *vectorLayer = new mpFXYVector(_T("Bar X²"), mpALIGN_NE, true);
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

void MathPlotDemoFrame::OnbLogClick(wxCommandEvent& event)
{
  mPlot->DelAllPlot(true);
  bottomAxis->SetAuto(false);
  bottomAxis->SetMinScale(0);
  bottomAxis->SetMaxScale(10);
  leftAxis->SetLogAxis(true);

  mPlot->AddLayer(new MyLOG());
  mPlot->Fit();
}
