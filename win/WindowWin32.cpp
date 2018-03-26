#include "WindowWin32.h"

#include "Application.h"

using namespace FlappyBird;

std::map<HWND, void*> WindowWin32::hWndMap;

WindowWin32::WindowWin32(int _width, int _height):
	width(_width),
	height(_height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= L"FlappyBird";						// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
		return;

	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;				// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;								// Windows Style

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								L"FlappyBird",							// Class Name
								L"FlappyBird",						// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		return;
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		32,											// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
		return;

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
		return;

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
		return;

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
		return;

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
		return;

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window

	hWndMap[hWnd] = this;

	glewInit();

	Application::GetInstance()->Initialize(width,height);
}

WindowWin32::~WindowWin32()
{
	hWndMap[hWnd] = nullptr;

	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(hRC);
	hRC = nullptr;

	ReleaseDC(hWnd, hDC);
	hDC = nullptr;

	DestroyWindow(hWnd);
	hWnd = nullptr;

	UnregisterClass(L"OpenGL", hInstance);
	hInstance = nullptr;
}

void WindowWin32::Update()
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
	{
		TranslateMessage(&msg);				// Translate The Message
		DispatchMessage(&msg);				// Dispatch The Message
	}

	SwapBuffers(hDC);
}
LRESULT CALLBACK WindowWin32::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WindowWin32* window = (WindowWin32*)hWndMap[hWnd];
	if( window )
	{
		auto width = window->width;
		auto height = window->height;

		switch (uMsg)
		{
			case WM_ACTIVATE:
			{
				if (wParam)
					Application::GetInstance()->Activate();
				else
					Application::GetInstance()->Deactivate();
				return 0;
			}
			case WM_MOUSEMOVE:
				Application::GetInstance()->MouseMove(
					LOWORD(lParam)*(float)width, 
					HIWORD(lParam)*(float)height);
				return 0;

			case WM_SIZE:
				Application::GetInstance()->Initialize(LOWORD(lParam), HIWORD(lParam));
				break;

			case WM_CLOSE:
				Application::GetInstance()->RequestForExit();
				return 0;

			case WM_KEYDOWN:
				Application::GetInstance()->KeyDown( wParam );
				break;

			case WM_KEYUP:
				Application::GetInstance()->KeyUp( wParam );
				break;

			case WM_LBUTTONDOWN:
				::SetCapture( hWnd );
				Application::GetInstance()->MouseDown(
					LOWORD(lParam)*(float)width,
					HIWORD(lParam)*(float)height );
				return 0;

			case WM_LBUTTONUP:
				::ReleaseCapture();
				Application::GetInstance()->MouseUp(
					LOWORD(lParam)*(float)width,
					HIWORD(lParam)*(float)height );
				return 0;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}