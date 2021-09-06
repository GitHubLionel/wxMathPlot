/***************************************************************
 * Name:      MathPlotConfigApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Lionel ()
 * Created:   2021-01-24
 * Copyright: Lionel ()
 * License:
 **************************************************************/

#include "MathPlotConfigApp.h"

//(*AppHeaders
#include "MathPlotConfigMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(MathPlotConfigApp);

bool MathPlotConfigApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	MathPlotConfigDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}
