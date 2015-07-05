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

#pragma once

#include <Windows.h>
#include <Xinput.h>
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