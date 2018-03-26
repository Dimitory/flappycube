#pragma once

namespace FlappyBird
{

	struct Point
	{
		union
		{
			struct
			{
				float			x, y;
			};

			float				v[2];
		};

		Point(): x( 0.0f ), y( 0.0f ) {}
		Point( float _x, float _y ) : x( _x ), y( _y ) {}
		Point( const Point& other ) : x( other.x ), y( other.y ) {}
	};

}

