#pragma once

namespace FlappyBird
{
	struct Color
	{
		float a, r, g, b;
	
		Color(): a(0), r(0), g(0), b(0) {}
		Color( float a, float r, float g, float b ): a(a), r(r), g(g), b(b) {}
		Color( const Color& other ): a(other.a), r(other.r), g(other.g), b(other.b) {}
	};
}

