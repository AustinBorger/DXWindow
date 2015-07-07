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

#include "DXWindow.h"
#include <dxgi.h>
#include <string>

#include "WindowMessageDispatcher.h"
#include "GamepadMessageDispatcher.h"
#include "SwapChainController.h"
#include "QueryInterface.h"

/* This is the final implementation of IDXWindow. */
class CDXWindow : public IDXWindow {
public:
	CDXWindow();

	~CDXWindow();

	//IUnknown methods

	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) final {
		QUERY_INTERFACE_CAST(IDXWindow);
		QUERY_INTERFACE_CAST(IUnknown);
		QUERY_INTERFACE_FAIL();
	}

	ULONG STDMETHODCALLTYPE AddRef() {
		//No thread safety protection.
		return ++m_RefCount;
	}

	ULONG STDMETHODCALLTYPE Release() {
		//No thread safety protection.
		m_RefCount--;

		if (m_RefCount <= 0) {
			delete this;
			return 0;
		}

		return m_RefCount;
	}

	//IDXWindow methods

	/* Sends all relevant messages to the application. */
	VOID STDMETHODCALLTYPE PumpMessages() final;

	/* Flips the back buffer of the swap chain. */
	VOID STDMETHODCALLTYPE Present(UINT SyncInterval, UINT Flags) final;

	/* Returns the width of the window in windowed or borderless mode. */
	WORD STDMETHODCALLTYPE GetWindowWidth() final {
		return m_WindowWidth;
	}

	/* Returns the height of the window in windowed or borderless mode. */
	WORD STDMETHODCALLTYPE GetWindowHeight() final {
		return m_WindowHeight;
	}

	/* Sets the dimensions of the window in windowed or borderless mode. */
	VOID STDMETHODCALLTYPE SetWindowResolution(WORD Width, WORD Height) final;

	/* Returns the state of the window. */
	DXWINDOW_STATE STDMETHODCALLTYPE GetState() final {
		return m_State;
	}

	/* Sets the state of the window. */
	VOID STDMETHODCALLTYPE SetState(DXWINDOW_STATE State) final;

	/* Indicates whether or not F11 toggles between fullscreen and windowed. */
	BOOL STDMETHODCALLTYPE GetAllowToggle() final {
		return m_AllowToggle;
	}

	/* Sets the window's ability to toggle between fullscreen and windowed on F11. */
	VOID STDMETHODCALLTYPE SetAllowToggle(BOOL AllowToggle) final {
		m_AllowToggle = AllowToggle;
	}

	/* Returns the back buffer in the requested interface. */
	VOID STDMETHODCALLTYPE GetBackBuffer(REFIID rIID, void** ppvBackBuffer) final;

	//New methods

	/* The WndProc, which deals with the remaining messages not handled by WindowMessageDispatcher. */
	LRESULT WindowProcess(UINT Message, WPARAM wParam, LPARAM lParam);

	/* Creates all objects and sets up the window/swap chain. */
	HRESULT Initialize(const DXWINDOW_DESC& Desc, IUnknown* pDevice, IDXWindowCallback* pDXWindowCallback);

private:
	long m_RefCount; //The reference count

	HWND m_Handle; //The window handle
	HINSTANCE m_Instance; //The application's instance handle
	OutputEnum m_OutputEnum; //The output enum object
	WindowMessageDispatcher m_WindowMessageDispatcher; //Dispatches window messages to the callback
	GamepadMessageDispatcher m_GamepadMessageDispatcher; //Dispatches gamepad messages to the callback
	SwapChainController m_SwapChainController; //The swap chain controller object

	WORD m_WindowWidth; //The width of the window in windowed or borderless
	WORD m_WindowHeight; //The height of the window in windowed or borderless

	CComPtr<IDXWindowCallback> m_Callback; //The application-supplied callback object

	DXWINDOW_STATE m_State; //The state of the window
	DXWINDOW_FULLSCREEN_STATE m_FullscreenState; //The state of the window when in fullscreen
	DXWINDOW_WINDOW_STATE m_WindowState; //The state of the window when in windowed

	DWORD m_WindowStyle; //The style of the window
	DWORD m_WindowExStyle; //The ex style of the window

	BOOL m_Closing; //Closing flag
	BOOL m_AllowToggle; //Toggle flag
	BOOL m_InFocus; //Indicates whether or not the window is currently in focus

	std::wstring m_ClassName; //The name of the window class

	/* Registers the window class with the Windows server. */
	HRESULT RegisterWindowClass(const DXWINDOW_DESC& Desc);

	/* Creates the window. */
	HRESULT MakeWindow(const DXWINDOW_DESC& Desc);

	/* Centers the window on the desktop. */
	VOID CenterWindow();

	/* Centers the cursor within the window. */
	VOID CenterCursor();

	/* Updates the look of the window based on the current window state. */
	VOID UpdateWindowState();

	/* Reacts to a WM_KILLFOCUS message. */
	VOID KillFocus();

	/* Reacts to a WM_SETFOCUS message. */
	VOID SetFocus();

	/* Toggles between the specified fullscreen mode and the specified window mode. */
	VOID ToggleFullscreen();

	/* Sets the window to fullscreen window mode. */
	VOID FullscreenWindow();

	/* Reacts to a WM_DISPLAYCHANGE message */
	VOID DisplayChange();

	/* Retrieves the desktop area of the output the window occupies. */
	VOID GetDesktopArea(RECT& DesktopArea);
};