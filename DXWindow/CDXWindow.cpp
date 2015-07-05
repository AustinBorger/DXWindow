#include "CDXWindow.h"
#include <time.h>

static const DWORD WINDOWED_STYLE = WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE;
static const DWORD WINDOWED_EX_STYLE = NULL;
static const DWORD BORDERLESS_STYLE = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE;
static const DWORD BORDERLESS_EX_STYLE = WS_EX_APPWINDOW;
static const DWORD FULLSCREEN_WINDOW_STYLE = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE;
static const DWORD FULLSCREEN_WINDOW_EX_STYLE = WS_EX_TOPMOST | WS_EX_APPWINDOW;

static LPCWSTR WINDOW_CLASS_NAME = L"DXWindow";

#define CHECK_HR(Line) if (FAILED(hr)) m_Callback->OnObjectFailure(L"CDXWindow.cpp", Line, hr)
#define CHECK_ERR(Line) if (err != ERROR_SUCCESS) m_Callback->OnObjectFailure(L"CDXWindow.cpp", Line, HRESULT_FROM_WIN32(err))
#define CHECK_BRESULT(Line) if (bresult == FALSE) m_Callback->OnObjectFailure(L"CDXWindow.cpp", Line, HRESULT_FROM_WIN32(GetLastError()))

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
m_AllowToggle(TRUE),
m_WindowMessageDispatcher(*this),
m_GamepadMessageDispatcher(*this),
m_SwapChainController(*this, m_OutputWatcher)
{ }

CDXWindow::~CDXWindow() {
	if (!m_ClassName.empty()) {
		UnregisterClassW(m_ClassName.c_str(), m_Instance);
	}
}

HRESULT CDXWindow::Initialize(const DXWINDOW_DESC& Desc, IUnknown* pDevice, IDXWindowCallback* pDXWindowCallback) {
	HRESULT hr = S_OK;
	CComPtr<IUnknown> DeviceUnk = pDevice;
	m_Callback = pDXWindowCallback;
	m_Instance = Desc.Instance;
	m_FullscreenState = Desc.FullscreenState;
	m_WindowState = Desc.WindowState;
	m_WindowWidth = Desc.Width;
	m_WindowHeight = Desc.Height;

	hr = m_OutputWatcher.Initialize(DeviceUnk);

	if (FAILED(hr)) {
		return hr;
	}

	RegisterWindowClass(Desc);

	MakeWindow(Desc);

	hr = m_WindowMessageDispatcher.Initialize(m_Handle, m_Callback);

	if (FAILED(hr)) {
		return hr;
	}

	hr = m_GamepadMessageDispatcher.Initialize(m_Callback);

	if (FAILED(hr)) {
		return hr;
	}

	hr = m_SwapChainController.Initialize(DeviceUnk, m_Callback, m_Handle);

	if (FAILED(hr)) {
		return hr;
	}

	DXWINDOW_STATE State = Desc.InitFullscreen != FALSE ? (DXWINDOW_STATE)(Desc.FullscreenState) : (DXWINDOW_STATE)(Desc.WindowState);

	SetState(State);

	return S_OK;
}

VOID CDXWindow::PumpMessages() {
	m_WindowMessageDispatcher.CheckWindowVisible();

	m_WindowMessageDispatcher.RunMessagePump();

	m_GamepadMessageDispatcher.CheckGamepads();
}

VOID CDXWindow::Present(UINT SyncInterval, UINT Flags) {
	HRESULT hr = S_OK;

	hr = m_SwapChainController.Present (
		SyncInterval,
		Flags
	); CHECK_HR(__LINE__);
}

VOID CDXWindow::GetBackBuffer(REFIID rIID, void** ppvBackBuffer) {
	HRESULT hr = S_OK;

	hr = m_SwapChainController.GetBackBuffer (
		rIID,
		ppvBackBuffer
	); CHECK_HR(__LINE__);
}

LRESULT CDXWindow::WindowProcess(UINT Message, WPARAM wParam, LPARAM lParam) {
	HRESULT hr = S_OK;

	switch (Message) {
		case WM_SYSCOMMAND: {
			switch (wParam) {
				case SC_KEYMENU: {
					return S_OK; //prevents F10 from losing window focus
				} break;

				default: {
					return DefWindowProcW(m_Handle, Message, wParam, lParam);
				} break;
			}
		} return 0;

		case WM_DISPLAYCHANGE: {

		} return 0;

		case WM_KEYDOWN: {
			if (wParam == VK_F11 && m_AllowToggle != FALSE) {
				ToggleFullscreen();
			}
		} return 0;

		case WM_SIZE: {
			hr = m_SwapChainController.ResizeBuffers(); CHECK_HR(__LINE__);
		} return S_OK;

		case WM_KILLFOCUS: {
			KillFocus();
		} return S_OK;

		case WM_SETFOCUS: {
			SetFocus();
		} return S_OK;
	}

	return DefWindowProcW(m_Handle, Message, wParam, lParam);
}

VOID CDXWindow::UpdateWindowState() {
	BOOL bresult = TRUE;
	DWORD err = ERROR_SUCCESS;
	HRESULT hr = S_OK;

	if (m_WindowState == DXWINDOW_WINDOW_STATE_WINDOWED) {
		m_WindowStyle = WINDOWED_STYLE;
		m_WindowExStyle = WINDOWED_EX_STYLE;
	} else if (m_WindowState == DXWINDOW_WINDOW_STATE_BORDERLESS) {
		m_WindowStyle = BORDERLESS_STYLE;
		m_WindowExStyle = BORDERLESS_EX_STYLE;
	}

	SetLastError(0);

	bresult = SetWindowLongW (
		m_Handle,
		GWL_STYLE,
		m_WindowStyle
	); CHECK_BRESULT(__LINE__);

	SetLastError(0);

	bresult = SetWindowLongW (
		m_Handle,
		GWL_EXSTYLE,
		m_WindowExStyle
	); CHECK_BRESULT(__LINE__);

	CenterWindow();

	CenterCursor();
}

VOID CDXWindow::ToggleFullscreen() {
	HRESULT hr = S_OK;

	if (m_State == DXWINDOW_STATE_FULLSCREEN || m_State == DXWINDOW_STATE_FULLSCREEN_WINDOW) {
		//I'm in fullscreen state now, switch to windowed
		if (m_State == DXWINDOW_STATE_FULLSCREEN) {
			hr = m_SwapChainController.ToggleFullscreen(); CHECK_HR(__LINE__);
		}

		m_State = (DXWINDOW_STATE)(m_WindowState);

		UpdateWindowState();
	} else {
		//I'm in windowed state now, switch to fullscreen
		if (m_FullscreenState == DXWINDOW_FULLSCREEN_STATE_FULLSCREEN) {
			hr = m_SwapChainController.ToggleFullscreen(); CHECK_HR(__LINE__);
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

	hr = m_SwapChainController.GetDesktopArea(DesktopArea); CHECK_HR(__LINE__);

	m_WindowStyle = FULLSCREEN_WINDOW_STYLE;
	m_WindowExStyle = FULLSCREEN_WINDOW_EX_STYLE;

	SetLastError(0);

	bresult = SetWindowLongW (
		m_Handle,
		GWL_STYLE,
		m_WindowStyle
	); CHECK_BRESULT(__LINE__);

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

	bresult = SetWindowLongW (
		m_Handle,
		GWL_EXSTYLE,
		m_WindowExStyle
	); CHECK_BRESULT(__LINE__);
}

VOID CDXWindow::RegisterWindowClass(const DXWINDOW_DESC& Desc) {
	HRESULT hr = S_OK;
	DWORD err = ERROR_SUCCESS;

	WNDCLASSEXW wc = { 0 };

	HINSTANCE LargeInstance = Desc.Instance;
	HINSTANCE SmallInstance = Desc.Instance;
	HINSTANCE CursorInstance = Desc.Instance;

	LPCWSTR LargeIcon = Desc.IconLarge;
	LPCWSTR SmallIcon = Desc.IconSmall;
	LPCWSTR Cursor = Desc.Cursor;

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

	m_ClassName = WINDOW_CLASS_NAME;
	m_ClassName.append(Desc.Title);

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
		CHECK_ERR(__LINE__);
	}
}

VOID CDXWindow::MakeWindow(const DXWINDOW_DESC& Desc) {
	HRESULT hr = S_OK;
	DWORD err = ERROR_SUCCESS;

	Output* output = m_OutputWatcher.PrimaryOutput();

	if (output == nullptr) {
		m_Callback->OnObjectFailure(L"CDXWindow.cpp", __LINE__, E_FAIL);
	}

	RECT WindowArea;

	hr = output->GetWindowCenter (
		m_WindowWidth,						    //UINT Width
		m_WindowHeight,					        //UINT Height
		&WindowArea,							//RECT* pRect
		m_WindowStyle,							//DWORD dwStyle
		m_WindowExStyle							//DWORD dwExStyle
	); CHECK_HR(__LINE__);

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
		CHECK_ERR(__LINE__);
	}
}

VOID CDXWindow::KillFocus() {
	m_InFocus = FALSE;

	if (m_State == DXWINDOW_STATE_FULLSCREEN || m_State == DXWINDOW_STATE_FULLSCREEN_WINDOW) {
		ToggleFullscreen();
	}
}

VOID CDXWindow::SetFocus() {
	m_InFocus = TRUE;
}

VOID CDXWindow::CenterCursor() {
	BOOL bresult = TRUE;
	HRESULT hr = S_OK;

	RECT rect;
	POINT point = { 0 };

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

	Output* output = m_OutputWatcher.SearchOutput(m_Handle);

	if (output == nullptr) {
		m_Callback->OnObjectFailure(L"CDXWindow.cpp", __LINE__, E_FAIL);
	}

	RECT WindowArea;

	hr = output->GetWindowCenter (
		m_WindowWidth,							//UINT Width
		m_WindowHeight,							//UINT Height
		&WindowArea,							//RECT* pRect
		m_WindowStyle,							//DWORD dwStyle
		m_WindowExStyle							//DWORD dwExStyle
	); CHECK_HR(__LINE__);

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
		if (m_State == DXWINDOW_STATE_FULLSCREEN || m_State == DXWINDOW_STATE_FULLSCREEN_WINDOW) { //It's a fullscreen state
			m_State = State;

			if (State == DXWINDOW_STATE_WINDOWED || State == DXWINDOW_STATE_BORDERLESS) {
				ToggleFullscreen();
			} else {
				HRESULT hr = S_OK;

				hr = m_SwapChainController.ToggleFullscreen(); CHECK_HR(__LINE__);

				if (State == DXWINDOW_STATE_FULLSCREEN_WINDOW) {
					FullscreenWindow();
				}
			}
		} else { //It's a windowed state
			if (State == DXWINDOW_STATE_FULLSCREEN || State == DXWINDOW_STATE_FULLSCREEN_WINDOW) {
				ToggleFullscreen();
				m_State = State;
			} else {
				m_State = State;
				UpdateWindowState();
			}
		}
	}
}

VOID CDXWindow::SetWindowResolution(WORD Width, WORD Height) {
	m_WindowWidth = Width;
	m_WindowHeight = Height;

	if (m_State == DXWINDOW_STATE_WINDOWED || m_State == DXWINDOW_STATE_BORDERLESS) {
		CenterWindow();

		CenterCursor();
	}
}