#pragma once

#include <Windows.h>

/* This class will handle dispatching window messages in the form of a callback interface
** to the application. */
class WindowMessageDispatcher {
public:
	WindowMessageDispatcher();

	~WindowMessageDispatcher();

	/* Initializes the dispatcher by mapping itself to the window handle. */
	HRESULT Initialize(HWND Handle);

private:
	HWND m_Handle; //The window handle

	/* Static WndProc */
	static LRESULT CALLBACK StaticWindowProcess(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);

	/* Non-static WndProc */
	LRESULT WindowProcess(UINT Message, WPARAM wParam, LPARAM lParam);
};