#pragma once

#include "Application.h"

namespace FlappyBird
{
	class WindowWin32;
	class ApplicationWin32 : public Application
	{
	public:
		ApplicationWin32();
		virtual ~ApplicationWin32();

		virtual void Tick() override;

		virtual FILE* OpenFile(const std::string& path, const std::string& mode) override;

	protected:
		WindowWin32* window;
	};
}

