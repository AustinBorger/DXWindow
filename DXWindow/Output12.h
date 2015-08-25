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

#include <dxgi1_4.h>
#include <comdef.h>
#include <atlbase.h>
#include <vector>

#include "DXWindow.h"

/* This class represents a DXGI output and contains several convenience methods
** for translating DXGI stuff into regular winapi stuff. */
class Output12 final {
public:
	/* Sets all the description variables to zero. */
	Output12();

	/* Initializes the description and DXGI object. */
	VOID Initialize(CComPtr<IDXGIOutput> obj, CComPtr<IDXWindowCallback> Callback);

	/* Returns a rect specifying the position of a centered window given its
	** dimensions and style. */
	VOID GetWindowCenter(UINT Width, UINT Height, RECT* pRect, DWORD dwStyle, DWORD dwExStyle);

	/* Returns the desktop work area of the output. */
	VOID GetWorkArea(RECT* pRect);

	/* Returns the desktop area of the output. */
	VOID GetDesktopArea(RECT* pRect);

	/* Returns the output's monitor handle. */
	HMONITOR GetMonitor() {
		return m_Desc.Monitor;
	}

	/* Returns the device name of the output. */
	LPCWSTR GetName() {
		return m_Desc.DeviceName;
	}

	/* Returns the associated DXGIOutput object. */
	CComPtr<IDXGIOutput4> GetObj() {
		return m_Obj;
	}

private:
	CComPtr<IDXGIOutput4> m_Obj; //The associated DXGIOutput object
	CComPtr<IDXWindowCallback> m_Callback; //The application-supplied callback
	DXGI_OUTPUT_DESC m_Desc; //The output's description
};