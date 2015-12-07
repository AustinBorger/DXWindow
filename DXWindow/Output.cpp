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

#include "Output.h"
#include <vector>

#ifdef _DEBUG

#include <iostream>

#endif

#define FILENAME L"Output.cpp"
#define CHECK_HR(Line) if (FAILED(hr)) m_Callback->OnObjectFailure(FILENAME, Line, hr)
#define CHECK_BRESULT(Line) if (bresult == FALSE) { HRESULT hr = HRESULT_FROM_WIN32(GetLastError()); if (FAILED(hr)) m_Callback->OnObjectFailure(L"Output.cpp", Line, hr); }

template <typename T>
static void Zero(T& t) {
	ZeroMemory(&t, sizeof(T));
}

static const DWORD DM_FIELDS = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFLAGS | DM_DISPLAYFREQUENCY;

//Zero out the description structure
Output::Output() {
	Zero(m_Desc);
}

//Initialize the description structure and DXGIOutput object reference
VOID Output::Initialize(CComPtr<IDXGIOutput> obj, CComPtr<IDXWindowCallback> Callback) {
	HRESULT hr;

	m_Obj = obj;

	hr = m_Obj->GetDesc (
		&m_Desc
	); CHECK_HR(__LINE__);

	// CONSOLE OUTPUT

#ifdef _DEBUG

	const wchar_t* rotation = nullptr;

	if (m_Desc.Rotation == DXGI_MODE_ROTATION_IDENTITY) {
		rotation = L"DXGI_MODE_ROTATION_IDENTITY";
	} else if (m_Desc.Rotation == DXGI_MODE_ROTATION_ROTATE90) {
		rotation = L"DXGI_MODE_ROTATION_ROTATE90";
	} else if (m_Desc.Rotation == DXGI_MODE_ROTATION_ROTATE180) {
		rotation = L"DXGI_MODE_ROTATION_ROTATE180";
	} else if (m_Desc.Rotation == DXGI_MODE_ROTATION_ROTATE270) {
		rotation = L"DXGI_MODE_ROTATION_ROTATE270";
	} else {
		rotation = L"DXGI_MODE_ROTATION_UNSPECIFIED";
	}

	std::wcout << "Output::Initialize():" << std::endl;
	std::wcout << "\t" << "AttachedToDesktop: " << (m_Desc.AttachedToDesktop ? "true" : "false") << std::endl;
	std::wcout << "\t" << "DesktopCoordinates.left: " << m_Desc.DesktopCoordinates.left << std::endl;
	std::wcout << "\t" << "DesktopCoordinates.right: " << m_Desc.DesktopCoordinates.right << std::endl;
	std::wcout << "\t" << "DesktopCoordinates.top: " << m_Desc.DesktopCoordinates.top << std::endl;
	std::wcout << "\t" << "DesktopCoordinates.bottom: " << m_Desc.DesktopCoordinates.bottom << std::endl;
	std::wcout << "\t" << "DesktopCoordinates - width: " << m_Desc.DesktopCoordinates.right - m_Desc.DesktopCoordinates.left << std::endl;
	std::wcout << "\t" << "DesktopCoordinates - height: " << m_Desc.DesktopCoordinates.bottom - m_Desc.DesktopCoordinates.top << std::endl;
	std::wcout << "\t" << "DeviceName: " << m_Desc.DeviceName << std::endl;
	std::wcout << "\t" << "Monitor: 0x" << (void*)(m_Desc.Monitor) << std::endl;
	std::wcout << "\t" << "Rotation: " << rotation << std::endl;

#endif

}

VOID Output::GetWindowCenter(UINT Width, UINT Height, RECT* pRect, DWORD dwStyle, DWORD dwExStyle) {
	RECT center;
	MONITORINFOEXW monitor_info;
	RECT& work = monitor_info.rcWork;
	UINT work_width = 0;
	UINT work_height = 0;
	BOOL bresult;

	Zero(monitor_info);

	monitor_info.cbSize = sizeof(monitor_info);

	bresult = GetMonitorInfoW (
		m_Desc.Monitor,
		&monitor_info
	); CHECK_BRESULT(__LINE__);

	work_width = work.right - work.left;
	work_height = work.bottom - work.top;

	//Generate a center rect for a pure client window
	center.left = work.left + (work_width - Width) / 2;
	center.right = center.left + Width;
	center.top = work.top + (work_height - Height) / 2;
	center.bottom = center.top + Height;

	//Adjusts the rect based on border dimensions, etc
	bresult = AdjustWindowRectEx (
		&center,
		dwStyle,
		FALSE,
		dwExStyle
	); CHECK_BRESULT(__LINE__);

	*pRect = center;

	// CONSOLE OUTPUT

#ifdef _DEBUG

	std::wcout << "Output::GetWindowCenter():" << std::endl;
	std::wcout << "\t" << "width (including border): " << pRect->right - pRect->left << std::endl;
	std::wcout << "\t" << "height (including border): " << pRect->bottom - pRect->top << std::endl;
	std::wcout << "\t" << "xPos: " << pRect->left << std::endl;
	std::wcout << "\t" << "yPos: " << pRect->top << std::endl;

#endif

}

VOID Output::GetWorkArea(RECT* pRect) {
	MONITORINFOEXW m;
	BOOL bresult;

	Zero(m);

	m.cbSize = sizeof(m);

	bresult = GetMonitorInfoW (
		m_Desc.Monitor,
		&m
	); CHECK_BRESULT(__LINE__);

	*pRect = m.rcWork;

	// CONSOLE OUTPUT

#ifdef _DEBUG

	std::wcout << "Output::GetWorkArea():" << std::endl;
	std::wcout << "\t" << "width: " << pRect->right - pRect->left << std::endl;
	std::wcout << "\t" << "height: " << pRect->bottom - pRect->top << std::endl;
	std::wcout << "\t" << "xPos: " << pRect->left << std::endl;
	std::wcout << "\t" << "yPos: " << pRect->top << std::endl;

#endif

}

VOID Output::GetDesktopArea(RECT* pRect) {
	MONITORINFOEXW m;
	BOOL bresult;

	Zero(m);

	m.cbSize = sizeof(m);

	bresult = GetMonitorInfoW (
		m_Desc.Monitor,
		&m
	); CHECK_BRESULT(__LINE__);

	*pRect = m.rcMonitor;

	// CONSOLE OUTPUT

#ifdef _DEBUG

	std::wcout << "Output::GetDesktopArea():" << std::endl;
	std::wcout << "\t" << "width: " << pRect->right - pRect->left << std::endl;
	std::wcout << "\t" << "height: " << pRect->bottom - pRect->top << std::endl;

#endif

}