#pragma once

#include <Xinput.h>
#include <Windows.h>
#include <comdef.h>
#include <atlbase.h>
#include "DXWindow.h"

class CDXWindow;

class GamepadMessageDispatcher {
public:
	GamepadMessageDispatcher(CDXWindow& Window);

	~GamepadMessageDispatcher();

	HRESULT Initialize(CComPtr<IDXWindowCallback> Callback);

	void CheckGamepads();

private:
	CDXWindow& m_Window;
	CComPtr<IDXWindowCallback> m_Callback;
	static const UINT nMaxGamepads = 4;
	XINPUT_STATE m_PrevState[nMaxGamepads];
};