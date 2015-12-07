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
#include <d3d11.h>
#include "CDXWindow.h"

#ifdef _DXWINDOW_SUPPORT_12

#include <d3d12.h>
#include "CDXWindow12.h"

#endif

#ifdef _DEBUG

#include <iostream>

#endif

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

	void* pv = nullptr;

	hr = pDevice->QueryInterface(__uuidof(ID3D11Device), &pv);

	if (SUCCEEDED(hr)) {
		// CONSOLE OUTPUT

#ifdef _DEBUG

		std::wcout << "DXWindowCreateWindow():" << std::endl;
		std::wcout << "\t" << "Direct3D 11 Device detected.  Using DirectX 11." << std::endl;

#endif

		pDevice->Release(); //reduce refcount after call to QueryInterface

		CComPtr<CDXWindow> Window = new CDXWindow();

		hr = Window->Initialize(*pDesc, pDevice, pDXWindowCallback);

		if (FAILED(hr)) {
			*ppDXWindow = nullptr;
			return hr;
		}

		*ppDXWindow = Window;

		return S_OK;
	}

#ifdef _DXWINDOW_SUPPORT_12

	hr = pDevice->QueryInterface(__uuidof(ID3D12CommandQueue), &pv);

	if (SUCCEEDED(hr)) {
		// CONSOLE OUTPUT

#ifdef _DEBUG

		std::wcout << "DXWindowCreateWindow():" << std::endl;
		std::wcout << "\t" << "Direct3D 12 Command Queue detected.  Using DirectX 12." << std::endl;

#endif

		pDevice->Release(); //reduce refcount after call to QueryInterface

		CComPtr<CDXWindow12> Window = new CDXWindow12();

		hr = Window->Initialize(*pDesc, pDevice, pDXWindowCallback);

		if (FAILED(hr)) {
			*ppDXWindow = nullptr;
			return hr;
		}

		*ppDXWindow = Window;

		return S_OK;
	}

#endif

	return E_INVALIDARG;
}