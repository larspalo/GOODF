/*
 * SwitchPanel.cpp is part of GOODF.
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

#include "SwitchPanel.h"
#include "GOODFFunctions.h"
#include "GOODFDef.h"
#include "GOODF.h"
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/msgdlg.h>

// Event table
BEGIN_EVENT_TABLE(SwitchPanel, wxPanel)
	EVT_TEXT(ID_SWITCH_NAME_TEXT, SwitchPanel::OnNameChange)
	EVT_CHOICE(ID_SWITCH_FUNCTION_CHOICE, SwitchPanel::OnFunctionChange)
	EVT_RADIOBUTTON(ID_SWITCH_INVERTED_STATE_YES, SwitchPanel::OnDisplayInvertedRadio)
	EVT_RADIOBUTTON(ID_SWITCH_INVERTED_STATE_NO, SwitchPanel::OnDisplayInvertedRadio)
	EVT_CHOICE(ID_SWITCH_GC_STATE_CHOICE, SwitchPanel::OnGcStateChange)
	EVT_RADIOBUTTON(ID_SWITCH_STORE_IN_DIV_YES, SwitchPanel::OnStoreInDivisionalChange)
	EVT_RADIOBUTTON(ID_SWITCH_STORE_IN_DIV_NO, SwitchPanel::OnStoreInDivisionalChange)
	EVT_RADIOBUTTON(ID_SWITCH_STORE_IN_GEN_YES, SwitchPanel::OnStoreInGeneralChange)
	EVT_RADIOBUTTON(ID_SWITCH_STORE_IN_GEN_NO, SwitchPanel::OnStoreInGeneralChange)
	EVT_RADIOBUTTON(ID_SWITCH_DEFAULT_TO_ENGAGED_YES, SwitchPanel::OnDefaultToEngagedChange)
	EVT_RADIOBUTTON(ID_SWITCH_DEFAULT_TO_ENGAGED_NO, SwitchPanel::OnDefaultToEngagedChange)
	EVT_BUTTON(ID_SWITCH_REMOVE_BTN, SwitchPanel::OnRemoveSwitchBtn)
	EVT_BUTTON(ID_SWITCH_ADD_SWITCH_BTN, SwitchPanel::OnAddSwitchReferenceBtn)
	EVT_BUTTON(ID_SWITCH_REMOVE_SWITCH_BTN, SwitchPanel::OnRemoveSwitchReferenceBtn)
	EVT_LISTBOX(ID_SWITCH_AVAILABLE_SWITCHES, SwitchPanel::OnSwitchListboxSelection)
	EVT_LISTBOX(ID_SWITCH_REFERENCED_SWITCHES, SwitchPanel::OnReferencedSwitchSelection)
END_EVENT_TABLE()


SwitchPanel::SwitchPanel(wxWindow *parent) : wxPanel(parent) {
	m_switch = NULL;
	functionChoices.Add(wxT("Input"));
	functionChoices.Add(wxT("And"));
	functionChoices.Add(wxT("Nand"));
	functionChoices.Add(wxT("Not"));
	functionChoices.Add(wxT("Nor"));
	functionChoices.Add(wxT("Or"));
	functionChoices.Add(wxT("Xor"));
	gcStateChoices.Add(wxT("No change"));
	gcStateChoices.Add(wxT("Off"));
	gcStateChoices.Add(wxT("On"));

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *switchNameText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("switch name: ")
	);
	firstRow->Add(switchNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_nameField = new wxTextCtrl(
		this,
		ID_SWITCH_NAME_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_nameField, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *functionText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Function: ")
	);
	secondRow->Add(functionText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_functionChoice = new wxChoice(
		this,
		ID_SWITCH_FUNCTION_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		functionChoices
	);
	secondRow->Add(m_functionChoice, 0, wxALL, 5);
	secondRow->AddStretchSpacer();
	wxStaticText *dispInvertedText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Display in inverted state: ")
	);
	secondRow->Add(dispInvertedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedYes = new wxRadioButton(
		this,
		ID_SWITCH_INVERTED_STATE_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	secondRow->Add(m_displayInvertedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedNo = new wxRadioButton(
		this,
		ID_SWITCH_INVERTED_STATE_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_displayInvertedNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedNo->SetValue(true);
	panelSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *thirdRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available switches")
	);
	thirdRow1stCol->Add(availableReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableSwitches = new wxListBox(
		this,
		ID_SWITCH_AVAILABLE_SWITCHES
	);
	thirdRow1stCol->Add(m_availableSwitches, 1, wxEXPAND|wxALL, 5);
	thirdRow->Add(thirdRow1stCol, 1, wxEXPAND);
	wxBoxSizer *thirdRow2ndCol = new wxBoxSizer(wxVERTICAL);
	thirdRow2ndCol->AddStretchSpacer();
	m_addReferencedSwitch = new wxButton(
		this,
		ID_SWITCH_ADD_SWITCH_BTN,
		wxT("Add ->")
	);
	thirdRow2ndCol->Add(m_addReferencedSwitch, 0, wxGROW|wxALIGN_CENTER|wxALL, 5);
	m_removeReferencedSwitch = new wxButton(
		this,
		ID_SWITCH_REMOVE_SWITCH_BTN,
		wxT("<- Remove")
	);
	thirdRow2ndCol->Add(m_removeReferencedSwitch, 0, wxGROW|wxALIGN_CENTER|wxALL, 5);
	thirdRow2ndCol->AddStretchSpacer();
	thirdRow->Add(thirdRow2ndCol, 0, wxEXPAND);
	wxBoxSizer *thirdRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Referenced switches")
	);
	thirdRow3rdCol->Add(chosenReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedSwitches = new wxListBox(
		this,
		ID_SWITCH_REFERENCED_SWITCHES
	);
	thirdRow3rdCol->Add(m_referencedSwitches, 1, wxEXPAND|wxALL, 5);
	thirdRow->Add(thirdRow3rdCol, 1, wxEXPAND);

	panelSizer->Add(thirdRow, 1, wxEXPAND);

	wxBoxSizer *forgottenRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *defaultToEngagedText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Default to engaged: ")
	);
	forgottenRow->Add(defaultToEngagedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_defaultToEngagedYes = new wxRadioButton(
		this,
		ID_SWITCH_DEFAULT_TO_ENGAGED_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_defaultToEngagedYes->SetValue(true);
	forgottenRow->Add(m_defaultToEngagedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_defaultToEngagedNo = new wxRadioButton(
		this,
		ID_SWITCH_DEFAULT_TO_ENGAGED_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	forgottenRow->Add(m_defaultToEngagedNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(forgottenRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *gcStateText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("State when GC is activated: ")
	);
	fourthRow->Add(gcStateText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_gcStateChoice = new wxChoice(
		this,
		ID_SWITCH_GC_STATE_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		gcStateChoices
	);
	fourthRow->Add(m_gcStateChoice, 0, wxALL, 5);
	panelSizer->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *storeInDivisionalText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Store in divisional: ")
	);
	fifthRow->Add(storeInDivisionalText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInDivisionalYes = new wxRadioButton(
		this,
		ID_SWITCH_STORE_IN_DIV_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_storeInDivisionalYes->SetValue(true);
	fifthRow->Add(m_storeInDivisionalYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInDivisionalNo = new wxRadioButton(
		this,
		ID_SWITCH_STORE_IN_DIV_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	fifthRow->Add(m_storeInDivisionalNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(fifthRow, 0, wxGROW);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *storeInGeneralText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Store in generals: ")
	);
	sixthRow->Add(storeInGeneralText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInGeneralYes = new wxRadioButton(
		this,
		ID_SWITCH_STORE_IN_GEN_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_storeInGeneralYes->SetValue(true);
	sixthRow->Add(m_storeInGeneralYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInGeneralNo = new wxRadioButton(
		this,
		ID_SWITCH_STORE_IN_GEN_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	sixthRow->Add(m_storeInGeneralNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(sixthRow, 0, wxGROW);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	removeSwitchBtn = new wxButton(
		this,
		ID_SWITCH_REMOVE_BTN,
		wxT("Remove this switch")
	);
	bottomRow->Add(removeSwitchBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

SwitchPanel::~SwitchPanel() {

}

void SwitchPanel::setSwitch(GoSwitch *sw) {
	m_switch = sw;
	m_nameField->SetValue(m_switch->getName());
	if (m_switch->isDisplayedInverted())
		m_displayInvertedYes->SetValue(true);
	else
		m_displayInvertedNo->SetValue(true);
	m_functionChoice->SetSelection(m_functionChoice->FindString(m_switch->getFunction()));
	if (m_switch->getFunction().IsSameAs(wxT("Input"))) {
		m_availableSwitches->Enable(false);
		m_referencedSwitches->Enable(false);
		m_addReferencedSwitch->Enable(false);
		m_removeReferencedSwitch->Enable(false);
	} else {
		m_availableSwitches->Enable(true);
		m_referencedSwitches->Enable(true);
		m_addReferencedSwitch->Enable(true);
		m_removeReferencedSwitch->Enable(true);
	}
	if (m_switch->isDefaultToEngaged())
		m_defaultToEngagedYes->SetValue(true);
	else
		m_defaultToEngagedNo->SetValue(true);
	m_gcStateChoice->SetSelection((unsigned) (m_switch->getGcState() + 1));
	if (m_switch->isStoreInDivisional())
		m_storeInDivisionalYes->SetValue(true);
	else
		m_storeInDivisionalNo->SetValue(true);
	if (m_switch->isStoreInGeneral())
		m_storeInGeneralYes->SetValue(true);
	else
		m_storeInGeneralNo->SetValue(true);

	// update/populate available switches
	wxArrayString organSwitches;
	unsigned nbSwitches = ::wxGetApp().m_frame->m_organ->getNumberOfSwitches();
	unsigned thisSwitchIndex = ::wxGetApp().m_frame->m_organ->getIndexOfOrganSwitch(m_switch) - 1;
	if (nbSwitches > 1 && thisSwitchIndex > 0) {
		// a switch can only reference an earlier switch
		for (unsigned i = 0; i < thisSwitchIndex; i++) {
			organSwitches.Add(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(i)->getName());
		}
		m_availableSwitches->Clear();
		m_availableSwitches->InsertItems(organSwitches, 0);
	} else {
		m_availableSwitches->Clear();
	}

	UpdateReferencedSwitches();

	m_addReferencedSwitch->Disable();
	m_removeReferencedSwitch->Disable();
}

void SwitchPanel::OnNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_nameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_nameField);
	m_switch->setName(m_nameField->GetValue());
	wxString updatedLabel = m_nameField->GetValue();
	::wxGetApp().m_frame->OrganTreeChildItemLabelChanged(updatedLabel);
	::wxGetApp().m_frame->m_organ->organElementHasChanged();
}

void SwitchPanel::OnDisplayInvertedRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_SWITCH_INVERTED_STATE_YES) {
		m_displayInvertedYes->SetValue(true);
		m_switch->setDisplayInverted(true);
	} else {
		m_displayInvertedNo->SetValue(true);
		m_switch->setDisplayInverted(false);
	}
}

void SwitchPanel::OnFunctionChange(wxCommandEvent& WXUNUSED(event)) {
	wxString selectedText = m_functionChoice->GetString(m_functionChoice->GetSelection());
	m_switch->setFunction(selectedText);
	if (m_switch->getFunction().IsSameAs(wxT("Input"))) {
		m_availableSwitches->Enable(false);
		m_referencedSwitches->Enable(false);
		m_addReferencedSwitch->Enable(false);
		m_removeReferencedSwitch->Enable(false);
	} else {
		m_availableSwitches->Enable(true);
		m_referencedSwitches->Enable(true);
		m_addReferencedSwitch->Enable(true);
		m_removeReferencedSwitch->Enable(true);
	}
}

void SwitchPanel::OnDefaultToEngagedChange(wxCommandEvent& event) {
	if (event.GetId() == ID_SWITCH_DEFAULT_TO_ENGAGED_YES) {
		m_defaultToEngagedYes->SetValue(true);
		m_switch->setDefaultToEngaged(true);
	} else {
		m_defaultToEngagedNo->SetValue(true);
		m_switch->setDefaultToEngaged(false);
	}
}

void SwitchPanel::OnGcStateChange(wxCommandEvent& WXUNUSED(event)) {
	int gcValue = (int) m_gcStateChoice->GetSelection() - 1;
	m_switch->setGcState(gcValue);
}

void SwitchPanel::OnStoreInDivisionalChange(wxCommandEvent& event) {
	if (event.GetId() == ID_SWITCH_STORE_IN_DIV_YES) {
		m_storeInDivisionalYes->SetValue(true);
		m_switch->setStoreInDivisional(true);
	} else {
		m_storeInDivisionalNo->SetValue(true);
		m_switch->setStoreInDivisional(false);
	}
}

void SwitchPanel::OnStoreInGeneralChange(wxCommandEvent& event) {
	if (event.GetId() == ID_SWITCH_STORE_IN_GEN_YES) {
		m_storeInGeneralYes->SetValue(true);
		m_switch->setStoreInGeneral(true);
	} else {
		m_storeInGeneralNo->SetValue(true);
		m_switch->setStoreInGeneral(false);
	}
}

void SwitchPanel::OnRemoveSwitchBtn(wxCommandEvent& WXUNUSED(event)) {
	wxMessageDialog msg(this, wxT("Are you really sure you want to delete this switch?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
	if (msg.ShowModal() == wxID_YES) {
		// remove all possible references to this switch in any tremulant first
		unsigned numberOfTremulants = ::wxGetApp().m_frame->m_organ->getNumberOfTremulants();
		for (unsigned i = 0; i < numberOfTremulants; i++) {
			if (::wxGetApp().m_frame->m_organ->getOrganTremulantAt(i)->hasSwitchReference(m_switch))
				::wxGetApp().m_frame->m_organ->getOrganTremulantAt(i)->removeSwitchReference(m_switch);
		}
		// then remove this switch
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
}

void SwitchPanel::UpdateReferencedSwitches() {
	wxArrayString referencedSwitches;
	unsigned nbSwitches = m_switch->getNumberOfSwitches();
	if (nbSwitches > 0) {
		for (unsigned i = 0; i < nbSwitches; i++) {
			referencedSwitches.Add(m_switch->getSwitchAtIndex(i)->getName());
		}
		m_referencedSwitches->Clear();
		m_referencedSwitches->InsertItems(referencedSwitches, 0);
	} else {
		m_referencedSwitches->Clear();
		m_removeReferencedSwitch->Disable();
	}
}

void SwitchPanel::OnAddSwitchReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableSwitches->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_availableSwitches->GetSelection();
		if (!m_switch->hasSwitchReference(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(selected))) {
			m_switch->addSwitchReference(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(selected));
			UpdateReferencedSwitches();
		}
	}
}

void SwitchPanel::OnRemoveSwitchReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedSwitches->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_referencedSwitches->GetSelection();
		m_switch->removeSwitchReference(m_switch->getSwitchAtIndex(selected));
		UpdateReferencedSwitches();
	}
}

void SwitchPanel::OnSwitchListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableSwitches->GetSelection() != wxNOT_FOUND) {
		m_addReferencedSwitch->Enable(true);
	}
}

void SwitchPanel::OnReferencedSwitchSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedSwitches->GetSelection() != wxNOT_FOUND) {
		m_removeReferencedSwitch->Enable(true);
	}
}
