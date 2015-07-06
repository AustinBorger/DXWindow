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

#include "DXWindow.h"
#include "QueryInterface.h"
#include <atlbase.h>
#include <d3d11.h>
#include <string>
#include <iostream>

#include "VertexShader.h"
#include "PixelShader.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DXWindow.lib")

void HandleHR(LPCWSTR File, UINT Line, HRESULT hr) {
	if (FAILED(hr)) {
		_com_error e(hr);
		std::wstring Message;

		Message.append(File);
		Message.append(L" @ Line ");
		Message.append(std::to_wstring(Line));
		Message.append(L":\n\n");
		Message.append(e.ErrorMessage());

		MessageBoxW(NULL, Message.c_str(), L"Error", MB_ICONERROR | MB_OK);

		exit(hr);
	}
}

int main() {
	CComPtr<IDXWindow> Window;

	class X : public CDXWindowCallback {
	public:
		X() : run(true) { }

		STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) {
			QUERY_INTERFACE_CAST(IDXWindowCallback);
			QUERY_INTERFACE_CAST(IUnknown);
			QUERY_INTERFACE_FAIL();
		}

		ULONG STDMETHODCALLTYPE AddRef() {
			return 1;
		}

		ULONG STDMETHODCALLTYPE Release() {
			return 1;
		}

		VOID STDMETHODCALLTYPE OnObjectFailure(LPCWSTR File, UINT Line, HRESULT hr) final {
			HandleHR(File, Line, hr);
		}

		VOID STDMETHODCALLTYPE OnWindowClose(IDXWindow* Window) final {
			run = false;
		}

		VOID STDMETHODCALLTYPE OnKeyDown(IDXWindow* Window, WPARAM wParam, LPARAM lParam) final {
			if (wParam == VK_F1) {
				Window->SetState(DXWINDOW_STATE_WINDOWED);
			} else if (wParam == VK_F2) {
				Window->SetState(DXWINDOW_STATE_BORDERLESS);
			} else if (wParam == VK_F3) {
				Window->SetState(DXWINDOW_STATE_FULLSCREEN_WINDOW);
			} else if (wParam == VK_F4) {
				Window->SetState(DXWINDOW_STATE_FULLSCREEN);
			}
		}

		VOID STDMETHODCALLTYPE OnBackBufferRelease(IDXWindow* Window) final {
			std::cout << "OnBackBufferRelease()" << std::endl;
			BackBufTex.Release();
			BackBufRTV.Release();
		}

		VOID STDMETHODCALLTYPE OnBackBufferCreate(IDXWindow* Window) final {
			HRESULT hr = S_OK;
			Window->GetBackBuffer(IID_PPV_ARGS(&BackBufTex));

			D3D11_TEXTURE2D_DESC Desc;
			BackBufTex->GetDesc(&Desc);

			viewport.MaxDepth = 1.0f;
			viewport.MinDepth = 0.0f;
			viewport.TopLeftX = 0.0f;
			viewport.TopLeftY = 0.0f;
			viewport.Width = (FLOAT)(Desc.Width);
			viewport.Height = (FLOAT)(Desc.Height);

			std::cout << "New back buffer: " << viewport.Width << ", " << viewport.Height << std::endl;

			hr = Device->CreateRenderTargetView (
				BackBufTex,
				nullptr,
				&BackBufRTV
			); HandleHR(L"Main.cpp", __LINE__, hr);
		}

		bool run;
		CComPtr<ID3D11Texture2D> BackBufTex;
		CComPtr<ID3D11RenderTargetView> BackBufRTV;
		CComPtr<ID3D11Device> Device;
		CComPtr<ID3D11DeviceContext> DeviceContext;
		D3D11_VIEWPORT viewport;
	} x;

	DXWINDOW_DESC Desc;

	Desc.AllowToggle = TRUE;
	Desc.Cursor = NULL;
	Desc.FullscreenState = DXWINDOW_FULLSCREEN_STATE_FULLSCREEN;
	Desc.Height = 720;
	Desc.IconLarge = NULL;
	Desc.IconSmall = NULL;
	Desc.InitFullscreen = FALSE;
	Desc.Instance = GetModuleHandleW(NULL);
	Desc.Title = L"DXWindowTest";
	Desc.Width = 1280;
	Desc.WindowState = DXWINDOW_WINDOW_STATE_WINDOWED;

	HRESULT hr = D3D11CreateDevice (
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&x.Device,
		NULL,
		&x.DeviceContext
	); HandleHR(L"Main.cpp", __LINE__, hr);

	CComPtr<ID3D11VertexShader> VS;
	CComPtr<ID3D11PixelShader> PS;
	CComPtr<ID3D11Buffer> TimeBuffer;

	hr = x.Device->CreateVertexShader (
		VertexShader,
		sizeof(VertexShader),
		nullptr,
		&VS
	); HandleHR(L"Main.cpp", __LINE__, hr);

	hr = x.Device->CreatePixelShader (
		PixelShader,
		sizeof(PixelShader),
		nullptr,
		&PS
	); HandleHR(L"Main.cpp", __LINE__, hr);

	struct Info {
		float time;
		float pad[7];
	} i;

	D3D11_BUFFER_DESC cBufferDesc = { 0 };
	cBufferDesc.ByteWidth = sizeof(Info);
	cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cBufferDesc.MiscFlags = NULL;
	cBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sub;
	sub.pSysMem = &i;
	sub.SysMemPitch = 0;
	sub.SysMemSlicePitch = 0;

	hr = x.Device->CreateBuffer (
		&cBufferDesc,
		&sub,
		&TimeBuffer
	); HandleHR(L"Main.cpp", __LINE__, hr);

	hr = DXWindowCreateWindow (
		&Desc,
		x.Device,
		&x,
		&Window
	); HandleHR(L"Main.cpp", __LINE__, hr);

	LARGE_INTEGER liFrequency;
	LARGE_INTEGER liCounter;
	double CounterSeconds;
	QueryPerformanceFrequency(&liFrequency);

	while (x.run) {
		Window->PumpMessages();

		FLOAT color[] = { 1.0, 1.0, 1.0, 1.0 };

		x.DeviceContext->ClearRenderTargetView(x.BackBufRTV, color);

		QueryPerformanceCounter(&liCounter);
		CounterSeconds = double(liCounter.QuadPart) / double(liFrequency.QuadPart);
		i.time = float(CounterSeconds);
		D3D11_MAPPED_SUBRESOURCE sub;
		ZeroMemory(&sub, sizeof(sub));
		x.DeviceContext->Map(TimeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
		memcpy(sub.pData, &i, sizeof(Info));
		x.DeviceContext->Unmap(TimeBuffer, 0);

		ID3D11RenderTargetView* rtv = x.BackBufRTV;
		ID3D11Buffer* timeBuffer = TimeBuffer;

		x.DeviceContext->RSSetViewports(1, &x.viewport);
		x.DeviceContext->VSSetShader(VS, nullptr, 0);
		x.DeviceContext->PSSetShader(PS, nullptr, 0);
		x.DeviceContext->PSSetConstantBuffers(0, 1, &timeBuffer);
		x.DeviceContext->OMSetRenderTargets(1, &rtv, nullptr);
		x.DeviceContext->IASetInputLayout(nullptr);
		x.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		x.DeviceContext->Draw(3, 0);

		Window->Present(1, 0);
	}

	return 0;
}