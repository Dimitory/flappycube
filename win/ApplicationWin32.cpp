#include "WindowWin32.h"

#include "ApplicationWin32.h"
#include "Level.h"
#include "WindowWin32.h"

using namespace FlappyBird;

ApplicationWin32::ApplicationWin32():
	Application(),
	window(new WindowWin32( 800, 600 ) )
{
}

ApplicationWin32::~ApplicationWin32()
{
	delete window;
}


void ApplicationWin32::Tick()
{
	Application::Tick();
	if (window)
	{
		window->Update();
	}
}

FILE* ApplicationWin32::OpenFile(const std::string& path, const std::string& mode)
{
	return fopen(path.c_str(), mode.c_str());
}