#pragma once

class Vector2
{
public:
	union
	{
		struct
		{
			float			x, y;
		};

		float				v[2];
	};

	explicit Vector2() : x(0), y(0) { }
	explicit Vector2(const float& nx, const float& ny) : x(nx), y(ny) { }

	Vector2 operator +(const Vector2& b) const { return Vector2(x + b.x, y + b.y); }
	Vector2 operator -(const Vector2& b) const { return Vector2(x - b.x, y - b.y); }
	Vector2 operator *(const Vector2& b) const { return Vector2(x * b.x, y * b.y); }
	Vector2 operator /(const Vector2& b) const { return Vector2(x / b.x, y / b.y); }
	Vector2 operator *(const float& b) const { return Vector2(x * b, y * b); }
	Vector2 operator /(const float& b) const { return Vector2(x / b, y / b); }

	float& operator [](const size_t i) { return v[i]; }
	const float& operator [](const size_t i) const { return v[i]; }
};

class Vector4
{
public:
	union
	{
		struct
		{
			float			x, y, z, w;
		};

		float				v[4];
	};

	explicit Vector4() : x(0), y(0), z(0), w(0) { }
	explicit Vector4(const float& nx, const float& ny, const float& nz, const float& nw) : x(nx), y(ny), z(nz), w(nw) { }

	Vector4 operator +(const Vector4& b) const { return Vector4(x + b.x, y + b.y, z + b.z, w + b.w); }
	Vector4 operator -(const Vector4& b) const { return Vector4(x - b.x, y - b.y, z - b.z, w - b.w); }
	Vector4 operator *(const Vector4& b) const { return Vector4(x * b.x, y * b.y, z * b.z, w * b.w); }
	Vector4 operator /(const Vector4& b) const { return Vector4(x / b.x, y / b.y, z / b.z, w / b.w); }
	Vector4 operator *(const float& b) const { return Vector4(x * b, y * b, z * b, w * b); }
	Vector4 operator /(const float& b) const { return Vector4(x / b, y / b, z / b, w / b); }

	float& operator [](const size_t i) { return v[i]; }
	const float& operator [](const size_t i) const { return v[i]; }
};

class Matrix
{
public:
	Vector4 value[4];

	explicit Matrix(const float& n)  // Diagonal constructor
	{
		value[0] = Vector4(n, 0, 0, 0);
		value[1] = Vector4(0, n, 0, 0);
		value[2] = Vector4(0, 0, n, 0);
		value[3] = Vector4(0, 0, 0, n);
	}

	static Matrix ortho(const float &left, const float &right, const float &bottom, const float &top)
	{
		Matrix m(1);

		m[0][0] = 2.0f / (right - left);
		m[1][1] = 2.0f / (top - bottom);
		m[2][2] = -1.0f;
		m[3][0] = -(right + left) / (right - left);
		m[3][1] = -(top + bottom) / (top - bottom);

		return m;
	}

	Vector4& operator [](const size_t i) { return value[i]; }
	const Vector4& operator [](const size_t i) const { return value[i]; }
};