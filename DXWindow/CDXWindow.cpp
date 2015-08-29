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

#include "CDXWindow.h"
#include <time.h>

//Window styles for each of the modes
static const DWORD WINDOWED_STYLE = WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE;
static const DWORD WINDOWED_EX_STYLE = NULL;
static const DWORD BORDERLESS_STYLE = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE;
static const DWORD BORDERLESS_EX_STYLE = WS_EX_APPWINDOW;
static const DWORD FULLSCREEN_WINDOW_STYLE = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE;
static const DWORD FULLSCREEN_WINDOW_EX_STYLE = WS_EX_TOPMOST | WS_EX_APPWINDOW;

//Class name prefix
static LPCWSTR WINDOW_CLASS_NAME = L"DXWindow";

#define FILENAME L"CDXWindow.cpp"
#define CHECK_HR(Line) if (FAILED(hr)) { m_Callback->OnObjectFailure(FILENAME, Line, hr); return; }
#define RETURN_HR(Line) if (FAILED(hr)) { m_Callback->OnObjectFailure(FILENAME, Line, hr); return E_FAIL; }
#define CHECK_ERR(Line) if (err != ERROR_SUCCESS) { m_Callback->OnObjectFailure(FILENAME, Line, HRESULT_FROM_WIN32(err)); return; }
#define RETURN_ERR(Line) if (err != ERROR_SUCCESS) { m_Callback->OnObjectFailure(FILENAME, Line, HRESULT_FROM_WIN32(err)); return E_FAIL; }
#define CHECK_BRESULT(Line) if (bresult == FALSE) { HRESULT hr = HRESULT_FROM_WIN32(GetLastError()); if (FAILED(hr)) { m_Callback->OnObjectFailure(FILENAME, Line, hr); return; } }
#define RETURN_BRESULT(Line) if (bresult == FALSE) { HRESULT hr = HRESULT_FROM_WIN32(GetLastError()); if (FAILED(hr)) { m_Callback->OnObjectFailure(FILENAME, Line, hr); return E_FAIL; } }

//Zero out data (except for refcount) and pass in references
CDXWindow::CDXWindow() :
m_RefCount(1),
m_Handle(NULL),
m_Instance(NULL),
m_WindowWidth(0),
m_WindowHeight(0),
m_State(DXWINDOW_STATE_WINDOWED),
m_FullscreenState(DXWINDOW_FULLSCREEN_STATE_FULLSCREEN_WINDOW),
m_WindowState(DXWINDOW_WINDOW_STATE_WINDOWED),
m_WindowStyle(WINDOWED_STYLE),
m_WindowExStyle(WINDOWED_EX_STYLE),
m_Closing(FALSE),
m_InFocus(FALSE),
m_AllowToggle(TRUE),
m_WindowMessageDispatcher(*this),
m_GamepadMessageDispatcher(*this),
m_SwapChainController(*this, m_OutputEnum)
{ }

//Delete dynamically allocated stuff
CDXWindow::~CDXWindow() {
	if (m_Handle != NULL) {
		DestroyWindow(m_Handle);
		m_Handle = NULL;
	}

	if (!m_ClassName.empty()) {
		UnregisterClassW(m_ClassName.c_str(), m_Instance);
	}
}

HRESULT CDXWindow::Initialize(const DXWINDOW_DESC& Desc, IUnknown* pDevice, IDXWindowCallback* pDXWindowCallback) {
	CComPtr<IUnknown> DeviceUnk = pDevice;
	HRESULT hr = S_OK;

	//Initialize variables based on description
	m_Callback = pDXWindowCallback;
	m_Instance = Desc.Instance;
	m_FullscreenState = Desc.FullscreenState;
	m_WindowState = Desc.WindowState;
	m_WindowWidth = Desc.Width;
	m_WindowHeight = Desc.Height;

	//Initialize the output enumeration
	hr = m_OutputEnum.Initialize(DeviceUnk, m_Callback);
	if (FAILED(hr)) return E_FAIL;

	//Register the window class
	RegisterWindowClass(Desc);

	//Create the window
	MakeWindow(Desc);

	//Initialize the window message dispatcher
	m_WindowMessageDispatcher.Initialize(m_Handle, m_Callback);

	//Initialize the gamepad message dispatcher
	m_GamepadMessageDispatcher.Initialize(m_Callback);

	//Initialize the swap chain controller
	hr = m_SwapChainController.Initialize(DeviceUnk, m_Callback, m_Handle, Desc.NumBuffers);
	if (FAILED(hr)) return E_FAIL;

	//Initialize the state
	DXWINDOW_STATE State = Desc.InitFullscreen != FALSE ? (DXWINDOW_STATE)(Desc.FullscreenState) : (DXWINDOW_STATE)(Desc.WindowState);

	if (State != m_State) {
		if (State == DXWINDOW_STATE_FULLSCREEN) {
			m_SwapChainController.ToggleFullscreen();
			CenterCursor();
		} else if (State == DXWINDOW_STATE_FULLSCREEN_WINDOW) {
			FullscreenWindow();
			CenterCursor();
		} else if (State == DXWINDOW_STATE_BORDERLESS) {
			m_State = State;
			UpdateWindowState();
			CenterCursor();
		}
	}

	m_State = State;

	m_Callback->OnBackBufferCreate(this);

	return S_OK;
}

VOID CDXWindow::PumpMessages() {
	//Sends a OnWindowShow or OnWindowHide event in a change of window visibility
	m_WindowMessageDispatcher.CheckWindowVisible();

	//Sends all other window messages
	m_WindowMessageDispatcher.RunMessagePump();

	//Sends all gamepad messages only if the window is in focus
	if (m_InFocus != FALSE) {
		m_GamepadMessageDispatcher.CheckGamepads();
	}
}

//Flips the swap chain at the requested frame interval
VOID CDXWindow::Present(UINT SyncInterval, UINT Flags) {
	m_SwapChainController.Present (
		SyncInterval,
		Flags
	);
}

//Returns the backbuffer in the requested interface
VOID CDXWindow::GetBuffer(UINT Buffer, REFIID rIID, void** ppvBackBuffer) {
	m_SwapChainController.GetBuffer (
		Buffer,
		rIID,
		ppvBackBuffer
	);
}

LRESULT CDXWindow::WindowProcess(UINT Message, WPARAM wParam, LPARAM lParam) {
	HRESULT hr = S_OK;

	switch (Message) {
		case WM_SYSCOMMAND: {
			switch (wParam) {
				case SC_KEYMENU: {
					return S_OK; //prevents F10 from losing window focus
				} break;

				default: { //Let DefwindowProc handle all other SYSCOMMANDS
					return DefWindowProcW(m_Handle, Message, wParam, lParam);
				} break;
			}
		} return 0;

		case WM_DISPLAYCHANGE: {
			DisplayChange();
		} return 0;

		case WM_KEYDOWN: {
			if (wParam == VK_F11 && m_AllowToggle != FALSE) {
				m_Callback->OnBackBufferRelease(this);
				ToggleFullscreen();
				m_Callback->OnBackBufferCreate(this);
			}
		} return 0;

		case WM_SIZE: {
			m_SwapChainController.ResizeBuffers();
		} return 0;

		case WM_KILLFOCUS: {
			KillFocus();
		} return 0;

		case WM_SETFOCUS: {
			SetFocus();
		} return 0;

		case WM_CLOSE: {
			m_Closing = TRUE;
		} break;
	}

	return DefWindowProcW(m_Handle, Message, wParam, lParam);
}

VOID CDXWindow::UpdateWindowState() {
	BOOL bresult = TRUE;
	DWORD err = ERROR_SUCCESS;
	HRESULT hr = S_OK;

	//Set the appropriate window style
	if (m_WindowState == DXWINDOW_WINDOW_STATE_WINDOWED) {
		m_WindowStyle = WINDOWED_STYLE;
		m_WindowExStyle = WINDOWED_EX_STYLE;
	} else if (m_WindowState == DXWINDOW_WINDOW_STATE_BORDERLESS) {
		m_WindowStyle = BORDERLESS_STYLE;
		m_WindowExStyle = BORDERLESS_EX_STYLE;
	}

	//SetLastError() has to be called before all calls to SetWindowLong.
	//This is because SetWindowLong can return either the previous value
	//of GWL_STYLE or GWL_EXSTYLE or zero on failure.  However, they might
	//have been zero before, in which case bresult is picking up on a false
	//positive.  To solve this problem, setting the last error to zero ensures
	//that the previous error is not mixed with the current one.
	SetLastError(0);

	//Sets the window style
	bresult = SetWindowLongW (
		m_Handle,
		GWL_STYLE,
		m_WindowStyle
	); CHECK_BRESULT(__LINE__);

	SetLastError(0);

	//Sets the window ex style
	bresult = SetWindowLongW (
		m_Handle,
		GWL_EXSTYLE,
		m_WindowExStyle
	); CHECK_BRESULT(__LINE__);

	//Center the window
	CenterWindow();
}

VOID CDXWindow::ToggleFullscreen() {
	HRESULT hr = S_OK;

	if (m_State == DXWINDOW_STATE_FULLSCREEN || m_State == DXWINDOW_STATE_FULLSCREEN_WINDOW) {
		//I'm in fullscreen state now, switch to windowed
		if (m_State == DXWINDOW_STATE_FULLSCREEN) {
			m_SwapChainController.ToggleFullscreen();
		}

		//Set the state before calling UpdateWindowState()
		m_State = (DXWINDOW_STATE)(m_WindowState);

		UpdateWindowState();

		CenterCursor();
	} else {
		//I'm in windowed state now, switch to fullscreen
		if (m_FullscreenState == DXWINDOW_FULLSCREEN_STATE_FULLSCREEN) {
			m_SwapChainController.ToggleFullscreen();
		} else {
			FullscreenWindow();
		}

		m_State = (DXWINDOW_STATE)(m_FullscreenState);

		CenterCursor();
	}
}

VOID CDXWindow::FullscreenWindow() {
	HRESULT hr = S_OK;
	BOOL bresult = TRUE;
	RECT DesktopArea;

	GetDesktopArea(DesktopArea);

	m_WindowStyle = FULLSCREEN_WINDOW_STYLE;
	m_WindowExStyle = FULLSCREEN_WINDOW_EX_STYLE;

	//Once again, SetLastError must be called before SetWindowLong
	SetLastError(0);

	//Set the window style
	bresult = SetWindowLongW (
		m_Handle,
		GWL_STYLE,
		m_WindowStyle
	); CHECK_BRESULT(__LINE__);

	//Fills the desktop that the window currently inhabits
	bresult = SetWindowPos (
		m_Handle,
		NULL,
		DesktopArea.left,
		DesktopArea.top,
		DesktopArea.right - DesktopArea.left,
		DesktopArea.bottom - DesktopArea.top,
		NULL
	); CHECK_BRESULT(__LINE__);

	SetLastError(0);

	//Set the window ex style
	bresult = SetWindowLongW (
		m_Handle,
		GWL_EXSTYLE,
		m_WindowExStyle
	); CHECK_BRESULT(__LINE__);
}

HRESULT CDXWindow::RegisterWindowClass(const DXWINDOW_DESC& Desc) {
	HRESULT hr = S_OK;
	DWORD err = ERROR_SUCCESS;

	WNDCLASSEXW wc = { 0 };

	HINSTANCE LargeInstance = Desc.Instance;
	HINSTANCE SmallInstance = Desc.Instance;
	HINSTANCE CursorInstance = Desc.Instance;

	LPCWSTR LargeIcon = Desc.IconLarge;
	LPCWSTR SmallIcon = Desc.IconSmall;
	LPCWSTR Cursor = Desc.Cursor;

	//If the application provided us NULL icons or cursors,
	//then the instance must be NULL when calling LoadCursor()
	//or LoadIcon().
	if (LargeIcon == NULL) {
		LargeIcon = IDI_APPLICATION;
		LargeInstance = NULL;
	}

	if (SmallIcon == NULL) {
		SmallIcon = IDI_APPLICATION;
		SmallInstance = NULL;
	}

	if (Cursor == NULL) {
		Cursor = IDC_ARROW;
		CursorInstance = NULL;
	}

	//Sets the class name to the prefix + the title of the window
	m_ClassName = WINDOW_CLASS_NAME;
	m_ClassName.append(L"-");

	GUID suffixGUID;
	LPOLESTR strGUID;

	hr = CoCreateGuid(&suffixGUID); RETURN_HR(__LINE__);
	hr = StringFromCLSID(suffixGUID, &strGUID); RETURN_HR(__LINE__);
	m_ClassName.append(strGUID);
	CoTaskMemFree(strGUID);
	strGUID = nullptr;

	wc.cbSize = sizeof(wc);
	wc.hbrBackground = NULL;
	wc.hCursor = LoadCursorW(CursorInstance, Cursor);
	wc.hIcon = LoadIconW(LargeInstance, LargeIcon);
	wc.hIconSm = LoadIconW(SmallInstance, SmallIcon);
	wc.hInstance = GetModuleHandleW(NULL);
	wc.lpfnWndProc = WindowMessageDispatcher::StaticWindowProcess;
	wc.lpszClassName = m_ClassName.c_str();
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (RegisterClassExW(&wc) == 0) {
		err = GetLastError();
		RETURN_ERR(__LINE__);
	}

	return S_OK;
}

HRESULT CDXWindow::MakeWindow(const DXWINDOW_DESC& Desc) {
	HRESULT hr = S_OK;
	DWORD err = ERROR_SUCCESS;

	Output* output = m_OutputEnum.PrimaryOutput();
	RECT WindowArea;

	output->GetWindowCenter (
		m_WindowWidth,						    //UINT Width
		m_WindowHeight,					        //UINT Height
		&WindowArea,							//RECT* pRect
		m_WindowStyle,							//DWORD dwStyle
		m_WindowExStyle							//DWORD dwExStyle
	);

	m_Handle = CreateWindowExW (
		m_WindowExStyle,						//DWORD dwExStyle
		m_ClassName.c_str(),					//LPCWSTR lpClassName
		Desc.Title,								//LPCWSTR lpWindowName
		m_WindowStyle,							//DWORD dwStyle
		WindowArea.left,						//int X
		WindowArea.top,							//int Y
		WindowArea.right - WindowArea.left,		//int nWidth
		WindowArea.bottom - WindowArea.top,		//int nHeight
		NULL,									//HWND hWndParent
		NULL,									//HMENU hMenu
		Desc.Instance,							//HINSTANCE hInstance
		NULL									//LPVOID lpParam
	);

	if (m_Handle == NULL) {
		err = GetLastError();
		RETURN_ERR(__LINE__);
	}

	m_InFocus = TRUE;

	return S_OK;
}

VOID CDXWindow::KillFocus() {
	m_InFocus = FALSE;

	//If the window is not in focus - say CTRL-ALT-DEL is pressed,
	//but the window was in fullscreen, make it enter its windowed
	//state (otherwise the result can get ugly).  We have to make sure
	//this isn't happening while the window is closing, though, otherwise
	//SetWindowPos() gives an error
	if (m_Closing == FALSE) {
		if (m_State == DXWINDOW_STATE_FULLSCREEN || m_State == DXWINDOW_STATE_FULLSCREEN_WINDOW) {
			m_Callback->OnBackBufferRelease(this);
			ToggleFullscreen();
			m_Callback->OnBackBufferCreate(this);
		}
	}
}

//Reacts to WM_SETFOCUS by setting the InFocus flag to true
VOID CDXWindow::SetFocus() {
	m_InFocus = TRUE;
}

VOID CDXWindow::CenterCursor() {
	BOOL bresult = TRUE;
	HRESULT hr = S_OK;

	RECT rect;
	POINT point = { 0 };

	//Gets the client rect, then translates it to screen coordinates
	//so that SetCursorPos isn't called with relative x and y values
	GetClientRect(m_Handle, &rect);
	ClientToScreen(m_Handle, &point);

	rect.left += point.x;
	rect.right += point.x;
	rect.top += point.y;
	rect.bottom += point.y;

	int x = rect.left + (rect.right - rect.left) / 2;
	int y = rect.top + (rect.bottom - rect.top) / 2;

	bresult = SetCursorPos(x, y); CHECK_BRESULT(__LINE__);
}

VOID CDXWindow::CenterWindow() {
	BOOL bresult = TRUE;
	HRESULT hr = S_OK;

	Output* output = m_OutputEnum.SearchOutput(m_Handle);

	RECT WindowArea;

	output->GetWindowCenter (
		m_WindowWidth,							//UINT Width
		m_WindowHeight,							//UINT Height
		&WindowArea,							//RECT* pRect
		m_WindowStyle,							//DWORD dwStyle
		m_WindowExStyle							//DWORD dwExStyle
	);

	bresult = SetWindowPos (
		m_Handle,								//HWND hWnd
		NULL,								    //HWND hWndInsertAfter
		WindowArea.left,						//int X
		WindowArea.top,							//int Y
		WindowArea.right - WindowArea.left,		//int nWidth
		WindowArea.bottom - WindowArea.top,		//int nHeight
		NULL								    //UINT uFlags
	); CHECK_BRESULT(__LINE__);
}

VOID CDXWindow::SetState(DXWINDOW_STATE State) {
	if (State != m_State) {
		if (State == DXWINDOW_STATE_FULLSCREEN) {
			m_Callback->OnBackBufferRelease(this);
			m_FullscreenState = DXWINDOW_FULLSCREEN_STATE_FULLSCREEN;
			m_State = DXWINDOW_STATE_FULLSCREEN;
			m_SwapChainController.ToggleFullscreen();
			m_Callback->OnBackBufferCreate(this);
			return;
		}

		if (m_State == DXWINDOW_STATE_FULLSCREEN || m_State == DXWINDOW_STATE_FULLSCREEN_WINDOW) { //We're in fullscreen state now
			m_Callback->OnBackBufferRelease(this);

			if (State == DXWINDOW_STATE_WINDOWED || State == DXWINDOW_STATE_BORDERLESS) { //Go to a windowed state
				m_WindowState = (DXWINDOW_WINDOW_STATE)(State);
				ToggleFullscreen(); //Toggle fullscreen handles distinctions between windowed states
			} else if (State == DXWINDOW_STATE_FULLSCREEN_WINDOW) { //Enter fullscreen window instead
				//Leave exclusive fullscreen mode
				m_SwapChainController.ToggleFullscreen();

				//Enter non-exclusive fullscreen mode
				FullscreenWindow();

				m_State = DXWINDOW_STATE_FULLSCREEN_WINDOW;
				m_FullscreenState = DXWINDOW_FULLSCREEN_STATE_FULLSCREEN_WINDOW;
			}

			m_Callback->OnBackBufferCreate(this);
		} else { //We're in a windowed state now
			if (State == DXWINDOW_STATE_FULLSCREEN_WINDOW) { //Go to fullscreen window
				m_Callback->OnBackBufferRelease(this);
				FullscreenWindow();
				m_FullscreenState = DXWINDOW_FULLSCREEN_STATE_FULLSCREEN_WINDOW;
				m_State = DXWINDOW_STATE_FULLSCREEN_WINDOW;
				m_Callback->OnBackBufferCreate(this);
			} else {
				m_Callback->OnBackBufferRelease(this);
				m_State = State; //Has to be set before calling UpdateWindowState()
				m_WindowState = (DXWINDOW_WINDOW_STATE)(State);
				UpdateWindowState();
				CenterCursor();
				m_Callback->OnBackBufferCreate(this);
			}
		}
	}
}

//Changes the resolution of the window
VOID CDXWindow::SetWindowResolution(WORD Width, WORD Height) {
	if (Width != m_WindowWidth || Height != m_WindowHeight) {
		m_WindowWidth = Width;
		m_WindowHeight = Height;

		//If we're in windowed mode, make the change immediately
		if (m_State == DXWINDOW_STATE_WINDOWED || m_State == DXWINDOW_STATE_BORDERLESS) {
			m_Callback->OnBackBufferRelease(this);
			CenterWindow();
			CenterCursor();
			m_Callback->OnBackBufferCreate(this);
		}
	}
}

//Retrieves the desktop area of the output the window occupies
VOID CDXWindow::GetDesktopArea(RECT& DesktopArea) {
	Output* output = m_OutputEnum.SearchOutput(m_Handle);
	output->GetDesktopArea(&DesktopArea);
}

//React to a change in resolution by centering the window
VOID CDXWindow::DisplayChange() {
	if (m_State == DXWINDOW_STATE_WINDOWED || m_State == DXWINDOW_STATE_BORDERLESS) {
		CenterWindow();
		CenterCursor();
	} else if (m_State == DXWINDOW_STATE_FULLSCREEN_WINDOW) {
		m_Callback->OnBackBufferRelease(this);
		FullscreenWindow();
		CenterCursor();
		m_Callback->OnBackBufferCreate(this);
	}
}