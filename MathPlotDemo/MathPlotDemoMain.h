/***************************************************************
 * Name:      MathPlotDemoMain.h
 * Purpose:   Defines Application Frame
 * Author:    Lionel ()
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
#include <wx/frame.h>
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
        //*)

        //(*Identifiers(MathPlotDemoFrame)
        static const long ID_BUTTON1;
        static const long ID_PANEL1;
        static const long ID_MATHPLOT1;
        static const long ID_PANEL2;
        //*)

        //(*Declarations(MathPlotDemoFrame)
        mpWindow* mPlot;
        wxAuiManager* AuiManager1;
        wxButton* bDraw;
        wxPanel* pLog;
        wxPanel* pPlot;
        //*)

        mpFXYVector* GetSeries(int Series);

        DECLARE_EVENT_TABLE()
};

#endif // MATHPLOTDEMOMAIN_H
