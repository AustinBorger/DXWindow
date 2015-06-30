#pragma once

#include "DXWindow.h"
#include <dxgi.h>

#include "WindowMessageDispatcher.h"

class CDXWindow : public IDXWindow {
public:
	LRESULT WindowProcess(UINT Message, WPARAM wParam, LPARAM lParam);

private:
	HWND m_Handle;
	WindowMessageDispatcher m_WindowMessageDispatcher;
};