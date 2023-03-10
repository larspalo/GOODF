/*
 * GOODFFunctions.h is part of GOODF.
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

#ifndef GOODF_FUNCTIONS_H
#define GOODF_FUNCTIONS_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/filename.h>
#include <vector>
#include "GOODF.h"

namespace GOODF_functions {

	inline wxString number_format(int number) {
		 wxString str = wxString::Format(wxT("%0.3d"), number);
		 return str;
	}

	inline void writeReferences(wxTextFile *outFile, wxString elementName, std::vector<int> list) {
		for (unsigned k = 0; k < list.size(); k++) {
			wxString refNumber = wxString::Format(wxT("%i"), list[k]);
			wxString outStr = elementName + number_format(k + 1) + wxT("=") + refNumber;
			outFile->AddLine(outStr);
		}
	}

	inline void CheckForStartingWhitespace(wxString *str, wxTextCtrl *ctrl) {
		if (str->StartsWith(wxT(" "))) {
			str->Replace(wxT(" "), wxT(""), false);
			ctrl->SetValue(*str);
		}
	}

	inline wxString fixSeparator(wxString pathToCheck) {
		if (pathToCheck.Contains("/")) {
			pathToCheck.Replace("/", "\\");
		}

		return pathToCheck;
	}

	inline wxString removeBaseOdfPath(wxString path) {
		wxString stringToReturn = path;
		if (stringToReturn != wxEmptyString) {
			stringToReturn.Replace(::wxGetApp().m_frame->m_organ->getOdfRoot(), wxT(""));
			if (stringToReturn.StartsWith(wxFILE_SEP_PATH))
				stringToReturn.erase(0, 1);
		}
		return stringToReturn;
	}

	inline wxString checkIfFileExist(wxString relativePath) {
		if (relativePath != wxEmptyString) {
			wxString fullFilePath = ::wxGetApp().m_frame->m_organ->getOdfRoot() + wxFILE_SEP_PATH + relativePath;
			wxFileName theFile = wxFileName(fullFilePath);
			if (theFile.FileExists()) {
				return theFile.GetFullPath();
			}
		}
		return wxEmptyString;
	}

	inline bool parseBoolean(wxString value, bool defaultValue = true) {
		if (value.IsSameAs(wxT("Y"), false))
			return true;
		if (value.IsSameAs(wxT("N"), false))
			return false;
		return defaultValue;
	}
}

#endif

