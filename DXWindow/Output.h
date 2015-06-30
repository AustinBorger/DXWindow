#pragma once

#include <dxgi.h>
#include <comdef.h>
#include <atlbase.h>
#include <vector>

class Output final {
public:
	HRESULT Initialize(CComPtr<IDXGIOutput> obj);

	HRESULT GetWindowCenter(UINT Width, UINT Height, RECT* pRect, DWORD dwStyle, DWORD dwExStyle);

	HRESULT GetWorkArea(RECT* pRect);

	HRESULT GetDesktopArea(RECT* pRect);

	HMONITOR GetMonitor();

	LPCWSTR GetName();

	CComPtr<IDXGIOutput> GetObj();

	static HRESULT InitializeList(CComPtr<IDXGIAdapter> adapter, std::vector<Output>& Outputs);

	static Output* SearchOutput(HWND Handle, std::vector<Output>& Outputs);

	static Output* PrimaryOutput(std::vector<Output>& Outputs);

private:
	CComPtr<IDXGIOutput> m_Obj;
	DXGI_OUTPUT_DESC m_Desc;
};