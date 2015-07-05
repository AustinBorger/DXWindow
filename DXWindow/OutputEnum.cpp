#include "OutputEnum.h"

#define CHECK_HR() if (FAILED(hr)) return hr

OutputEnum::OutputEnum()
{ }

OutputEnum::~OutputEnum() 
{ }

#include <iostream>

HRESULT OutputEnum::Initialize(CComPtr<IUnknown> DeviceUnk) {
	HRESULT hr = S_OK;
	CComPtr<IDXGIDevice> DxgiDevice;

	//All D3D devices implement IDXGIDevice
	hr = DeviceUnk->QueryInterface (
		IID_PPV_ARGS(&DxgiDevice)
	); CHECK_HR();

	//The parent of a DXGI device is an adapter
	hr = DxgiDevice->GetParent (
		IID_PPV_ARGS(&m_Adapter)
	); CHECK_HR();

	//Populate the output list
	UINT index = 0;
	CComPtr<IDXGIOutput> output;

	while (true) {
		//Retrieves all outputs, even inactive ones
		hr = m_Adapter->EnumOutputs (
			index,
			&output
		);

		if (hr == DXGI_ERROR_NOT_FOUND) {
			break;
		} else {
			CHECK_HR();
		}

		m_Outputs.push_back(Output());

		hr = m_Outputs.back().Initialize(output);

		CHECK_HR();

		output.Release();

		index++;
	}

	return S_OK;
}

Output* OutputEnum::SearchOutput(HWND Handle) {
	//Gets the nearest monitor
	HMONITOR monitor = MonitorFromWindow (
		Handle,
		MONITOR_DEFAULTTONEAREST
	);

	//Finds the output with the same monitor handle
	for (Output& output : m_Outputs) {
		if (output.GetMonitor() == monitor) {
			return &output;
		}
	}

	//This probably will never happen
	return nullptr;
}