/*
 * CouplerPanel.h is part of GOODF.
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

#ifndef COUPLERPANEL_H
#define COUPLERPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Coupler.h"

class CouplerPanel : public wxPanel {
public:
	CouplerPanel(wxWindow *parent);
	~CouplerPanel();

	void setCoupler(Coupler *coupler);

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_nameField;
	wxChoice *m_functionChoice;
	wxRadioButton *m_defaultToEngagedYes;
	wxRadioButton *m_defaultToEngagedNo;
	wxListBox *m_availableSwitches;
	wxListBox *m_referencedSwitches;
	wxButton *m_addReferencedSwitch;
	wxButton *m_removeReferencedSwitch;
	wxChoice *m_gcStateChoice;
	wxRadioButton *m_storeInDivisionalYes;
	wxRadioButton *m_storeInDivisionalNo;
	wxRadioButton *m_storeInGeneralYes;
	wxRadioButton *m_storeInGeneralNo;
	wxChoice *m_destinationManualChoice;
	wxSpinCtrl *m_destinationKeyShiftSpin;
	wxChoice *m_couplerTypeChoice;
	wxRadioButton *m_coupleToSubsequentUnisonIntermanualCouplersYes;
	wxRadioButton *m_coupleToSubsequentUnisonIntermanualCouplersNo;
	wxRadioButton *m_coupleToSubsequentUpwardIntermanualCouplersYes;
	wxRadioButton *m_coupleToSubsequentUpwardIntermanualCouplersNo;
	wxRadioButton *m_coupleToSubsequentDownwardIntermanualCouplersYes;
	wxRadioButton *m_coupleToSubsequentDownwardIntermanualCouplersNo;
	wxRadioButton *m_coupleToSubsequentUpwardIntramanualCouplersYes;
	wxRadioButton *m_coupleToSubsequentUpwardIntramanualCouplersNo;
	wxRadioButton *m_coupleToSubsequentDownwardIntramanualCouplersYes;
	wxRadioButton *m_coupleToSubsequentDownwardIntramanualCouplersNo;
	wxSpinCtrl *m_firstMIDINoteNumberSpin;
	wxSpinCtrl *m_numberOfKeysSpin;
	wxButton *removeCouplerBtn;

	Coupler *m_coupler;
	wxArrayString functionChoices;
	wxArrayString gcStateChoices;
	wxArrayString couplerTypes;
	wxArrayString availableManuals;

	void OnNameChange(wxCommandEvent& event);
	void OnFunctionChange(wxCommandEvent& event);
	void OnDefaultToEngagedChange(wxCommandEvent& event);
	void OnGcStateChange(wxCommandEvent& event);
	void OnStoreInDivisionalChange(wxCommandEvent& event);
	void OnStoreInGeneralChange(wxCommandEvent& event);
	void OnDestinationManualChoice(wxCommandEvent& event);
	void OnDestinationKeyShiftSpin(wxSpinEvent& event);
	void OnCouplerTypeChoice(wxCommandEvent& event);
	void OnCoupleToSubsequentUnisonIntermanualCouplers(wxCommandEvent& event);
	void OnCoupleToSubsequentUpwardIntermanualCouplers(wxCommandEvent& event);
	void OnCoupleToSubsequentDownwardIntermanualCouplers(wxCommandEvent& event);
	void OnCoupleToSubsequentUpwardIntramanualCouplers(wxCommandEvent& event);
	void OnCoupleToSubsequentDownwardIntramanualCouplers(wxCommandEvent& event);
	void OnFirstMidiNoteNumberSpin(wxSpinEvent& event);
	void OnNumberOfKeysSpin(wxSpinEvent& event);
	void OnRemoveCouplerBtn(wxCommandEvent& event);
	void OnAddSwitchReferenceBtn(wxCommandEvent& event);
	void OnRemoveSwitchReferenceBtn(wxCommandEvent& event);
	void OnSwitchListboxSelection(wxCommandEvent& event);
	void OnReferencedSwitchSelection(wxCommandEvent& event);
	void UpdateReferencedSwitches();

};

#endif
