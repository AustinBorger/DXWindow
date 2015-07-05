#include "SwapChainController.h"
#include "CDXWindow.h"

#define CHECK_HR() if (FAILED(hr)) return hr
#define CHECK_BRESULT() if (bresult == FALSE) return HRESULT_FROM_WIN32(GetLastError())

template <typename T>
static void Zero(T& t) {
	ZeroMemory(&t, sizeof(T));
}

static const DXGI_FORMAT GLOBAL_DXGI_FORMAT = DXGI_FORMAT_B8G8R8A8_UNORM;

SwapChainController::SwapChainController(CDXWindow& Window, OutputEnum& Enum) :
m_Window(Window),
m_OutputEnum(Enum)
{ }

HRESULT SwapChainController::Initialize(CComPtr<IUnknown> DeviceUnk, CComPtr<IDXWindowCallback> Callback, HWND Handle) {
	HRESULT hr = S_OK;

	m_Callback = Callback;
	m_Handle = Handle;

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

	bresult = GetClientRect (
		m_Handle,
		&WindowRect
	); CHECK_BRESULT();

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

	hr = m_OutputEnum.GetAdapter()->GetParent (
		IID_PPV_ARGS(&factory)
	); CHECK_HR();

	hr = factory->CreateSwapChain (
		DeviceUnk,
		&desc,
		&m_SwapChain
	); CHECK_HR();

	hr = factory->MakeWindowAssociation (
		m_Handle,
		DXGI_MWA_NO_WINDOW_CHANGES
	); CHECK_HR();

	return S_OK;
}

HRESULT SwapChainController::Present(UINT SyncInterval, UINT Flags) {
	return m_SwapChain->Present(SyncInterval, Flags);
}

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

	m_Callback->OnBackBufferRelease(&m_Window);

	hr = m_SwapChain->ResizeBuffers (
		0,										//UINT BufferCount
		0,										//UINT Width
		0,										//UINT Height
		DXGI_FORMAT_UNKNOWN,					//DXGI_FORMAT NewFormat
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH	//UINT SwapChainFlags
	); CHECK_HR();

	m_Callback->OnBackBufferCreate(&m_Window);

	return S_OK;
}

HRESULT SwapChainController::GetDesktopArea(RECT& DesktopArea) {
	HRESULT hr = S_OK;

	Output* output = m_OutputEnum.SearchOutput(m_Handle);

	if (output == nullptr) {
		return E_FAIL;
	}

	hr = output->GetDesktopArea (
		&DesktopArea
	); CHECK_HR();

	return S_OK;
}