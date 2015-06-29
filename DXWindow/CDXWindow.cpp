#include "CDXWindow.h"

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
				hr = ToggleFullscreen(); CHECK_HR();
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