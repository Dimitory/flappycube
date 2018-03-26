#pragma once

#include "../src/Application.h"

namespace FlappyBird
{
	class ApplicationAndroid : public Application
	{
	public:
		ApplicationAndroid();
		virtual ~ApplicationAndroid();

		virtual FILE* OpenFile(const std::string& path, const std::string& mode) override;
	};
}

