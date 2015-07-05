#pragma once

#include <dxgi.h>
#include <comdef.h>
#include <atlbase.h>
#include <vector>

/* This class represents a DXGI output and contains several convenience methods
** for translating DXGI stuff into regular winapi stuff. */
class Output final {
public:
	/* Sets all the description variables to zero. */
	Output();

	/* Initializes the description and DXGI object. */
	HRESULT Initialize(CComPtr<IDXGIOutput> obj);

	/* Returns a rect specifying the position of a centered window given its
	** dimensions and style. */
	HRESULT GetWindowCenter(UINT Width, UINT Height, RECT* pRect, DWORD dwStyle, DWORD dwExStyle);

	/* Returns the desktop work area of the output. */
	HRESULT GetWorkArea(RECT* pRect);

	/* Returns the desktop area of the output. */
	HRESULT GetDesktopArea(RECT* pRect);

	/* Returns the output's monitor handle. */
	HMONITOR GetMonitor() {
		return m_Desc.Monitor;
	}

	/* Returns the device name of the output. */
	LPCWSTR GetName() {
		return m_Desc.DeviceName;
	}

	/* Returns the associated DXGIOutput object. */
	CComPtr<IDXGIOutput> GetObj() {
		return m_Obj;
	}

private:
	CComPtr<IDXGIOutput> m_Obj; //The associated DXGIOutput object
	DXGI_OUTPUT_DESC m_Desc; //The output's description
};