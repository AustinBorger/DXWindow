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

/* This class detects changes in gamepad state and translates them to events
** in the form of method calls to the callback interface supplied by the
** application. */
class GamepadMessageDispatcher {
public:
	//Initialize reference to window, zero out m_PrevState
	GamepadMessageDispatcher(CDXWindow& Window);

	/* Initializes reference to the callback. */
	VOID Initialize(CComPtr<IDXWindowCallback> Callback);

	/* This is the event dispatch method. */
	VOID CheckGamepads();

private:
	CDXWindow& m_Window; //Reference to the associated window
	CComPtr<IDXWindowCallback> m_Callback; //Reference to the application-supplied callback
	static const UINT nMaxGamepads = 4; //Maximum number of gamepads that can be connected at once
	XINPUT_STATE m_PrevState[nMaxGamepads]; //The state of all the gamepads
};