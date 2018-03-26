#pragma once

#include "ShaderProgram.h"

namespace FlappyBird
{


	//struct VERTEX_XYZ_C_UV2
	//{
	//	float	x0, y0, z0;
	//	float	x1, y1, z1;
	//	float	nx, ny;
	//	float	u, v;
	//};

	class LineShaderProgram : public ShaderProgram
	{
	public:
		LineShaderProgram();
		~LineShaderProgram();

		void SetProjection(const Matrix& matrix);
		void SetColor(const Color& color);
		void SetTexture(const std::string& texture);
		void SetWidth(const float& width );

		void Begin();
		void DrawLine(const Vector2& p1, const Vector2& p2);
		void End();

	private:

		void Flush();

	private:
		std::vector<Vector2> mVerts;
		std::vector<Vector2> mTexCoords;
		std::vector<Vector4> mColors;
		std::vector<uint16_t> mIndices;
		Matrix mProjection;
		GLuint textureID;		
		float width;
		Color color;
	};
}