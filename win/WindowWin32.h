#pragma once

#include "Common.h"

namespace FlappyBird
{
	class WindowWin32
	{
	public:
		WindowWin32(int width,int height);
		virtual ~WindowWin32();
	
		virtual void Update();

	private:
		static LRESULT	CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
		static std::map<HWND, void*> hWndMap;
	private:
		int			width;
		int			height;
		HDC			hDC;
		HGLRC		hRC;
		HWND		hWnd;
		HINSTANCE	hInstance;
	};
}

