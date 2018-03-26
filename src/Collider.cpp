#include "Common.h"
#include "Collider.h"

using namespace FlappyBird;


bool Itersects(const Circle* circle, const Box* box)
{
	Point Diff1;
	Point Diff2;
	Point Rotation;
	Point HalfSize;

	Diff1.x = circle->position.x - box->position.x;
	Diff1.y = circle->position.y - box->position.y;

	Rotation.x = cos(box->rotation);
	Rotation.y = sin(box->rotation);

	Diff2.x = Diff1.x * Rotation.x - Diff1.y * Rotation.y;
	Diff2.y = Diff1.y * Rotation.x + Diff1.x * Rotation.y;

	Diff2.x = abs(Diff2.x);
	Diff2.y = abs(Diff2.y);

	HalfSize.x = box->size.x / 2.0f;
	HalfSize.y = box->size.y / 2.0f;

	// intersection AABB - circle
	if (Diff2.x > HalfSize.x + circle->radius ||
		Diff2.y > HalfSize.y + circle->radius)
	{
		return false;
	}

	if (Diff2.x <= HalfSize.x || Diff2.y <= HalfSize.y)
	{
		return true;
	}

	float CornerDistSquared =
		pow(Diff2.x - HalfSize.x, 2.0f) +
		pow(Diff2.y - HalfSize.y, 2.0f);

	if (CornerDistSquared <= circle->radius * circle->radius)
	{
		return true;
	}
	return false;
}


bool Itersects(const Box* box, const Box* box2)
{
	Rect rect(
		box->position.x + (box->size.x / 2.f),
		box->position.y + (box->size.y / 2.f),
		box->position.x + box->size.x,
		box->position.y + box->size.y);

	Rect rect2(
		box2->position.x + (box2->size.x / 2.f),
		box2->position.y + (box2->size.y / 2.f),
		box2->position.x + box2->size.x,
		box2->position.y + box2->size.y);
	return rect.Intersects(rect2);
}

Collider::Collider() :
	box( nullptr ),
	circle( nullptr )
{
}

Collider::~Collider()
{
	delete box;
	delete circle;
}

void Collider::SetPosition(const Point& _position)
{
	if ( box )
		box->position = _position;
	else if ( circle )
		circle->position = _position;
}

void Collider::SetSize(const Point& size)
{
	if (box)
		box->size = size;
	else if (circle)
		circle->radius = size.x/2;
}

bool Collider::Collides(const Collider* other) const
{
	if (box && other->box)
	{
		return Itersects(box, other->box);
	}
	else if (box && other->circle)
	{
		return Itersects(other->circle, box);
	}
	else if (circle && other->box)
	{
		return Itersects(circle, other->box);
	}
	return false;
}

BoxCollider::BoxCollider()
{
	box = new Box();
}

SphereCollider::SphereCollider()
{
	circle = new Circle();
}