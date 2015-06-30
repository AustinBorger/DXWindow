#pragma once

#include "DXWindow.h"
#include <dxgi.h>

#include "WindowMessageDispatcher.h"
#include "GamepadMessageDispatcher.h"

class CDXWindow : public IDXWindow {
public:
	LRESULT WindowProcess(UINT Message, WPARAM wParam, LPARAM lParam);

	HRESULT Initialize(const DXWINDOW_DESC& Desc, IUnknown* pDevice, IDXWindowCallback* pDXWindowCallback);

private:
	HWND m_Handle;
	WindowMessageDispatcher m_WindowMessageDispatcher;
	GamepadMessageDispatcher m_GamepadMessageDispatcher;
};