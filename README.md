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
