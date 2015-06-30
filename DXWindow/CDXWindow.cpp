#include "CDXWindow.h"

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
				hr = m_SwapChainController.ToggleFullscreen(); CHECK_HR();
			}
		} return 0;

		case WM_SIZE: {
			hr = ResizeBuffers(); CHECK_HR();
		} return S_OK;

		case WM_KILLFOCUS: {
			hr = KillFocus(); CHECK_HR();
		} return S_OK;

		case WM_SETFOCUS: {
			hr = SetFocus(); CHECK_HR();
		} return S_OK;
	}

	return DefWindowProcW(m_Handle, Message, wParam, lParam);
}

HRESULT CDXWindow::ToggleFullscreen() {
	HRESULT hr = S_OK;

	if (m_Fullscreen == FALSE) {
		RECT DesktopArea;

		Output* output = m_DXWindowFactory.SearchOutput(m_Handle);

		if (output == nullptr) {
			return E_FAIL;
		}

		hr = output->GetDesktopArea (
			&DesktopArea
		); CHECK_HR();

		if (m_FullscreenMode == DXWINDOW_FULLSCREEN_MODE_FULLSCREEN) {
			DXGI_MODE_DESC mode; Zero(mode);
			CComPtr<IDXGIOutput> obj = output->GetObj();

			mode.Width = DesktopArea.right - DesktopArea.left;
			mode.Height = DesktopArea.bottom - DesktopArea.top;

			hr = m_SwapChain->ResizeTarget (
				&mode
			); CHECK_HR();

			hr = m_SwapChain->SetFullscreenState (
				TRUE,
				obj
			); CHECK_HR();

			hr = ResizeBuffers(); CHECK_HR();

			hr = CenterCursor(); CHECK_HR();
		} else { //FULLSCREEN_MODE_FULLSCREEN_WINDOW
			hr = FullscreenWindow (
				DesktopArea
			); CHECK_HR();

			hr = CenterCursor(); CHECK_HR();
		}
	} else {
		if (m_FullscreenMode == DXWINDOW_FULLSCREEN_MODE_FULLSCREEN) {
			hr = m_SwapChain->SetFullscreenState (
				FALSE,
				nullptr
			); CHECK_HR();
		}

		hr = UpdateWindowMode(); CHECK_HR();
	}

	m_Fullscreen = TRUE - m_Fullscreen; //m_Fullscreen = !m_Fullscreen

	return S_OK;
}