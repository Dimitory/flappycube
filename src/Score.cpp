#include "Common.h"
#include "Score.h"

#include "Renderer.h"

using namespace FlappyBird;

bool numbers[11][7][5] =
{
	{ // 0
		{ 0, 1, 1, 1, 0 },
		{ 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1 },
		{ 0, 1, 1, 1, 0 },
	},
	{ // 1
		{ 0, 0, 1, 0, 0 },
		{ 0, 1, 1, 0, 0 },
		{ 0, 0, 1, 0, 0 },
		{ 0, 0, 1, 0, 0 },
		{ 0, 0, 1, 0, 0 },
		{ 0, 0, 1, 0, 0 },
		{ 0, 0, 1, 0, 0 },
	},
	{ // 2
		{ 0, 1, 1, 1, 0 },
		{ 1, 0, 0, 0, 1 },
		{ 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 1, 0 },
		{ 0, 0, 1, 0, 0 },
		{ 0, 1, 1, 0, 0 },
		{ 1, 1, 1, 1, 1 },
	},
	{ // 3
		{ 0, 1, 1, 1, 0 },
		{ 1, 0, 0, 0, 1 },
		{ 0, 0, 0, 0, 1 },
		{ 0, 0, 1, 1, 0 },
		{ 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1 },
		{ 0, 1, 1, 1, 0 },
	},
	{ // 4
		{ 0, 0, 0, 1, 0 },
		{ 0, 0, 1, 1, 0 },
		{ 0, 1, 0, 1, 0 },
		{ 1, 0, 0, 1, 0 },
		{ 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 1, 0 },
	},
	{ // 5
		{ 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 0 },
		{ 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1 },
		{ 0, 1, 1, 1, 0 },
	},
	{ // 6
		{ 0, 1, 1, 1, 0 },
		{ 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 0 },
		{ 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1 },
		{ 0, 1, 1, 1, 0 },
	},
	{ // 7
		{ 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 1, 0 },
		{ 0, 0, 1, 0, 0 },
		{ 0, 0, 1, 0, 0 },
		{ 0, 0, 1, 0, 0 },
		{ 0, 0, 1, 0, 0 },
	},
	{ // 8
		{ 0, 1, 1, 1, 0 },
		{ 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1 },
		{ 0, 1, 1, 1, 0 },
		{ 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1 },
		{ 0, 1, 1, 1, 0 },
	},
	{ // 9
		{ 0, 1, 1, 1, 0 },
		{ 1, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1 },
		{ 0, 1, 1, 1, 0 },
		{ 0, 0, 0, 0, 1 },
		{ 1, 0, 0, 0, 1 },
		{ 0, 1, 1, 1, 0 },
	}
};

Score::Score()
{
}

Score::~Score()
{
}

void Score::SetValue(int value)
{
	int first = value % 10;
	int second = (value % 100 - first) / 10;

	int i = 0;
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 7; y++)
		{
			blocks[i].show = numbers[second][y][x];
			i++;
		}
	}

	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 7; y++)
		{
			blocks[i].show = numbers[first][y][x];
			i++;
		}
	}
}

void Score::AddToLevel(Level* level)
{
	GameObject::AddToLevel(level);
	Point position;
	position.x -= 230;
	position.y -= 240;

	int i = 0;
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 7; y++)
		{
			blocks[i].show = true;
			blocks[i].rect = Rect(position.x + x * 50, position.y + y * 50, 5, 5);
			i++;
		}
	}

	position.x += 250;

	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 7; y++)
		{
			blocks[i].show = true;
			blocks[i].rect = Rect(position.x + x * 50, position.y + y * 50, 5, 5);
			i++;
		}
	}

	SetValue(0);
}

void Score::Draw(Renderer* renderer)
{
	Point position = GetPosition();
	Renderer::LinesArray lines;
	Color color = GetColor();
	for (auto&  block : blocks)
	{
		if (!block.show)
			continue;
		lines.push_back(Point(position.x + block.rect.x, position.y + block.rect.y));
		lines.push_back(Point(position.x + block.rect.x + block.rect.width, position.y + block.rect.y + block.rect.height));
	}
	renderer->DrawSeparateLines(lines, color);
}
