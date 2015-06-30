#pragma once

#include "DXWindow.h"
#include <dxgi.h>

#include "WindowMessageDispatcher.h"
#include "GamepadMessageDispatcher.h"
#include "SwapChainController.h"
#include "QueryInterface.h"

class CDXWindow : public IDXWindow {
public:
	CDXWindow();

	~CDXWindow();

	//IUnknown methods

	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) final {
		QUERY_INTERFACE_CAST(IDXWindow);
		QUERY_INTERFACE_CAST(IUnknown);
		QUERY_INTERFACE_FAIL();
	}

	ULONG STDMETHODCALLTYPE AddRef() {
		//No thread safety protection.
		return ++m_RefCount;
	}

	ULONG STDMETHODCALLTYPE Release() {
		//No thread safety protection.
		m_RefCount--;

		if (m_RefCount <= 0) {
			delete this;
			return 0;
		}

		return m_RefCount;
	}

	//IDXWindow methods

	VOID STDMETHODCALLTYPE PumpMessages() final;

	VOID STDMETHODCALLTYPE Present(UINT SyncInterval, UINT Flags) final;

	WORD STDMETHODCALLTYPE GetWindowWidth() final;

	WORD STDMETHODCALLTYPE GetWindowHeight() final;

	VOID STDMETHODCALLTYPE SetWindowResolution(WORD Width, WORD Height) final;

	DXWINDOW_STATE STDMETHODCALLTYPE GetState() final {
		return m_SwapChainController.GetState();
	}

	VOID STDMETHODCALLTYPE SetState(DXWINDOW_STATE State) final;

	BOOL STDMETHODCALLTYPE GetAllowToggle() final {
		return m_AllowToggle;
	}

	VOID STDMETHODCALLTYPE SetAllowToggle(BOOL AllowToggle) final {
		m_AllowToggle = AllowToggle;
	}

	VOID STDMETHODCALLTYPE GetBackBuffer(REFIID rIID, void** ppvBackBuffer) final;

	//New methods

	LRESULT WindowProcess(UINT Message, WPARAM wParam, LPARAM lParam);

	HRESULT Initialize(const DXWINDOW_DESC& Desc, IUnknown* pDevice, IDXWindowCallback* pDXWindowCallback);

	VOID CenterCursor();

private:
	long m_RefCount;

	HWND m_Handle; //The window handle
	WindowMessageDispatcher m_WindowMessageDispatcher; //Dispatches window messages to the callback
	GamepadMessageDispatcher m_GamepadMessageDispatcher; //Dispatches gamepad messages to the callback
	SwapChainController m_SwapChainController;

	BOOL m_AllowToggle;
};