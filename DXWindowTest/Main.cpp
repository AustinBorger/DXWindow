#include "DXWindow.h"
#include "QueryInterface.h"
#include <atlbase.h>
#include <d3d11.h>
#include <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXWindow.lib")

void HandleHR(LPCWSTR File, UINT Line, HRESULT hr) {
	if (FAILED(hr)) {
		_com_error e(hr);
		std::wstring Message;

		Message.append(File);
		Message.append(L" @ Line ");
		Message.append(std::to_wstring(Line));
		Message.append(L":\n\n");
		Message.append(e.ErrorMessage());

		MessageBoxW(NULL, Message.c_str(), L"Error", MB_ICONERROR | MB_OK);

		exit(hr);
	}
}

int main() {
	CComPtr<IDXWindow> Window;

	class X : public CDXWindowCallback {
	public:
		X() : run(true) { }

		STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) {
			QUERY_INTERFACE_CAST(IDXWindowCallback);
			QUERY_INTERFACE_CAST(IUnknown);
			QUERY_INTERFACE_FAIL();
		}

		ULONG STDMETHODCALLTYPE AddRef() {
			return 1;
		}

		ULONG STDMETHODCALLTYPE Release() {
			return 1;
		}

		VOID STDMETHODCALLTYPE OnObjectFailure(LPCWSTR File, UINT Line, HRESULT hr) {
			HandleHR(File, Line, hr);
		}

		VOID STDMETHODCALLTYPE OnWindowClose(IDXWindow* Window) {
			run = false;
		}

		bool run;
	} x;

	DXWINDOW_DESC Desc;

	Desc.AllowToggle = TRUE;
	Desc.Cursor = NULL;
	Desc.FullscreenState = DXWINDOW_FULLSCREEN_STATE_FULLSCREEN;
	Desc.Height = 720;
	Desc.IconLarge = NULL;
	Desc.IconSmall = NULL;
	Desc.InitFullscreen = TRUE;
	Desc.Instance = GetModuleHandleW(NULL);
	Desc.Title = L"DXWindowTest";
	Desc.Width = 1280;
	Desc.WindowState = DXWINDOW_WINDOW_STATE_WINDOWED;

	CComPtr<ID3D11Device> Device;
	CComPtr<ID3D11DeviceContext> DeviceContext;

	HRESULT hr = D3D11CreateDevice (
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&Device,
		NULL,
		&DeviceContext
	); HandleHR(L"Main.cpp", __LINE__, hr);

	hr = DXWindowCreateWindow (
		&Desc,
		Device,
		&x,
		&Window
	); HandleHR(L"Main.cpp", __LINE__, hr);

	while (x.run) {
		Window->PumpMessages();

		Window->Present(1, 0);
	}

	return 0;
}