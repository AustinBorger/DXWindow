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

#include "GamepadMessageDispatcher.h"
#include "CDXWindow.h"

#define CHECK_ERR(Line) if (err != ERROR_SUCCESS) m_Callback->OnObjectFailure(L"GamepadMessageDispatcher.cpp", Line, HRESULT_FROM_WIN32(err)); return

GamepadMessageDispatcher::GamepadMessageDispatcher(CDXWindow& Window) :
m_Window(Window) {
	ZeroMemory(m_PrevState, sizeof(m_PrevState));
}

GamepadMessageDispatcher::~GamepadMessageDispatcher() 
{ }

HRESULT GamepadMessageDispatcher::Initialize(CComPtr<IDXWindowCallback> Callback) {
	m_Callback = Callback;

	return S_OK;
}

void GamepadMessageDispatcher::CheckGamepads() {
	DWORD err = ERROR_SUCCESS;
	XINPUT_STATE xstate;

	for (DWORD i = 0; i < nMaxGamepads; i++) {
		err = XInputGetState(i, &xstate);

		if (err == ERROR_DEVICE_NOT_CONNECTED) {
			continue;
		} else {
			CHECK_ERR(__LINE__);
		}

		if (xstate.dwPacketNumber > m_PrevState[i].dwPacketNumber) {
			const WORD wButtonDelta = xstate.Gamepad.wButtons ^ m_PrevState[i].Gamepad.wButtons;
			const WORD wButtonDown = xstate.Gamepad.wButtons & wButtonDelta;
			const WORD wButtonUp = wButtonDown ^ wButtonDelta;

			if (wButtonDown) {
				for (UINT j = 0; j < sizeof(WORD) * 8; j++) {
					if ((wButtonDown >> j) & 0x1) {
						m_Callback->OnGamepadButtonDown(&m_Window, i, (0x1 << j));
					}
				}
			}

			if (wButtonUp) {
				for (UINT j = 0; j < sizeof(WORD) * 8; j++) {
					if ((wButtonUp >> j) & 0x1) {
						m_Callback->OnGamepadButtonUp(&m_Window, i, (0x1 << j));
					}
				}
			}

			if (xstate.Gamepad.bLeftTrigger != m_PrevState[i].Gamepad.bLeftTrigger) {
				m_Callback->OnGamepadLeftTriggerMove(&m_Window, i, xstate.Gamepad.bLeftTrigger);
			}

			if (xstate.Gamepad.bRightTrigger != m_PrevState[i].Gamepad.bRightTrigger) {
				m_Callback->OnGamepadRightTriggerMove(&m_Window, i, xstate.Gamepad.bRightTrigger);
			}

			if (!(xstate.Gamepad.sThumbLX == m_PrevState[i].Gamepad.sThumbLX &&
				  xstate.Gamepad.sThumbLY == m_PrevState[i].Gamepad.sThumbLY)) {
				m_Callback->OnGamepadLeftAnalogMove(&m_Window, i, xstate.Gamepad.sThumbLX, xstate.Gamepad.sThumbLY);
			}

			if (!(xstate.Gamepad.sThumbRX == m_PrevState[i].Gamepad.sThumbRX &&
				  xstate.Gamepad.sThumbRY == m_PrevState[i].Gamepad.sThumbRY)) {
				m_Callback->OnGamepadRightAnalogMove(&m_Window, i, xstate.Gamepad.sThumbRX, xstate.Gamepad.sThumbRY);
			}

			m_PrevState[i] = xstate;
		}
	}
}