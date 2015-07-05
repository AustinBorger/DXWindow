#pragma once

#include <dxgi.h>
#include <comdef.h>
#include <atlbase.h>
#include <vector>

/* This class represents a DXGI output and contains several convenience methods
** for translating DXGI stuff into regular winapi stuff. */
class Output final {
public:
	HRESULT Initialize(CComPtr<IDXGIOutput> obj);

	HRESULT GetWindowCenter(UINT Width, UINT Height, RECT* pRect, DWORD dwStyle, DWORD dwExStyle);

	HRESULT GetWorkArea(RECT* pRect);

	HRESULT GetDesktopArea(RECT* pRect);

	HMONITOR GetMonitor() {
		return m_Desc.Monitor;
	}

	LPCWSTR GetName() {
		return m_Desc.DeviceName;
	}

	CComPtr<IDXGIOutput> GetObj() {
		return m_Obj;
	}

private:
	CComPtr<IDXGIOutput> m_Obj;
	DXGI_OUTPUT_DESC m_Desc;
};