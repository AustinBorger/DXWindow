#include "CDXWindow.h"

static const DWORD WINDOWED_STYLE = WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE;
static const DWORD WINDOWED_EX_STYLE = NULL;
static const DWORD BORDERLESS_STYLE = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE;
static const DWORD BORDERLESS_EX_STYLE = WS_EX_APPWINDOW;
static const DWORD FULLSCREEN_WINDOW_STYLE = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE;
static const DWORD FULLSCREEN_WINDOW_EX_STYLE = WS_EX_TOPMOST | WS_EX_APPWINDOW;

#define CHECK_HR() if (FAILED(hr)) m_Callback->OnObjectFailure(hr)
#define CHECK_BRESULT() if (bresult == FALSE) m_Callback->OnObjectFailure(HRESULT_FROM_WIN32(GetLastError()))

HRESULT CDXWindow::Initialize(const DXWINDOW_DESC& Desc, IUnknown* pDevice, IDXWindowCallback* pDXWindowCallback) {
	CComPtr<IUnknown> DeviceUnk = pDevice;
	m_Callback = pDXWindowCallback;
}

VOID CDXWindow::PumpMessages() {
	m_WindowMessageDispatcher.CheckWindowVisible();

	m_WindowMessageDispatcher.RunMessagePump();

	m_GamepadMessageDispatcher.CheckGamepads();
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
			hr = m_SwapChainController.ResizeBuffers(); CHECK_HR();
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
	); CHECK_BRESULT();

	SetLastError(0);

	bresult = SetWindowLongW (
		m_Handle,
		GWL_EXSTYLE,
		m_WindowExStyle
	); CHECK_BRESULT();

	CenterWindow();

	CenterCursor();
}

VOID CDXWindow::ToggleFullscreen() {
	HRESULT hr = S_OK;

	if (m_State == DXWINDOW_STATE_FULLSCREEN || m_State == DXWINDOW_STATE_FULLSCREEN_WINDOW) {
		//I'm in fullscreen state now, switch to windowed
		if (m_State == DXWINDOW_STATE_FULLSCREEN) {
			hr = m_SwapChainController.ToggleFullscreen(); CHECK_HR();
		}

		UpdateWindowState();
	} else {
		//I'm in windowed state now, switch to fullscreen
		if (m_FullscreenState == DXWINDOW_FULLSCREEN_STATE_FULLSCREEN) {
			hr = m_SwapChainController.ToggleFullscreen(); CHECK_HR();
		} else {
			FullscreenWindow();
		}

		CenterCursor();
	}
}

VOID CDXWindow::FullscreenWindow() {
	HRESULT hr = S_OK;
	BOOL bresult = TRUE;
	RECT DesktopArea;

	hr = m_SwapChainController.GetDesktopArea(DesktopArea); CHECK_HR();

	m_WindowStyle = FULLSCREEN_WINDOW_STYLE;
	m_WindowExStyle = FULLSCREEN_WINDOW_EX_STYLE;

	SetLastError(0);

	bresult = SetWindowLongW (
		m_Handle,
		GWL_STYLE,
		m_WindowStyle
	); CHECK_BRESULT();

	bresult = SetWindowPos (
		m_Handle,
		NULL,
		DesktopArea.left,
		DesktopArea.top,
		DesktopArea.right - DesktopArea.left,
		DesktopArea.bottom - DesktopArea.top,
		NULL
	); CHECK_BRESULT();

	SetLastError(0);

	bresult = SetWindowLongW (
		m_Handle,
		GWL_EXSTYLE,
		m_WindowExStyle
	); CHECK_BRESULT();
}