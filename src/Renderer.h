#pragma once

namespace FlappyBird
{
	class GameObject;
	class LineShaderProgram;
	class Renderer
	{
	public:
		typedef std::vector<Point> LinesArray;
	public:
		Renderer( float width, float height );
		virtual ~Renderer();

		void BeginDraw();

		void DrawQuad(const Rect& rect, const Color& color);
		void DrawLines(LinesArray& lines, const Color& color);
		void DrawSeparateLines(LinesArray& lines, const Color& color);
		
		float GetRealWidth() { return real_width; }
		float GetRealHeight() { return real_height; }

		void EndDraw();

		void SetLineWidth(float width);

	private:
		LineShaderProgram* lineShader;
		float screen_width;
		float screen_height;
		float virtual_width;
		float virtual_height;
		float real_width;
		float real_height;
	};
}