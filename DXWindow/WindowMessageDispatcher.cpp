#include "WindowMessageDispatcher.h"
#include "CDXWindow.h"

#include <map>
#include <mutex>

//Used for mapping a window handle to a dispatcher object
static std::map<HWND, WindowMessageDispatcher*> g_WindowMap;
//Used to prevent race conditions while using the window handle map
static std::mutex g_WindowMapMutex;

//Zero out all data
WindowMessageDispatcher::WindowMessageDispatcher(CDXWindow& Window) :
m_Handle(NULL),
m_Window(Window),
m_PrevWindowVisible(FALSE)
{ }

//If the handle and dispatcher object were mapped, remove the entry from the map
WindowMessageDispatcher::~WindowMessageDispatcher() {
	if (m_Handle != NULL) {
		g_WindowMapMutex.lock();
		g_WindowMap.erase(m_Handle);
		g_WindowMapMutex.unlock();
	}
}

//Add the handle-dispatcher relationship to the global window map
HRESULT WindowMessageDispatcher::Initialize(HWND Handle, CComPtr<IDXWindowCallback> Callback) {
	g_WindowMapMutex.lock();
	g_WindowMap[Handle] = this;
	g_WindowMapMutex.unlock();

	m_Callback = Callback;
	m_PrevWindowVisible = TRUE;

	return S_OK;
}

//Locate the dispatcher object and re-direct the call towards it
LRESULT CALLBACK WindowMessageDispatcher::StaticWindowProcess(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam) {
	g_WindowMapMutex.lock();
	WindowMessageDispatcher* l_Dispatcher = g_WindowMap[Handle];
	g_WindowMapMutex.unlock();

	return l_Dispatcher->WindowProcess(Message, wParam, lParam);
}

void WindowMessageDispatcher::CheckWindowVisible() {
	BOOL l_IsWindowVisible = IsWindowVisible(m_Handle);

	if (l_IsWindowVisible != m_PrevWindowVisible) {
		if (l_IsWindowVisible == FALSE) {
			m_Callback->OnWindowHide(&m_Window);
		} else { //TRUE
			m_Callback->OnWindowShow(&m_Window);
		}
	}

	m_PrevWindowVisible = l_IsWindowVisible;
}

//React to messages by dispatching them to the application
LRESULT WindowMessageDispatcher::WindowProcess(UINT Message, WPARAM wParam, LPARAM lParam) {
	HRESULT hr = S_OK;

	switch (Message) {
		case WM_KEYDOWN: {
			m_Callback->OnKeyDown(&m_Window, wParam, lParam);
		} return 0;

		case WM_KEYUP: {
			m_Callback->OnKeyUp(&m_Window, wParam, lParam);
		} return 0;

		case WM_LBUTTONDOWN: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);

			m_Callback->OnMouseLButtonDown(&m_Window, wParam, x, y);
		} return 0;

		case WM_LBUTTONUP: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);

			m_Callback->OnMouseLButtonUp(&m_Window, wParam, x, y);
		} return 0;

		case WM_RBUTTONDOWN: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);

			m_Callback->OnMouseRButtonDown(&m_Window, wParam, x, y);
		} return 0;

		case WM_RBUTTONUP: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);

			m_Callback->OnMouseRButtonUp(&m_Window, wParam, x, y);
		} return 0;

		case WM_MBUTTONDOWN: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);

			m_Callback->OnMouseMButtonDown(&m_Window, wParam, x, y);
		} return 0;

		case WM_MBUTTONUP: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);

			m_Callback->OnMouseLButtonUp(&m_Window, wParam, x, y);
		} return 0;

		case WM_MOUSEMOVE: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);

			m_Callback->OnMouseMove(&m_Window, wParam, x, y);
		} return 0;

		case WM_MOUSEWHEEL: {
			WORD x = LOWORD(lParam);
			WORD y = HIWORD(lParam);
			SHORT delta = GET_WHEEL_DELTA_WPARAM(wParam);
			WORD flags = LOWORD(wParam);

			m_Callback->OnMouseWheel(&m_Window, flags, delta, x, y);
		} return 0;

		case WM_CLOSE: {
			m_Callback->OnWindowClose(&m_Window);
		} return 0;
	}

	return m_Window.WindowProcess(Message, wParam, lParam);
}