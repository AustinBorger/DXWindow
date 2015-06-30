#include "SwapChainController.h"
#include "CDXWindow.h"

#define CHECK_HR() if (FAILED(hr)) return hr

template <typename T>
static void Zero(T& t) {
	ZeroMemory(&t, sizeof(T));
}

HRESULT SwapChainController::ToggleFullscreen() {
	HRESULT hr = S_OK;

	if (IsFullscreen() == FALSE) {
		RECT DesktopArea;

		Output* output = m_DXWindowFactory.SearchOutput(m_Handle);

		if (output == nullptr) {
			return E_FAIL;
		}

		hr = output->GetDesktopArea (
			&DesktopArea
		); CHECK_HR();

		if (m_FullscreenState == DXWINDOW_FULLSCREEN_STATE_FULLSCREEN) {
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

			m_Window.CenterCursor();
		} else { //FULLSCREEN_MODE_FULLSCREEN_WINDOW
			hr = FullscreenWindow (
				DesktopArea
			); CHECK_HR();

			m_Window.CenterCursor();
		}
	} else {
		if (m_FullscreenState == DXWINDOW_FULLSCREEN_STATE_FULLSCREEN) {
			hr = m_SwapChain->SetFullscreenState (
				FALSE,
				nullptr
			); CHECK_HR();
		}

		hr = UpdateWindowMode(); CHECK_HR();
	}

	return S_OK;
}

HRESULT SwapChainController::ResizeBuffers() {

}