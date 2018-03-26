#include "WindowWin32.h"

#include "FileUtils.h"

using namespace FlappyBird;

static FILE* FileOpen(const std::string& path)
{
	return fopen(path.c_str(), "rb");
}