///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MathPlotConfigBuilder.h"

///////////////////////////////////////////////////////////////////////////

MathPlotConfigDialogBuilder::MathPlotConfigDialogBuilder( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* sizerMain;
	sizerMain = new wxBoxSizer( wxVERTICAL );

	nbConfig = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	Panel1 = new wxPanel( nbConfig, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* BoxSizer3;
	BoxSizer3 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* StaticBoxSizer1;
	StaticBoxSizer1 = new wxStaticBoxSizer( new wxStaticBox( Panel1, wxID_ANY, _("Title ") ), wxVERTICAL );

	wxFlexGridSizer* FlexGridSizer1;
	FlexGridSizer1 = new wxFlexGridSizer( 2, 3, 0, 0 );
	FlexGridSizer1->AddGrowableCol( 1 );
	FlexGridSizer1->SetFlexibleDirection( wxBOTH );
	FlexGridSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText11 = new wxStaticText( StaticBoxSizer1->GetStaticBox(), wxID_ANY, _("Title :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText11->Wrap( -1 );
	FlexGridSizer1->Add( StaticText11, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	edTitle = new wxTextCtrl( StaticBoxSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	FlexGridSizer1->Add( edTitle, 1, wxALL|wxEXPAND, 5 );

	bFontTitle = new wxButton( StaticBoxSizer1->GetStaticBox(), wxID_ANY, _("Font"), wxDefaultPosition, wxSize( 64,-1 ), 0 );

	bFontTitle->SetDefault();
	bFontTitle->SetAuthNeeded();
	bFontTitle->Enable( false );

	FlexGridSizer1->Add( bFontTitle, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	cbTitleVisible = new wxCheckBox( StaticBoxSizer1->GetStaticBox(), wxID_ANY, _("Visible"), wxDefaultPosition, wxDefaultSize, 0 );
	cbTitleVisible->SetValue(true);
	FlexGridSizer1->Add( cbTitleVisible, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );


	StaticBoxSizer1->Add( FlexGridSizer1, 1, wxLEFT|wxRIGHT|wxEXPAND, 5 );


	BoxSizer3->Add( StaticBoxSizer1, 0, wxALL|wxEXPAND, 2 );

	wxStaticBoxSizer* StaticBoxSizer2;
	StaticBoxSizer2 = new wxStaticBoxSizer( new wxStaticBox( Panel1, wxID_ANY, _("Margins ") ), wxVERTICAL );

	wxBoxSizer* BoxSizer17;
	BoxSizer17 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* BoxSizer20;
	BoxSizer20 = new wxBoxSizer( wxHORIZONTAL );

	StaticText41 = new wxStaticText( StaticBoxSizer2->GetStaticBox(), wxID_ANY, _("Top :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText41->Wrap( -1 );
	BoxSizer20->Add( StaticText41, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	edMarginTop = new wxTextCtrl( StaticBoxSizer2->GetStaticBox(), wxID_ANY, _("50"), wxDefaultPosition, wxSize( 40,-1 ), wxTE_RIGHT );
	edMarginTop->SetToolTip( _("Top") );

	BoxSizer20->Add( edMarginTop, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	StaticText38 = new wxStaticText( StaticBoxSizer2->GetStaticBox(), wxID_ANY, _("Bottom :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText38->Wrap( -1 );
	BoxSizer20->Add( StaticText38, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	edMarginBottom = new wxTextCtrl( StaticBoxSizer2->GetStaticBox(), wxID_ANY, _("50"), wxDefaultPosition, wxSize( 40,-1 ), wxTE_RIGHT );
	edMarginBottom->SetToolTip( _("Bottom") );

	BoxSizer20->Add( edMarginBottom, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	StaticText39 = new wxStaticText( StaticBoxSizer2->GetStaticBox(), wxID_ANY, _("Left :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText39->Wrap( -1 );
	BoxSizer20->Add( StaticText39, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	edMarginLeft = new wxTextCtrl( StaticBoxSizer2->GetStaticBox(), wxID_ANY, _("50"), wxDefaultPosition, wxSize( 40,-1 ), wxTE_RIGHT );
	edMarginLeft->SetToolTip( _("Left") );

	BoxSizer20->Add( edMarginLeft, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	StaticText40 = new wxStaticText( StaticBoxSizer2->GetStaticBox(), wxID_ANY, _("Right :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText40->Wrap( -1 );
	BoxSizer20->Add( StaticText40, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	edMarginRight = new wxTextCtrl( StaticBoxSizer2->GetStaticBox(), wxID_ANY, _("50"), wxDefaultPosition, wxSize( 40,-1 ), wxTE_RIGHT );
	edMarginRight->SetToolTip( _("Right") );

	BoxSizer20->Add( edMarginRight, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	BoxSizer17->Add( BoxSizer20, 1, wxLEFT|wxRIGHT|wxEXPAND, 5 );

	wxBoxSizer* BoxSizer21;
	BoxSizer21 = new wxBoxSizer( wxHORIZONTAL );

	cbDrawBox = new wxCheckBox( StaticBoxSizer2->GetStaticBox(), wxID_ANY, _("Draw box"), wxDefaultPosition, wxDefaultSize, 0 );
	cbDrawBox->SetValue(true);
	BoxSizer21->Add( cbDrawBox, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	StaticText37 = new wxStaticText( StaticBoxSizer2->GetStaticBox(), wxID_ANY, _("Extra margin :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText37->Wrap( -1 );
	BoxSizer21->Add( StaticText37, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	edExtraMargin = new wxTextCtrl( StaticBoxSizer2->GetStaticBox(), wxID_ANY, _("8"), wxDefaultPosition, wxSize( 30,-1 ), wxTE_RIGHT );
	edExtraMargin->SetToolTip( _("Extra margin for plot area") );

	BoxSizer21->Add( edExtraMargin, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	BoxSizer21->Add( 0, 0, 1, wxEXPAND, 5 );

	bBGColor = new wxButton( StaticBoxSizer2->GetStaticBox(), wxID_ANY, _("bg color"), wxDefaultPosition, wxDefaultSize, 0 );

	bBGColor->SetDefault();
	bBGColor->SetAuthNeeded();
	bBGColor->SetToolTip( _("Color of the background of the plot area") );

	BoxSizer21->Add( bBGColor, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	BoxSizer17->Add( BoxSizer21, 1, wxLEFT|wxRIGHT|wxEXPAND, 5 );


	StaticBoxSizer2->Add( BoxSizer17, 1, wxALL|wxEXPAND, 0 );


	BoxSizer3->Add( StaticBoxSizer2, 0, wxALL|wxEXPAND, 2 );

	wxStaticBoxSizer* StaticBoxSizer3;
	StaticBoxSizer3 = new wxStaticBoxSizer( new wxStaticBox( Panel1, wxID_ANY, _("Mouse coordinates ") ), wxHORIZONTAL );

	wxBoxSizer* BoxSizer1;
	BoxSizer1 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* BoxSizer12;
	BoxSizer12 = new wxBoxSizer( wxHORIZONTAL );

	StaticText18 = new wxStaticText( StaticBoxSizer3->GetStaticBox(), wxID_ANY, _("Position :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText18->Wrap( -1 );
	BoxSizer12->Add( StaticText18, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString cbCoordChoices;
	cbCoord = new wxChoice( StaticBoxSizer3->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, cbCoordChoices, 0 );
	cbCoord->SetSelection( 0 );
	BoxSizer12->Add( cbCoord, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	BoxSizer1->Add( BoxSizer12, 0, wxALL|wxALIGN_LEFT, 0 );

	cbCoordVisible = new wxCheckBox( StaticBoxSizer3->GetStaticBox(), wxID_ANY, _("Visible"), wxDefaultPosition, wxDefaultSize, 0 );
	cbCoordVisible->SetValue(true);
	BoxSizer1->Add( cbCoordVisible, 0, wxALL|wxEXPAND, 5 );

	cbCoordOutside = new wxCheckBox( StaticBoxSizer3->GetStaticBox(), wxID_ANY, _("Coordinates in Margins"), wxDefaultPosition, wxDefaultSize, 0 );
	cbCoordOutside->SetValue(true);
	cbCoordOutside->SetToolTip( _("Shows the coordinates when the mouse is in the margins") );

	BoxSizer1->Add( cbCoordOutside, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5 );

	cbCoordinates = new wxCheckBox( StaticBoxSizer3->GetStaticBox(), wxID_ANY, _("Series coordinates"), wxDefaultPosition, wxDefaultSize, 0 );
	cbCoordinates->SetValue(true);
	cbCoordinates->SetToolTip( _("Shows the coordinates of the series closest to the mouse position") );

	BoxSizer1->Add( cbCoordinates, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5 );

	cbMagnetize = new wxCheckBox( StaticBoxSizer3->GetStaticBox(), wxID_ANY, _("Magnetize"), wxDefaultPosition, wxDefaultSize, 0 );
	cbMagnetize->SetValue(true);
	cbMagnetize->SetToolTip( _("Follow the mouse by drawing a horizontal and vertical line") );

	BoxSizer1->Add( cbMagnetize, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5 );


	StaticBoxSizer3->Add( BoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	wxStaticBoxSizer* StaticBoxSizer4;
	StaticBoxSizer4 = new wxStaticBoxSizer( new wxStaticBox( StaticBoxSizer3->GetStaticBox(), wxID_ANY, _("Brush ") ), wxHORIZONTAL );

	wxFlexGridSizer* FlexGridSizer4;
	FlexGridSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	FlexGridSizer4->SetFlexibleDirection( wxBOTH );
	FlexGridSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText22 = new wxStaticText( StaticBoxSizer4->GetStaticBox(), wxID_ANY, _("Color :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText22->Wrap( -1 );
	FlexGridSizer4->Add( StaticText22, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	bCoordBrushColor = new wxButton( StaticBoxSizer4->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );

	bCoordBrushColor->SetDefault();
	bCoordBrushColor->SetAuthNeeded();
	FlexGridSizer4->Add( bCoordBrushColor, 1, wxALL|wxEXPAND, 2 );

	StaticText23 = new wxStaticText( StaticBoxSizer4->GetStaticBox(), wxID_ANY, _("Style :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText23->Wrap( -1 );
	FlexGridSizer4->Add( StaticText23, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString cbCoordBrushStyleChoices;
	cbCoordBrushStyle = new wxChoice( StaticBoxSizer4->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, cbCoordBrushStyleChoices, 0 );
	cbCoordBrushStyle->SetSelection( 0 );
	FlexGridSizer4->Add( cbCoordBrushStyle, 1, wxALL|wxEXPAND, 2 );


	StaticBoxSizer4->Add( FlexGridSizer4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0 );


	StaticBoxSizer3->Add( StaticBoxSizer4, 0, wxALL|wxALIGN_TOP, 5 );


	BoxSizer3->Add( StaticBoxSizer3, 0, wxALL|wxEXPAND, 2 );

	wxStaticBoxSizer* StaticBoxSizer12;
	StaticBoxSizer12 = new wxStaticBoxSizer( new wxStaticBox( Panel1, wxID_ANY, _("Mouse interaction") ), wxHORIZONTAL );

	wxBoxSizer* BoxSizer22;
	BoxSizer22 = new wxBoxSizer( wxHORIZONTAL );

	StaticText36 = new wxStaticText( StaticBoxSizer12->GetStaticBox(), wxID_ANY, _("Left mouse action :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText36->Wrap( -1 );
	BoxSizer22->Add( StaticText36, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString ChoiceLeftMouseActionChoices[] = { _("Box zoom"), _("Drag zoom") };
	int ChoiceLeftMouseActionNChoices = sizeof( ChoiceLeftMouseActionChoices ) / sizeof( wxString );
	ChoiceLeftMouseAction = new wxChoice( StaticBoxSizer12->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoiceLeftMouseActionNChoices, ChoiceLeftMouseActionChoices, 0 );
	ChoiceLeftMouseAction->SetSelection( 0 );
	BoxSizer22->Add( ChoiceLeftMouseAction, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	StaticBoxSizer12->Add( BoxSizer22, 0, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	BoxSizer3->Add( StaticBoxSizer12, 0, wxALL|wxEXPAND, 2 );


	Panel1->SetSizer( BoxSizer3 );
	Panel1->Layout();
	BoxSizer3->Fit( Panel1 );
	nbConfig->AddPage( Panel1, _("General"), true );
	Panel2 = new wxPanel( nbConfig, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* BoxSizer16;
	BoxSizer16 = new wxBoxSizer( wxHORIZONTAL );

	wxFlexGridSizer* FlexGridSizer6;
	FlexGridSizer6 = new wxFlexGridSizer( 4, 2, 0, 0 );
	FlexGridSizer6->SetFlexibleDirection( wxBOTH );
	FlexGridSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText16 = new wxStaticText( Panel2, wxID_ANY, _("Position :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText16->Wrap( -1 );
	FlexGridSizer6->Add( StaticText16, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString cbLegendPositionChoices;
	cbLegendPosition = new wxChoice( Panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, cbLegendPositionChoices, 0 );
	cbLegendPosition->SetSelection( 0 );
	FlexGridSizer6->Add( cbLegendPosition, 1, wxALL|wxEXPAND, 2 );

	StaticText24 = new wxStaticText( Panel2, wxID_ANY, _("Style :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText24->Wrap( -1 );
	FlexGridSizer6->Add( StaticText24, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	wxString cbLegendStyleChoices[] = { _("Line"), _("Square"), _("Symbol") };
	int cbLegendStyleNChoices = sizeof( cbLegendStyleChoices ) / sizeof( wxString );
	cbLegendStyle = new wxChoice( Panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, cbLegendStyleNChoices, cbLegendStyleChoices, 0 );
	cbLegendStyle->SetSelection( 0 );
	FlexGridSizer6->Add( cbLegendStyle, 1, wxALL|wxEXPAND, 2 );

	StaticText25 = new wxStaticText( Panel2, wxID_ANY, _("Direction :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText25->Wrap( -1 );
	FlexGridSizer6->Add( StaticText25, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	wxString cbLegendDirectionChoices[] = { _("Vertical"), _("Horizontal") };
	int cbLegendDirectionNChoices = sizeof( cbLegendDirectionChoices ) / sizeof( wxString );
	cbLegendDirection = new wxChoice( Panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, cbLegendDirectionNChoices, cbLegendDirectionChoices, 0 );
	cbLegendDirection->SetSelection( 0 );
	FlexGridSizer6->Add( cbLegendDirection, 1, wxALL|wxEXPAND, 2 );

	cbLegendVisible = new wxCheckBox( Panel2, wxID_ANY, _("Visible"), wxDefaultPosition, wxDefaultSize, 0 );
	cbLegendVisible->SetValue(true);
	FlexGridSizer6->Add( cbLegendVisible, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );


	BoxSizer16->Add( FlexGridSizer6, 1, wxALL|wxALIGN_TOP, 5 );

	wxBoxSizer* BoxSizer15;
	BoxSizer15 = new wxBoxSizer( wxVERTICAL );

	bFontLegend = new wxButton( Panel2, wxID_ANY, _("Font"), wxDefaultPosition, wxDefaultSize, 0 );

	bFontLegend->SetDefault();
	bFontLegend->SetAuthNeeded();
	bFontLegend->Enable( false );

	BoxSizer15->Add( bFontLegend, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxStaticBoxSizer* StaticBoxSizer5;
	StaticBoxSizer5 = new wxStaticBoxSizer( new wxStaticBox( Panel2, wxID_ANY, _("Brush ") ), wxVERTICAL );

	wxFlexGridSizer* FlexGridSizer7;
	FlexGridSizer7 = new wxFlexGridSizer( 2, 2, 0, 0 );
	FlexGridSizer7->SetFlexibleDirection( wxBOTH );
	FlexGridSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText20 = new wxStaticText( StaticBoxSizer5->GetStaticBox(), wxID_ANY, _("Color :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText20->Wrap( -1 );
	FlexGridSizer7->Add( StaticText20, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	bLegendBrushColor = new wxButton( StaticBoxSizer5->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );

	bLegendBrushColor->SetDefault();
	bLegendBrushColor->SetAuthNeeded();
	FlexGridSizer7->Add( bLegendBrushColor, 1, wxALL|wxEXPAND, 2 );

	StaticText21 = new wxStaticText( StaticBoxSizer5->GetStaticBox(), wxID_ANY, _("Style :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText21->Wrap( -1 );
	FlexGridSizer7->Add( StaticText21, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString cbLegendBrushStyleChoices;
	cbLegendBrushStyle = new wxChoice( StaticBoxSizer5->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, cbLegendBrushStyleChoices, 0 );
	cbLegendBrushStyle->SetSelection( 0 );
	FlexGridSizer7->Add( cbLegendBrushStyle, 1, wxALL|wxEXPAND, 2 );


	StaticBoxSizer5->Add( FlexGridSizer7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	BoxSizer15->Add( StaticBoxSizer5, 0, wxALL|wxEXPAND, 0 );


	BoxSizer16->Add( BoxSizer15, 1, wxALL|wxALIGN_TOP, 5 );


	Panel2->SetSizer( BoxSizer16 );
	Panel2->Layout();
	BoxSizer16->Fit( Panel2 );
	nbConfig->AddPage( Panel2, _("Legend"), true );
	Panel3 = new wxPanel( nbConfig, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerAxis;
	sizerAxis = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* FlexGridSizer3;
	FlexGridSizer3 = new wxFlexGridSizer( 1, 4, 0, 0 );
	FlexGridSizer3->AddGrowableCol( 0 );
	FlexGridSizer3->SetFlexibleDirection( wxBOTH );
	FlexGridSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxArrayString ChoiceAxisChoices;
	ChoiceAxis = new wxChoice( Panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoiceAxisChoices, 0 );
	ChoiceAxis->SetSelection( 0 );
	FlexGridSizer3->Add( ChoiceAxis, 1, wxALL|wxEXPAND, 5 );

	bAddXAxis = new wxButton( Panel3, wxID_ANY, _("Add X axis"), wxDefaultPosition, wxDefaultSize, 0 );

	bAddXAxis->SetDefault();
	bAddXAxis->SetAuthNeeded();
	FlexGridSizer3->Add( bAddXAxis, 0, wxTOP|wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	bAddYAxis = new wxButton( Panel3, wxID_ANY, _("Add Y axis"), wxDefaultPosition, wxDefaultSize, 0 );

	bAddYAxis->SetDefault();
	bAddYAxis->SetAuthNeeded();
	FlexGridSizer3->Add( bAddYAxis, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	bDelAxis = new wxButton( Panel3, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );

	bDelAxis->SetDefault();
	bDelAxis->SetAuthNeeded();
	FlexGridSizer3->Add( bDelAxis, 1, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	sizerAxis->Add( FlexGridSizer3, 0, wxALL|wxEXPAND, 2 );

	wxFlexGridSizer* FlexGridSizer8;
	FlexGridSizer8 = new wxFlexGridSizer( 1, 3, 0, 0 );
	FlexGridSizer8->AddGrowableCol( 1 );
	FlexGridSizer8->SetFlexibleDirection( wxBOTH );
	FlexGridSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText8 = new wxStaticText( Panel3, wxID_ANY, _("Name :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText8->Wrap( -1 );
	FlexGridSizer8->Add( StaticText8, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	edAxisName = new wxTextCtrl( Panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	FlexGridSizer8->Add( edAxisName, 0, wxALL|wxEXPAND, 5 );

	bFontAxis = new wxButton( Panel3, wxID_ANY, _("Font"), wxDefaultPosition, wxSize( 64,-1 ), 0 );

	bFontAxis->SetDefault();
	bFontAxis->SetAuthNeeded();
	bFontAxis->Enable( false );

	FlexGridSizer8->Add( bFontAxis, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	sizerAxis->Add( FlexGridSizer8, 0, wxALL|wxEXPAND, 2 );

	wxBoxSizer* BoxSizer6;
	BoxSizer6 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* BoxSizer7;
	BoxSizer7 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* StaticBoxSizer6;
	StaticBoxSizer6 = new wxStaticBoxSizer( new wxStaticBox( Panel3, wxID_ANY, _("Pen ") ), wxHORIZONTAL );

	wxFlexGridSizer* FlexGridSizer10;
	FlexGridSizer10 = new wxFlexGridSizer( 3, 2, 0, 0 );
	FlexGridSizer10->SetFlexibleDirection( wxBOTH );
	FlexGridSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText12 = new wxStaticText( StaticBoxSizer6->GetStaticBox(), wxID_ANY, _("Color :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText12->Wrap( -1 );
	FlexGridSizer10->Add( StaticText12, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	bAxisPenColor = new wxButton( StaticBoxSizer6->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );

	bAxisPenColor->SetDefault();
	bAxisPenColor->SetAuthNeeded();
	FlexGridSizer10->Add( bAxisPenColor, 1, wxALL|wxEXPAND, 2 );

	StaticText13 = new wxStaticText( StaticBoxSizer6->GetStaticBox(), wxID_ANY, _("Width :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText13->Wrap( -1 );
	FlexGridSizer10->Add( StaticText13, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString cbAxisPenWidthChoices;
	cbAxisPenWidth = new wxChoice( StaticBoxSizer6->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, cbAxisPenWidthChoices, 0 );
	cbAxisPenWidth->SetSelection( 0 );
	FlexGridSizer10->Add( cbAxisPenWidth, 1, wxALL|wxEXPAND, 2 );

	StaticText14 = new wxStaticText( StaticBoxSizer6->GetStaticBox(), wxID_ANY, _("Style :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText14->Wrap( -1 );
	FlexGridSizer10->Add( StaticText14, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString cbAxisPenStyleChoices;
	cbAxisPenStyle = new wxChoice( StaticBoxSizer6->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, cbAxisPenStyleChoices, 0 );
	cbAxisPenStyle->SetSelection( 0 );
	FlexGridSizer10->Add( cbAxisPenStyle, 1, wxALL|wxEXPAND, 2 );


	StaticBoxSizer6->Add( FlexGridSizer10, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0 );


	BoxSizer7->Add( StaticBoxSizer6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2 );

	wxStaticBoxSizer* StaticBoxSizer7;
	StaticBoxSizer7 = new wxStaticBoxSizer( new wxStaticBox( Panel3, wxID_ANY, _("Scale ") ), wxHORIZONTAL );

	wxFlexGridSizer* FlexGridSizer12;
	FlexGridSizer12 = new wxFlexGridSizer( 3, 2, 0, 0 );
	FlexGridSizer12->SetFlexibleDirection( wxBOTH );
	FlexGridSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	cbAutoScale = new wxCheckBox( StaticBoxSizer7->GetStaticBox(), wxID_ANY, _("Auto"), wxDefaultPosition, wxDefaultSize, 0 );
	cbAutoScale->SetValue(true);
	FlexGridSizer12->Add( cbAutoScale, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );


	FlexGridSizer12->Add( 0, 0, 1, wxEXPAND, 5 );

	StaticText27 = new wxStaticText( StaticBoxSizer7->GetStaticBox(), wxID_ANY, _("Min :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText27->Wrap( -1 );
	FlexGridSizer12->Add( StaticText27, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	edScaleMin = new wxTextCtrl( StaticBoxSizer7->GetStaticBox(), wxID_ANY, _("-1"), wxDefaultPosition, wxSize( 64,-1 ), wxTE_RIGHT );
	edScaleMin->Enable( false );

	FlexGridSizer12->Add( edScaleMin, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2 );

	StaticText28 = new wxStaticText( StaticBoxSizer7->GetStaticBox(), wxID_ANY, _("Max :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText28->Wrap( -1 );
	FlexGridSizer12->Add( StaticText28, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	edScaleMax = new wxTextCtrl( StaticBoxSizer7->GetStaticBox(), wxID_ANY, _("1"), wxDefaultPosition, wxSize( 64,-1 ), wxTE_RIGHT );
	edScaleMax->Enable( false );

	FlexGridSizer12->Add( edScaleMax, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2 );


	StaticBoxSizer7->Add( FlexGridSizer12, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0 );


	BoxSizer7->Add( StaticBoxSizer7, 0, wxALL|wxEXPAND, 2 );


	BoxSizer6->Add( BoxSizer7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2 );

	wxBoxSizer* BoxSizer5;
	BoxSizer5 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* BoxSizer13;
	BoxSizer13 = new wxBoxSizer( wxHORIZONTAL );

	StaticText17 = new wxStaticText( Panel3, wxID_ANY, _("Position :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText17->Wrap( -1 );
	BoxSizer13->Add( StaticText17, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString cbAxisPositionChoices;
	cbAxisPosition = new wxChoice( Panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, cbAxisPositionChoices, 0 );
	cbAxisPosition->SetSelection( 0 );
	BoxSizer13->Add( cbAxisPosition, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	BoxSizer5->Add( BoxSizer13, 0, wxALL|wxALIGN_LEFT, 0 );

	cbAxisVisible = new wxCheckBox( Panel3, wxID_ANY, _("Visible"), wxDefaultPosition, wxDefaultSize, 0 );
	cbAxisVisible->SetValue(true);
	BoxSizer5->Add( cbAxisVisible, 0, wxALL|wxALIGN_LEFT, 5 );

	cbGridVisible = new wxCheckBox( Panel3, wxID_ANY, _("Show grid"), wxDefaultPosition, wxDefaultSize, 0 );
	cbGridVisible->SetValue(true);
	BoxSizer5->Add( cbGridVisible, 0, wxALL|wxALIGN_LEFT, 5 );

	cbAxisOutside = new wxCheckBox( Panel3, wxID_ANY, _("Draw Outside Margins"), wxDefaultPosition, wxDefaultSize, 0 );
	cbAxisOutside->SetValue(true);
	BoxSizer5->Add( cbAxisOutside, 0, wxALL|wxALIGN_LEFT, 5 );

	wxBoxSizer* BoxSizer14;
	BoxSizer14 = new wxBoxSizer( wxHORIZONTAL );

	StaticText29 = new wxStaticText( Panel3, wxID_ANY, _("Format :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText29->Wrap( -1 );
	BoxSizer14->Add( StaticText29, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString cbFormatChoices[] = { _("Normal"), _("Time"), _("Hours"), _("Date"), _("DateTime"), _("User"), _("None") };
	int cbFormatNChoices = sizeof( cbFormatChoices ) / sizeof( wxString );
	cbFormat = new wxChoice( Panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, cbFormatNChoices, cbFormatChoices, 0 );
	cbFormat->SetSelection( 0 );
	BoxSizer14->Add( cbFormat, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	BoxSizer5->Add( BoxSizer14, 0, wxALL|wxALIGN_LEFT, 0 );

	wxFlexGridSizer* FlexGridSizer11;
	FlexGridSizer11 = new wxFlexGridSizer( 1, 2, 0, 0 );
	FlexGridSizer11->SetFlexibleDirection( wxBOTH );
	FlexGridSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText26 = new wxStaticText( Panel3, wxID_ANY, _("User Label Format :"), wxPoint( 192,200 ), wxDefaultSize, 0 );
	StaticText26->Wrap( -1 );
	FlexGridSizer11->Add( StaticText26, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	edFormat = new wxTextCtrl( Panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), 0 );
	edFormat->Enable( false );
	edFormat->SetToolTip( _("Format of the label for the axis. Should be like c++ format.") );

	FlexGridSizer11->Add( edFormat, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	BoxSizer5->Add( FlexGridSizer11, 1, wxALL|wxALIGN_LEFT, 0 );

	cbLogAxis = new wxCheckBox( Panel3, wxID_ANY, _("Logarithmic axis"), wxDefaultPosition, wxDefaultSize, 0 );
	cbLogAxis->SetValue(true);
	BoxSizer5->Add( cbLogAxis, 0, wxALL|wxALIGN_LEFT, 5 );


	BoxSizer6->Add( BoxSizer5, 0, wxALL|wxALIGN_TOP, 2 );


	sizerAxis->Add( BoxSizer6, 0, wxALL|wxEXPAND, 0 );


	Panel3->SetSizer( sizerAxis );
	Panel3->Layout();
	sizerAxis->Fit( Panel3 );
	nbConfig->AddPage( Panel3, _("Axis"), true );
	Panel4 = new wxPanel( nbConfig, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* BoxSizer8;
	BoxSizer8 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* FlexGridSizer13;
	FlexGridSizer13 = new wxFlexGridSizer( 1, 3, 0, 0 );
	FlexGridSizer13->AddGrowableCol( 1 );
	FlexGridSizer13->SetFlexibleDirection( wxBOTH );
	FlexGridSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText1 = new wxStaticText( Panel4, wxID_ANY, _("Series name :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText1->Wrap( -1 );
	FlexGridSizer13->Add( StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString ChoiceSeriesChoices;
	ChoiceSeries = new wxChoice( Panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoiceSeriesChoices, 0 );
	ChoiceSeries->SetSelection( 0 );
	FlexGridSizer13->Add( ChoiceSeries, 1, wxALL|wxEXPAND, 5 );

	bDelSeries = new wxButton( Panel4, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );

	bDelSeries->SetDefault();
	bDelSeries->SetAuthNeeded();
	bDelSeries->Enable( false );

	FlexGridSizer13->Add( bDelSeries, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	BoxSizer8->Add( FlexGridSizer13, 0, wxALL|wxEXPAND, 2 );

	wxFlexGridSizer* FlexGridSizer14;
	FlexGridSizer14 = new wxFlexGridSizer( 2, 2, 0, 0 );
	FlexGridSizer14->AddGrowableCol( 1 );
	FlexGridSizer14->SetFlexibleDirection( wxBOTH );
	FlexGridSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText2 = new wxStaticText( Panel4, wxID_ANY, _("Name :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText2->Wrap( -1 );
	FlexGridSizer14->Add( StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	edSeriesName = new wxTextCtrl( Panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	FlexGridSizer14->Add( edSeriesName, 1, wxALL|wxEXPAND, 5 );

	StaticText35 = new wxStaticText( Panel4, wxID_ANY, _("Y axis :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText35->Wrap( -1 );
	FlexGridSizer14->Add( StaticText35, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString ChoiceSeriesYAxisChoices;
	ChoiceSeriesYAxis = new wxChoice( Panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoiceSeriesYAxisChoices, 0 );
	ChoiceSeriesYAxis->SetSelection( 0 );
	FlexGridSizer14->Add( ChoiceSeriesYAxis, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5 );


	BoxSizer8->Add( FlexGridSizer14, 0, wxALL|wxEXPAND, 2 );

	wxFlexGridSizer* FlexGridSizer15;
	FlexGridSizer15 = new wxFlexGridSizer( 1, 2, 0, 0 );
	FlexGridSizer15->SetFlexibleDirection( wxBOTH );
	FlexGridSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* BoxSizer9;
	BoxSizer9 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* StaticBoxSizer8;
	StaticBoxSizer8 = new wxStaticBoxSizer( new wxStaticBox( Panel4, wxID_ANY, _("Pen ") ), wxHORIZONTAL );

	wxFlexGridSizer* FlexGridSizer17;
	FlexGridSizer17 = new wxFlexGridSizer( 3, 2, 0, 0 );
	FlexGridSizer17->SetFlexibleDirection( wxBOTH );
	FlexGridSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText3 = new wxStaticText( StaticBoxSizer8->GetStaticBox(), wxID_ANY, _("Color :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText3->Wrap( -1 );
	FlexGridSizer17->Add( StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	bSeriesPenColor = new wxButton( StaticBoxSizer8->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );

	bSeriesPenColor->SetDefault();
	bSeriesPenColor->SetAuthNeeded();
	FlexGridSizer17->Add( bSeriesPenColor, 1, wxALL|wxEXPAND, 2 );

	StaticText4 = new wxStaticText( StaticBoxSizer8->GetStaticBox(), wxID_ANY, _("Width :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText4->Wrap( -1 );
	FlexGridSizer17->Add( StaticText4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString cbSeriesPenWidthChoices;
	cbSeriesPenWidth = new wxChoice( StaticBoxSizer8->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, cbSeriesPenWidthChoices, 0 );
	cbSeriesPenWidth->SetSelection( 0 );
	FlexGridSizer17->Add( cbSeriesPenWidth, 1, wxALL|wxEXPAND, 2 );

	StaticText5 = new wxStaticText( StaticBoxSizer8->GetStaticBox(), wxID_ANY, _("Style :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText5->Wrap( -1 );
	FlexGridSizer17->Add( StaticText5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString cbSeriesPenStyleChoices;
	cbSeriesPenStyle = new wxChoice( StaticBoxSizer8->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, cbSeriesPenStyleChoices, 0 );
	cbSeriesPenStyle->SetSelection( 0 );
	FlexGridSizer17->Add( cbSeriesPenStyle, 1, wxALL|wxEXPAND, 2 );


	StaticBoxSizer8->Add( FlexGridSizer17, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0 );


	BoxSizer9->Add( StaticBoxSizer8, 0, wxALL|wxALIGN_LEFT, 2 );

	wxBoxSizer* BoxSizer11;
	BoxSizer11 = new wxBoxSizer( wxVERTICAL );

	cbSeriesVisible = new wxCheckBox( Panel4, wxID_ANY, _("Visible"), wxDefaultPosition, wxDefaultSize, 0 );
	cbSeriesVisible->SetValue(true);
	BoxSizer11->Add( cbSeriesVisible, 1, wxALL|wxALIGN_LEFT, 3 );

	cbSeriesContinuity = new wxCheckBox( Panel4, wxID_ANY, _("Continuity"), wxDefaultPosition, wxDefaultSize, 0 );
	cbSeriesContinuity->SetValue(true);
	BoxSizer11->Add( cbSeriesContinuity, 1, wxALL|wxALIGN_LEFT, 3 );

	cbSeriesOutside = new wxCheckBox( Panel4, wxID_ANY, _("Draw Outside Margins"), wxDefaultPosition, wxDefaultSize, 0 );
	cbSeriesOutside->SetValue(true);
	BoxSizer11->Add( cbSeriesOutside, 1, wxALL|wxALIGN_LEFT, 3 );

	cbSeriesShowName = new wxCheckBox( Panel4, wxID_ANY, _("Show name"), wxDefaultPosition, wxDefaultSize, 0 );
	cbSeriesShowName->SetValue(true);
	BoxSizer11->Add( cbSeriesShowName, 1, wxALL|wxALIGN_LEFT, 3 );

	cbTractable = new wxCheckBox( Panel4, wxID_ANY, _("Tractable"), wxDefaultPosition, wxDefaultSize, 0 );
	cbTractable->SetValue(true);
	cbTractable->SetToolTip( _("Allow mouse coordinates") );

	BoxSizer11->Add( cbTractable, 1, wxALL|wxALIGN_LEFT, 3 );


	BoxSizer9->Add( BoxSizer11, 0, wxALL|wxALIGN_LEFT, 5 );


	FlexGridSizer15->Add( BoxSizer9, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	wxBoxSizer* BoxSizer10;
	BoxSizer10 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* StaticBoxSizer9;
	StaticBoxSizer9 = new wxStaticBoxSizer( new wxStaticBox( Panel4, wxID_ANY, _("Brush ") ), wxHORIZONTAL );

	wxFlexGridSizer* FlexGridSizer16;
	FlexGridSizer16 = new wxFlexGridSizer( 2, 2, 0, 0 );
	FlexGridSizer16->SetFlexibleDirection( wxBOTH );
	FlexGridSizer16->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText6 = new wxStaticText( StaticBoxSizer9->GetStaticBox(), wxID_ANY, _("Color :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText6->Wrap( -1 );
	FlexGridSizer16->Add( StaticText6, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	bSeriesBrushColor = new wxButton( StaticBoxSizer9->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );

	bSeriesBrushColor->SetDefault();
	bSeriesBrushColor->SetAuthNeeded();
	FlexGridSizer16->Add( bSeriesBrushColor, 1, wxALL|wxEXPAND, 2 );

	StaticText7 = new wxStaticText( StaticBoxSizer9->GetStaticBox(), wxID_ANY, _("Style :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText7->Wrap( -1 );
	FlexGridSizer16->Add( StaticText7, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	wxString cbSeriesBrushStyleChoices[] = { _("Solid"), _("Transparent"), _("BDiagonal"), _("CrossDiagonal"), _("FDiagonal"), _("Cross"), _("Horizontal"), _("Vertical") };
	int cbSeriesBrushStyleNChoices = sizeof( cbSeriesBrushStyleChoices ) / sizeof( wxString );
	cbSeriesBrushStyle = new wxChoice( StaticBoxSizer9->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, cbSeriesBrushStyleNChoices, cbSeriesBrushStyleChoices, 0 );
	cbSeriesBrushStyle->SetSelection( 0 );
	FlexGridSizer16->Add( cbSeriesBrushStyle, 1, wxALL|wxEXPAND, 2 );


	StaticBoxSizer9->Add( FlexGridSizer16, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0 );


	BoxSizer10->Add( StaticBoxSizer9, 0, wxALL|wxALIGN_LEFT, 2 );

	wxStaticBoxSizer* StaticBoxSizer10;
	StaticBoxSizer10 = new wxStaticBoxSizer( new wxStaticBox( Panel4, wxID_ANY, _("Symbol ") ), wxHORIZONTAL );

	wxFlexGridSizer* FlexGridSizer18;
	FlexGridSizer18 = new wxFlexGridSizer( 2, 2, 0, 0 );
	FlexGridSizer18->SetFlexibleDirection( wxBOTH );
	FlexGridSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText9 = new wxStaticText( StaticBoxSizer10->GetStaticBox(), wxID_ANY, _("Type :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText9->Wrap( -1 );
	FlexGridSizer18->Add( StaticText9, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	wxString cbSeriesSymbolTypeChoices[] = { _("None"), _("Circle"), _("Square"), _("UpTriangle"), _("DownTriangle"), _("Cross"), _("Plus") };
	int cbSeriesSymbolTypeNChoices = sizeof( cbSeriesSymbolTypeChoices ) / sizeof( wxString );
	cbSeriesSymbolType = new wxChoice( StaticBoxSizer10->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, cbSeriesSymbolTypeNChoices, cbSeriesSymbolTypeChoices, 0 );
	cbSeriesSymbolType->SetSelection( 0 );
	FlexGridSizer18->Add( cbSeriesSymbolType, 1, wxALL|wxEXPAND, 2 );

	StaticText10 = new wxStaticText( StaticBoxSizer10->GetStaticBox(), wxID_ANY, _("Size :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText10->Wrap( -1 );
	FlexGridSizer18->Add( StaticText10, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	cbSeriesSymbolSize = new wxSpinCtrl( StaticBoxSizer10->GetStaticBox(), wxID_ANY, wxT("4"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 4, 100, 4 );
	FlexGridSizer18->Add( cbSeriesSymbolSize, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2 );


	StaticBoxSizer10->Add( FlexGridSizer18, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0 );


	BoxSizer10->Add( StaticBoxSizer10, 0, wxALL, 2 );

	wxFlexGridSizer* FlexGridSizer19;
	FlexGridSizer19 = new wxFlexGridSizer( 1, 2, 0, 0 );
	FlexGridSizer19->SetFlexibleDirection( wxBOTH );
	FlexGridSizer19->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText19 = new wxStaticText( Panel4, wxID_ANY, _("Skip point over :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText19->Wrap( -1 );
	FlexGridSizer19->Add( StaticText19, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	cbSeriesStep = new wxSpinCtrl( Panel4, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100, 1 );
	FlexGridSizer19->Add( cbSeriesStep, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	BoxSizer10->Add( FlexGridSizer19, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	cbBar = new wxCheckBox( Panel4, wxID_ANY, _("View as bar"), wxDefaultPosition, wxDefaultSize, 0 );
	cbBar->SetValue(true);
	cbBar->Enable( false );

	BoxSizer10->Add( cbBar, 1, wxALL|wxEXPAND, 5 );


	FlexGridSizer15->Add( BoxSizer10, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5 );


	BoxSizer8->Add( FlexGridSizer15, 0, wxALL|wxEXPAND, 2 );


	Panel4->SetSizer( BoxSizer8 );
	Panel4->Layout();
	BoxSizer8->Fit( Panel4 );
	nbConfig->AddPage( Panel4, _("Series"), true );
	Panel5 = new wxPanel( nbConfig, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerLines;
	sizerLines = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* FlexGridSizer5;
	FlexGridSizer5 = new wxFlexGridSizer( 1, 5, 0, 0 );
	FlexGridSizer5->AddGrowableCol( 0 );
	FlexGridSizer5->SetFlexibleDirection( wxBOTH );
	FlexGridSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxArrayString ChoiceLinesChoices;
	ChoiceLines = new wxChoice( Panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoiceLinesChoices, 0 );
	ChoiceLines->SetSelection( 0 );
	FlexGridSizer5->Add( ChoiceLines, 1, wxALL|wxEXPAND, 5 );

	bAddLines = new wxButton( Panel5, wxID_ANY, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );

	bAddLines->SetDefault();
	bAddLines->SetAuthNeeded();
	FlexGridSizer5->Add( bAddLines, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	bDelLines = new wxButton( Panel5, wxID_ANY, _("Delete"), wxDefaultPosition, wxDefaultSize, 0 );

	bDelLines->SetDefault();
	bDelLines->SetAuthNeeded();
	bDelLines->Enable( false );

	FlexGridSizer5->Add( bDelLines, 0, wxTOP|wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );


	sizerLines->Add( FlexGridSizer5, 0, wxALL|wxEXPAND, 2 );

	pLines = new wxPanel( Panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* BoxSizer4;
	BoxSizer4 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* FlexGridSizer9;
	FlexGridSizer9 = new wxFlexGridSizer( 3, 3, 0, 0 );
	FlexGridSizer9->AddGrowableCol( 1 );
	FlexGridSizer9->SetFlexibleDirection( wxBOTH );
	FlexGridSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText31 = new wxStaticText( pLines, wxID_ANY, _("Name :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText31->Wrap( -1 );
	FlexGridSizer9->Add( StaticText31, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	edLinesName = new wxTextCtrl( pLines, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	FlexGridSizer9->Add( edLinesName, 1, wxALL|wxEXPAND, 5 );

	rbLinesDirection = new wxRadioButton( pLines, wxID_ANY, _("Horizontal"), wxDefaultPosition, wxDefaultSize, 0 );
	rbLinesDirection->SetValue( true );
	rbLinesDirection->Enable( false );

	FlexGridSizer9->Add( rbLinesDirection, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	StaticText30 = new wxStaticText( pLines, wxID_ANY, _("Value :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText30->Wrap( -1 );
	FlexGridSizer9->Add( StaticText30, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	edLinesValue = new wxTextCtrl( pLines, wxID_ANY, _("1"), wxDefaultPosition, wxSize( 64,-1 ), wxTE_RIGHT );
	FlexGridSizer9->Add( edLinesValue, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );


	FlexGridSizer9->Add( 0, 0, 1, wxEXPAND, 5 );

	stLinesYIndexLabel = new wxStaticText( pLines, wxID_ANY, _("Y axis :"), wxDefaultPosition, wxDefaultSize, 0 );
	stLinesYIndexLabel->Wrap( -1 );
	FlexGridSizer9->Add( stLinesYIndexLabel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString ChoiceLinesYAxisChoices;
	ChoiceLinesYAxis = new wxChoice( pLines, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChoiceLinesYAxisChoices, 0 );
	ChoiceLinesYAxis->SetSelection( 0 );
	FlexGridSizer9->Add( ChoiceLinesYAxis, 1, wxALL|wxEXPAND, 5 );


	BoxSizer4->Add( FlexGridSizer9, 0, wxALL|wxEXPAND, 5 );

	wxFlexGridSizer* FlexGridSizer20;
	FlexGridSizer20 = new wxFlexGridSizer( 1, 2, 0, 0 );
	FlexGridSizer20->SetFlexibleDirection( wxBOTH );
	FlexGridSizer20->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxBoxSizer* BoxSizer18;
	BoxSizer18 = new wxBoxSizer( wxHORIZONTAL );

	wxStaticBoxSizer* StaticBoxSizer11;
	StaticBoxSizer11 = new wxStaticBoxSizer( new wxStaticBox( pLines, wxID_ANY, _("Pen ") ), wxHORIZONTAL );

	wxFlexGridSizer* FlexGridSizer21;
	FlexGridSizer21 = new wxFlexGridSizer( 3, 2, 0, 0 );
	FlexGridSizer21->SetFlexibleDirection( wxBOTH );
	FlexGridSizer21->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	StaticText32 = new wxStaticText( StaticBoxSizer11->GetStaticBox(), wxID_ANY, _("Color :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText32->Wrap( -1 );
	FlexGridSizer21->Add( StaticText32, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	bLinesPenColor = new wxButton( StaticBoxSizer11->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );

	bLinesPenColor->SetDefault();
	bLinesPenColor->SetAuthNeeded();
	FlexGridSizer21->Add( bLinesPenColor, 1, wxALL|wxEXPAND, 2 );

	StaticText33 = new wxStaticText( StaticBoxSizer11->GetStaticBox(), wxID_ANY, _("Width :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText33->Wrap( -1 );
	FlexGridSizer21->Add( StaticText33, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString cbLinesPenWidthChoices;
	cbLinesPenWidth = new wxChoice( StaticBoxSizer11->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, cbLinesPenWidthChoices, 0 );
	cbLinesPenWidth->SetSelection( 0 );
	FlexGridSizer21->Add( cbLinesPenWidth, 1, wxALL|wxEXPAND, 2 );

	StaticText34 = new wxStaticText( StaticBoxSizer11->GetStaticBox(), wxID_ANY, _("Style :"), wxDefaultPosition, wxDefaultSize, 0 );
	StaticText34->Wrap( -1 );
	FlexGridSizer21->Add( StaticText34, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString cbLinesPenStyleChoices;
	cbLinesPenStyle = new wxChoice( StaticBoxSizer11->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, cbLinesPenStyleChoices, 0 );
	cbLinesPenStyle->SetSelection( 0 );
	FlexGridSizer21->Add( cbLinesPenStyle, 1, wxALL|wxEXPAND, 2 );


	StaticBoxSizer11->Add( FlexGridSizer21, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0 );


	BoxSizer18->Add( StaticBoxSizer11, 0, wxALL, 2 );

	wxBoxSizer* BoxSizer19;
	BoxSizer19 = new wxBoxSizer( wxVERTICAL );

	cbLinesVisible = new wxCheckBox( pLines, wxID_ANY, _("Visible"), wxDefaultPosition, wxDefaultSize, 0 );
	cbLinesVisible->SetValue(true);
	BoxSizer19->Add( cbLinesVisible, 1, wxALL|wxALIGN_LEFT, 3 );

	cbLinesOutside = new wxCheckBox( pLines, wxID_ANY, _("Draw Outside Margins"), wxDefaultPosition, wxDefaultSize, 0 );
	cbLinesOutside->SetValue(true);
	BoxSizer19->Add( cbLinesOutside, 1, wxALL|wxALIGN_LEFT, 3 );

	cbLinesShowName = new wxCheckBox( pLines, wxID_ANY, _("Show name"), wxDefaultPosition, wxDefaultSize, 0 );
	cbLinesShowName->SetValue(true);
	BoxSizer19->Add( cbLinesShowName, 1, wxALL|wxALIGN_LEFT, 3 );


	BoxSizer18->Add( BoxSizer19, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	FlexGridSizer20->Add( BoxSizer18, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0 );


	BoxSizer4->Add( FlexGridSizer20, 0, wxALL|wxEXPAND, 5 );


	pLines->SetSizer( BoxSizer4 );
	pLines->Layout();
	BoxSizer4->Fit( pLines );
	sizerLines->Add( pLines, 1, wxEXPAND, 5 );


	Panel5->SetSizer( sizerLines );
	Panel5->Layout();
	sizerLines->Fit( Panel5 );
	nbConfig->AddPage( Panel5, _("Lines"), true );

	sizerMain->Add( nbConfig, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* BoxSizer2;
	BoxSizer2 = new wxBoxSizer( wxHORIZONTAL );

	bApply = new wxButton( this, wxID_ANY, _("Apply"), wxDefaultPosition, wxDefaultSize, 0 );

	bApply->SetDefault();
	bApply->SetAuthNeeded();
	BoxSizer2->Add( bApply, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4 );

	bClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );

	bClose->SetDefault();
	bClose->SetAuthNeeded();
	BoxSizer2->Add( bClose, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4 );


	sizerMain->Add( BoxSizer2, 0, wxEXPAND, 4 );


	this->SetSizer( sizerMain );
	this->Layout();
	sizerMain->Fit( this );

	// Connect Events
	nbConfig->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( MathPlotConfigDialogBuilder::OnnbConfigPageChanged ), NULL, this );
	bFontTitle->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbFontClick ), NULL, this );
	bBGColor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbColorClick ), NULL, this );
	bFontLegend->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbFontClick ), NULL, this );
	bLegendBrushColor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbColorClick ), NULL, this );
	ChoiceAxis->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnAxisSelect ), NULL, this );
	bAddXAxis->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbAddAxisClick ), NULL, this );
	bAddYAxis->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbAddAxisClick ), NULL, this );
	bDelAxis->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbDelAxisClick ), NULL, this );
	bFontAxis->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbFontClick ), NULL, this );
	bAxisPenColor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbColorClick ), NULL, this );
	cbAutoScale->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OncbAutoScaleClick ), NULL, this );
	cbFormat->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OncbFormatSelect ), NULL, this );
	ChoiceSeries->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnChoiceSeries ), NULL, this );
	bDelSeries->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbDelSeriesClick ), NULL, this );
	bSeriesPenColor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbColorClick ), NULL, this );
	bSeriesBrushColor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbColorClick ), NULL, this );
	ChoiceLines->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnChoiceLinesSelect ), NULL, this );
	bAddLines->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbAddLinesClick ), NULL, this );
	bDelLines->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbDelLinesClick ), NULL, this );
	bLinesPenColor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbColorClick ), NULL, this );
	bApply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbApplyClick ), NULL, this );
	bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnQuit ), NULL, this );
}

MathPlotConfigDialogBuilder::~MathPlotConfigDialogBuilder()
{
	// Disconnect Events
	nbConfig->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( MathPlotConfigDialogBuilder::OnnbConfigPageChanged ), NULL, this );
	bFontTitle->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbFontClick ), NULL, this );
	bBGColor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbColorClick ), NULL, this );
	bFontLegend->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbFontClick ), NULL, this );
	bLegendBrushColor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbColorClick ), NULL, this );
	ChoiceAxis->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnAxisSelect ), NULL, this );
	bAddXAxis->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbAddAxisClick ), NULL, this );
	bAddYAxis->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbAddAxisClick ), NULL, this );
	bDelAxis->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbDelAxisClick ), NULL, this );
	bFontAxis->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbFontClick ), NULL, this );
	bAxisPenColor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbColorClick ), NULL, this );
	cbAutoScale->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OncbAutoScaleClick ), NULL, this );
	cbFormat->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OncbFormatSelect ), NULL, this );
	ChoiceSeries->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnChoiceSeries ), NULL, this );
	bDelSeries->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbDelSeriesClick ), NULL, this );
	bSeriesPenColor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbColorClick ), NULL, this );
	bSeriesBrushColor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbColorClick ), NULL, this );
	ChoiceLines->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnChoiceLinesSelect ), NULL, this );
	bAddLines->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbAddLinesClick ), NULL, this );
	bDelLines->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbDelLinesClick ), NULL, this );
	bLinesPenColor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbColorClick ), NULL, this );
	bApply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnbApplyClick ), NULL, this );
	bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MathPlotConfigDialogBuilder::OnQuit ), NULL, this );

}
