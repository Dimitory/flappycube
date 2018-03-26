#pragma once

namespace FlappyBird
{
	struct Rect
	{
		float x, y, width, height;
	
		Rect(): x(0), y(0), width(0), height(0) {}
		Rect( float x, float y, float w, float h ): x(x), y(y), width(w), height(h) {}
		Rect( const Rect& other ): x(other.x), y(other.y), width(other.width), height(other.height) {}

		bool Intersects( const Rect& rect ) const
		{
			if ( x + width < rect.x )			return false;
			if ( x > rect.x + rect.width )		return false;
			if ( y + height < rect.y )			return false;
			if ( y > rect.y + rect.height )		return false;
			return true;
		}
	};
}

