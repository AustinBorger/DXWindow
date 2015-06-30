#pragma once

#include <Windows.h>
#include <comdef.h>
#include <atlbase.h>
#include <dxgi.h>
#include <vector>

#include "Output.h"
#include "QueryInterface.h"

class __declspec(uuid("7113460f-dbdd-4e41-bac2-92a22facbfaf")) DXGIServer : public IUnknown {
public:
	DXGIServer();

	~DXGIServer();

	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) {
		QUERY_INTERFACE_CAST(DXGIServer);
		QUERY_INTERFACE_CAST(IUnknown);
		QUERY_INTERFACE_FAIL();
	}

	ULONG STDMETHODCALLTYPE AddRef() {
		return InterlockedIncrement(&m_RefCount);
	}

	ULONG STDMETHODCALLTYPE Release() {
		long l_RefCount = InterlockedDecrement(&m_RefCount);

		if (l_RefCount <= 0) {
			delete this;
			return 0;
		}

		return l_RefCount;
	}

	HRESULT Initialize();

	Output* SearchOutput(HWND Handle) {
		return Output::SearchOutput(Handle, m_Outputs);
	}

	Output* PrimaryOutput() {
		return Output::PrimaryOutput(m_Outputs);
	}

private:
	long m_RefCount;

	std::vector<Output> m_Outputs;
	CComPtr<IDXGIAdapter> m_Adapter;
};