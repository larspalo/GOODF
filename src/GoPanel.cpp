/*
 * GoPanel.cpp is part of GOODF.
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

#include "GoPanel.h"
#include "GOODFFunctions.h"
#include "GUITremulant.h"
#include "GUIStop.h"
#include "GUICoupler.h"
#include "GUIDivisional.h"
#include "GUISwitch.h"
#include "GUIDivisionalCoupler.h"
#include "GUIReversiblePiston.h"
#include "GUIGeneral.h"

GoPanel::GoPanel() {
	m_name = wxT("New Panel");
	m_group = wxEmptyString;
	m_hasPedals = false;
}

GoPanel::~GoPanel() {
	m_guiElements.remove_if([](GUIElement *element){delete element; return true;});
}

void GoPanel::write(wxTextFile *outFile, unsigned panelNbr) {
	if (panelNbr > 0) {
		outFile->AddLine(wxT("Name=") + m_name);
		if (m_group != wxEmptyString)
			outFile->AddLine(wxT("Group=") + m_group);
	}
	if (m_hasPedals)
		outFile->AddLine(wxT("HasPedals=Y"));
	else
		outFile->AddLine(wxT("HasPedals=N"));
	unsigned nbImages = getNumberOfImages();
	outFile->AddLine(wxT("NumberOfImages=") + wxString::Format(wxT("%u"), nbImages));

	unsigned nbGUIElements = m_guiElements.size();
	outFile->AddLine(wxT("NumberOfGUIElements=") + wxString::Format(wxT("%u"), nbGUIElements));

	m_displayMetrics.write(outFile);
	outFile->AddLine(wxT(""));

	unsigned i = 1;
	// images
	for (auto& img :  m_images) {
		wxString imgId = wxT("[Panel") + GOODF_functions::number_format(panelNbr) + wxT("Image") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(imgId);
		img.write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	i = 1;
	// GUI Elements
	for (GUIElement* gui : m_guiElements) {
		wxString guiId = wxT("[Panel") + GOODF_functions::number_format(panelNbr) + wxT("Element") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(guiId);
		gui->write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}
}

wxString GoPanel::getName() {
	return m_name;
}

void GoPanel::setName(wxString name) {
	m_name = name;
}

wxString GoPanel::getGroup() {
	return m_group;
}

void GoPanel::setGroup(wxString group) {
	m_group = group;
}

bool GoPanel::getHasPedals() {
	return m_hasPedals;
}

void GoPanel::setHasPedals(bool hasPedals) {
	m_hasPedals = hasPedals;
}

unsigned GoPanel::getNumberOfImages() {
	return m_images.size();
}

unsigned GoPanel::getIndexOfImage(GoImage *image) {
	unsigned i = 0;
	bool found = false;
	for (auto& img : m_images) {
		i++;
		if (&img == image) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

GoImage* GoPanel::getImageAt(unsigned index) {
	auto iterator = std::next(m_images.begin(), index);
	return &(*iterator);
}

void GoPanel::addImage(GoImage image) {
	m_images.push_back(image);
}

void GoPanel::removeImageAt(unsigned index) {
	std::list<GoImage>::iterator it = m_images.begin();
	std::advance(it, index);
	m_images.erase(it);
}

void GoPanel::removeImage(GoImage *image) {
	unsigned index = 0;
	for (GoImage& i : m_images) {
		if (&i == image) {
			removeImageAt(index);
			break;
		}
		index++;
	}
}

DisplayMetrics* GoPanel::getDisplayMetrics() {
	return &m_displayMetrics;
}

void GoPanel::addGuiElement(GUIElement *element) {
	m_guiElements.push_back(element);
}

void GoPanel::removeGuiElementAt(unsigned index) {
	std::list<GUIElement*>::iterator it = m_guiElements.begin();
	std::advance(it, index);
	delete *it;
	m_guiElements.erase(it);
}

int GoPanel::getNumberOfGuiElements() {
	return m_guiElements.size();
}

GUIElement* GoPanel::getGuiElementAt(unsigned index) {
	auto iterator = std::next(m_guiElements.begin(), index);
	return *iterator;
}

bool GoPanel::hasItemAsGuiElement(Tremulant* trem) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Tremulant")) {
			GUITremulant *tr = dynamic_cast<GUITremulant*>(e);
			if (tr) {
				if (tr->isReferencing(trem))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(Tremulant* trem) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("Tremulant")) {
			GUITremulant *tr = dynamic_cast<GUITremulant*>(*it);
			if (tr) {
				if (tr->isReferencing(trem)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(Enclosure* enclosure) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Enclosure")) {
			GUIEnclosure *enc = dynamic_cast<GUIEnclosure*>(e);
			if (enc) {
				if (enc->isReferencing(enclosure))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(Enclosure* enclosure) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("Enclosure")) {
			GUIEnclosure *enc = dynamic_cast<GUIEnclosure*>(*it);
			if (enc) {
				if (enc->isReferencing(enclosure)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(Manual *manual) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Manual")) {
			GUIManual *man = dynamic_cast<GUIManual*>(e);
			if (man) {
				if (man->isReferencing(manual))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(Manual *manual) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("Manual")) {
			GUIManual *man = dynamic_cast<GUIManual*>(*it);
			if (man) {
				if (man->isReferencing(manual)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(Stop *stop) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Stop")) {
			GUIStop *st = dynamic_cast<GUIStop*>(e);
			if (st) {
				if (st->isReferencing(stop))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(Stop *stop) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("Stop")) {
			GUIStop *st = dynamic_cast<GUIStop*>(*it);
			if (st) {
				if (st->isReferencing(stop)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(Coupler *coupler) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Coupler")) {
			GUICoupler *cplr = dynamic_cast<GUICoupler*>(e);
			if (cplr) {
				if (cplr->isReferencing(coupler))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(Coupler *coupler) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("Coupler")) {
			GUICoupler *cplr = dynamic_cast<GUICoupler*>(*it);
			if (cplr) {
				if (cplr->isReferencing(coupler)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(Divisional *divisional) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Divisional")) {
			GUIDivisional *div = dynamic_cast<GUIDivisional*>(e);
			if (div) {
				if (div->isReferencing(divisional))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(Divisional *divisional) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("Divisional")) {
			GUIDivisional *div = dynamic_cast<GUIDivisional*>(*it);
			if (div) {
				if (div->isReferencing(divisional)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(GoSwitch *sw) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Switch")) {
			GUISwitch *s = dynamic_cast<GUISwitch*>(e);
			if (s) {
				if (s->isReferencing(sw))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(GoSwitch *sw) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("Switch")) {
			GUISwitch *s = dynamic_cast<GUISwitch*>(*it);
			if (s) {
				if (s->isReferencing(sw)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(DivisionalCoupler *divCplr) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("DivisionalCoupler")) {
			GUIDivisionalCoupler *div = dynamic_cast<GUIDivisionalCoupler*>(e);
			if (div) {
				if (div->isReferencing(divCplr))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(DivisionalCoupler *divCplr) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("DivisionalCoupler")) {
			GUIDivisionalCoupler *div = dynamic_cast<GUIDivisionalCoupler*>(*it);
			if (div) {
				if (div->isReferencing(divCplr)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(ReversiblePiston *revPist) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("ReversiblePiston")) {
			GUIReversiblePiston *p = dynamic_cast<GUIReversiblePiston*>(e);
			if (p) {
				if (p->isReferencing(revPist))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(ReversiblePiston *revPist) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("ReversiblePiston")) {
			GUIReversiblePiston *p = dynamic_cast<GUIReversiblePiston*>(*it);
			if (p) {
				if (p->isReferencing(revPist)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(General *general) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("General")) {
			GUIGeneral *g = dynamic_cast<GUIGeneral*>(e);
			if (g) {
				if (g->isReferencing(general))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(General *general) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("General")) {
			GUIGeneral *g = dynamic_cast<GUIGeneral*>(*it);
			if (g) {
				if (g->isReferencing(general)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

void GoPanel::updateGuiElementsDisplayNames() {
	for (GUIElement* e : m_guiElements) {
		e->updateDisplayName();
	}
}
