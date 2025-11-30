/***************************************************************
 * Name:      MathPlotDemoMain.h
 * Purpose:   Defines Application Frame
 * Author:    Lionel Reynaud
 * Created:   2020-11-26
 * Copyright: Lionel ()
 * License:
 **************************************************************/

#ifndef MATHPLOTDEMOMAIN_H_INCLUDED
#define MATHPLOTDEMOMAIN_H_INCLUDED

//(*Headers(MathPlotDemoFrame)
#include <mathplot.h>
#ifdef ENABLE_MP_NAMESPACE
  using namespace MathPlot;
#endif // ENABLE_MP_NAMESPACE

#include <wx/aui/aui.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
//*)

class MathPlotDemoFrame: public wxFrame
{
    public:

        MathPlotDemoFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~MathPlotDemoFrame();

    private:

        //(*Handlers(MathPlotDemoFrame)
        void OnbDrawClick(wxCommandEvent& event);
        void OnbSampleClick(wxCommandEvent& event);
        void OnbBarClick(wxCommandEvent& event);
        void OnbLogClick(wxCommandEvent& event);
        void OnbLogXYClick(wxCommandEvent& event);
        void OnmiQuitSelected(wxCommandEvent& event);
        void OnmiAboutSelected(wxCommandEvent& event);
        void OnmiPreviewSelected(wxCommandEvent& event);
        void OnmiPrintSelected(wxCommandEvent& event);
        void OnbBarChartClick(wxCommandEvent& event);
        void OncbFreeLineClick(wxCommandEvent& event);
        void OnbImageClick(wxCommandEvent& event);
        void OnbMultiYAxisClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(MathPlotDemoFrame)
        static const wxWindowID ID_PANEL1;
        static const wxWindowID ID_MATHPLOT1;
        static const wxWindowID ID_PANEL2;
        static const wxWindowID idMenuPreview;
        static const wxWindowID idMenuPrint;
        static const wxWindowID idMenuExit;
        static const wxWindowID idMenuAbout;
        //*)

        //(*Declarations(MathPlotDemoFrame)
        mpWindow* mPlot;
        wxAuiManager* AuiManager1;
        wxButton* bBar;
        wxButton* bBarChart;
        wxButton* bDraw;
        wxButton* bImage;
        wxButton* bLog;
        wxButton* bLogXY;
        wxButton* bMultiYAxis;
        wxButton* bSample;
        wxCheckBox* cbFreeLine;
        wxMenuItem* miPrint;
        wxPanel* pLog;
        wxPanel* pPlot;
        //*)

        mpScaleX *bottomAxis = NULL;
        mpScaleY *leftAxis = NULL;
        // Extra Y axis
        mpScaleY* axis1 = NULL;
        mpScaleY* axis2 = NULL;
        mpScaleY* axis3 = NULL;
        mpScaleY* axis4 = NULL;
        void InitializePlot(void);
        void CleanPlot(void);

        // Free Line part
        bool isDragging = false;          // Track if the user is dragging the mouse
        mpFXYVector* CurrentPolyline = NULL;
        void OnUserMouseAction(void *Sender, wxMouseEvent &event, bool &cancel);

        DECLARE_EVENT_TABLE()
};

#endif // MATHPLOTDEMOMAIN_H_INCLUDED
