#pragma once

namespace FlappyBird
{
	class Level;
	class Renderer;
	class Application
	{
	public:

		static Application* GetInstance();

	protected:
		Application();
		virtual ~Application();

	public:
		virtual void Initialize(int width, int height);
		virtual void Tick();

		void Activate();
		void Deactivate();

		void RequestForExit();
		bool IsRequestForExit();

		void KeyDown(int key);
		void KeyUp(int key);
		void MouseMove(float x, float y);
		void MouseDown(float x, float y);
		void MouseUp(float x, float y);
				
		int GetWidth() { return width; }
		int GetHeight() { return height; }

		virtual FILE* OpenFile(const std::string& path, const std::string& mode);

	protected:
		void Draw();

	protected:
		Level*		level;
		Renderer*	renderer;
		int			width;
		int			height;
		bool		requestForExit;
	};
}

