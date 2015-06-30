#include "DXWindow.h"
#include <atlbase.h>
#include "CDXWindow.h"

#pragma comment(lib, "XInput9_1_0.lib")
#pragma comment(lib, "dxgi.lib")

HRESULT DXWindowCreateWindow (
	const DXWINDOW_DESC* pDesc,
	IUnknown* pDevice,
	IDXWindowCallback* pDXWindowCallback,
	IDXWindow** ppDXWindow
) {
	HRESULT hr = S_OK;

	CComPtr<CDXWindow> Window = new CDXWindow();

	hr = Window->Initialize(*pDesc, pDevice, pDXWindowCallback);

	if (FAILED(hr)) {
		*ppDXWindow = nullptr;
		return hr;
	}

	*ppDXWindow = Window;

	return S_OK;
}