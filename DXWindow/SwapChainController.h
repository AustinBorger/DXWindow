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

class SwapChainController {
public:
	SwapChainController(CDXWindow& Window, OutputEnum& Enum);

	HRESULT Initialize(CComPtr<IUnknown> DeviceUnk, CComPtr<IDXWindowCallback> Callback, HWND Handle);

	HRESULT ToggleFullscreen();

	HRESULT ResizeBuffers();

	HRESULT GetDesktopArea(RECT& DesktopArea);

	HRESULT Present(UINT SyncInterval, UINT Flags);

	HRESULT GetBackBuffer(REFIID rIID, void** ppvBackBuffer);

private:
	CDXWindow& m_Window;
	CComPtr<IDXWindowCallback> m_Callback;
	CComPtr<IDXGISwapChain> m_SwapChain;
	HWND m_Handle;
	OutputEnum& m_OutputEnum;

	BOOL IsFullscreen() {
		BOOL Fullscreen;

		m_SwapChain->GetFullscreenState (
			&Fullscreen,
			nullptr
		);

		return Fullscreen;
	}

	HRESULT CreateSwapChain(CComPtr<IUnknown> DeviceUnk);
};