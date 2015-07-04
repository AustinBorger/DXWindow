#pragma once

#include <Windows.h>
#include <comdef.h>
#include <atlbase.h>
#include <dxgi.h>
#include <vector>

#include "Output.h"
#include "QueryInterface.h"

class OutputWatcher {
public:
	OutputWatcher();

	~OutputWatcher();

	HRESULT Initialize(CComPtr<IUnknown> DeviceUnk);

	Output* SearchOutput(HWND Handle) {
		return Output::SearchOutput(Handle, m_Outputs);
	}

	Output* PrimaryOutput() {
		return Output::PrimaryOutput(m_Outputs);
	}

	CComPtr<IDXGIAdapter> GetAdapter() {
		return m_Adapter;
	}

private:
	std::vector<Output> m_Outputs;
	CComPtr<IDXGIAdapter> m_Adapter;
};