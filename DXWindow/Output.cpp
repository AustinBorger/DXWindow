#include "Output.h"
#include <vector>

#define CHECK_HR() if (FAILED(hr)) return hr
#define CHECK_BRESULT() if (bresult == FALSE) return HRESULT_FROM_WIN32(GetLastError())

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
HRESULT Output::Initialize(CComPtr<IDXGIOutput> obj) {
	HRESULT hr;

	m_Obj = obj;

	hr = m_Obj->GetDesc (
		&m_Desc
	); CHECK_HR();

	return S_OK;
}

HRESULT Output::GetWindowCenter(UINT Width, UINT Height, RECT* pRect, DWORD dwStyle, DWORD dwExStyle) {
	RECT center;
	MONITORINFOEXW monitor_info;
	RECT& work = monitor_info.rcWork;
	UINT work_width = 0;
	UINT work_height = 0;
	BOOL bresult;

	Zero(monitor_info);

	monitor_info.cbSize = sizeof(monitor_info);

	bresult = GetMonitorInfoW(m_Desc.Monitor, &monitor_info);
	CHECK_BRESULT();

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
	); CHECK_BRESULT();

	*pRect = center;

	return S_OK;
}

HRESULT Output::GetWorkArea(RECT* pRect) {
	MONITORINFOEXW m;
	BOOL bresult;

	Zero(m);

	m.cbSize = sizeof(m);

	bresult = GetMonitorInfoW (
		m_Desc.Monitor,
		&m
	); CHECK_BRESULT();

	*pRect = m.rcWork;

	return S_OK;
}

HRESULT Output::GetDesktopArea(RECT* pRect) {
	MONITORINFOEXW m;
	BOOL bresult;

	Zero(m);

	m.cbSize = sizeof(m);

	bresult = GetMonitorInfoW (
		m_Desc.Monitor,
		&m
	); CHECK_BRESULT();

	*pRect = m.rcMonitor;

	return S_OK;
}