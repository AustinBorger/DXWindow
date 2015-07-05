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