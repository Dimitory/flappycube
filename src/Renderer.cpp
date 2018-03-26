#include "Common.h"
#include "Renderer.h"

#include "GameObject.h"
#include "LineShaderProgram.h"

using namespace FlappyBird;

Renderer::Renderer( float _width, float _height) :
	lineShader( nullptr ),
	screen_width( _width ),
	screen_height( _height ),
	virtual_width(600),
	virtual_height(800)
{
	float targetAspectRatio = screen_width / screen_height;
	real_height = virtual_height;
	real_width = real_height*targetAspectRatio;
	
	lineShader = new LineShaderProgram();
	lineShader->SetColor(Color(1, 0, 0, 0));
	lineShader->SetWidth(0.05f);
	lineShader->SetTexture("textures/glow.bmp");
	lineShader->SetProjection(Matrix::ortho(0, real_width, real_height, 0));

	glClearColor(0, 0, 0, 1);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
}

Renderer::~Renderer()
{
	delete lineShader;
}

void Renderer::BeginDraw()
{
	//glViewport(0, 0, (GLsizei)screen_width, (GLsizei)screen_height/2);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(0.0, (GLdouble)width, (GLdouble)height, 0.0, -1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glShadeModel(GL_SMOOTH);
	//
	//glClearDepth(1.0f);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	//glClearColor(0, 0, 0, 1);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glHint(GL_GENERATE_MIPMAP_HINT, GL_DONT_CARE);

	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);

	lineShader->Begin();
}

void Renderer::DrawQuad(const Rect& rect, const Color& color)
{
	//float r = 2;
	//glColor4f(0.f, 0.f, 0.f, 1.f);
	//glBegin(GL_QUADS);
	//glVertex3f(rect.x + rect.width - r, rect.y + rect.height - r, 0.0f);
	//glVertex3f(rect.x + r, rect.y + rect.height - r, 0.0f);
	//glVertex3f(rect.x + r, rect.y + r, 0.0f);
	//glVertex3f(rect.x + rect.width - r, rect.y + r, 0.0f);
	//glEnd();
}

//void Renderer::DrawQuadLines(const Rect& rect, const Color& color)
//{
//	Renderer::LinesArray lines;
//	lines.push_back(Point(rect.x, rect.y));
//	lines.push_back(Point(rect.x + rect.width, rect.y));
//	lines.push_back(Point(rect.x + rect.width, rect.y + rect.height));
//	lines.push_back(Point(rect.x, rect.y + rect.height));
//	lines.push_back(Point(rect.x, rect.y));
//	DrawLines(lines, color);
//}

void  Renderer::DrawSeparateLines(LinesArray& linesList, const Color& color)
{
	lineShader->SetColor(color);

	for (uint i = 0; i < linesList.size(); )
	{
		lineShader->DrawLine(Vector2(linesList[i].x, linesList[i].y), Vector2(linesList[i+1].x, linesList[i+1].y));
		i += 2;
	}
}

void Renderer::DrawLines(LinesArray& linesList, const Color& color)
{
	lineShader->SetColor(color);

	for (uint i = 0; i < linesList.size()-1; i++)
	{
		lineShader->DrawLine(Vector2(linesList[i].x, linesList[i].y), Vector2(linesList[i + 1].x, linesList[i + 1].y));
	}
	//for (auto& line : linesList)
	//{
	//	lineShader->DrawLine(line.start, line.end);
	//}
	
}

void Renderer::EndDraw()
{
	lineShader->End();
	glFlush();
}

void Renderer::SetLineWidth(float width)
{
	lineShader->SetWidth(width);
}