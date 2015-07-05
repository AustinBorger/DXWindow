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

	/* Runs the message pump by draining and addressing the message queue. */
	void RunMessagePump();

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