///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/panel.h>
#include <wx/spinctrl.h>
#include <wx/radiobut.h>
#include <wx/notebook.h>

#include <wx/valnum.h>

// List of string message used
const wxString MESS_TRANSPARENT = _("Transparent not work on Linux");
const wxString MESS_COLOUR = _("Please choose the background colour");
const wxString MESS_AXIS_DELETE = _("Delete the selected axis ?");
const wxString MESS_DELETE = _("Delete the serie ?");
const wxString MESS_LINES_ADD = _("Add horizontal line (vertical if No) ?");
const wxString MESS_LINES_DELETE = _("Delete the line ?");
const wxString MESS_CONFIRM = _("Confirmation");
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MathPlotConfigDialogBuilder
///////////////////////////////////////////////////////////////////////////////
class MathPlotConfigDialogBuilder : public wxDialog
{
	private:

	protected:
		wxNotebook* nbConfig;
		wxPanel* Panel1;
		wxStaticText* StaticText11;
		wxTextCtrl* edTitle;
		wxButton* bFontTitle;
		wxCheckBox* cbTitleVisible;
		wxStaticText* StaticText41;
		wxTextCtrl* edMarginTop;
		wxStaticText* StaticText38;
		wxTextCtrl* edMarginBottom;
		wxStaticText* StaticText39;
		wxTextCtrl* edMarginLeft;
		wxStaticText* StaticText40;
		wxTextCtrl* edMarginRight;
		wxCheckBox* cbDrawBox;
		wxStaticText* StaticText37;
		wxTextCtrl* edExtraMargin;
		wxButton* bBGColor;
		wxStaticText* StaticText18;
		wxChoice* cbCoord;
		wxCheckBox* cbCoordVisible;
		wxCheckBox* cbCoordOutside;
		wxCheckBox* cbCoordinates;
		wxCheckBox* cbMagnetize;
		wxStaticText* StaticText22;
		wxButton* bCoordBrushColor;
		wxStaticText* StaticText23;
		wxChoice* cbCoordBrushStyle;
		wxStaticText* StaticText36;
		wxChoice* ChoiceLeftMouseAction;
		wxPanel* Panel2;
		wxStaticText* StaticText16;
		wxChoice* cbLegendPosition;
		wxStaticText* StaticText24;
		wxChoice* cbLegendStyle;
		wxStaticText* StaticText25;
		wxChoice* cbLegendDirection;
		wxCheckBox* cbLegendVisible;
		wxButton* bFontLegend;
		wxStaticText* StaticText20;
		wxButton* bLegendBrushColor;
		wxStaticText* StaticText21;
		wxChoice* cbLegendBrushStyle;
		wxPanel* Panel3;
		wxChoice* ChoiceAxis;
		wxButton* bAddXAxis;
		wxButton* bAddYAxis;
		wxButton* bDelAxis;
		wxStaticText* StaticText8;
		wxTextCtrl* edAxisName;
		wxButton* bFontAxis;
		wxStaticText* StaticText12;
		wxButton* bAxisPenColor;
		wxStaticText* StaticText13;
		wxChoice* cbAxisPenWidth;
		wxStaticText* StaticText14;
		wxChoice* cbAxisPenStyle;
		wxCheckBox* cbAutoScale;
		wxStaticText* StaticText27;
		wxTextCtrl* edScaleMin;
		wxStaticText* StaticText28;
		wxTextCtrl* edScaleMax;
		wxStaticText* StaticText17;
		wxChoice* cbAxisPosition;
		wxCheckBox* cbAxisVisible;
		wxCheckBox* cbGridVisible;
		wxCheckBox* cbAxisOutside;
		wxStaticText* StaticText29;
		wxChoice* cbFormat;
		wxStaticText* StaticText26;
		wxTextCtrl* edFormat;
		wxCheckBox* cbLogAxis;
		wxPanel* Panel4;
		wxStaticText* StaticText1;
		wxChoice* ChoiceSeries;
		wxButton* bDelSeries;
		wxStaticText* StaticText2;
		wxTextCtrl* edSeriesName;
		wxStaticText* StaticText35;
		wxChoice* ChoiceSeriesYAxis;
		wxStaticText* StaticText3;
		wxButton* bSeriesPenColor;
		wxStaticText* StaticText4;
		wxChoice* cbSeriesPenWidth;
		wxStaticText* StaticText5;
		wxChoice* cbSeriesPenStyle;
		wxCheckBox* cbSeriesVisible;
		wxCheckBox* cbSeriesContinuity;
		wxCheckBox* cbSeriesOutside;
		wxCheckBox* cbSeriesShowName;
		wxCheckBox* cbTractable;
		wxStaticText* StaticText6;
		wxButton* bSeriesBrushColor;
		wxStaticText* StaticText7;
		wxChoice* cbSeriesBrushStyle;
		wxStaticText* StaticText9;
		wxChoice* cbSeriesSymbolType;
		wxStaticText* StaticText10;
		wxSpinCtrl* cbSeriesSymbolSize;
		wxStaticText* StaticText19;
		wxSpinCtrl* cbSeriesStep;
		wxCheckBox* cbBar;
		wxPanel* Panel5;
		wxChoice* ChoiceLines;
		wxButton* bAddLines;
		wxButton* bDelLines;
		wxPanel* pLines;
		wxStaticText* StaticText31;
		wxTextCtrl* edLinesName;
		wxRadioButton* rbLinesDirection;
		wxStaticText* StaticText30;
		wxTextCtrl* edLinesValue;
		wxStaticText* stLinesYIndexLabel;
		wxChoice* ChoiceLinesYAxis;
		wxStaticText* StaticText32;
		wxButton* bLinesPenColor;
		wxStaticText* StaticText33;
		wxChoice* cbLinesPenWidth;
		wxStaticText* StaticText34;
		wxChoice* cbLinesPenStyle;
		wxCheckBox* cbLinesVisible;
		wxCheckBox* cbLinesOutside;
		wxCheckBox* cbLinesShowName;
		wxButton* bApply;
		wxButton* bClose;

		// Margin validator
		unsigned int int_top, int_bottom, int_left, int_right, int_extra;
		// Scale validator
		double scale_min, scale_max;
		// Line position validator
		double line_value;

		// Virtual event handlers, override them in your derived class
		virtual void OnnbConfigPageChanged( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnbFontClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnbColorClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAxisSelect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnbAddAxisClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnbDelAxisClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OncbAutoScaleClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OncbFormatSelect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChoiceSeries( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnbDelSeriesClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChoiceLinesSelect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnbAddLinesClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnbDelLinesClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnbApplyClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }


	public:

		MathPlotConfigDialogBuilder( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("MathPlot Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSTAY_ON_TOP|wxDEFAULT_DIALOG_STYLE );

		~MathPlotConfigDialogBuilder();

};

