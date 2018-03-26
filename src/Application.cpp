#include "Common.h"
#include "Application.h"
#include "Level.h"
#include "Renderer.h"

using namespace FlappyBird;

static Application* appInstance = nullptr;

Application* Application::GetInstance()
{
	return appInstance;
}

Application::Application():
	level(new Level()),
	renderer( nullptr ),
	requestForExit(false)
{
	appInstance = this;
}

Application::~Application()
{
	delete level;
	delete renderer;
}

void Application::Initialize(int _width, int _height)
{
	width = _width;//min(_width, _height);
	height = _height; //max(_width, _height);
	delete renderer;

	renderer = new Renderer(width, height);
	level->Resize(renderer->GetRealWidth(), renderer->GetRealHeight());
}

void Application::Tick()
{
	static int64 timeEnd = getTimeInMilliseconds();
	static float step = 0.0f;

	int64 timeStart = getTimeInMilliseconds();
	int64 delta = timeStart - timeEnd;
	timeEnd = timeStart;

	step = min(delta * 0.001f, 0.16f);
	
	level->Update(step);

	Draw();
}

void Application::Draw()
{
	if (renderer)
	{
		renderer->BeginDraw();
		level->Draw(renderer);
		renderer->EndDraw();
	}
}

void Application::Activate()
{
}

void Application::Deactivate()
{
	level->Pause();
}


void Application::RequestForExit()
{
	requestForExit = true;
}

bool Application::IsRequestForExit()
{
	return requestForExit;
}

void Application::KeyDown(int key)
{
	level->KeyDown(key);
}

void Application::KeyUp(int key)
{
}

void Application::MouseMove(float x, float y)
{
}

void Application::MouseDown(float x, float y)
{
	level->MouseDown(x, y);
}

void Application::MouseUp(float x, float y)
{
}

FILE* Application::OpenFile(const std::string& path, const std::string& mode)
{
	return nullptr;
}