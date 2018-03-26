#include "Common.h"

#include "ApplicationWin32.h"

using namespace FlappyBird;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ApplicationWin32 app;

	while (!app.IsRequestForExit())
	{
		app.Tick();
	}
		
	return 0;
}