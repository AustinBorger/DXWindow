/*
** Copyright (C) 2015 Austin Borger <aaborger@gmail.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
** API documentation is available here:
**		https://github.com/AustinBorger/DXWindow
*/

#pragma once

#include <Windows.h>
#include <comdef.h>
#include <atlbase.h>
#include <dxgi.h>
#include <vector>

#include "DXWindow.h"
#include "Output.h"
#include "QueryInterface.h"

/* This class acts as a collection of all the outputs on the device's adapter. */
class OutputEnum {
public:
	OutputEnum();

	~OutputEnum();

	/* Populates the output list and initializes callback reference. */
	HRESULT Initialize(CComPtr<IUnknown> DeviceUnk, CComPtr<IDXWindowCallback> Callback);

	/* Searches the output list for the output that the window occupies. */
	Output* SearchOutput(HWND Handle);

	/* Returns the primary output of the adapter. */
	Output* PrimaryOutput();

	/* Returns the adapter. */
	CComPtr<IDXGIAdapter> GetAdapter() {
		return m_Adapter;
	}

private:
	std::vector<Output> m_Outputs; //The output list
	CComPtr<IDXGIAdapter> m_Adapter; //The adapter
	CComPtr<IDXWindowCallback> m_Callback; //The application-supplied callback
};