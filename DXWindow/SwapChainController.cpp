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

#include "SwapChainController.h"
#include "CDXWindow.h"

#define CHECK_HR() if (FAILED(hr)) return hr
#define CHECK_BRESULT() if (bresult == FALSE) return HRESULT_FROM_WIN32(GetLastError())

template <typename T>
static void Zero(T& t) {
	ZeroMemory(&t, sizeof(T));
}

//The back and front buffers will be BGRA.  The reason for this is that
//the Windows desktop is usually rendered in this format, which reduces
//the chance of a mode change when entering exclusive fullscreen mode,
//which in turn reduces the time it takes to enter it.  Also, Direct2D
//requires that this format be enabled, and we don't want to prevent
//the application from using it.
static const DXGI_FORMAT GLOBAL_DXGI_FORMAT = DXGI_FORMAT_B8G8R8A8_UNORM;

//Initialize our reference
SwapChainController::SwapChainController(CDXWindow& Window, OutputEnum& Enum) :
m_Window(Window),
m_OutputEnum(Enum),
m_Handle(NULL)
{ }

HRESULT SwapChainController::Initialize(CComPtr<IUnknown> DeviceUnk, CComPtr<IDXWindowCallback> Callback, HWND Handle) {
	HRESULT hr = S_OK;

	//Initialize references
	m_Callback = Callback;
	m_Handle = Handle;

	//Create the swap chain
	hr = CreateSwapChain (
		DeviceUnk
	); CHECK_HR();

	return S_OK;
}

HRESULT SwapChainController::CreateSwapChain(CComPtr<IUnknown> DeviceUnk) {
	HRESULT hr = S_OK;
	BOOL bresult = TRUE;

	DXGI_SWAP_CHAIN_DESC desc; Zero(desc);
	CComPtr<IDXGIFactory> factory;

	RECT WindowRect;

	//Retrieves the client area of the window, which tells us its renderable width and height
	bresult = GetClientRect (
		m_Handle,
		&WindowRect
	); CHECK_BRESULT();

	//Fill the swap chain description
	desc.BufferCount = 1;
	desc.BufferDesc.Format = GLOBAL_DXGI_FORMAT;
	desc.BufferDesc.Width = WindowRect.right - WindowRect.left;
	desc.BufferDesc.Height = WindowRect.bottom - WindowRect.top;
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.BufferDesc.RefreshRate.Denominator = 0;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.OutputWindow = m_Handle;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
	desc.Windowed = TRUE;

	//This is the factory that made the application's device
	hr = m_OutputEnum.GetAdapter()->GetParent (
		IID_PPV_ARGS(&factory)
	); CHECK_HR();

	//Use that factory to create the swap chain
	hr = factory->CreateSwapChain (
		DeviceUnk,
		&desc,
		&m_SwapChain
	); CHECK_HR();

	//Turn off alt-enter, since we're using F11 instead
	hr = factory->MakeWindowAssociation (
		m_Handle,
		DXGI_MWA_NO_WINDOW_CHANGES
	); CHECK_HR();

	return S_OK;
}

//Flip the swap chain at the requested frame interval
HRESULT SwapChainController::Present(UINT SyncInterval, UINT Flags) {
	return m_SwapChain->Present(SyncInterval, Flags);
}

//Retrieve the back buffer from the swap chain
HRESULT SwapChainController::GetBackBuffer(REFIID rIID, void** ppvBackBuffer) {
	return m_SwapChain->GetBuffer(0, rIID, ppvBackBuffer);
}

HRESULT SwapChainController::ToggleFullscreen() {
	HRESULT hr = S_OK;

	BOOL Fullscreen = IsFullscreen();

	if (Fullscreen == FALSE) {
		RECT DesktopArea;

		Output* output = m_OutputEnum.SearchOutput(m_Handle);

		if (output == nullptr) {
			return E_FAIL;
		}

		hr = output->GetDesktopArea (
			&DesktopArea
		); CHECK_HR();

		DXGI_MODE_DESC mode; Zero(mode);
		CComPtr<IDXGIOutput> obj = output->GetObj();

		mode.Width = DesktopArea.right - DesktopArea.left;
		mode.Height = DesktopArea.bottom - DesktopArea.top;

		//ResizeTarget() should be called before SetFullscreenState(), as per MSDN recommendations
		hr = m_SwapChain->ResizeTarget (
			&mode
		); CHECK_HR();

		hr = m_SwapChain->SetFullscreenState (
			TRUE,
			obj
		); CHECK_HR();

		hr = ResizeBuffers(); CHECK_HR();
	} else {
		hr = m_SwapChain->SetFullscreenState (
			FALSE,
			nullptr
		); CHECK_HR();
	}

	return S_OK;
}

HRESULT SwapChainController::ResizeBuffers() {
	HRESULT hr = S_OK;

	//The application should release all references to the back buffer,
	//otherwise ResizeBuffers() will fail
	m_Callback->OnBackBufferRelease(&m_Window);

	hr = m_SwapChain->ResizeBuffers (
		0,										//UINT BufferCount
		0,										//UINT Width
		0,										//UINT Height
		DXGI_FORMAT_UNKNOWN,					//DXGI_FORMAT NewFormat
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH	//UINT SwapChainFlags
	); CHECK_HR();

	//Now that the buffer is created, the application should now
	//retrieve its reference to the back buffer again
	m_Callback->OnBackBufferCreate(&m_Window);

	return S_OK;
}