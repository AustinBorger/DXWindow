#pragma once

#include <Windows.h>
#include <comdef.h>

enum DXWINDOW_STATE {
	DXWINDOW_STATE_FULLSCREEN = 1,
	DXWINDOW_STATE_FULLSCREEN_WINDOW,
	DXWINDOW_STATE_WINDOWED,
	DXWINDOW_STATE_BORDERLESS,
	DXWINDOW_STATE_MAXIMIZED
};

enum DXWINDOW_FULLSCREEN_STATE {
	DXWINDOW_FULLSCREEN_STATE_FULLSCREEN = DXWINDOW_STATE_FULLSCREEN,
	DXWINDOW_FULLSCREEN_STATE_FULLSCREEN_WINDOW = DXWINDOW_STATE_FULLSCREEN_WINDOW
};

enum DXWINDOW_WINDOW_STATE {
	DXWINDOW_WINDOW_STATE_WINDOWED = DXWINDOW_STATE_WINDOWED,
	DXWINDOW_WINDOW_STATE_BORDERLESS = DXWINDOW_STATE_BORDERLESS,
	DXWINDOW_WINDOW_STATE_MAXIMIZED = DXWINDOW_STATE_MAXIMIZED
};

struct DXWINDOW_DESC {
	LPCWSTR IconSmall;
	LPCWSTR IconLarge;
	LPCWSTR Cursor;
	LPCWSTR Title;
	HINSTANCE Instance;
	DXWINDOW_FULLSCREEN_STATE FullscreenState;
	DXWINDOW_WINDOW_STATE WindowState;
	WORD Width;
	WORD Height;
	BOOL InitFullscreen;
	BOOL AllowToggle;
};

/* Used as a callback mechanism for window and gamepad messages.  Also used for error reporting. */
struct __declspec(uuid("af69111a-a1f8-4cd1-afd3-94abdcfb011c")) IDXWindowCallback : public IUnknown {
	//Error callbacks

	/* Called when an error has occured anywhere within the API. */
	virtual VOID STDMETHODCALLTYPE OnObjectFailure(HRESULT hr) PURE;

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

struct __declspec(uuid("20203c63-f6f4-47ea-93cd-2784f02ecd61")) IDXWindow : public IUnknown {
	virtual VOID STDMETHODCALLTYPE PumpMessages() PURE;

	virtual VOID STDMETHODCALLTYPE Present(UINT SyncInterval, UINT Flags) PURE;

	virtual WORD STDMETHODCALLTYPE GetWidth() PURE;

	virtual WORD STDMETHODCALLTYPE GetHeight() PURE;

	virtual VOID STDMETHODCALLTYPE SetResolution(WORD Width, WORD Height) PURE;

	virtual DXWINDOW_STATE STDMETHODCALLTYPE GetState() PURE;

	virtual VOID STDMETHODCALLTYPE SetState(DXWINDOW_STATE State) PURE;

	virtual VOID STDMETHODCALLTYPE GetBackBuffer(REFIID rIID, void** ppvBackBuffer) PURE;
};

#ifdef _DXWINDOW_DLL_PROJECT
#define _DXWINDOW_EXPORT_TAG __declspec(dllexport)
#else
#define _DXWINDOW_EXPORT_TAG __declspec(dllimport)
#endif

extern "C" HRESULT _DXWINDOW_EXPORT_TAG DXWindowCreateWindow (
	const DXWINDOW_DESC* pDesc,
	IUnknown* pDevice,
	IDXWindowCallback* pDXWindowCallback,
	IDXWindow** ppDXWindow
);