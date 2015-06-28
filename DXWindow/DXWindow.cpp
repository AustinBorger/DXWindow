#include "DXWindow.h"

#pragma comment(lib, "XInput9_1_0.lib")
#pragma comment(lib, "dxgi.lib")

HRESULT DXWindowCreateWindow (
	const DXWINDOW_DESC* pDesc,
	IUnknown* pDevice,
	IDXWindowCallback* pDXWindowCallback,
	IDXWindow** ppDXWindow
) {
	return S_OK;
}