#pragma once

#include <Windows.h>
#include <comdef.h>
#include <atlbase.h>
#include "DXWindow.h"

class CDXWindow; //Forward declaration of window class

/* This class will handle dispatching window messages in the form of a callback interface
** to the application. */
class WindowMessageDispatcher {
public:
	WindowMessageDispatcher(CDXWindow& Window);

	~WindowMessageDispatcher();

	/* Initializes the dispatcher by mapping itself to the window handle. */
	/* Also initializes the reference to the callback object. */
	HRESULT Initialize(HWND Handle, CComPtr<IDXWindowCallback> Callback);

	/* Static WndProc */
	static LRESULT CALLBACK StaticWindowProcess(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);

	/* Used to determine if there was a change in the visibility of the window.
	** If there was a change, let the callback know. */
	void CheckWindowVisible();

private:
	CDXWindow& m_Window; //The window object
	HWND m_Handle; //The window handle
	CComPtr<IDXWindowCallback> m_Callback; //The callback object
	BOOL m_PrevWindowVisible; //Used to determine if there was a change in the visibility of the window

	/* Non-static WndProc */
	LRESULT WindowProcess(UINT Message, WPARAM wParam, LPARAM lParam);
};