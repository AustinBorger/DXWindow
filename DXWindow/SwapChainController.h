#pragma once

#include <Windows.h>
#include <comdef.h>
#include <atlbase.h>
#include <dxgi.h>

#include "DXWindow.h"

class CDXWindow;

class SwapChainController {
public:
	SwapChainController(CDXWindow& Window);

	DXWINDOW_STATE GetState() {
		return m_State;
	}

	HRESULT SetState(DXWINDOW_STATE State);

	HRESULT ToggleFullscreen();

private:
	CDXWindow& m_Window;
	CComPtr<IDXGISwapChain> m_SwapChain;
	DXWINDOW_STATE m_State;
	DXWINDOW_FULLSCREEN_STATE m_FullscreenState;
	DXWINDOW_WINDOW_STATE m_WindowState;

	BOOL IsFullscreen() {
		if (m_State == DXWINDOW_STATE_FULLSCREEN ||
			m_State == DXWINDOW_STATE_FULLSCREEN_WINDOW) {
			return TRUE;
		}

		return FALSE;
	}

	HRESULT ResizeBuffers();
};