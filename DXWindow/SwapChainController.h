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

#include "DXWindow.h"
#include "Output.h"
#include "OutputEnum.h"

class CDXWindow;

/* This class deals with the swap chain. */
class SwapChainController {
public:
	/* Initializes references. */
	SwapChainController(CDXWindow& Window, OutputEnum& Enum);

	/* Creates the swap chain and initializes references. */
	HRESULT Initialize(CComPtr<IUnknown> DeviceUnk, CComPtr<IDXWindowCallback> Callback, HWND Handle);

	/* Toggles between exclusive fullscreen. */
	HRESULT ToggleFullscreen();

	/* Resizes the buffers in response to a target or window resize. */
	HRESULT ResizeBuffers();

	/* Flips the back and front buffers. */
	HRESULT Present(UINT SyncInterval, UINT Flags);

	/* Retrieves the back buffer in the requested interface. */
	HRESULT GetBackBuffer(REFIID rIID, void** ppvBackBuffer);

private:
	CDXWindow& m_Window; //Reference to the window object
	CComPtr<IDXWindowCallback> m_Callback; //Reference to the application-supplied callback
	CComPtr<IDXGISwapChain> m_SwapChain; //Our swap chain
	HWND m_Handle; //The window handle (assigned by CDXWindow)
	OutputEnum& m_OutputEnum; //Reference to the output enumeration

	/* Indicates whether or not the swap chain is in exclusive fullscreen mode. */
	BOOL IsFullscreen() {
		BOOL Fullscreen;

		m_SwapChain->GetFullscreenState (
			&Fullscreen,
			nullptr
		);

		return Fullscreen;
	}

	/* Creates the swap chain. */
	HRESULT CreateSwapChain(CComPtr<IUnknown> DeviceUnk);
};