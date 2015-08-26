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

#ifdef _DXWINDOW_SUPPORT_12

#include "WindowMessageDispatcher12.h"
#include "CDXWindow12.h"

#include <map>
#include <mutex>

//Used for mapping a window handle to a dispatcher object
static std::map<HWND, WindowMessageDispatcher12*> g_WindowMap;
//Used to prevent race conditions while using the window handle map
static std::mutex g_WindowMapMutex;

//Zero out all data
WindowMessageDispatcher12::WindowMessageDispatcher12(CDXWindow12& Window) :
m_Handle(NULL),
m_Window(Window),
m_PrevWindowVisible(FALSE)
{ }

//If the handle and dispatcher object were mapped, remove the entry from the map
WindowMessageDispatcher12::~WindowMessageDispatcher12() {
	if (m_Handle != NULL) {
		g_WindowMapMutex.lock();
		g_WindowMap.erase(m_Handle);
		g_WindowMapMutex.unlock();
	}
}

//Add the handle-dispatcher relationship to the global window map
VOID WindowMessageDispatcher12::Initialize(HWND Handle, CComPtr<IDXWindowCallback> Callback) {
	g_WindowMapMutex.lock();
	g_WindowMap[Handle] = this;
	g_WindowMapMutex.unlock();

	m_Callback = Callback;
	m_PrevWindowVisible = TRUE;
}

//Handle all messages in the queue
VOID WindowMessageDispatcher12::RunMessagePump() {
	MSG Message;

	while (PeekMessageW(&Message, m_Handle, 0, 0, PM_REMOVE) > 0) {
		TranslateMessage(&Message);
		DispatchMessageW(&Message);
	}
}

//Locate the dispatcher object and re-direct the call towards it
LRESULT CALLBACK WindowMessageDispatcher12::StaticWindowProcess(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam) {
	g_WindowMapMutex.lock();
	WindowMessageDispatcher12* l_Dispatcher = g_WindowMap[Handle];
	g_WindowMapMutex.unlock();

	if (l_Dispatcher == nullptr) {
		return DefWindowProcW(Handle, Message, wParam, lParam);
	}

	return l_Dispatcher->WindowProcess(Message, wParam, lParam);
}

//Check to see if there was a change in window visibility.
//Alert the application if there was.
void WindowMessageDispatcher12::CheckWindowVisible() {
	BOOL l_IsWindowVisible = IsWindowVisible(m_Handle);

	if (l_IsWindowVisible != m_PrevWindowVisible) {
		if (l_IsWindowVisible == FALSE) {
			m_Callback->OnWindowHide(&m_Window);
		} else { //TRUE
			m_Callback->OnWindowShow(&m_Window);
		}
	}

	m_PrevWindowVisible = l_IsWindowVisible;
}

//React to messages by dispatching them to the application
LRESULT WindowMessageDispatcher12::WindowProcess(UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case WM_KEYDOWN: {
			m_Callback->OnKeyDown(&m_Window, wParam, lParam);
		} break;

		case WM_KEYUP: {
			m_Callback->OnKeyUp(&m_Window, wParam, lParam);
		} break;

		case WM_LBUTTONDOWN: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);

			m_Callback->OnMouseLButtonDown(&m_Window, wParam, x, y);
		} break;

		case WM_LBUTTONUP: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);

			m_Callback->OnMouseLButtonUp(&m_Window, wParam, x, y);
		} break;

		case WM_RBUTTONDOWN: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);

			m_Callback->OnMouseRButtonDown(&m_Window, wParam, x, y);
		} break;

		case WM_RBUTTONUP: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);

			m_Callback->OnMouseRButtonUp(&m_Window, wParam, x, y);
		} break;

		case WM_MBUTTONDOWN: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);

			m_Callback->OnMouseMButtonDown(&m_Window, wParam, x, y);
		} break;

		case WM_MBUTTONUP: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);

			m_Callback->OnMouseLButtonUp(&m_Window, wParam, x, y);
		} break;

		case WM_MOUSEMOVE: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);

			m_Callback->OnMouseMove(&m_Window, wParam, x, y);
		} break;

		case WM_MOUSEWHEEL: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);
			SHORT delta = GET_WHEEL_DELTA_WPARAM(wParam);
			WORD flags = LOWORD(wParam);

			m_Callback->OnMouseWheel(&m_Window, flags, delta, x, y);
		} break;

		case WM_CLOSE: {
			m_Callback->OnWindowClose(&m_Window);
		} break;
	}

	return m_Window.WindowProcess(Message, wParam, lParam);
}

#endif