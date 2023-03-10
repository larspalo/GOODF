/*
 * GoPanel.h is part of GOODF.
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

#ifndef GOPANEL_H
#define GOPANEL_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <list>
#include "GoImage.h"
#include "DisplayMetrics.h"
#include "GUIElements.h"
#include "Tremulant.h"
#include "DivisionalCoupler.h"
#include "ReversiblePiston.h"
#include "General.h"

class GoPanel {
public:
	GoPanel();
	~GoPanel();

	void write(wxTextFile *outFile, unsigned panelNbr);

	wxString getName();
	void setName(wxString name);
	wxString getGroup();
	void setGroup(wxString group);
	bool getHasPedals();
	void setHasPedals(bool hasPedals);
	unsigned getNumberOfImages();
	unsigned getIndexOfImage(GoImage *image);
	GoImage* getImageAt(unsigned index);
	void addImage(GoImage image);
	void removeImageAt(unsigned index);
	void removeImage(GoImage *image);
	DisplayMetrics* getDisplayMetrics();
	void addGuiElement(GUIElement *element);
	void removeGuiElementAt(unsigned index);
	int getNumberOfGuiElements();
	GUIElement* getGuiElementAt(unsigned index);
	bool hasItemAsGuiElement(Tremulant *trem);
	void removeItemFromPanel(Tremulant *trem);
	bool hasItemAsGuiElement(Enclosure *enclosure);
	void removeItemFromPanel(Enclosure *enclosure);
	bool hasItemAsGuiElement(Manual *manual);
	void removeItemFromPanel(Manual *manual);
	bool hasItemAsGuiElement(Stop *stop);
	void removeItemFromPanel(Stop *stop);
	bool hasItemAsGuiElement(Coupler *coupler);
	void removeItemFromPanel(Coupler *coupler);
	bool hasItemAsGuiElement(Divisional *divisional);
	void removeItemFromPanel(Divisional *divisional);
	bool hasItemAsGuiElement(GoSwitch *sw);
	void removeItemFromPanel(GoSwitch *sw);
	bool hasItemAsGuiElement(DivisionalCoupler *divCplr);
	void removeItemFromPanel(DivisionalCoupler *divCplr);
	bool hasItemAsGuiElement(ReversiblePiston *revPist);
	void removeItemFromPanel(ReversiblePiston *revPist);
	bool hasItemAsGuiElement(General *general);
	void removeItemFromPanel(General *general);
	void updateGuiElementsDisplayNames();

private:
	wxString m_name;
	wxString m_group;
	bool m_hasPedals;
	std::list<GoImage> m_images;
	DisplayMetrics m_displayMetrics;
	std::list<GUIElement*> m_guiElements;

};

#endif
