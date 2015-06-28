#include "WindowMessageDispatcher.h"

#include <map>
#include <mutex>

//Used for mapping a window handle to a dispatcher object
static std::map<HWND, WindowMessageDispatcher*> g_WindowMap;
//Used to prevent race conditions while using the window handle map
static std::mutex g_WindowMapMutex;

//Zero out all data
WindowMessageDispatcher::WindowMessageDispatcher() :
m_Handle(NULL)
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
HRESULT WindowMessageDispatcher::Initialize(HWND Handle) {
	g_WindowMapMutex.lock();
	g_WindowMap[Handle] = this;
	g_WindowMapMutex.unlock();

	return S_OK;
}

//Locate the dispatcher object and re-direct the call towards it
LRESULT CALLBACK WindowMessageDispatcher::StaticWindowProcess(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam) {
	g_WindowMapMutex.lock();
	WindowMessageDispatcher* l_Dispatcher = g_WindowMap[Handle];
	g_WindowMapMutex.unlock();

	return l_Dispatcher->WindowProcess(Message, wParam, lParam);
}

//React to messages by dispatching them to the application
LRESULT WindowMessageDispatcher::WindowProcess(UINT Message, WPARAM wParam, LPARAM lParam) {
	return S_OK;
}