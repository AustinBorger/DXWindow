#include "OutputEnum.h"

#define CHECK_HR() if (FAILED(hr)) return hr

OutputEnum::OutputEnum()
{ }

OutputEnum::~OutputEnum() 
{ }

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
	hr = Output::InitializeList (
		m_Adapter,
		m_Outputs
	); CHECK_HR();

	return S_OK;
}