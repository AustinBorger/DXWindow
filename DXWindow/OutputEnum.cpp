#include "OutputEnum.h"

#define CHECK_HR() if (FAILED(hr)) return hr

OutputEnum::OutputEnum()
{ }

OutputEnum::~OutputEnum() 
{ }

HRESULT OutputEnum::Initialize(CComPtr<IUnknown> DeviceUnk) {
	HRESULT hr = S_OK;
	CComPtr<IDXGIDevice> DxgiDevice;

	hr = DeviceUnk->QueryInterface (
		IID_PPV_ARGS(&DxgiDevice)
	); CHECK_HR();

	hr = DxgiDevice->GetParent (
		IID_PPV_ARGS(&m_Adapter)
	); CHECK_HR();

	hr = Output::InitializeList (
		m_Adapter,
		m_Outputs
	); CHECK_HR();

	return S_OK;
}