/*
 * RankPanel.cpp is part of GOODF.
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

#include "RankPanel.h"
#include <wx/statline.h>
#include <wx/button.h>
#include "GOODFFunctions.h"
#include "GOODF.h"
#include <wx/msgdlg.h>
#include <wx/dirdlg.h>
#include <wx/stdpaths.h>
#include "PipeDialog.h"
#include "ReleaseDialog.h"
#include "AttackDialog.h"
#include "StopPanel.h"
#include "PipeBorrowingDialog.h"

// Event table
BEGIN_EVENT_TABLE(RankPanel, wxPanel)
	EVT_TEXT(ID_RANK_NAME_TEXT, RankPanel::OnNameChange)
	EVT_CHOICE(ID_RANK_WINDCHEST_CHOICE, RankPanel::OnWindchestChoice)
	EVT_SPINCTRL(ID_RANK_FIRST_MIDI_NOTE_SPIN, RankPanel::OnMidiNoteSpin)
	EVT_SPINCTRL(ID_RANK_LOGICAL_PIPES_SPIN, RankPanel::OnLogicalPipeSpin)
	EVT_SPINCTRL(ID_RANK_HARMONIC_NBR_SPIN, RankPanel::OnHarmonicNbrSpin)
	EVT_SPINCTRLDOUBLE(ID_RANK_PITCH_CORR_SPIN, RankPanel::OnPitchCorrectionSpin)
	EVT_RADIOBUTTON(ID_RANK_PERCUSSIVE_YES, RankPanel::OnPercussiveSelection)
	EVT_RADIOBUTTON(ID_RANK_PERCUSSIVE_NO, RankPanel::OnPercussiveSelection)
	EVT_SPINCTRLDOUBLE(ID_RANK_MIN_VEL_VOL_SPIN, RankPanel::OnMinVelocitySpin)
	EVT_SPINCTRLDOUBLE(ID_RANK_MAX_VEL_VOL_SPIN, RankPanel::OnMaxVelocitySpin)
	EVT_RADIOBUTTON(ID_RANK_ACC_RETUNING_YES, RankPanel::OnRetuningSelection)
	EVT_RADIOBUTTON(ID_RANK_ACC_RETUNING_NO, RankPanel::OnRetuningSelection)
	EVT_BUTTON(ID_RANK_READ_PIPES_BTN, RankPanel::OnReadPipesBtn)
	EVT_BUTTON(ID_RANK_REMOVE_BTN, RankPanel::OnRemoveRankBtn)
	EVT_BUTTON(ID_RANK_CLEAR_PIPES, RankPanel::OnClearPipesBtn)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_RANK_PIPE_TREE, RankPanel::OnPipeTreeItemRightClick)
	EVT_SPINCTRLDOUBLE(ID_RANK_AMP_LVL_SPIN, RankPanel::OnAmplitudeLevelSpin)
	EVT_SPINCTRLDOUBLE(ID_RANK_GAIN_SPIN, RankPanel::OnGainSpin)
	EVT_SPINCTRLDOUBLE(ID_RANK_PITCH_SPIN, RankPanel::OnPitchTuningSpin)
	EVT_SPINCTRL(ID_RANK_TRACKER_DELAY_SPIN, RankPanel::OnTrackerDelaySpin)
	EVT_BUTTON(ID_RANK_ADD_PIPES_BTN, RankPanel::OnAddPipesBtn)
	EVT_BUTTON(ID_RANK_ADD_TREMULANT_PIPES_BTN, RankPanel::OnAddTremulantPipesBtn)
	EVT_BUTTON(ID_RANK_EXPAND_TREE_BTN, RankPanel::OnExpandTreeBtn)
	EVT_BUTTON(ID_RANK_ADD_RELEASES_BTN, RankPanel::OnAddReleaseSamplesBtn)
END_EVENT_TABLE()

RankPanel::RankPanel(wxWindow *parent) : wxPanel(parent) {
	m_rank = NULL;

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *rankNameText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Rank name: ")
	);
	firstRow->Add(rankNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_nameField = new wxTextCtrl(
		this,
		ID_RANK_NAME_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_nameField, 1, wxEXPAND|wxALL, 5);
	wxStaticText *windchestText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Windchestgroup: ")
	);
	firstRow->Add(windchestText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_windchestChoice = new wxChoice(
		this,
		ID_RANK_WINDCHEST_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		availableWindchests
	);
	firstRow->Add(m_windchestChoice, 0, wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *firstMidiNoteText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("First Midi note: ")
	);
	secondRow->Add(firstMidiNoteText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_firstMidiNoteNumberSpin = new wxSpinCtrl(
		this,
		ID_RANK_FIRST_MIDI_NOTE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		256,
		36
	);
	secondRow->Add(m_firstMidiNoteNumberSpin, 0, wxEXPAND|wxALL, 5);
	secondRow->AddStretchSpacer();
	wxStaticText *logicalPipeNbrText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Number of logical pipes: ")
	);
	secondRow->Add(logicalPipeNbrText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_numberOfLogicalPipesSpin = new wxSpinCtrl(
		this,
		ID_RANK_LOGICAL_PIPES_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		192,
		1
	);
	secondRow->Add(m_numberOfLogicalPipesSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *harmonicNumberText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Harmonic number: ")
	);
	thirdRow->Add(harmonicNumberText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_harmonicNumberSpin = new wxSpinCtrl(
		this,
		ID_RANK_HARMONIC_NBR_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1024,
		8
	);
	thirdRow->Add(m_harmonicNumberSpin, 0, wxEXPAND|wxALL, 5);
	thirdRow->AddStretchSpacer();
	wxStaticText *trackerDelayText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TrackerDelay: ")
	);
	thirdRow->Add(trackerDelayText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_trackerDelaySpin = new wxSpinCtrl(
		this,
		ID_RANK_TRACKER_DELAY_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		10000,
		0
	);
	thirdRow->Add(m_trackerDelaySpin, 0, wxGROW|wxALL, 5);
	panelSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *pitchRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *pitchText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("PitchTuning: ")
	);
	pitchRow->Add(pitchText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pitchTuningSpin = new wxSpinCtrlDouble(
		this,
		ID_RANK_PITCH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1800,
		1000,
		0,
		0.000001
	);
	pitchRow->Add(m_pitchTuningSpin, 0, wxEXPAND|wxALL, 5);
	pitchRow->AddStretchSpacer();
	wxStaticText *pitchCorrectionText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Pitch correction: ")
	);
	pitchRow->Add(pitchCorrectionText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pitchCorrectionSpin = new wxSpinCtrlDouble(
		this,
		ID_RANK_PITCH_CORR_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1800,
		1800,
		0,
		0.000001
	);
	pitchRow->Add(m_pitchCorrectionSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(pitchRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *ampLvlText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("AmplitudeLevel: ")
	);
	fourthRow->Add(ampLvlText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_amplitudeLevelSpin = new wxSpinCtrlDouble(
		this,
		ID_RANK_AMP_LVL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1000,
		100,
		0.000001
	);
	fourthRow->Add(m_amplitudeLevelSpin, 0, wxEXPAND|wxALL, 5);
	fourthRow->AddStretchSpacer();
	wxStaticText *gainText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Gain: ")
	);
	fourthRow->Add(gainText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_gainSpin = new wxSpinCtrlDouble(
		this,
		ID_RANK_GAIN_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-120,
		40,
		0,
		0.000001
	);
	fourthRow->Add(m_gainSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *minVelocityVolText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Min velocity volume: ")
	);
	fifthRow->Add(minVelocityVolText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_minVelocityVolumeSpin = new wxSpinCtrlDouble(
		this,
		ID_RANK_MIN_VEL_VOL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1000,
		100,
		0.000001
	);
	fifthRow->Add(m_minVelocityVolumeSpin, 0, wxEXPAND|wxALL, 5);
	fifthRow->AddStretchSpacer();
	wxStaticText *maxVelocityVolText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Max velocity volume: ")
	);
	fifthRow->Add(maxVelocityVolText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maxVelocityVolumeSpin = new wxSpinCtrlDouble(
		this,
		ID_RANK_MAX_VEL_VOL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1000,
		100,
		0.000001
	);
	fifthRow->Add(m_maxVelocityVolumeSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(fifthRow, 0, wxGROW);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	m_expandTreeBtn = new wxButton(
		this,
		ID_RANK_EXPAND_TREE_BTN,
		wxT("Expand the pipe tree")
	);
	sixthRow->Add(m_expandTreeBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sixthRow->AddStretchSpacer();
	wxStaticText *isPercussiveText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Percussive: ")
	);
	sixthRow->Add(isPercussiveText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isPercussiveYes = new wxRadioButton(
		this,
		ID_RANK_PERCUSSIVE_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_isPercussiveYes->SetValue(false);
	sixthRow->Add(m_isPercussiveYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isPercussiveNo = new wxRadioButton(
		this,
		ID_RANK_PERCUSSIVE_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	sixthRow->Add(m_isPercussiveNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isPercussiveNo->SetValue(true);
	sixthRow->AddStretchSpacer();
	wxStaticText *acceptsRetuningText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Accepts retuning: ")
	);
	sixthRow->Add(acceptsRetuningText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_acceptsRetuningYes = new wxRadioButton(
		this,
		ID_RANK_ACC_RETUNING_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_acceptsRetuningYes->SetValue(true);
	sixthRow->Add(m_acceptsRetuningYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_acceptsRetuningNo = new wxRadioButton(
		this,
		ID_RANK_ACC_RETUNING_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	sixthRow->Add(m_acceptsRetuningNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(sixthRow, 0, wxGROW);

	wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
	m_pipeTreeCtrl = new wxTreeCtrl(
		this,
		ID_RANK_PIPE_TREE,
		wxDefaultPosition,
		wxDefaultSize,
		wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT
	);
	m_tree_rank_root = m_pipeTreeCtrl->AddRoot("Rank");
	seventhRow->Add(m_pipeTreeCtrl, 1, wxEXPAND);
	wxBoxSizer *pipeReadingOptions = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *actionButtons = new wxBoxSizer(wxHORIZONTAL);
	readPipesFromFolderBtn = new wxButton(
		this,
		ID_RANK_READ_PIPES_BTN,
		wxT("Create new pipes by MIDI nbr from...")
	);
	actionButtons->Add(readPipesFromFolderBtn, 0, wxALL, 5);
	actionButtons->AddStretchSpacer();
	wxButton *clearPipesBtn = new wxButton(
		this,
		ID_RANK_CLEAR_PIPES,
		wxT("Reset/Clear all pipes")
	);
	actionButtons->Add(clearPipesBtn, 0, wxALL, 5);
	pipeReadingOptions->Add(actionButtons, 0, wxGROW);
	wxStaticBoxSizer *readingOptions = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Pipe reading options"));
	wxBoxSizer *optionsRow1 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *extraAttackText = new wxStaticText (
		readingOptions->GetStaticBox(),
		wxID_STATIC,
		wxT("Attack subfolder prefix: ")
	);
	optionsRow1->Add(extraAttackText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);
	m_optionsAttackField = new wxTextCtrl(
		readingOptions->GetStaticBox(),
		ID_RANK_ATTACK_SUBFOLDER_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	optionsRow1->Add(m_optionsAttackField, 1, wxEXPAND|wxALL, 4);
	readingOptions->Add(optionsRow1, 0, wxGROW);
	wxBoxSizer *optionsRowOnlyOne = new wxBoxSizer(wxHORIZONTAL);
	m_optionsOnlyOneAttack = new wxCheckBox(
		readingOptions->GetStaticBox(),
		ID_RANK_ONLY_ONE_ATK_OPTION,
		wxT("Load only one attack"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_optionsOnlyOneAttack->SetValue(false);
	optionsRowOnlyOne->Add(m_optionsOnlyOneAttack, 0, wxALL, 2);
	optionsRowOnlyOne->AddStretchSpacer();
	m_optionsLoadReleaseInAttack = new wxCheckBox(
		readingOptions->GetStaticBox(),
		ID_RANK_LOAD_RELEASE_OPTION,
		wxT("Load release in attack"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_optionsLoadReleaseInAttack->SetValue(true);
	optionsRowOnlyOne->Add(m_optionsLoadReleaseInAttack, 0, wxALL, 2);
	readingOptions->Add(optionsRowOnlyOne,0, wxGROW);
	wxBoxSizer *optionsRow2 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *releaseFolderText = new wxStaticText (
		readingOptions->GetStaticBox(),
		wxID_STATIC,
		wxT("Release subfolder prefix: ")
	);
	optionsRow2->Add(releaseFolderText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);
	m_optionsReleaseField = new wxTextCtrl(
		readingOptions->GetStaticBox(),
		ID_RANK_RELEASE_SUBFOLDER_TEXT,
		wxT("rel"),
		wxDefaultPosition,
		wxDefaultSize
	);
	optionsRow2->Add(m_optionsReleaseField, 1, wxEXPAND|wxALL, 4);
	readingOptions->Add(optionsRow2, 0, wxGROW);
	wxBoxSizer *optionsRow3 = new wxBoxSizer(wxHORIZONTAL);
	m_optionsKeyPressTime = new wxCheckBox(
		readingOptions->GetStaticBox(),
		ID_RANK_KEY_PRESS_TIME_OPTION,
		wxT("Extract MaxKeyPressTime from foldername"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_optionsKeyPressTime->SetValue(true);
	optionsRow3->Add(m_optionsKeyPressTime, 0, wxALL, 2);
	readingOptions->Add(optionsRow3, 0, wxGROW);
	wxBoxSizer *optionsRow4 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *tremulantFolderText = new wxStaticText (
		readingOptions->GetStaticBox(),
		wxID_STATIC,
		wxT("Tremulant folder prefix: ")
	);
	optionsRow4->Add(tremulantFolderText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);
	m_optionsTremulantField = new wxTextCtrl(
		readingOptions->GetStaticBox(),
		ID_RANK_TREMULANT_SUBFOLDER_TEXT,
		wxT("trem"),
		wxDefaultPosition,
		wxDefaultSize
	);
	optionsRow4->Add(m_optionsTremulantField, 1, wxEXPAND|wxALL, 4);
	readingOptions->Add(optionsRow4, 0, wxGROW);
	wxBoxSizer *optionsRow5 = new wxBoxSizer(wxHORIZONTAL);
	m_addPipesFromFolderBtn = new wxButton(
		readingOptions->GetStaticBox(),
		ID_RANK_ADD_PIPES_BTN,
		wxT("Add more samples from...")
	);
	optionsRow5->Add(m_addPipesFromFolderBtn, 0, wxALL, 2);
	optionsRow5->AddStretchSpacer();
	m_addTremulantPipesBtn = new wxButton(
		readingOptions->GetStaticBox(),
		ID_RANK_ADD_TREMULANT_PIPES_BTN,
		wxT("Add tremulant samples from...")
	);
	optionsRow5->Add(m_addTremulantPipesBtn, 0, wxALL, 2);
	optionsRow5->AddStretchSpacer();
	readingOptions->Add(optionsRow5, 0, wxGROW);
	wxBoxSizer *optionsRow6 = new wxBoxSizer(wxHORIZONTAL);
	optionsRow6->AddStretchSpacer();
	m_addReleaseSamplesBtn = new wxButton(
		readingOptions->GetStaticBox(),
		ID_RANK_ADD_RELEASES_BTN,
		wxT("Add (only) releases from...")
	);
	optionsRow6->Add(m_addReleaseSamplesBtn, 0, wxALL, 2);
	optionsRow6->AddStretchSpacer();
	readingOptions->Add(optionsRow6, 0, wxGROW);
	pipeReadingOptions->Add(readingOptions, 1, wxEXPAND);
	seventhRow->Add(pipeReadingOptions, 1, wxEXPAND);
	panelSizer->Add(seventhRow, 1, wxEXPAND|wxALL, 5);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	removeRankBtn = new wxButton(
		this,
		ID_RANK_REMOVE_BTN,
		wxT("Remove this rank")
	);
	bottomRow->Add(removeRankBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

RankPanel::~RankPanel() {

}

void RankPanel::setRank(Rank *rank) {
	m_rank = rank;

	// update/populate available windchests
	if (!availableWindchests.IsEmpty())
		availableWindchests.Empty();
	unsigned nbWc = ::wxGetApp().m_frame->m_organ->getNumberOfWindchestgroups();
	if (nbWc > 0) {
		for (unsigned i = 0; i < nbWc; i++) {
			availableWindchests.Add(::wxGetApp().m_frame->m_organ->getOrganWindchestgroupAt(i)->getName());
		}
		m_windchestChoice->Clear();
		m_windchestChoice->Insert(availableWindchests, 0);
	}

	m_nameField->SetValue(m_rank->getName());
	if (m_rank->getWindchest() != NULL)
		m_windchestChoice->SetSelection((int) (::wxGetApp().m_frame->m_organ->getIndexOfOrganWindchest(m_rank->getWindchest()) - 1));
	m_firstMidiNoteNumberSpin->SetValue(m_rank->getFirstMidiNoteNumber());
	m_numberOfLogicalPipesSpin->SetValue(m_rank->getNumberOfLogicalPipes());

	// create the necessary pipes in tree
	RebuildPipeTree();
	UpdatePipeTree();

	m_harmonicNumberSpin->SetValue(m_rank->getHarmonicNumber());
	m_pitchCorrectionSpin->SetValue(m_rank->getPitchCorrection());
	if (m_rank->isPercussive()) {
		m_isPercussiveYes->SetValue(true);
		m_isPercussiveNo->SetValue(false);
	} else {
		m_isPercussiveNo->SetValue(true);
		m_isPercussiveYes->SetValue(false);
	}
	m_minVelocityVolumeSpin->SetValue(m_rank->getMinVelocityVolume());
	m_maxVelocityVolumeSpin->SetValue(m_rank->getMaxVelocityVolume());
	if (m_rank->doesAcceptsRetuning()) {
		m_acceptsRetuningYes->SetValue(true);
		m_acceptsRetuningNo->SetValue(false);
	} else {
		m_acceptsRetuningYes->SetValue(false);
		m_acceptsRetuningNo->SetValue(true);
	}
	m_amplitudeLevelSpin->SetValue(m_rank->getAmplitudeLevel());
	m_gainSpin->SetValue(m_rank->getGain());
	m_pitchTuningSpin->SetValue(m_rank->getPitchTuning());
	m_trackerDelaySpin->SetValue(m_rank->getTrackerDelay());
}

void RankPanel::setNameFieldValue(wxString name) {
	m_nameField->SetValue(name);
}

void RankPanel::disableNameFieldInput() {
	m_nameField->Disable();
}

void RankPanel::OnNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_nameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_nameField);
	m_rank->setName(m_nameField->GetValue());
	wxString updatedLabel = m_nameField->GetValue();
	::wxGetApp().m_frame->OrganTreeChildItemLabelChanged(updatedLabel);
}

void RankPanel::OnWindchestChoice(wxCommandEvent& WXUNUSED(event)) {
	if (m_windchestChoice->GetSelection() != wxNOT_FOUND) {
		unsigned selectedIndex = m_windchestChoice->GetSelection();
		m_rank->setWindchest(::wxGetApp().m_frame->m_organ->getOrganWindchestgroupAt(selectedIndex));
	}
}

void RankPanel::OnMidiNoteSpin(wxSpinEvent& WXUNUSED(event)) {
	if (m_rank->hasOnlyDummyPipes()) {
		m_rank->setFirstMidiNoteNumber(m_firstMidiNoteNumberSpin->GetValue());
	} else {
		wxMessageDialog msg(this, wxT("Pipes already exist in rank! Changing this value now causes changes in logical placement and removal of pipes!"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			int spinValue = m_firstMidiNoteNumberSpin->GetValue();
			int inRankValue = m_rank->getFirstMidiNoteNumber();
			m_rank->setFirstMidiNoteNumber(spinValue);

			if (spinValue > inRankValue) {
				// this means that the pipes need to be removed from the bottom
				// and new ones will be created in the top
				int pipesToAddTop = spinValue - inRankValue;

				while (pipesToAddTop > 0) {
					m_rank->addDummyPipeBack();
					m_rank->removePipeFront();
					pipesToAddTop--;
				}

			} else if (inRankValue > spinValue) {
				// pipes need to be removed from the top and new ones created below
				int pipesToAddBottom = inRankValue - spinValue;

				while (pipesToAddBottom > 0) {
					m_rank->addDummyPipeFront();
					m_rank->removePipeBack();
					pipesToAddBottom--;
				}

			}
			RebuildPipeTree();
			UpdatePipeTree();
		}
	}
}

void RankPanel::OnLogicalPipeSpin(wxSpinEvent& WXUNUSED(event)) {
	int pipeSpinValue = m_numberOfLogicalPipesSpin->GetValue();
	int pipesAlreadyInRank = m_rank->getNumberOfLogicalPipes();
	if (pipeSpinValue > pipesAlreadyInRank) {
		m_rank->setNumberOfLogicalPipes(pipeSpinValue);
		int pipesToAdd = pipeSpinValue - pipesAlreadyInRank;
		for (int i = 0; i < pipesToAdd; i++) {
			m_rank->addDummyPipeBack();
		}
		RebuildPipeTree();
		UpdatePipeTree();
	} else if (pipesAlreadyInRank > pipeSpinValue) {
		wxMessageDialog msg(this, wxT("Pipes will be deleted! Are you really sure you want to delete them?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			// we must remove all pipes lost
			int pipesToRemove = pipesAlreadyInRank - pipeSpinValue;
			m_rank->setNumberOfLogicalPipes(pipeSpinValue);

			// then adjust the tree view
			while (pipesToRemove > 0) {
				m_rank->removePipeBack();
				m_pipeTreeCtrl->Delete(m_pipeTreeCtrl->GetLastChild(m_tree_rank_root));
				m_rank->m_pipes.pop_back();
				pipesToRemove--;
			}
		} else {
			m_numberOfLogicalPipesSpin->SetValue(pipesAlreadyInRank);
		}
	}
	// try to notify parent if it's a StopPanel
	StopPanel *theParent = wxDynamicCast(this->GetGrandParent(), StopPanel);
	if (theParent) {
		theParent->internalRankLogicalPipesChanged(m_rank->getNumberOfLogicalPipes());
	}
}

void RankPanel::OnHarmonicNbrSpin(wxSpinEvent& WXUNUSED(event)) {
	m_rank->setHarmonicNumber(m_harmonicNumberSpin->GetValue());
}

void RankPanel::OnPitchCorrectionSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_rank->setPitchCorrection((float) m_pitchCorrectionSpin->GetValue());
}

void RankPanel::OnPercussiveSelection(wxCommandEvent& event) {
	if (event.GetId() == ID_RANK_PERCUSSIVE_YES) {

		m_rank->setPercussive(true);

	} else {

		m_rank->setPercussive(false);

	}
	RebuildPipeTree();
	UpdatePipeTree();
}

void RankPanel::OnMinVelocitySpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_rank->setMinVelocityVolume((float) m_minVelocityVolumeSpin->GetValue());
}

void RankPanel::OnMaxVelocitySpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_rank->setMaxVelocityVolume((float) m_maxVelocityVolumeSpin->GetValue());
}

void RankPanel::OnRetuningSelection(wxCommandEvent& event) {
	if (event.GetId() == ID_RANK_ACC_RETUNING_YES) {
		m_acceptsRetuningYes->SetValue(true);
		m_rank->setAcceptsRetuning(true);
	} else {
		m_acceptsRetuningNo->SetValue(true);
		m_rank->setAcceptsRetuning(false);
	}
}

void RankPanel::OnReadPipesBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString defaultPath;
	if (m_rank->getPipesRootPath() != wxEmptyString)
		defaultPath = m_rank->getPipesRootPath();
	else
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();

	wxDirDialog rankPipesPathDialog(
		this,
		wxT("Pick a root directory to read new pipes from"),
		defaultPath,
		wxDD_DIR_MUST_EXIST
	);

	if (rankPipesPathDialog.ShowModal() == wxID_OK) {
		m_rank->setPipesRootPath(rankPipesPathDialog.GetPath());
		wxString extraAttackFolderPrefix = m_optionsAttackField->GetValue();
		bool loadOnlyOneAttack = m_optionsOnlyOneAttack->GetValue();
		bool loadRelease = m_optionsLoadReleaseInAttack->GetValue();
		wxString releaseFolderPrefix = m_optionsReleaseField->GetValue();
		bool extractKeyPressTime = m_optionsKeyPressTime->GetValue();
		wxString tremulantFolderPrefix = m_optionsTremulantField->GetValue();

		m_rank->readPipes(
			extraAttackFolderPrefix,
			loadOnlyOneAttack,
			loadRelease,
			releaseFolderPrefix,
			extractKeyPressTime,
			tremulantFolderPrefix
		);

		RebuildPipeTree();
		UpdatePipeTree();
	}
}

void RankPanel::OnRemoveRankBtn(wxCommandEvent& WXUNUSED(event)) {
	wxMessageDialog msg(this, wxT("Are you really sure you want to delete this rank?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
	if (msg.ShowModal() == wxID_YES) {
		// remove all possible references in stops first
		::wxGetApp().m_frame->m_organ->removeReferenceToRankInStops(m_rank);

		// then remove this rank
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
}

void RankPanel::OnClearPipesBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_rank->hasOnlyDummyPipes())
		return;

	wxMessageDialog msg(this, wxT("Are you really sure you want to clear all pipes?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
	if (msg.ShowModal() == wxID_YES) {
		m_rank->clearAllPipes();
		m_rank->createDummyPipes();
		RebuildPipeTree();
		UpdatePipeTree();
	}
}

void RankPanel::OnPipeTreeItemRightClick(wxTreeEvent &evt) {
	wxTreeItemId selectedItem = evt.GetItem();
	wxMenu mnu;

	bool showMenu = false;

	if (m_pipeTreeCtrl->GetItemParent(selectedItem) == m_tree_rank_root) {
		// for a pipe selection
		mnu.Append(ID_PIPE_MENU_ADD_ATTACK, "Add new attack from...");
		mnu.Append(ID_PIPE_MENU_ADD_RELEASE, "Add new release from...");
		mnu.Append(ID_PIPE_MENU_CREATE_REFERENCE, "Borrow pipe...");
		mnu.Append(ID_PIPE_MENU_CLEAR_PIPE, "Reset this pipe");
		mnu.Append(ID_PIPE_MENU_EDIT_PIPE, "Edit pipe properties");
		showMenu = true;
	} else if (m_pipeTreeCtrl->GetItemText(m_pipeTreeCtrl->GetItemParent(selectedItem)) == wxT("Attack(s)") ||
		(m_pipeTreeCtrl->GetItemParent(m_pipeTreeCtrl->GetItemParent(selectedItem)) == m_tree_rank_root &&
			m_pipeTreeCtrl->GetItemText(selectedItem) != wxT("Attack(s)") &&
			m_pipeTreeCtrl->GetItemText(selectedItem) != wxT("Release(s)")
		)
	) {
		// for an attack
		mnu.Append(ID_PIPE_MENU_EDIT_ATTACK, "Edit attack properties");
		mnu.Append(ID_PIPE_MENU_REMOVE_SELECTED_ATTACK, "Delete attack");
		showMenu = true;
	} else if (m_pipeTreeCtrl->GetItemText(m_pipeTreeCtrl->GetItemParent(selectedItem)) == wxT("Release(s)")) {
		// for a release
		mnu.Append(ID_PIPE_MENU_EDIT_RELEASE, "Edit release properties");
		mnu.Append(ID_PIPE_MENU_REMOVE_SELECTED_RELEASE, "Delete release");
		showMenu = true;
	}

	if (showMenu) {
		mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(RankPanel::OnPopupMenuClick), NULL, this);
		PopupMenu(&mnu);
	}
}

void RankPanel::OnPopupMenuClick(wxCommandEvent &evt) {
	switch(evt.GetId()) {
		case ID_PIPE_MENU_ADD_ATTACK:
			OnAddNewAttack();
			break;
		case ID_PIPE_MENU_ADD_RELEASE:
			OnAddNewRelease();
			break;
		case ID_PIPE_MENU_CLEAR_PIPE:
			OnClearPipe();
			break;
		case ID_PIPE_MENU_CREATE_REFERENCE:
			OnCreateReference();
			break;
		case ID_PIPE_MENU_EDIT_PIPE:
			OnEditPipe();
			break;
		case ID_PIPE_MENU_EDIT_ATTACK:
			OnEditAttack();
			break;
		case ID_PIPE_MENU_EDIT_RELEASE:
			OnEditRelease();
			break;
		case ID_PIPE_MENU_REMOVE_SELECTED_ATTACK:
			OnRemoveSelectedAttack();
			break;
		case ID_PIPE_MENU_REMOVE_SELECTED_RELEASE:
			OnRemoveSelectedRelease();
			break;
	}
}

void RankPanel::UpdatePipeTree() {
	wxTreeItemIdValue cookie;
	bool firstItem = true;
	for (Pipe p : m_rank->m_pipes) {
		wxTreeItemId currentPipe;
		if (firstItem) {
			currentPipe = m_pipeTreeCtrl->GetFirstChild(m_tree_rank_root, cookie);
			firstItem = false;
		} else {
			currentPipe = m_pipeTreeCtrl->GetNextChild(m_tree_rank_root, cookie);
		}

		if (m_rank->isPercussive()) {
			// we can just dump the (attacks of the) pipes into the tree (if they exist)
			if (!p.m_attacks.empty()) {
				for (Attack atk : p.m_attacks) {
					m_pipeTreeCtrl->AppendItem(currentPipe, atk.fileName);
				}
			}
		} else {
			// we must handle attacks and releases separately
			wxTreeItemId releases = m_pipeTreeCtrl->GetLastChild(currentPipe);
			wxTreeItemId attacks = m_pipeTreeCtrl->GetPrevSibling(releases);

			for (Attack atk : p.m_attacks) {
				m_pipeTreeCtrl->AppendItem(attacks, atk.fileName);
			}

			if (!p.m_releases.empty()) {
				for (Release rel : p.m_releases) {
					m_pipeTreeCtrl->AppendItem(releases, rel.fileName);
				}
			}
		}
	}
}

void RankPanel::RebuildPipeTree() {
	m_pipeTreeCtrl->DeleteChildren(m_tree_rank_root);

	for (int i = 0; i < m_rank->getNumberOfLogicalPipes(); i++) {
		wxString pipeName = wxT("Pipe") + GOODF_functions::number_format(i + 1);
		wxTreeItemId thisPipe = m_pipeTreeCtrl->AppendItem(m_tree_rank_root, pipeName);

		// for a percussive rank there's no release(s) to load but
		// otherwise there can be both attacks and releases for this pipe
		if (!m_rank->isPercussive()) {
			m_pipeTreeCtrl->AppendItem(thisPipe, wxT("Attack(s)"));
			m_pipeTreeCtrl->AppendItem(thisPipe, wxT("Release(s)"));
		}
	}
}

int RankPanel::GetSelectedItemIndexRelativeParent() {
	// return a valid index or -1 if it's not
	int selectedIndex = -1;
	wxTreeItemId selectedItem = m_pipeTreeCtrl->GetSelection();
	wxTreeItemId parent = m_pipeTreeCtrl->GetItemParent(selectedItem);

	if (selectedItem.IsOk() && parent.IsOk()) {

		int numberOfItems = m_pipeTreeCtrl->GetChildrenCount(parent, false);
		wxTreeItemIdValue cookie;

		for (int i = 0; i < numberOfItems; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_pipeTreeCtrl->GetFirstChild(parent, cookie);
			else
				currentId = m_pipeTreeCtrl->GetNextChild(parent, cookie);

			if (selectedItem == currentId) {
				selectedIndex = i;
				break;
			}
		}
		return selectedIndex;
	}

	return selectedIndex;
}

int RankPanel::GetItemIndexRelativeParent(wxTreeItemId item) {
	// return a valid index or -1 if it's not
	int index = -1;

	wxTreeItemId parent = m_pipeTreeCtrl->GetItemParent(item);

	if (item.IsOk() && parent.IsOk()) {

		int numberOfItems = m_pipeTreeCtrl->GetChildrenCount(parent, false);
		wxTreeItemIdValue cookie;

		for (int i = 0; i < numberOfItems; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_pipeTreeCtrl->GetFirstChild(parent, cookie);
			else
				currentId = m_pipeTreeCtrl->GetNextChild(parent, cookie);

			if (item == currentId) {
				index = i;
				break;
			}
		}
		return index;
	}

	return index;
}

void RankPanel::OnAddNewAttack() {
	wxString attackPath;
	wxString defaultPath;
	if (m_rank->getPipesRootPath() != wxEmptyString)
		defaultPath = m_rank->getPipesRootPath();
	else
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();

	wxFileDialog fileDialog(
		this,
		wxT("Select wave sample file as attack"),
		defaultPath,
		"",
		"WAVE files (*.wav;*.wv)|*.wav;*.wv",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() == wxID_CANCEL)
		return;

	attackPath = fileDialog.GetPath();

	int pipeIndex = GetSelectedItemIndexRelativeParent();
	if  (pipeIndex < 0)
		return;

	bool loadRelease = m_optionsLoadReleaseInAttack->GetValue();
	m_rank->createNewAttackInPipe(pipeIndex, attackPath, loadRelease);

	RebuildPipeTree();
	UpdatePipeTree();

	wxTreeItemId toSelect = GetPipeTreeItemAt(pipeIndex);
	if (toSelect.IsOk()) {
		m_pipeTreeCtrl->SelectItem(toSelect);
		m_pipeTreeCtrl->ExpandAllChildren(toSelect);
	}

	m_rank->setPipesRootPath(fileDialog.GetDirectory());
}

void RankPanel::OnAddNewRelease() {
	wxString releasePath;
	wxString defaultPath;
	if (m_rank->getPipesRootPath() != wxEmptyString)
		defaultPath = m_rank->getPipesRootPath();
	else
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();

	wxFileDialog fileDialog(
		this,
		wxT("Select wave sample file as release"),
		defaultPath,
		"",
		"WAVE files (*.wav;*.wv)|*.wav;*.wv",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() == wxID_CANCEL)
		return;

	releasePath = fileDialog.GetPath();

	int pipeIndex = GetSelectedItemIndexRelativeParent();
	if  (pipeIndex < 0)
		return;

	bool extractKeyPressTime = m_optionsKeyPressTime->GetValue();
	m_rank->createNewReleaseInPipe(pipeIndex, releasePath, extractKeyPressTime);

	RebuildPipeTree();
	UpdatePipeTree();

	wxTreeItemId toSelect = GetPipeTreeItemAt(pipeIndex);
	if (toSelect.IsOk()) {
		m_pipeTreeCtrl->SelectItem(toSelect);
		m_pipeTreeCtrl->ExpandAllChildren(toSelect);
	}

	m_rank->setPipesRootPath(fileDialog.GetDirectory());
}

void RankPanel::OnClearPipe() {
	int pipeIndex = GetSelectedItemIndexRelativeParent();
	if  (pipeIndex < 0)
		return;

	m_rank->clearPipeAt((unsigned) pipeIndex);

	RebuildPipeTree();
	UpdatePipeTree();

	wxTreeItemId toSelect = GetPipeTreeItemAt(pipeIndex);
	if (toSelect.IsOk()) {
		m_pipeTreeCtrl->SelectItem(toSelect);
		m_pipeTreeCtrl->ExpandAllChildren(toSelect);
	}
}

void RankPanel::OnEditPipe() {
	PipeDialog dlg(m_rank->m_pipes, (unsigned) GetSelectedItemIndexRelativeParent(), this);
	dlg.ShowModal();

	RebuildPipeTree();
	UpdatePipeTree();

	wxTreeItemId toSelect = GetPipeTreeItemAt(dlg.GetSelectedPipeIndex());
	if (toSelect.IsOk()) {
		m_pipeTreeCtrl->SelectItem(toSelect);
		// m_pipeTreeCtrl->ExpandAllChildren(toSelect);
	}
}

void RankPanel::OnCreateReference() {
	int pipeIndex = GetSelectedItemIndexRelativeParent();
	int highestPipeIdx = m_rank->m_pipes.size() - 1;
	int pipesAboveThis = highestPipeIdx - pipeIndex;
	PipeBorrowingDialog dlg(this);

	if (dlg.ShowModal() == wxID_OK) {
		if(dlg.IsSelectionOk()) {
			int manId = dlg.GetSelectedManual();
			if (!::wxGetApp().m_frame->m_organ->doesHavePedals())
				manId += 1;
			int stopId = dlg.GetSelectedStop() + 1;
			int pipeId = dlg.GetSelectedPipe();
			int pipesToRef = 1; // At least one REF should be made
			int followingPipesFromDialog = dlg.GetFollowingPipes();
			if (followingPipesFromDialog > 0) {
				// limit of max pipes to reference can be either from how many were available in source
				// or how many pipes we actually have in this target rank
				pipesToRef += pipesAboveThis > followingPipesFromDialog ? followingPipesFromDialog : pipesAboveThis;
			}
			for (int i = 0; i < pipesToRef; i++) {
				wxString refString = wxT("REF:") + GOODF_functions::number_format(manId) + wxT(":") + GOODF_functions::number_format(stopId) + wxT(":") + GOODF_functions::number_format(pipeId + i);
				m_rank->clearPipeAt(pipeIndex + i);
				m_rank->getPipeAt(pipeIndex + i)->m_attacks.front().fileName = refString;
				m_rank->getPipeAt(pipeIndex + i)->m_attacks.front().fullPath = refString;
			}

			RebuildPipeTree();
			UpdatePipeTree();
		}
	}
}

void RankPanel::OnEditAttack() {
	wxTreeItemId selectedPipe = GetPipeOfSelection();
	int selectedPipeIndex = GetItemIndexRelativeParent(selectedPipe);
	Pipe *currentPipe = m_rank->getPipeAt(selectedPipeIndex);
	AttackDialog atk_dlg(currentPipe->m_attacks, (unsigned) GetSelectedItemIndexRelativeParent(), this);

	if (atk_dlg.ShowModal() == wxID_OK) {
		// the user wants to copy properties of the selected attack to other
		// attacks in the same directory
		auto sourceAttack = std::next(atk_dlg.m_attacklist.begin(), atk_dlg.m_selectedAttackIndex);
		wxString sourceDir = sourceAttack->fullPath.BeforeLast(wxFILE_SEP_PATH);
		for (Pipe &p : m_rank->m_pipes) {
			for (std::list<Attack>::iterator atk = p.m_attacks.begin(); atk != p.m_attacks.end(); ++atk) {
				if (atk->fullPath.BeforeLast(wxFILE_SEP_PATH).IsSameAs(sourceDir) && atk != sourceAttack) {
					atk->attackStart = sourceAttack->attackStart;
					atk->attackVelocity = sourceAttack->attackVelocity;
					atk->cuePoint = sourceAttack->cuePoint;
					atk->isTremulant = sourceAttack->isTremulant;
					atk->loadRelease = sourceAttack->loadRelease;
					atk->maxKeyPressTime = sourceAttack->maxKeyPressTime;
					atk->maxTimeSinceLastRelease = sourceAttack->maxTimeSinceLastRelease;
					atk->releaseEnd = sourceAttack->releaseEnd;
				}
			}
		}
	}
}

void RankPanel::OnEditRelease() {
	wxTreeItemId selectedPipe = GetPipeOfSelection();
	int selectedPipeIndex = GetItemIndexRelativeParent(selectedPipe);
	Pipe *currentPipe = m_rank->getPipeAt(selectedPipeIndex);
	ReleaseDialog dlg(currentPipe->m_releases, (unsigned) GetSelectedItemIndexRelativeParent(), this);

	if (dlg.ShowModal() == wxID_OK) {
		// the user wants to copy properties of the selected release to other
		// releases from the same directory
		Release *sourceRelease = dlg.GetCurrentRelease();
		wxString sourceDir = sourceRelease->fullPath.BeforeLast(wxFILE_SEP_PATH);
		for (Pipe &p : m_rank->m_pipes) {
			for (std::list<Release>::iterator rel = p.m_releases.begin(); rel != p.m_releases.end(); ++rel) {
				if (rel->fullPath.BeforeLast(wxFILE_SEP_PATH).IsSameAs(sourceDir) && &(*rel) != sourceRelease) {
					rel->cuePoint = sourceRelease->cuePoint;
					rel->isTremulant = sourceRelease->isTremulant;
					rel->maxKeyPressTime = sourceRelease->maxKeyPressTime;
					rel->releaseEnd = sourceRelease->releaseEnd;
				}
			}
		}

	}
}

void RankPanel::OnRemoveSelectedAttack() {
	int attackIndex = GetSelectedItemIndexRelativeParent();
	int pipeIndex = -1;

	wxTreeItemId pipe = GetPipeOfSelection();

	pipeIndex = GetItemIndexRelativeParent(pipe);

	if (attackIndex > -1 && pipeIndex > -1) {
		bool sucessfullyDeleted = m_rank->deleteAttackInPipe((unsigned) pipeIndex, (unsigned) attackIndex);

		if (!sucessfullyDeleted) {
			wxMessageDialog msg(this, wxT("Last attack for a pipe cannot be deleted!"), wxT("Couldn't delete attack!"), wxOK|wxCENTRE);
			msg.ShowModal();
		} else {
			RebuildPipeTree();
			UpdatePipeTree();
			wxTreeItemId toSelect = GetPipeTreeItemAt(pipeIndex);
			if (toSelect.IsOk()) {
				m_pipeTreeCtrl->SelectItem(toSelect);
				m_pipeTreeCtrl->ExpandAllChildren(toSelect);
			}
		}
	}
}

void RankPanel::OnRemoveSelectedRelease() {
	int releaseIndex = GetSelectedItemIndexRelativeParent();
	int pipeIndex = -1;

	wxTreeItemId pipe = GetPipeOfSelection();

	pipeIndex = GetItemIndexRelativeParent(pipe);

	if (releaseIndex > -1 && pipeIndex > -1)
		m_rank->deleteReleaseInPipe((unsigned) pipeIndex, (unsigned) releaseIndex);

	RebuildPipeTree();
	UpdatePipeTree();
	wxTreeItemId toSelect = GetPipeTreeItemAt(pipeIndex);
	if (toSelect.IsOk()) {
		m_pipeTreeCtrl->SelectItem(toSelect);
		m_pipeTreeCtrl->ExpandAllChildren(toSelect);
	}
}

wxTreeItemId RankPanel::GetPipeTreeItemAt(int index) {
	int pipesInTree = m_pipeTreeCtrl->GetChildrenCount(m_tree_rank_root, false);
	wxTreeItemIdValue cookie;
	for (int i = 0; i < pipesInTree; i++) {
		wxTreeItemId currentId;
		if (i == 0)
			currentId = m_pipeTreeCtrl->GetFirstChild(m_tree_rank_root, cookie);
		else
			currentId = m_pipeTreeCtrl->GetNextChild(m_tree_rank_root, cookie);

		if (i == index) {
			return currentId;
		}
	}
	return wxTreeItemId();
}

wxTreeItemId RankPanel::GetPipeOfSelection() {
	wxTreeItemId item = m_pipeTreeCtrl->GetSelection();
	wxTreeItemId parent;
	bool cont = true;
	while (cont) {
		parent = m_pipeTreeCtrl->GetItemParent(item);

		if (parent == m_tree_rank_root) {
			cont = false;
		} else {
			item = parent;
		}
	}
	return item;
}

void RankPanel::OnAmplitudeLevelSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_rank->setAmplitudeLevel(m_amplitudeLevelSpin->GetValue());
}

void RankPanel::OnGainSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_rank->setGain(m_gainSpin->GetValue());
}

void RankPanel::OnPitchTuningSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_rank->setPitchTuning(m_pitchTuningSpin->GetValue());
}

void RankPanel::OnTrackerDelaySpin(wxSpinEvent& WXUNUSED(event)) {
	m_rank->setTrackerDelay(m_trackerDelaySpin->GetValue());
}

void RankPanel::OnAddPipesBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString defaultPath;
	if (m_rank->getPipesRootPath() != wxEmptyString)
		defaultPath = m_rank->getPipesRootPath();
	else
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();

	wxDirDialog rankPipesPathDialog(
		this,
		wxT("Pick a directory to add pipe attack/releases from"),
		defaultPath,
		wxDD_DIR_MUST_EXIST
	);

	if (rankPipesPathDialog.ShowModal() == wxID_OK) {
		m_rank->setPipesRootPath(rankPipesPathDialog.GetPath());
		wxString extraAttackFolderPrefix = m_optionsAttackField->GetValue();
		bool loadOnlyOneAttack = m_optionsOnlyOneAttack->GetValue();
		bool loadRelease = m_optionsLoadReleaseInAttack->GetValue();
		wxString releaseFolderPrefix = m_optionsReleaseField->GetValue();
		bool extractKeyPressTime = m_optionsKeyPressTime->GetValue();
		wxString tremulantFolderPrefix = m_optionsTremulantField->GetValue();

		m_rank->addToPipes(
			extraAttackFolderPrefix,
			loadOnlyOneAttack,
			loadRelease,
			releaseFolderPrefix,
			extractKeyPressTime,
			tremulantFolderPrefix
		);

		RebuildPipeTree();
		UpdatePipeTree();
	}
}

void RankPanel::OnAddTremulantPipesBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString defaultPath;
	if (m_rank->getPipesRootPath() != wxEmptyString)
		defaultPath = m_rank->getPipesRootPath();
	else
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();

	wxDirDialog rankPipesPathDialog(
		this,
		wxT("Pick a directory to add (wave) tremulant attack/releases from"),
		defaultPath,
		wxDD_DIR_MUST_EXIST
	);

	if (rankPipesPathDialog.ShowModal() == wxID_OK) {
		m_rank->setPipesRootPath(rankPipesPathDialog.GetPath());
		wxString extraAttackFolderPrefix = m_optionsAttackField->GetValue();
		bool loadOnlyOneAttack = m_optionsOnlyOneAttack->GetValue();
		bool loadRelease = m_optionsLoadReleaseInAttack->GetValue();
		wxString releaseFolderPrefix = m_optionsReleaseField->GetValue();
		bool extractKeyPressTime = m_optionsKeyPressTime->GetValue();

		m_rank->addTremulantToPipes(
			extraAttackFolderPrefix,
			loadOnlyOneAttack,
			loadRelease,
			releaseFolderPrefix,
			extractKeyPressTime
		);

		RebuildPipeTree();
		UpdatePipeTree();
	}
}

void RankPanel::OnExpandTreeBtn(wxCommandEvent& WXUNUSED(event)) {
	m_pipeTreeCtrl->ExpandAll();
}

void RankPanel::OnAddReleaseSamplesBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString defaultPath;
	if (m_rank->getPipesRootPath() != wxEmptyString)
		defaultPath = m_rank->getPipesRootPath();
	else
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();

	wxDirDialog rankPipesPathDialog(
		this,
		wxT("Pick a directory to add release samples from"),
		defaultPath,
		wxDD_DIR_MUST_EXIST
	);

	if (rankPipesPathDialog.ShowModal() == wxID_OK) {
		m_rank->setPipesRootPath(rankPipesPathDialog.GetPath());

		m_rank->addReleasesToPipes();

		RebuildPipeTree();
		UpdatePipeTree();
	}
}
