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

#include "DXWindow.h"
#include <atlbase.h>
#include "CDXWindow.h"

#pragma comment(lib, "XInput9_1_0.lib")
#pragma comment(lib, "dxgi.lib")

//Create the window
HRESULT DXWindowCreateWindow (
	const DXWINDOW_DESC* pDesc,
	IUnknown* pDevice,
	IDXWindowCallback* pDXWindowCallback,
	IDXWindow** ppDXWindow
) {
	HRESULT hr = S_OK;

	if (pDesc == nullptr || pDevice == nullptr ||
		pDXWindowCallback == nullptr || ppDXWindow == nullptr) {
		return E_POINTER;
	}

	CComPtr<CDXWindow> Window = new CDXWindow();

	Window->Initialize(*pDesc, pDevice, pDXWindowCallback);

	if (FAILED(hr)) {
		*ppDXWindow = nullptr;
		return hr;
	}

	*ppDXWindow = Window;

	return S_OK;
}