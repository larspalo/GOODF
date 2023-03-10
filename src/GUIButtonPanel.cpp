/*
 * GUIButtonPanel.cpp is part of GOODF.
 * Copyright (C) 2023 Lars Palo
 *
 * GOODF is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GOODF is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GOODF.  If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo.se
 */

#include "GUIButtonPanel.h"
#include "GOODFFunctions.h"
#include "GOODFDef.h"
#include "GOODF.h"
#include <wx/statline.h>
#include <wx/stdpaths.h>
#include <algorithm>
#include <wx/msgdlg.h>
#include <wx/filename.h>

// Event table
BEGIN_EVENT_TABLE(GUIButtonPanel, wxPanel)
	EVT_TEXT(ID_GUIBUTTONPANEL_LABEL_TEXT, GUIButtonPanel::OnLabelTextChange)
	EVT_FONTPICKER_CHANGED(ID_GUIBUTTONPANEL_FONT_PICKER, GUIButtonPanel::OnLabelFontChange)
	EVT_CHOICE(ID_GUIBUTTONPANEL_COLOR_CHOICE, GUIButtonPanel::OnLabelColourChoice)
	EVT_COLOURPICKER_CHANGED(ID_GUIBUTTONPANEL_COLOR_PICKER, GUIButtonPanel::OnLabelColourPick)
	EVT_RADIOBUTTON(ID_GUIBUTTONPANEL_DISP_AS_PISTON_YES, GUIButtonPanel::OnDisplayAsPistonRadio)
	EVT_RADIOBUTTON(ID_GUIBUTTONPANEL_DISP_AS_PISTON_NO, GUIButtonPanel::OnDisplayAsPistonRadio)
	EVT_RADIOBUTTON(ID_GUIBUTTONPANEL_DISP_LABEL_LEFT_YES, GUIButtonPanel::OnDisplayKeyLabelLeftRadio)
	EVT_RADIOBUTTON(ID_GUIBUTTONPANEL_DISP_LABEL_LEFT_NO, GUIButtonPanel::OnDisplayKeyLabelLeftRadio)
	EVT_COMBOBOX(ID_GUIBUTTONPANEL_IMAGE_NBR_BOX, GUIButtonPanel::OnImageNumberChoice)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_BUTTONROW_SPIN, GUIButtonPanel::OnButtonRowSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_BUTTONCOL_SPIN, GUIButtonPanel::OnButtonColSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_DRAWSTOPROW_SPIN, GUIButtonPanel::OnDrawstopRowSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_DRAWSTOPCOL_SPIN, GUIButtonPanel::OnDrawstopColSpin)
	EVT_BUTTON(ID_GUIBUTTONPANEL_IMAGE_ON_BTN, GUIButtonPanel::OnAddImageOnBtn)
	EVT_BUTTON(ID_GUIBUTTONPANEL_IMAGE_OFF_BTN, GUIButtonPanel::OnAddImageOffBtn)
	EVT_BUTTON(ID_GUIBUTTONPANEL_MASK_ON_BTN, GUIButtonPanel::OnAddMaskOnBtn)
	EVT_BUTTON(ID_GUIBUTTONPANEL_MASK_OFF_BTN, GUIButtonPanel::OnAddMaskOffBtn)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_WIDTH_SPIN, GUIButtonPanel::OnWidthSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_HEIGHT_SPIN, GUIButtonPanel::OnHeightSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_TILE_X_SPIN, GUIButtonPanel::OnTileOffsetXSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_TILE_Y_SPIN, GUIButtonPanel::OnTileOffsetYSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_MOUSE_RECT_LEFT_SPIN, GUIButtonPanel::OnMouseRectLeftSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_MOUSE_RECT_TOP_SPIN, GUIButtonPanel::OnMouseRectTopSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_MOUSE_RECT_WIDTH_SPIN, GUIButtonPanel::OnMouseRectWidthSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_MOUSE_RECT_HEIGHT_SPIN, GUIButtonPanel::OnMouseRectHeightSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_MOUSE_RADIUS_SPIN, GUIButtonPanel::OnMouseRadiusSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_TEXT_RECT_LEFT_SPIN, GUIButtonPanel::OnTextRectLeftSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_TEXT_RECT_TOP_SPIN, GUIButtonPanel::OnTextRectTopSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_TEXT_RECT_WIDTH_SPIN, GUIButtonPanel::OnTextRectWidthSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_TEXT_RECT_HEIGHT_SPIN, GUIButtonPanel::OnTextRectHeightSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_TEXT_BREAK_SPIN, GUIButtonPanel::OnTextBreakWidthSpin)
	EVT_SPINCTRL(ID_GUIELEMENT_POS_X_SPIN, GUIButtonPanel::OnPositionXSpin)
	EVT_SPINCTRL(ID_GUIELEMENT_POS_Y_SPIN, GUIButtonPanel::OnPositionYSpin)
	EVT_BUTTON(ID_GUIBUTTONPANEL_REMOVE_BTN, GUIButtonPanel::OnRemoveButtonBtn)
END_EVENT_TABLE()

GUIButtonPanel::GUIButtonPanel(wxWindow *parent) : wxPanel(parent) {
	m_button = NULL;
	GoColor col;
	m_colors = col.getColorNames();
	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *labelText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispLabelText: ")
	);
	firstRow->Add(labelText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_labelTextField = new wxTextCtrl(
		this,
		ID_GUIBUTTONPANEL_LABEL_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_labelTextField, 1, wxEXPAND|wxALL, 5);
	wxStaticText *labelFontText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Label font: ")
	);
	firstRow->Add(labelFontText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_labelFont = new wxFontPickerCtrl(
		this,
		ID_GUIBUTTONPANEL_FONT_PICKER
	);
	firstRow->Add(m_labelFont, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *colorRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *labelColourText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("DispLabelColour: ")
	);
	colorRow->Add(labelColourText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_labelColourChoice = new wxChoice(
		this,
		ID_GUIBUTTONPANEL_COLOR_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		m_colors
	);
	colorRow->Add(m_labelColourChoice, 1, wxEXPAND, 0);
	m_labelColourPick = new wxColourPickerCtrl(
		this,
		ID_GUIBUTTONPANEL_COLOR_PICKER
	);
	colorRow->Add(m_labelColourPick, 1, wxEXPAND, 0);
	panelSizer->Add(colorRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *pistonText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Display as piston: ")
	);
	secondRow->Add(pistonText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayAsPistonYes = new wxRadioButton(
		this,
		ID_GUIBUTTONPANEL_DISP_AS_PISTON_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_displayAsPistonYes->SetValue(false);
	secondRow->Add(m_displayAsPistonYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayAsPistonNo = new wxRadioButton(
		this,
		ID_GUIBUTTONPANEL_DISP_AS_PISTON_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_displayAsPistonNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	secondRow->AddStretchSpacer();
	wxStaticText *keyLabelLeftText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispKeyLabelOnLeft: ")
	);
	secondRow->Add(keyLabelLeftText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayKeyLabelLeftYes = new wxRadioButton(
		this,
		ID_GUIBUTTONPANEL_DISP_LABEL_LEFT_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_displayKeyLabelLeftYes->SetValue(true);
	secondRow->Add(m_displayKeyLabelLeftYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayKeyLabelLeftNo = new wxRadioButton(
		this,
		ID_GUIBUTTONPANEL_DISP_LABEL_LEFT_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_displayKeyLabelLeftNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *positionRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *posXText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("PositionX: ")
	);
	positionRow->Add(posXText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_elementPosXSpin = new wxSpinCtrl(
		this,
		ID_GUIELEMENT_POS_X_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1,
		1,
		-1
	);
	positionRow->Add(m_elementPosXSpin, 0, wxEXPAND|wxALL, 5);
	positionRow->AddStretchSpacer();
	wxStaticText *posYText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("PositionY: ")
	);
	positionRow->Add(posYText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_elementPosYSpin = new wxSpinCtrl(
		this,
		ID_GUIELEMENT_POS_Y_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1,
		1,
		-1
	);
	positionRow->Add(m_elementPosYSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(positionRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *imageNbrText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("DispImageNum: ")
	);
	thirdRow->Add(imageNbrText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_dispImageNbrBox = new wxBitmapComboBox(
		this,
		ID_GUIBUTTONPANEL_IMAGE_NBR_BOX
	);
	for (unsigned i = 0; i < ::wxGetApp().m_drawstopBitmaps.size(); i++) {
		wxString imgNumber = wxString::Format(wxT("%d"), i + 1);
		m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_drawstopBitmaps[i]);
	}
	thirdRow->Add(m_dispImageNbrBox, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *btnRowText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispButtonRow: ")
	);
	thirdRow->Add(btnRowText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_buttonRowSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_BUTTONROW_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1,
		1
	);
	thirdRow->Add(m_buttonRowSpin, 0, wxEXPAND|wxALL, 5);
	wxStaticText *btnColText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispButtonCol: ")
	);
	thirdRow->Add(btnColText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_buttonColSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_BUTTONCOL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1,
		1
	);
	thirdRow->Add(m_buttonColSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *stopRowText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispDrawstopRow: ")
	);
	fourthRow->Add(stopRowText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopRowSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_DRAWSTOPROW_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1,
		1
	);
	fourthRow->Add(m_drawstopRowSpin, 0, wxEXPAND|wxALL, 5);
	fourthRow->AddStretchSpacer();
	wxStaticText *stopColText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispDrawstopCol: ")
	);
	fourthRow->Add(stopColText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopColSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_DRAWSTOPCOL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1,
		1
	);
	fourthRow->Add(m_drawstopColSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(fourthRow, 0, wxEXPAND);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *imageOnText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("ImageOn: ")
	);
	fifthRow->Add(imageOnText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_imageOnPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	fifthRow->Add(m_imageOnPathField, 1, wxEXPAND|wxALL, 5);
	m_addImageOnBtn = new wxButton(
		this,
		ID_GUIBUTTONPANEL_IMAGE_ON_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	fifthRow->Add(m_addImageOnBtn, 0, wxALL, 5);
	wxStaticText *imageOffText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("ImageOff: ")
	);
	fifthRow->Add(imageOffText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_imageOffPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	fifthRow->Add(m_imageOffPathField, 1, wxEXPAND|wxALL, 5);
	m_addImageOffBtn = new wxButton(
		this,
		ID_GUIBUTTONPANEL_IMAGE_OFF_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	fifthRow->Add(m_addImageOffBtn, 0, wxALL, 5);
	panelSizer->Add(fifthRow, 0, wxEXPAND);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *maskOnText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MaskOn: ")
	);
	sixthRow->Add(maskOnText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maskOnPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	sixthRow->Add(m_maskOnPathField, 1, wxEXPAND|wxALL, 5);
	m_addMaskOnBtn = new wxButton(
		this,
		ID_GUIBUTTONPANEL_MASK_ON_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	sixthRow->Add(m_addMaskOnBtn, 0, wxALL, 5);
	wxStaticText *maskOffText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MaskOff: ")
	);
	sixthRow->Add(maskOffText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maskOffPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	sixthRow->Add(m_maskOffPathField, 1, wxEXPAND|wxALL, 5);
	m_addMaskOffBtn = new wxButton(
		this,
		ID_GUIBUTTONPANEL_MASK_OFF_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	sixthRow->Add(m_addMaskOffBtn, 0, wxALL, 5);
	panelSizer->Add(sixthRow, 0, wxEXPAND);

	wxBoxSizer *seventhRow  = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *widthText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Width: ")
	);
	seventhRow->Add(widthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_widthSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_WIDTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	seventhRow->Add(m_widthSpin, 0, wxEXPAND|wxALL, 5);
	seventhRow->AddStretchSpacer();
	wxStaticText *heightText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Height: ")
	);
	seventhRow->Add(heightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_heightSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_HEIGHT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	seventhRow->Add(m_heightSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(seventhRow, 0, wxEXPAND);

	wxBoxSizer *eighthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *tileOffsetXText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TileOffsetX: ")
	);
	eighthRow->Add(tileOffsetXText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_tileOffsetXSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_TILE_X_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	eighthRow->Add(m_tileOffsetXSpin, 0, wxEXPAND|wxALL, 5);
	eighthRow->AddStretchSpacer();
	wxStaticText *tileOffsetYText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TileOffsetY: ")
	);
	eighthRow->Add(tileOffsetYText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_tileOffsetYSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_TILE_Y_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	eighthRow->Add(m_tileOffsetYSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(eighthRow, 0, wxEXPAND);

	wxBoxSizer *ninthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *mouseRectLeftText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRectLeft: ")
	);
	ninthRow->Add(mouseRectLeftText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_mouseRectLeftSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_MOUSE_RECT_LEFT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	ninthRow->Add(m_mouseRectLeftSpin, 0, wxEXPAND|wxALL, 5);
	ninthRow->AddStretchSpacer();
	wxStaticText *mouseRectTopText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRectTop: ")
	);
	ninthRow->Add(mouseRectTopText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_mouseRectTopSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_MOUSE_RECT_TOP_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	ninthRow->Add(m_mouseRectTopSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(ninthRow, 0, wxEXPAND);

	wxBoxSizer *tenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *mouseRectWidthText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRectWidth: ")
	);
	tenthRow->Add(mouseRectWidthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_mouseRectWidthSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_MOUSE_RECT_WIDTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	tenthRow->Add(m_mouseRectWidthSpin, 0, wxEXPAND|wxALL, 5);
	tenthRow->AddStretchSpacer();
	wxStaticText *mouseRectHeightText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRectHeight: ")
	);
	tenthRow->Add(mouseRectHeightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_mouseRectHeightSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_MOUSE_RECT_HEIGHT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	tenthRow->Add(m_mouseRectHeightSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(tenthRow, 0, wxEXPAND);

	wxBoxSizer *eleventh = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *mouseRadiusText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRadius: ")
	);
	eleventh->Add(mouseRadiusText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_mouseRadiusSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_MOUSE_RADIUS_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	eleventh->Add(m_mouseRadiusSpin, 0, wxEXPAND|wxALL, 5);
	eleventh->AddStretchSpacer();
	wxStaticText *textBreakText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TextBreakWidth: ")
	);
	eleventh->Add(textBreakText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_textBreakWidthSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_TEXT_BREAK_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	eleventh->Add(m_textBreakWidthSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(eleventh, 0, wxEXPAND);

	wxBoxSizer *twelfthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textRectLeftText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TextRectLeft: ")
	);
	twelfthRow->Add(textRectLeftText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_textRectLeftSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_TEXT_RECT_LEFT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	twelfthRow->Add(m_textRectLeftSpin, 0, wxEXPAND|wxALL, 5);
	twelfthRow->AddStretchSpacer();
	wxStaticText *textRectTopText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TextRectTop: ")
	);
	twelfthRow->Add(textRectTopText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_textRectTopSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_TEXT_RECT_TOP_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	twelfthRow->Add(m_textRectTopSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(twelfthRow, 0, wxEXPAND);

	wxBoxSizer *thirteenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textRectWidthText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TextRectWidth: ")
	);
	thirteenthRow->Add(textRectWidthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_textRectWidthSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_TEXT_RECT_WIDTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	thirteenthRow->Add(m_textRectWidthSpin, 0, wxEXPAND|wxALL, 5);
	thirteenthRow->AddStretchSpacer();
	wxStaticText *textRectHeightText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TextRectHeight: ")
	);
	thirteenthRow->Add(textRectHeightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_textRectHeightSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_TEXT_RECT_HEIGHT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	thirteenthRow->Add(m_textRectHeightSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(thirteenthRow, 0, wxEXPAND);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	m_removeButtonBtn = new wxButton(
		this,
		ID_GUIBUTTONPANEL_REMOVE_BTN,
		wxT("Remove this element")
	);
	bottomRow->Add(m_removeButtonBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

GUIButtonPanel::~GUIButtonPanel() {

}

void GUIButtonPanel::setButton(GUIButton *button) {
	m_button = button;
	m_labelTextField->SetValue(m_button->getDispLabelText());
	m_labelFont->SetSelectedFont(m_button->getDispLabelFont());
	if (m_button->getDispLabelColour()->getSelectedColorIndex() == 0) {
		// it's a custom color
		m_labelColourChoice->SetSelection(0);
		m_labelColourPick->Enable();
		m_labelColourPick->SetColour(m_button->getDispLabelColour()->getColor());
	} else {
		// the color is text specified from the available ones in GO
		m_labelColourChoice->SetSelection(m_button->getDispLabelColour()->getSelectedColorIndex());
		m_labelColourPick->SetColour(m_button->getDispLabelColour()->getColor());
		m_labelColourPick->Disable();
	}
	if (m_button->isDisplayAsPiston()) {
		m_displayAsPistonYes->SetValue(true);
		m_displayAsPistonNo->SetValue(false);
		m_buttonRowSpin->Enable();
		m_buttonColSpin->Enable();
		m_drawstopRowSpin->Disable();
		m_drawstopColSpin->Disable();
	} else {
		m_displayAsPistonYes->SetValue(false);
		m_displayAsPistonNo->SetValue(true);
		m_buttonRowSpin->Disable();
		m_buttonColSpin->Disable();
		m_drawstopRowSpin->Enable();
		m_drawstopColSpin->Enable();
	}
	SetupImageNbrBoxContent();
	if (m_button->isDispKeyLabelOnLeft()) {
		m_displayKeyLabelLeftYes->SetValue(true);
		m_displayKeyLabelLeftNo->SetValue(false);
	} else {
		m_displayKeyLabelLeftYes->SetValue(false);
		m_displayKeyLabelLeftNo->SetValue(true);
	}
	m_dispImageNbrBox->SetSelection(m_button->getDispImageNum() - 1);
	m_buttonRowSpin->SetValue(m_button->getDispButtonRow());
	m_buttonColSpin->SetValue(m_button->getDispButtonCol());
	m_drawstopRowSpin->SetValue(m_button->getDispDrawstopRow());
	m_drawstopColSpin->SetValue(m_button->getDispDrawstopCol());
	if (m_button->getImageOn() != wxEmptyString) {
		wxString relativeImageOn = GOODF_functions::removeBaseOdfPath(m_button->getImageOn());
		m_imageOnPathField->SetValue(relativeImageOn);
		m_addImageOffBtn->Enable();
	} else {
		m_imageOnPathField->SetValue(wxEmptyString);
		m_addImageOffBtn->Disable();
		m_addMaskOnBtn->Disable();
		m_addMaskOffBtn->Disable();
		UpdateBuiltinBitmapValues();
	}
	if (m_button->getImageOff() != wxEmptyString) {
		wxString relativeImageOff = GOODF_functions::removeBaseOdfPath(m_button->getImageOff());
		m_imageOffPathField->SetValue(relativeImageOff);
	} else {
		m_imageOffPathField->SetValue(wxEmptyString);
		m_addMaskOnBtn->Disable();
		m_addMaskOffBtn->Disable();
	}
	if (m_button->getMaskOn() != wxEmptyString) {
		wxString relativeMaskOn = GOODF_functions::removeBaseOdfPath(m_button->getMaskOn());
		m_maskOnPathField->SetValue(relativeMaskOn);
	} else {
		m_maskOnPathField->SetValue(wxEmptyString);
		m_addMaskOffBtn->Disable();
	}
	if (m_button->getMaskOff() != wxEmptyString) {
		wxString relativeMaskOff = GOODF_functions::removeBaseOdfPath(m_button->getMaskOff());
		m_maskOffPathField->SetValue(relativeMaskOff);
	} else {
		m_maskOffPathField->SetValue(wxEmptyString);
	}
	UpdateSpinRanges();
	UpdateDefaultSpinValues();
}

void GUIButtonPanel::OnLabelTextChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_labelTextField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_labelTextField);
	m_button->setDispLabelText(m_labelTextField->GetValue());
}

void GUIButtonPanel::OnLabelFontChange(wxFontPickerEvent& WXUNUSED(event)) {
	m_button->setDispLabelFont(m_labelFont->GetSelectedFont());
	m_button->setDispLabelFontSize(m_labelFont->GetFont().GetPointSize());
}

void GUIButtonPanel::OnLabelColourChoice(wxCommandEvent& event) {
	if (event.GetId() == ID_GUIBUTTONPANEL_COLOR_CHOICE) {
		if (m_labelColourChoice->GetSelection() == 0) {
			m_labelColourPick->Enable();
		} else {
			m_button->getDispLabelColour()->setSelectedColorIndex(m_labelColourChoice->GetSelection());
			m_labelColourPick->SetColour(m_button->getDispLabelColour()->getColor());
			m_labelColourPick->Disable();
		}
	}
}

void GUIButtonPanel::OnLabelColourPick(wxColourPickerEvent& event) {
	if (event.GetId() == ID_GUIBUTTONPANEL_COLOR_PICKER) {
		m_button->getDispLabelColour()->setColorValue(m_labelColourPick->GetColour());
	}
}

void GUIButtonPanel::OnDisplayAsPistonRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_GUIBUTTONPANEL_DISP_AS_PISTON_YES) {
		m_displayAsPistonYes->SetValue(true);
		m_button->setDisplayAsPiston(true);
		if (m_button->getPosX() == -1 && m_button->getPosY() == -1) {
			m_buttonRowSpin->Enable();
			m_buttonColSpin->Enable();
			m_drawstopRowSpin->Disable();
			m_drawstopColSpin->Disable();
		}
	} else {
		m_displayAsPistonNo->SetValue(true);
		m_button->setDisplayAsPiston(false);
		if (m_button->getPosX() == -1 && m_button->getPosY() == -1) {
			m_buttonRowSpin->Disable();
			m_buttonColSpin->Disable();
			m_drawstopRowSpin->Enable();
			m_drawstopColSpin->Enable();
		}
	}
	SetupImageNbrBoxContent();
	m_dispImageNbrBox->SetSelection(m_button->getDispImageNum() - 1);
	if (m_button->getImageOn() == wxEmptyString) {
		UpdateBuiltinBitmapValues();
		UpdateSpinRanges();
		UpdateDefaultSpinValues();
	}
}

void GUIButtonPanel::OnDisplayKeyLabelLeftRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_GUIBUTTONPANEL_DISP_LABEL_LEFT_YES) {
		m_displayKeyLabelLeftYes->SetValue(true);
		m_button->setDispKeyLabelOnLeft(true);
	} else  {
		m_displayKeyLabelLeftNo->SetValue(true);
		m_button->setDispKeyLabelOnLeft(false);
	}
}

void GUIButtonPanel::OnImageNumberChoice(wxCommandEvent& WXUNUSED(event)) {
	m_button->setDispImageNum(m_dispImageNbrBox->GetSelection() + 1);
}

void GUIButtonPanel::OnButtonRowSpin(wxSpinEvent& WXUNUSED(event)) {
	// TODO: Logic checking if jump over impossible values
	m_button->setDispButtonRow(m_buttonRowSpin->GetValue());
}

void GUIButtonPanel::OnButtonColSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setDispButtonCol(m_buttonColSpin->GetValue());
}

void GUIButtonPanel::OnDrawstopRowSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setDispDrawstopRow(m_drawstopRowSpin->GetValue());
}

void GUIButtonPanel::OnDrawstopColSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setDispDrawstopCol(m_drawstopColSpin->GetValue());
}

void GUIButtonPanel::OnAddImageOnBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			m_button->setImageOn(path);
			int width = img.GetWidth();
			int height = img.GetHeight();
			m_button->setBitmapWidth(width);
			m_button->setBitmapHeight(height);
			m_button->setWidth(width);
			m_button->setHeight(height);
			m_button->setMouseRectWidth(width);
			m_button->setMouseRectHeight(height);
			m_button->setMouseRadius(std::min(width, height) / 2);
			m_button->setTextRectWidth(width);
			m_button->setTextRectHeight(height);
			m_button->setTextBreakWidth(width);
			UpdateSpinRanges();
			UpdateDefaultSpinValues();
			wxString relativePath = GOODF_functions::removeBaseOdfPath(m_button->getImageOn());
			m_imageOnPathField->SetValue(relativePath);
			m_addImageOffBtn->Enable();
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_button->getImageOn() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Image on value is not empty! Do you want to empty it?"), wxT("Empty image on value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				m_button->setImageOn(wxEmptyString);
				UpdateBuiltinBitmapValues();
				UpdateSpinRanges();
				UpdateDefaultSpinValues();
				m_imageOnPathField->SetValue(wxEmptyString);
				m_addImageOffBtn->Disable();
				m_addMaskOnBtn->Disable();
				m_addMaskOffBtn->Disable();
			}
		} else {
			m_addImageOffBtn->Disable();
			m_addMaskOnBtn->Disable();
			m_addMaskOffBtn->Disable();
		}
	}
}

void GUIButtonPanel::OnAddImageOffBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			int width = img.GetWidth();
			int height = img.GetHeight();
			if (width == m_button->getBitmapWidth() && height == m_button->getBitmapHeight()) {
				m_button->setImageOff(path);
				wxString relativePath = GOODF_functions::removeBaseOdfPath(m_button->getImageOff());
				m_imageOffPathField->SetValue(relativePath);
				m_addMaskOnBtn->Enable();
			}
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_button->getImageOff() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Image off value is not empty! Do you want to empty it?"), wxT("Empty image off value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				m_button->setImageOff(wxEmptyString);
				m_imageOffPathField->SetValue(wxEmptyString);
				m_addMaskOnBtn->Disable();
				m_addMaskOffBtn->Disable();
			}
		} else {
			m_addMaskOnBtn->Disable();
			m_addMaskOffBtn->Disable();
		}
	}
}

void GUIButtonPanel::OnAddMaskOnBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			int width = img.GetWidth();
			int height = img.GetHeight();
			if (width == m_button->getBitmapWidth() && height == m_button->getBitmapHeight()) {
				m_button->setMaskOn(path);
				wxString relativePath = GOODF_functions::removeBaseOdfPath(m_button->getMaskOn());
				m_maskOnPathField->SetValue(relativePath);
				m_addMaskOffBtn->Enable();
			}
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_button->getMaskOn() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Mask on value is not empty! Do you want to empty it?"), wxT("Empty mask on value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				m_button->setMaskOn(wxEmptyString);
				m_maskOnPathField->SetValue(wxEmptyString);
				m_addMaskOffBtn->Disable();
			}
		} else {
			m_addMaskOffBtn->Disable();
		}
	}
}

void GUIButtonPanel::OnAddMaskOffBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			int width = img.GetWidth();
			int height = img.GetHeight();
			if (width == m_button->getBitmapWidth() && height == m_button->getBitmapHeight()) {
				m_button->setMaskOff(path);
				wxString relativePath = GOODF_functions::removeBaseOdfPath(m_button->getMaskOff());
				m_maskOffPathField->SetValue(relativePath);
			}
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_button->getMaskOff() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Mask off value is not empty! Do you want to empty it?"), wxT("Empty mask off value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				m_button->setMaskOff(wxEmptyString);
				m_maskOffPathField->SetValue(wxEmptyString);
			}
		}
	}
}

void GUIButtonPanel::OnWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setWidth(m_widthSpin->GetValue());
}

void GUIButtonPanel::OnHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setHeight(m_heightSpin->GetValue());
}

void GUIButtonPanel::OnTileOffsetXSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setTileOffsetX(m_tileOffsetXSpin->GetValue());
}

void GUIButtonPanel::OnTileOffsetYSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setTileOffsetY(m_tileOffsetYSpin->GetValue());
}

void GUIButtonPanel::OnMouseRectLeftSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setMouseRectLeft(m_mouseRectLeftSpin->GetValue());
}

void GUIButtonPanel::OnMouseRectTopSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setMouseRectTop(m_mouseRectTopSpin->GetValue());
}

void GUIButtonPanel::OnMouseRectWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setMouseRectWidth(m_mouseRectWidthSpin->GetValue());
}

void GUIButtonPanel::OnMouseRectHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setMouseRectHeight(m_mouseRectHeightSpin->GetValue());
}

void GUIButtonPanel::OnMouseRadiusSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setMouseRadius(m_mouseRadiusSpin->GetValue());
}

void GUIButtonPanel::OnTextRectLeftSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setTextRectLeft(m_textRectLeftSpin->GetValue());
}

void GUIButtonPanel::OnTextRectTopSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setTextRectTop(m_textRectTopSpin->GetValue());
}

void GUIButtonPanel::OnTextRectWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setTextRectWidth(m_textRectWidthSpin->GetValue());
}

void GUIButtonPanel::OnTextRectHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setTextRectHeight(m_textRectHeightSpin->GetValue());
}

void GUIButtonPanel::OnTextBreakWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setTextBreakWidth(m_textBreakWidthSpin->GetValue());
}

void GUIButtonPanel::OnPositionXSpin(wxSpinEvent& WXUNUSED(event)) {
	// a value of -1 indicate that button or drawstop row/col values are used
	// if it's not -1 then all they should be reset and disabled
	int value = m_elementPosXSpin->GetValue();
	m_button->setPosX(value);
	if (value != -1) {
		m_button->setDispButtonRow(1);
		m_buttonRowSpin->SetValue(1);
		m_button->setDispButtonCol(1);
		m_buttonColSpin->SetValue(1);
		m_button->setDispDrawstopRow(1);
		m_drawstopRowSpin->SetValue(1);
		m_button->setDispDrawstopCol(1);
		m_drawstopColSpin->SetValue(1);
		m_buttonRowSpin->Disable();
		m_buttonColSpin->Disable();
		m_drawstopRowSpin->Disable();
		m_drawstopColSpin->Disable();
	} else {
		if (m_button->isDisplayAsPiston()) {
			m_buttonRowSpin->Enable();
			m_buttonColSpin->Enable();
			m_drawstopRowSpin->Disable();
			m_drawstopColSpin->Disable();
		} else {
			m_buttonRowSpin->Disable();
			m_buttonColSpin->Disable();
			m_drawstopRowSpin->Enable();
			m_drawstopColSpin->Enable();
		}
	}
}

void GUIButtonPanel::OnPositionYSpin(wxSpinEvent& WXUNUSED(event)) {
	// a value of -1 indicate that button or drawstop row/col values are used
	// if it's not -1 then all they should be reset and disabled
	int value = m_elementPosYSpin->GetValue();
	m_button->setPosY(value);
	if (value != -1) {
		m_button->setDispButtonRow(1);
		m_buttonRowSpin->SetValue(1);
		m_button->setDispButtonCol(1);
		m_buttonColSpin->SetValue(1);
		m_button->setDispDrawstopRow(1);
		m_drawstopRowSpin->SetValue(1);
		m_button->setDispDrawstopCol(1);
		m_drawstopColSpin->SetValue(1);
		m_buttonRowSpin->Disable();
		m_buttonColSpin->Disable();
		m_drawstopRowSpin->Disable();
		m_drawstopColSpin->Disable();
	} else {
		if (m_button->isDisplayAsPiston()) {
			m_buttonRowSpin->Enable();
			m_buttonColSpin->Enable();
			m_drawstopRowSpin->Disable();
			m_drawstopColSpin->Disable();
		} else {
			m_buttonRowSpin->Disable();
			m_buttonColSpin->Disable();
			m_drawstopRowSpin->Enable();
			m_drawstopColSpin->Enable();
		}
	}
}

void GUIButtonPanel::OnRemoveButtonBtn(wxCommandEvent& WXUNUSED(event)) {
	wxMessageDialog msg(this, wxT("Are you really sure you want to delete this element?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
	if (msg.ShowModal() == wxID_YES) {
		// then remove this item from the owning panel
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
}

void GUIButtonPanel::SetupImageNbrBoxContent() {
	if (m_displayAsPistonYes->GetValue()) {
		// if displayed as piston the image num box should be filled with pistons
		m_dispImageNbrBox->Clear();
		for (unsigned i = 0; i < ::wxGetApp().m_buttonBitmaps.size(); i++) {
			wxString imgNumber = wxString::Format(wxT("%d"), i + 1);
			m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_buttonBitmaps[i]);
		}
	} else {
		// if not displayed as piston the image num box should be filled with drawstops
		m_dispImageNbrBox->Clear();
		for (unsigned i = 0; i < ::wxGetApp().m_drawstopBitmaps.size(); i++) {
			wxString imgNumber = wxString::Format(wxT("%d"), i + 1);
			m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_drawstopBitmaps[i]);
		}
	}
}

void GUIButtonPanel::UpdateSpinRanges() {
	m_elementPosXSpin->SetRange(-1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - m_button->getWidth());
	m_elementPosYSpin->SetRange(-1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue() - m_button->getHeight());
	m_widthSpin->SetRange(0, m_button->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue()); // panel width!
	m_heightSpin->SetRange(0, m_button->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue()); // panel height!
	m_tileOffsetXSpin->SetRange(0, m_button->getBitmapWidth());
	m_tileOffsetYSpin->SetRange(0, m_button->getBitmapHeight());
	m_mouseRectLeftSpin->SetRange(0, m_button->getWidth());
	m_mouseRectTopSpin->SetRange(0, m_button->getHeight());
	m_mouseRectWidthSpin->SetRange(0, m_button->getWidth());
	m_mouseRectHeightSpin->SetRange(0, m_button->getHeight());
	m_mouseRadiusSpin->SetRange(0, (std::max(m_button->getMouseRectWidth(), m_button->getMouseRectHeight()) / 2));
	m_textRectLeftSpin->SetRange(0, m_button->getWidth());
	m_textRectTopSpin->SetRange(0, m_button->getHeight());
	m_textRectWidthSpin->SetRange(0, m_button->getWidth());
	m_textRectHeightSpin->SetRange(0, m_button->getHeight());
	m_textBreakWidthSpin->SetRange(0, m_button->getTextRectWidth());

	// TODO: The ranges should be checked so that extra button and drawstop rows/cols be accounted for some start at 100!
	m_buttonRowSpin->SetRange(0, 199);
	m_buttonColSpin->SetRange(1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispButtonCols);
	m_drawstopRowSpin->SetRange(1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispDrawstopRows);
	m_drawstopColSpin->SetRange(1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispDrawstopCols);
}

void GUIButtonPanel::UpdateDefaultSpinValues() {
	m_elementPosXSpin->SetValue(m_button->getPosX());
	m_elementPosYSpin->SetValue(m_button->getPosY());
	m_widthSpin->SetValue(m_button->getWidth());
	m_heightSpin->SetValue(m_button->getHeight());
	m_tileOffsetXSpin->SetValue(m_button->getTileOffsetX());
	m_tileOffsetYSpin->SetValue(m_button->getTileOffsetY());
	m_mouseRectLeftSpin->SetValue(m_button->getMouseRectLeft());
	m_mouseRectTopSpin->SetValue(m_button->getMouseRectTop());
	m_mouseRectWidthSpin->SetValue(m_button->getMouseRectWidth());
	m_mouseRectHeightSpin->SetValue(m_button->getMouseRectHeight());
	m_mouseRadiusSpin->SetValue(m_button->getMouseRadius());
	m_textRectLeftSpin->SetValue(m_button->getTextRectLeft());
	m_textRectTopSpin->SetValue(m_button->getTextRectTop());
	m_textRectWidthSpin->SetValue(m_button->getTextRectWidth());
	m_textRectHeightSpin->SetValue(m_button->getTextRectHeight());
	m_textBreakWidthSpin->SetValue(m_button->getTextBreakWidth());
}

wxString GUIButtonPanel::GetPathForImageFile() {
	wxString imageFilePath;
	wxString defaultPath = wxEmptyString;
	if (m_button->getImageOn() != wxEmptyString) {
		wxFileName filePath = wxFileName(m_button->getImageOn());
		if (filePath.FileExists())
			defaultPath = filePath.GetPath();
	} else {
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();
	}
	if (defaultPath == wxEmptyString)
		defaultPath = wxStandardPaths::Get().GetDocumentsDir();

	wxFileDialog fileDialog(
		this,
		wxT("Select image file (all image pixel sizes for this element must match)"),
		defaultPath,
		"",
		"PNG files (*.png)|*.png|BMP and GIF files (*.bmp;*.gif)|*.bmp;*.gif|JPG files (*.jpg)|*.jpg|ICO files (*.ico)|*.ico",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() == wxID_CANCEL)
		return wxEmptyString;

	imageFilePath = fileDialog.GetPath();
	return imageFilePath;
}

void GUIButtonPanel::UpdateBuiltinBitmapValues() {
	int width, height;
	if (m_button->isDisplayAsPiston()) {
		width = 32;
		height = 32;
	} else {
		width = 65;
		height = 65;
	}
	m_button->setBitmapWidth(width);
	m_button->setBitmapHeight(height);
	m_button->setWidth(width);
	m_button->setHeight(height);
	m_button->setMouseRectWidth(width);
	m_button->setMouseRectHeight(height);
	m_button->setMouseRadius(width / 2);
	m_button->setTextRectWidth(width);
	m_button->setTextRectHeight(height);
	m_button->setTextBreakWidth(width);
}
