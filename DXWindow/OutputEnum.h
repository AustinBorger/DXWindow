#pragma once

#include <Windows.h>
#include <comdef.h>
#include <atlbase.h>
#include <dxgi.h>
#include <vector>

#include "Output.h"
#include "QueryInterface.h"

/* This class acts as a collection of all the outputs on the device's adapter. */
class OutputEnum {
public:
	OutputEnum();

	~OutputEnum();

	/* Populates the output list. */
	HRESULT Initialize(CComPtr<IUnknown> DeviceUnk);

	/* Searches the output list for the output that the window occupies. */
	Output* SearchOutput(HWND Handle);

	/* Returns the primary output of the adapter. */
	Output* PrimaryOutput() {
		return &m_Outputs.front();
	}

	/* Returns the adapter. */
	CComPtr<IDXGIAdapter> GetAdapter() {
		return m_Adapter;
	}

private:
	std::vector<Output> m_Outputs; //The output list
	CComPtr<IDXGIAdapter> m_Adapter; //The adapter
};