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

#ifdef _DXWINDOW_SUPPORT_12

#include "GamepadMessageDispatcher12.h"
#include "CDXWindow12.h"

#define FILENAME L"GamepadMessageDispatcher12.cpp"
#define CHECK_ERR(Line) if (err != ERROR_SUCCESS) m_Callback->OnObjectFailure(FILENAME, Line, HRESULT_FROM_WIN32(err)); return

//Initialize reference to the window and zero out m_PrevState
GamepadMessageDispatcher12::GamepadMessageDispatcher12(CDXWindow12& Window) :
m_Window(Window) {
	ZeroMemory(m_PrevState, sizeof(m_PrevState));
}

//Initializes the reference to the callback object
VOID GamepadMessageDispatcher12::Initialize(CComPtr<IDXWindowCallback> Callback) {
	m_Callback = Callback;
}

VOID GamepadMessageDispatcher12::CheckGamepads() {
	DWORD err = ERROR_SUCCESS;
	XINPUT_STATE xstate;

	for (DWORD i = 0; i < nMaxGamepads; i++) {
		err = XInputGetState(i, &xstate);

		//If the device isn't connected, just skip it
		if (err == ERROR_DEVICE_NOT_CONNECTED) {
			continue;
		} else {
			CHECK_ERR(__LINE__);
		}

		if (xstate.dwPacketNumber > m_PrevState[i].dwPacketNumber) {
			const WORD wButtonDelta = xstate.Gamepad.wButtons ^ m_PrevState[i].Gamepad.wButtons; //Change in button orientation
			const WORD wButtonDown = xstate.Gamepad.wButtons & wButtonDelta; //Buttons that have been pressed
			const WORD wButtonUp = wButtonDown ^ wButtonDelta; //Buttons that have been released

			//Check the button presses
			if (wButtonDown) {
				for (UINT j = 0; j < sizeof(WORD) * 8; j++) {
					if ((wButtonDown >> j) & 0x1) {
						m_Callback->OnGamepadButtonDown(&m_Window, i, (0x1 << j));
					}
				}
			}

			//Check the button releases
			if (wButtonUp) {
				for (UINT j = 0; j < sizeof(WORD) * 8; j++) {
					if ((wButtonUp >> j) & 0x1) {
						m_Callback->OnGamepadButtonUp(&m_Window, i, (0x1 << j));
					}
				}
			}

			//Check the position of the left trigger
			if (xstate.Gamepad.bLeftTrigger != m_PrevState[i].Gamepad.bLeftTrigger) {
				m_Callback->OnGamepadLeftTriggerMove(&m_Window, i, xstate.Gamepad.bLeftTrigger);
			}

			//Check the position of the right trigger
			if (xstate.Gamepad.bRightTrigger != m_PrevState[i].Gamepad.bRightTrigger) {
				m_Callback->OnGamepadRightTriggerMove(&m_Window, i, xstate.Gamepad.bRightTrigger);
			}

			//Check the position of the left analog
			if (!(xstate.Gamepad.sThumbLX == m_PrevState[i].Gamepad.sThumbLX &&
				  xstate.Gamepad.sThumbLY == m_PrevState[i].Gamepad.sThumbLY)) {
				m_Callback->OnGamepadLeftAnalogMove(&m_Window, i, xstate.Gamepad.sThumbLX, xstate.Gamepad.sThumbLY);
			}

			//Check the position of the right analog
			if (!(xstate.Gamepad.sThumbRX == m_PrevState[i].Gamepad.sThumbRX &&
				  xstate.Gamepad.sThumbRY == m_PrevState[i].Gamepad.sThumbRY)) {
				m_Callback->OnGamepadRightAnalogMove(&m_Window, i, xstate.Gamepad.sThumbRX, xstate.Gamepad.sThumbRY);
			}

			//Store the current state in the previous state for the next frame to check against
			m_PrevState[i] = xstate;
		}
	}
}

#endif