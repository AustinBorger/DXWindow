Important Notices
=========

- Currently, the DirectX 12 examples and code do not work.  This is because there
is not a sufficient amount of information out there yet for me to write the library
to support it.  Once this changes, I will be updating this repository to reflect that.

DXWindow
=========
DXWindow is a small layer over DXGI written for C++ applications that
simplifies the process of creating and using a window.  Think of it as
GLUT for DirectX.  As a window-specific library, it makes use of the
COM ABI.  Its intent is to provide a fast, easy way to create a window
that behaves well.

What it does.
-------------
DXWindow absolves an application developer of the responsibility of learning
the more frustrating (yet necessary) intricacies of DXGI.  Rather than wasting
time writing windowing code that behaves the same as just about every other
game out there, it lets you skip right to the graphics.

Some interesting points:

- DXWindow is device-agnostic, so long as the device cooperates with DXGI - this means you can use any version of Direct3D between 10 and 12 (maybe even 13 and beyond).  Just provide the device as an `IUnknown` interface, and DXWindow does the rest.
- DXWindow handles movement between monitors so that proper functionality isn't just limited to the primary output device.
- DXWindow supports all of the common window modes used in games: exclusive fullscreen, fullscreen window, windowed, and borderless.
- DXWindow is interoperable with Direct2D as well, as it uses the BGRA texture format that Direct2D render targets require.
- DXWindow takes care of the most common forms of game input - keyboard, mouse, and gamepad.

The library abstracts all window behavior away from the application developer.  This way, you only ever have to worry about what's happening in your game and not the window.

What it doesn't do.
-------------
DXWindow does not attempt to provide complete control over window setup.  It handles only the most common use cases, and does not make itself easily extensible without modification of the source code.  It also does not provide a way to make the back buffer MSAA-enabled - the reason for this is that it makes for a simpler API, and you can still make a secondary buffer that supports MSAA and resolve it to the back buffer anyway.  This is what DXGI does in the background, so there are no added inefficiencies from doing things this way.  DXWindow also does not handle multiple adapters.  It uses whatever adapter the device that you hand it occupies, and does not alert you when the window has moved between adapters.  However, this occurrence is very, very uncommon, as most gaming and non-gaming setups provide only one virtualized adapter when SLI or Crossfire are considered.

Usage
-------------
A window can be created in a four simple steps.

#### 1. Create the window description

`DXWINDOW_DESC` is a structure describing the initial setup of the window:

    struct DXWINDOW_DESC {
        LPCWSTR IconSmall;
        LPCWSTR IconLarge;
        LPCWSTR Cursor;
        LPCWSTR Title;
        HINSTANCE Instance;
        DXWINDOW_FULLSCREEN_STATE FullscreenState;
        DXWINDOW_WINDOW_STATE WindowState;
        WORD Width;
        WORD Height;
        BOOL InitFullscreen;
        BOOL AllowToggle;
    };

- `IconSmall`, `IconLarge`, and `Cursor` are all resources.  The former two asign the icon of the application's window, while the latter assigns the cursor that appears when the mouse is in window focus.  These cannot be changed once the window has been created.  If set to NULL, DXWindow will choose the default setup for each of these items.
- `Title` is the name of the window.  This also cannot be changed.
- `Instance` is the process `HINSTANCE` handle.
- `FullscreenState` is the type of fullscreen the window should use when it's in fullscreen mode.  This can be one of either `DXWINDOW_FULLSCREEN_STATE_FULLSCREEN` or `DXWINDOW_FULLSCREEN_STATE_FULLSCREEN_WINDOW`.
- Conversely, `WindowState` is the type of window layout that should be used when the window is not in fullscreen mode.  It can be one of either `DXWINDOW_WINDOW_STATE_WINDOWED` or `DXWINDOW_WINDOW_STATE_BORDERLESS`.
- `Width` and `Height` are the dimensions of the client area of the window when it's in a windowed mode.  DXWindow will automatically adjust the full dimensions of the window when the border is added or removed when moving between window modes.
- `InitFullscreen` is a flag indicating whether or not the window should enter its fullscreen state upon creation.
- Finally, `AllowToggle` is a flag indicating whether or not the window should toggle between its window and fullscreen states when the user presses F11.

#### 2. Design your callback class

DXWindow uses a callback mechanism to relay events and errors to the application.  The `IDXWindowCallback` has a large number of methods, most of them self-explanatory.  There are a few important ones to note, however, that must be implemented in order for your application to work correctly:

    VOID OnObjectFailure(LPCWSTR File, UINT Line, HRESULT hr);
    
`OnObjectFailure()` functions kind of like an exception, in that it exists so that you don't have to check a return value every time you make an API call.  Since exceptions are illegal across DLL boundaries, `OnObjectFailure()` does the next best thing by providing you with the filename and line where the error occurred, as well as the error itself.

    VOID OnBackBufferCreate(IDXWindow* pDXWindow);
    VOID OnBackBufferRelease(IDXWindow* pDXWindow);
    
`OnBackBufferCreate()` and `OnBackBufferRelease()` are called once every time the window needs to re-create the back buffer.  This happens any time the size of the window is changed or when the window changes between states.  These must be implemented if you wish your window to change modes at any point, since DXGI requires that all references to the back buffer be released before it can be resized.  Each method will be called on window startup and shutdown respectively, so you can keep all of your back buffer code here.

    VOID OnWindowClose(IDXWindow* pDXWindow);
    
`OnWindowClose()` is sent when the window closes.  This is where you might decide to terminate your game loop.

While all of the methods in `IDXWindowCallback` are pure, DXWindow provides a helper implementation class `CDXWindowCallback` that pre-defines all methods as empty (besides `OnObjectFailure()`, which is mandatory - kind of like a Java exception) so that you can choose the ones you want to implement and which ones you want to ignore.

#### 3. Create the window

A window can be created through `DXWindowCreateWindow()`:

    HRESULT DXWindowCreateWindow (
        const DXWINDOW_DESC* pDesc,
        IUnknown* pDeviceUnk,
        IDXWindowCallback* pDXWindowCallback,
        IDXWindow** ppDXWindow
    );
    
The use of this should be pretty self-explanatory.  `pDeviceUnk` is the device you want to use, which can be any device interface from Direct3D 10 onward.  None of these variables can be `NULL` - if any of them are `NULL`, then the function will return `E_POINTER`.

#### 4. Use the window

The `IDXWindow` interface is defined below:

    struct IDXWindow : public IUnknown {
    	VOID PumpMessages();
    	VOID Present(UINT SyncInterval, UINT Flags);
    	WORD GetWindowWidth();
    	WORD GetWindowHeight();
    	VOID SetWindowResolution(WORD Width, WORD Height);
    	DXWINDOW_STATE STDMETHODCALLTYPE GetState();
    	VOID SetState(DXWINDOW_STATE State);
    	BOOL GetAllowToggle();
    	VOID SetAllowToggle(BOOL AllowToggle);
    	VOID GetBuffer(UINT Index, REFIID rIID, void** ppvBackBuffer);
    };
    
- `PumpMessages()` checks the window message queue and changes in gamepad orientation.  The events are then interpreted and sent to your `IDXWindowCallback` implementation.  The method returns once the window message queue is empty and all gamepads have been checked.
- `Present()` behaves exactly the same as `IDXGISwapChain::Present()`.  `SyncInterval` refers to the interval number of frames between each flip, coordinated with VSync.  A value of 0 will flip the swap chain with no regard for VSync, a value of 1 will wait for the next v-blank before flipping, a value of 2 will wait two v-blanks, etc.  For `Flags`, refer to [this page][swap-chain] for more details.
- `GetWindowWidth()` and `GetWindowHeight()` return the width and height of the window when in windowed mode, respectively.  These methods do not return the width and height of the window in fullscreen mode - to query that resolution, you must retrieve it from the back buffer texture's description structure.  You should only use these methods to obtain supplemental information - while you can use them for getting the back buffer dimensions in windowed mode, it's better to use a more consistent method as previously described.
- `SetWindowResolution()` sets the client resolution of the window in windowed mode.
- `GetState()` returns the state the window is currently in.  It can be one of `DXWINDOW_STATE_WINDOWED`, `DXWINDOW_STATE_BORDERLESS`, `DXWINDOW_STATE_FULLSCREEN`, or `DXWINDOW_STATE_FULLSCREEN_WINDOW`.  `SetState()` will immediately change this property of the window.
- `GetAllowToggle()` and `SetAllowToggle()` are the getters and setters for the toggle flag.
- Finally, `GetBuffer()` retrieves the back buffer texture in the interface requested by the application.  The accepted interfaces depend on the version of Direct3D you're using - IE, if you're using Direct3D 11, you may use `ID3D11Texture2D`.  `IDXGISurface` is supported only for Direct3D 11 - this is the interface you would use for Direct2D interop.

#### And that's it!

All you have to do to include DXWindow in your project is to download the "DXWindow.h" header and dll and link the library.
These builds will be kept up-to-date with the source code, so you don't have to build it if you don't want to.  Note that
only Visual Studio is supported for compilation and project files.

[swap-chain]: https://msdn.microsoft.com/en-us/library/windows/desktop/bb509554(v=vs.85).aspx

License
-------------
DXWindow is released under the GPLv3 license.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Contributors
-------------
DXWindow uses no external libraries other than the ones included in the Windows SDKs.

New contributors are very welcome, as currently this code base is maintained by
just one person (me).  If DXWindow proves useful for you, but doesn't meet your
standards, please send feedback to my email at <aaborger@gmail.com>.

- A thank you goes out to the user _startas_, who reported an issue with the back buffer size upon entering
exclusive fullscreen mode.
