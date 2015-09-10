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

#ifdef _DXWINDOW_SUPPORT_12

#include "OutputEnum12.h"
#include <d3d12.h>

#define FILENAME L"OutputEnum12.cpp"
#define CHECK_HR(Line) if (FAILED(hr)) { m_Callback->OnObjectFailure(FILENAME, Line, hr); return E_FAIL; }

OutputEnum12::OutputEnum12()
{ }

OutputEnum12::~OutputEnum12() 
{ }

HRESULT OutputEnum12::Initialize(CComPtr<IUnknown> DeviceUnk, CComPtr<IDXWindowCallback> Callback) {
	HRESULT hr = S_OK;
	CComPtr<ID3D12CommandQueue> CommandQueue;
	CComPtr<ID3D12Device> Device;
	CComPtr<IDXGIFactory4> Factory;

	m_Callback = Callback;

	//All D3D11 devices implement IDXGIDevice
	hr = DeviceUnk->QueryInterface (
		IID_PPV_ARGS(&CommandQueue)
	); CHECK_HR(__LINE__);

	hr = CommandQueue->GetDevice (
		IID_PPV_ARGS(&Device)
	); CHECK_HR(__LINE__);

	//Get the adapter's luid
	LUID AdapterLuid = Device->GetAdapterLuid();

	//Create a factory and find the adapter
	hr = CreateDXGIFactory (
		IID_PPV_ARGS(&Factory)
	); CHECK_HR(__LINE__);

	hr = Factory->EnumAdapterByLuid (
		AdapterLuid,
		IID_PPV_ARGS(&m_Adapter)
	); CHECK_HR(__LINE__);

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
			CHECK_HR(__LINE__);
		}

		m_Outputs.push_back(Output12());

		m_Outputs.back().Initialize(output, m_Callback);

		output.Release();

		index++;
	}

	return S_OK;
}

Output12* OutputEnum12::SearchOutput(HWND Handle) {
	//Gets the nearest monitor
	HMONITOR monitor = MonitorFromWindow (
		Handle,
		MONITOR_DEFAULTTONEAREST
	);

	//Finds the output with the same monitor handle
	for (Output12& output : m_Outputs) {
		if (output.GetMonitor() == monitor) {
			return &output;
		}
	}

	//This probably will never happen, but if the output isn't found we have a problem.
	m_Callback->OnObjectFailure(FILENAME, __LINE__, E_FAIL);

	return nullptr;
}

//Return the first output in the list
Output12* OutputEnum12::PrimaryOutput() {
	return &m_Outputs.front();
}

#endif