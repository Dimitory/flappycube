#pragma once

#ifdef _WIN32
#include <windows.h>
#include "glew\glew.h"
#include <gl\gl.h>
#include <gl\glu.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <map>
#include <list>
#include <vector>
#include <fstream>

typedef unsigned char			byte;
typedef unsigned short			ushort;
typedef unsigned int			uint;
typedef unsigned long			ulong;
typedef int						int32;
typedef signed long long		int64;
typedef unsigned short			uint16_t;

#define PI 3.14159265f
#define clamp(value, min, max) (((value) < (min)) ? (min) : (((value) > (max)) ? (max) : (value)))

#ifndef min
#define min(x,y) (((x) > (y)) ? (y) : (x))
#endif

#ifndef max
#define max(x,y) (((x) < (y)) ? (y) : (x))
#endif

inline float randomf()
{
	return ::rand() / ((float)RAND_MAX + 1);
}

inline float random( float min, float max )
{
	return min + (max - min)*randomf();
}

inline bool is_big_endian()
{
	int i = 1;
	return !((char*)&i)[0];
}

inline int32 to_int32(char* buffer, int length)
{
	int32 i = 0;
	if (!is_big_endian())
	for (int j = 0; j<length; j++)
		((char*)&i)[j] = buffer[j];
	else
	for (int j = 0; j<length; j++)
		((char*)&i)[sizeof(int) - 1 - j] = buffer[j];
	return i;
}

inline float wave(float t, float amp, float period)
{
	return amp * sinf(2.f * PI * t / period);
}

inline float lerp(float f1, float f2, float alpha)
{
	return f1 + (f2 - f1)*alpha;
}

inline float radians(float deg)
{
	return  deg*((float)PI / 180.0f);
}

inline float degrees(float rad)
{
	return rad*(180.0f / (float)PI);
}



#ifdef _WIN32
#define  LOG(...)  printlog(__VA_ARGS__)

inline void printlog(const char* szFormat, ...)
{
	char szBuff[1024];
	va_list arg;
	va_start(arg, szFormat);
	_vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
	va_end(arg);

	OutputDebugStringA(szBuff);
}


inline int64 getTimeInMilliseconds()
{
	return ::timeGetTime();
}
#else

#define  LOG_TAG    "FlappyCube"
#define  LOG(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

inline int64 getTimeInMilliseconds()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
#endif

inline void printOglError()
{
	for (GLint error = glGetError(); error; error = glGetError()) {
		LOG("after glError (0x%x)\n", error);
	}
}

#include "Point.h"
#include "Rectangle.h"
#include "Color.h"
#include "AnimTimer.h"
#include "Matrix.h"