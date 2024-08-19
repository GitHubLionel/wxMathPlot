/***************************************************************
 * Name:      MathPlotDemoMain.h
 * Purpose:   Defines Application Frame
 * Author:    Lionel Reynaud
 * Created:   2020-11-26
 * Copyright: Lionel ()
 * License:
 **************************************************************/

#ifndef MATHPLOTDEMOMAIN_H
#define MATHPLOTDEMOMAIN_H

//(*Headers(MathPlotDemoFrame)
#include <mathplot.h>
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
        //*)

        //(*Identifiers(MathPlotDemoFrame)
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON3;
        static const long ID_BUTTON4;
        static const long ID_BUTTON5;
        static const long ID_BUTTON6;
        static const long ID_CHECKBOX1;
        static const long ID_PANEL1;
        static const long ID_MATHPLOT1;
        static const long ID_PANEL2;
        static const long idMenuPreview;
        static const long idMenuPrint;
        static const long idMenuExit;
        static const long idMenuAbout;
        //*)

        //(*Declarations(MathPlotDemoFrame)
        mpWindow* mPlot;
        wxAuiManager* AuiManager1;
        wxButton* bBar;
        wxButton* bBarChart;
        wxButton* bDraw;
        wxButton* bLog;
        wxButton* bLogXY;
        wxButton* bSample;
        wxCheckBox* cbFreeLine;
        wxMenuItem* miPrint;
        wxPanel* pLog;
        wxPanel* pPlot;
        //*)

        mpScaleX *bottomAxis = NULL;
        mpScaleY *leftAxis = NULL;
        void InitializePlot(void);
        void CleanPlot(void);

        // Free Line part
        bool isDragging = false;          // Track if the user is dragging the mouse
        mpFXYVector* CurrentPolyline = NULL;
        void OnUserMouseAction(void *Sender, wxMouseEvent &event, bool &cancel);

        DECLARE_EVENT_TABLE()
};

#endif // MATHPLOTDEMOMAIN_H
