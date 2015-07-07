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

/* Controls the way the window appears and behaves. */
enum DXWINDOW_STATE {
	DXWINDOW_STATE_FULLSCREEN = 1, //Exclusive fullscreen
	DXWINDOW_STATE_FULLSCREEN_WINDOW, //Non-exclusive fullscreen window
	DXWINDOW_STATE_WINDOWED, //Movable window with border
	DXWINDOW_STATE_BORDERLESS //Non-movable window with no border
};

/* DXWINDOW_STATE restricted to fullscreen modes. */
enum DXWINDOW_FULLSCREEN_STATE {
	DXWINDOW_FULLSCREEN_STATE_FULLSCREEN = DXWINDOW_STATE_FULLSCREEN,
	DXWINDOW_FULLSCREEN_STATE_FULLSCREEN_WINDOW = DXWINDOW_STATE_FULLSCREEN_WINDOW
};

/* DXWINDOW_STATE restricted to windowed modes. */
enum DXWINDOW_WINDOW_STATE {
	DXWINDOW_WINDOW_STATE_WINDOWED = DXWINDOW_STATE_WINDOWED,
	DXWINDOW_WINDOW_STATE_BORDERLESS = DXWINDOW_STATE_BORDERLESS
};

/* Used when creating the window. */
struct DXWINDOW_DESC {
	LPCWSTR IconSmall; //The small window icon
	LPCWSTR IconLarge; //The large window icon
	LPCWSTR Cursor; //The cursor
	LPCWSTR Title; //The title of the window
	HINSTANCE Instance; //hInstance from process entry
	DXWINDOW_FULLSCREEN_STATE FullscreenState; //The fullscreen type to use
	DXWINDOW_WINDOW_STATE WindowState; //The window type to use
	WORD Width; //The width of the window in windowed or borderless
	WORD Height; //The height of the window in windowed or borderless
	BOOL InitFullscreen; //Whether or not to immediately go to fullscreen
	BOOL AllowToggle; //Whether or not to allow toggle between fullscreen and windowed via F11
};

struct IDXWindow;

/* Used as a callback mechanism for window and gamepad messages.  Also used for error reporting. */
struct __declspec(uuid("af69111a-a1f8-4cd1-afd3-94abdcfb011c")) IDXWindowCallback : public IUnknown {
	//Error callbacks

	/* Called when an error has occured anywhere within the API. */
	virtual VOID STDMETHODCALLTYPE OnObjectFailure(LPCWSTR File, UINT Line, HRESULT hr) PURE;

	//Gamepad callbacks

	/* Called when a button on the gamepad is pressed. */
	virtual VOID STDMETHODCALLTYPE OnGamepadButtonDown(IDXWindow* pDXWindow, DWORD dwUserIndex, WORD wButton) PURE;

	/* Called when a button on the gamepad is released. */
	virtual VOID STDMETHODCALLTYPE OnGamepadButtonUp(IDXWindow* pDXWindow, DWORD dwUserIndex, WORD wButton) PURE;

	/* Called when the left analog stick on the gamepad changes position. */
	virtual VOID STDMETHODCALLTYPE OnGamepadLeftAnalogMove(IDXWindow* pDXWindow, DWORD dwUserIndex, SHORT xPos, SHORT yPos) PURE;

	/* Called when the right analog stick on the gamepad changes position. */
	virtual VOID STDMETHODCALLTYPE OnGamepadRightAnalogMove(IDXWindow* pDXWindow, DWORD dwUserIndex, SHORT xPos, SHORT yPos) PURE;

	/* Called when the left trigger on the gamepad changes position. */
	virtual VOID STDMETHODCALLTYPE OnGamepadLeftTriggerMove(IDXWindow* pDXWindow, DWORD dwUserIndex, BYTE Val) PURE;

	/* Called when the right trigger on the gamepad changes position. */
	virtual VOID STDMETHODCALLTYPE OnGamepadRightTriggerMove(IDXWindow* pDXWindow, DWORD dwUserIndex, BYTE Val) PURE;

	//Keyboard callbacks

	/* Called when a key on the keyboard is pressed. */
	virtual VOID STDMETHODCALLTYPE OnKeyDown(IDXWindow* pDXWindow, WPARAM Key, LPARAM lParam) PURE;

	/* Called when a key on the keyboard is released. */
	virtual VOID STDMETHODCALLTYPE OnKeyUp(IDXWindow* pDXWindow, WPARAM Key, LPARAM lParam) PURE;

	//Mouse callbacks

	/* Called when the left mouse button is clicked. */
	virtual VOID STDMETHODCALLTYPE OnMouseLButtonDown(IDXWindow* pDXWindow, WORD Flags, WORD xPos, WORD yPos) PURE;

	/* Called when the left mouse button is released. */
	virtual VOID STDMETHODCALLTYPE OnMouseLButtonUp(IDXWindow* pDXWindow, WORD Flags, WORD xPos, WORD yPos) PURE;

	/* Called when the right mouse button is clicked. */
	virtual VOID STDMETHODCALLTYPE OnMouseRButtonDown(IDXWindow* pDXWindow, WORD Flags, WORD xPos, WORD yPos) PURE;

	/* Called when the right mouse button is released. */
	virtual VOID STDMETHODCALLTYPE OnMouseRButtonUp(IDXWindow* pDXWindow, WORD Flags, WORD xPos, WORD yPos) PURE;

	/* Called when the middle mouse button is clicked. */
	virtual VOID STDMETHODCALLTYPE OnMouseMButtonDown(IDXWindow* pDXWindow, WORD Flags, WORD xPos, WORD yPos) PURE;

	/* Called when the middle mouse button is released. */
	virtual VOID STDMETHODCALLTYPE OnMouseMButtonUp(IDXWindow* pDXWindow, WORD Flags, WORD xPos, WORD yPos) PURE;

	/* Called when the mouse is moved. */
	virtual VOID STDMETHODCALLTYPE OnMouseMove(IDXWindow* pDXWindow, WORD Flags, WORD xPos, WORD yPos) PURE;

	/* Called when the mouse wheel is moved. */
	virtual VOID STDMETHODCALLTYPE OnMouseWheel(IDXWindow* pDXWindow, WORD Flags, SHORT Delta, WORD xPos, WORD yPos) PURE;

	//Window state callbacks

	/* Called when the window is no longer visible. */
	virtual VOID STDMETHODCALLTYPE OnWindowHide(IDXWindow* pDXWindow) PURE;

	/* Called when the window is becoming visible. */
	virtual VOID STDMETHODCALLTYPE OnWindowShow(IDXWindow* pDXWindow) PURE;

	/* Called when the window is closing. */
	virtual VOID STDMETHODCALLTYPE OnWindowClose(IDXWindow* pDXWindow) PURE;

	//Back buffer callbacks

	/* Called when a new back buffer is being created. */
	virtual VOID STDMETHODCALLTYPE OnBackBufferCreate(IDXWindow* pDXWindow) PURE;

	/* Called when the back buffer is being released on resize */
	virtual VOID STDMETHODCALLTYPE OnBackBufferRelease(IDXWindow* pDXWindow) PURE;
};

#ifndef _DXWINDOW_DLL_PROJECT

/* Convenience class that pre-defines all callback methods as empty. */
class CDXWindowCallback : public IDXWindowCallback {
public:
	//Gamepad callbacks

	/* Called when a button on the gamepad is pressed. */
	virtual VOID STDMETHODCALLTYPE OnGamepadButtonDown(IDXWindow* pDXWindow, DWORD dwUserIndex, WORD wButton) { }

	/* Called when a button on the gamepad is released. */
	virtual VOID STDMETHODCALLTYPE OnGamepadButtonUp(IDXWindow* pDXWindow, DWORD dwUserIndex, WORD wButton) { }

	/* Called when the left analog stick on the gamepad changes position. */
	virtual VOID STDMETHODCALLTYPE OnGamepadLeftAnalogMove(IDXWindow* pDXWindow, DWORD dwUserIndex, SHORT xPos, SHORT yPos) { }

	/* Called when the right analog stick on the gamepad changes position. */
	virtual VOID STDMETHODCALLTYPE OnGamepadRightAnalogMove(IDXWindow* pDXWindow, DWORD dwUserIndex, SHORT xPos, SHORT yPos) { }

	/* Called when the left trigger on the gamepad changes position. */
	virtual VOID STDMETHODCALLTYPE OnGamepadLeftTriggerMove(IDXWindow* pDXWindow, DWORD dwUserIndex, BYTE Val) { }

	/* Called when the right trigger on the gamepad changes position. */
	virtual VOID STDMETHODCALLTYPE OnGamepadRightTriggerMove(IDXWindow* pDXWindow, DWORD dwUserIndex, BYTE Val) { }

	//Keyboard callbacks

	/* Called when a key on the keyboard is pressed. */
	virtual VOID STDMETHODCALLTYPE OnKeyDown(IDXWindow* pDXWindow, WPARAM Key, LPARAM lParam) { }

	/* Called when a key on the keyboard is released. */
	virtual VOID STDMETHODCALLTYPE OnKeyUp(IDXWindow* pDXWindow, WPARAM Key, LPARAM lParam) { }

	//Mouse callbacks

	/* Called when the left mouse button is clicked. */
	virtual VOID STDMETHODCALLTYPE OnMouseLButtonDown(IDXWindow* pDXWindow, WORD Flags, WORD xPos, WORD yPos) { }

	/* Called when the left mouse button is released. */
	virtual VOID STDMETHODCALLTYPE OnMouseLButtonUp(IDXWindow* pDXWindow, WORD Flags, WORD xPos, WORD yPos) { }

	/* Called when the right mouse button is clicked. */
	virtual VOID STDMETHODCALLTYPE OnMouseRButtonDown(IDXWindow* pDXWindow, WORD Flags, WORD xPos, WORD yPos) { }

	/* Called when the right mouse button is released. */
	virtual VOID STDMETHODCALLTYPE OnMouseRButtonUp(IDXWindow* pDXWindow, WORD Flags, WORD xPos, WORD yPos) { }

	/* Called when the middle mouse button is clicked. */
	virtual VOID STDMETHODCALLTYPE OnMouseMButtonDown(IDXWindow* pDXWindow, WORD Flags, WORD xPos, WORD yPos) { }

	/* Called when the middle mouse button is released. */
	virtual VOID STDMETHODCALLTYPE OnMouseMButtonUp(IDXWindow* pDXWindow, WORD Flags, WORD xPos, WORD yPos) { }

	/* Called when the mouse is moved. */
	virtual VOID STDMETHODCALLTYPE OnMouseMove(IDXWindow* pDXWindow, WORD Flags, WORD xPos, WORD yPos) { }

	/* Called when the mouse wheel is moved. */
	virtual VOID STDMETHODCALLTYPE OnMouseWheel(IDXWindow* pDXWindow, WORD Flags, SHORT Delta, WORD xPos, WORD yPos) { }

	//Window state callbacks

	/* Called when the window is no longer visible. */
	virtual VOID STDMETHODCALLTYPE OnWindowHide(IDXWindow* pDXWindow) { }

	/* Called when the window is becoming visible. */
	virtual VOID STDMETHODCALLTYPE OnWindowShow(IDXWindow* pDXWindow) { }

	/* Called when the window is closing. */
	virtual VOID STDMETHODCALLTYPE OnWindowClose(IDXWindow* pDXWindow) { }

	//Back buffer callbacks

	/* Called when a new back buffer is being created. */
	virtual VOID STDMETHODCALLTYPE OnBackBufferCreate(IDXWindow* pDXWindow) { }

	/* Called when the back buffer is being released on resize */
	virtual VOID STDMETHODCALLTYPE OnBackBufferRelease(IDXWindow* pDXWindow) { }
};

#endif

/* The windowing interface. */
struct __declspec(uuid("20203c63-f6f4-47ea-93cd-2784f02ecd61")) IDXWindow : public IUnknown {
	/* Goes through every message in the queue and dispatches them to your callback. */
	virtual VOID STDMETHODCALLTYPE PumpMessages() PURE;

	/* Flips the back buffer.  See IDXGISwapChain::Present() for more info. */
	virtual VOID STDMETHODCALLTYPE Present(UINT SyncInterval, UINT Flags) PURE;

	/* Returns the windowed-mode width of the window. */
	virtual WORD STDMETHODCALLTYPE GetWindowWidth() PURE;

	/* Returns the windowed-mode height of the window. */
	virtual WORD STDMETHODCALLTYPE GetWindowHeight() PURE;

	/* Sets the windowed-mode resolution of the window. */
	virtual VOID STDMETHODCALLTYPE SetWindowResolution(WORD Width, WORD Height) PURE;

	/* Returns the state of the window. */
	virtual DXWINDOW_STATE STDMETHODCALLTYPE GetState() PURE;

	/* Sets the state of the window. */
	virtual VOID STDMETHODCALLTYPE SetState(DXWINDOW_STATE State) PURE;

	/* Indicates whether or not the window can be toggled between fullscreen and windowed via F11. */
	virtual BOOL STDMETHODCALLTYPE GetAllowToggle() PURE;

	/* Controls whether or not the window can be toggle between fullscreen and windowed via F11. */
	virtual VOID STDMETHODCALLTYPE SetAllowToggle(BOOL AllowToggle) PURE;

	/* Retrieves the back buffer in the interface of your choosing. */
	virtual VOID STDMETHODCALLTYPE GetBackBuffer(REFIID rIID, void** ppvBackBuffer) PURE;
};

#ifdef _DXWINDOW_DLL_PROJECT
#define _DXWINDOW_EXPORT_TAG __declspec(dllexport)
#else
#define _DXWINDOW_EXPORT_TAG __declspec(dllimport)
#endif

/* Creates a window.  pDevice can be any D3D device version 11 or later. */
extern "C" HRESULT _DXWINDOW_EXPORT_TAG DXWindowCreateWindow (
	const DXWINDOW_DESC* pDesc,
	IUnknown* pDeviceUnk,
	IDXWindowCallback* pDXWindowCallback,
	IDXWindow** ppDXWindow
);