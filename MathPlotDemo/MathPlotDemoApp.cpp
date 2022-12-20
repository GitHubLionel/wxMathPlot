/***************************************************************
 * Name:      MathPlotDemoApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Lionel ()
 * Created:   2020-11-26
 * Copyright: Lionel ()
 * License:
 **************************************************************/

#include "MathPlotDemoApp.h"

//(*AppHeaders
#include "MathPlotDemoMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(MathPlotDemoApp);

bool MathPlotDemoApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	MathPlotDemoFrame* Frame = new MathPlotDemoFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
