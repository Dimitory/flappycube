#include "Common.h"
#include "Bird.h"

#include "Collider.h"
#include "Renderer.h"
#include "Level.h"

using namespace FlappyBird;

const float kFlapSpeed = 400.0f;
const float kFlapMaxSpeed = 600.0f;
const float kGravity = -1200.0f;

Bird::Bird() :
frezed(true),
started(false),
stunned(false),
angle(0)
{
	flapSpeed = 0;

	//eye[0]->SetSize(600.f, 10.f);
	//eye[0]->SetPosition(300.f, 800.f);
	//eye[0]->SetColor(floorColor);
	//eye[0]->SetLayer(ELayer_Foreground);
	//eye[0]->AddToLevel(this);

	starts[0] = new GameObject();
	starts[1] = new GameObject();
	starts[2] = new GameObject();
	starts[3] = new GameObject();
	starts[4] = new GameObject();




	anim.SetLength(1.f);

	SetCollider(new SphereCollider());
}

Bird::~Bird()
{
}

void Bird::AddToLevel(Level* _level)
{
	GameObject::AddToLevel(_level);

	starts[0]->SetSize(10.f, 10.f);
	starts[1]->SetSize(10.f, 10.f);
	starts[2]->SetSize(10.f, 10.f);
	starts[3]->SetSize(10.f, 10.f);
	starts[4]->SetSize(10.f, 10.f);

	starts[0]->SetColor(GetColor());
	starts[1]->SetColor(GetColor());
	starts[2]->SetColor(GetColor());
	starts[3]->SetColor(GetColor());
	starts[4]->SetColor(GetColor());

	starts[0]->SetLayer(GetLayer());
	starts[1]->SetLayer(GetLayer());
	starts[2]->SetLayer(GetLayer());
	starts[3]->SetLayer(GetLayer());
	starts[4]->SetLayer(GetLayer());

	starts[0]->AddToLevel(_level);
	starts[1]->AddToLevel(_level);
	starts[2]->AddToLevel(_level);
	starts[3]->AddToLevel(_level);
	starts[4]->AddToLevel(_level);
	//AddObject(starts[1]);
	//AddObject(starts[2]);
	//AddObject(starts[3]);
	//AddObject(starts[4]);
}

void Bird::Update( float delta )
{	
	GameObject::Update( delta );

	Point position = GetPosition();
	Point size = GetSize();

	if (frezed)
	{
		float w = wave(anim.GetTime(), 1.0f, anim.GetLength() * 2.0f);
		position.y += w;
		if (started)
		{
			position.x = lerp(oldPosition.x, flyPosition.x, anim.GetProgress());
			position.y = lerp(oldPosition.y, flyPosition.y, anim.GetProgress());
			if (anim.IsOvertimed())
			{
				position = flyPosition;
				frezed = false;
			}
		}
		SetPosition(position);
	}
	else if (!stunned)
	{
		position.y = clamp(position.y - flapSpeed * delta, 0.f, 800.f);
		SetPosition(Point(position.x, position.y));
		flapSpeed += kGravity * delta / 2.0f;
		flapSpeed = clamp(flapSpeed, -kFlapMaxSpeed, kFlapMaxSpeed);
		angle = clamp(angle + flapSpeed*0.01, -45, 45);
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			float rad = (anim.GetProgress()*80 + 72 * i) * PI / 180.f;
			float x = position.x + 100 * cos(rad);
			float y = position.y - size.y / 2 - 50 + 20 * sin(rad);
			starts[i]->SetPosition(x, y);
		}
	}
}

void Bird::Start()
{
	started = true;
	anim.SetTime(0.f);
	oldPosition = GetPosition();
}

void Bird::Destroy()
{
}

void Bird::Restart()
{
	stunned = false;
	frezed = true;
	started = false;
	for (int i = 0; i < 5; i++)
		starts[i]->SetVisible(false);
}

void Bird::PlayStunAnim()
{
	stunned = true;
	anim.SetTime(0.f);
	for (int i = 0; i < 5; i++)
		starts[i]->SetVisible(true);
}

void Bird::SetFlyPosition(const float x, const float y)
{
	flyPosition.x = x;
	flyPosition.y = y;
}

static void rotate(Point& center, float angle, Point& target)
{
	float s = sin(angle* PI / 180);
	float c = cos(angle* PI / 180);

	// translate point back to origin:
	target.x -= center.x;
	target.y -= center.y;

	// rotate point
	float xnew = target.x * c - target.y * s;
	float ynew = target.x * s + target.y * c;

	// translate point back:
	target.x = xnew + center.x;
	target.y = ynew + center.y;
}

void Bird::Draw(Renderer* renderer)
{
	Point position = GetPosition();
	Point size = GetSize();
	Color color = GetColor();

	if (!stunned)
	{
		float radius = size.x / 2.f;

		float uX = position.x - radius;
		float uY = position.y - radius;

		Point pt1(uX, uY), pt2(uX, uY), pt3(uX, uY), pt4(uX, uY), pt5(uX, uY), pt6(uX, uY);

		float a, b, c;
		a = radius / 2;
		c = a / sin(30 * PI / 180);
		b = c * cos(30 * PI / 180);

		pt1.x += 0;			pt1.y += b;
		pt2.x += a;			pt2.y += 0;
		pt3.x += a + c;		pt3.y += 0;
		pt4.x += 2 * c;		pt4.y += b;
		pt5.x += a + c;		pt5.y += 2 * b;
		pt6.x += a;			pt6.y += 2 * b;

		rotate(position, angle, pt1);
		rotate(position, angle, pt2);
		rotate(position, angle, pt3);
		rotate(position, angle, pt4);
		rotate(position, angle, pt5);
		rotate(position, angle, pt6);

		Renderer::LinesArray lines;
		lines.push_back(pt1);
		lines.push_back(pt2);
		lines.push_back(pt3);
		lines.push_back(pt4);
		lines.push_back(pt5);
		lines.push_back(pt6);
		lines.push_back(pt1);
		renderer->DrawLines(lines, color);
	}
	else
	{
		float w = wave(anim.GetTime(), 8.0f, anim.GetLength() * 2.0f);

		Rect bounds = GetBoundsRect();

		Renderer::LinesArray lines;
		lines.push_back(Point(bounds.x + w, bounds.y));
		lines.push_back(Point(bounds.x + bounds.width + w, bounds.y));
		lines.push_back(Point(bounds.x + bounds.width, bounds.y + bounds.height));
		lines.push_back(Point(bounds.x, bounds.y + bounds.height));
		lines.push_back(Point(bounds.x + w, bounds.y));
		renderer->DrawLines(lines, color);

		Point left = Point(position.x - size.x / 4 + w, position.y - size.y / 4 );
		Point right = Point(position.x + size.x / 4 + w, position.y - size.y / 4 );

		Renderer::LinesArray lines1;
		lines1.push_back(Point(left.x - 10, left.y - 10));
		lines1.push_back(Point(left.x + 10, left.y + 10));
		renderer->DrawLines(lines1, color);
		Renderer::LinesArray lines2;
		lines2.push_back(Point(left.x - 10, left.y + 10));
		lines2.push_back(Point(left.x + 10, left.y - 10));
		renderer->DrawLines(lines2, color);

		Renderer::LinesArray lines3;
		lines3.push_back(Point(right.x - 10, right.y - 10));
		lines3.push_back(Point(right.x + 10, right.y + 10));
		renderer->DrawLines(lines3, color);
		Renderer::LinesArray lines4;
		lines4.push_back(Point(right.x - 10, right.y + 10));
		lines4.push_back(Point(right.x + 10, right.y - 10));
		renderer->DrawLines(lines4, color);

		Renderer::LinesArray lines5;
		lines5.push_back(Point(position.x - 50, position.y + size.y / 4));
		lines5.push_back(Point(position.x + 50, position.y + size.y / 4));
		renderer->DrawLines(lines5, color);
	}
}

void Bird::Flap()
{
	flapSpeed = kFlapSpeed;
}