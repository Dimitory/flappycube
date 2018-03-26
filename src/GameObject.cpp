#include "Common.h"
#include "GameObject.h"
#include "Collider.h"
#include "Level.h"
#include "Renderer.h"

using namespace FlappyBird;

GameObject::GameObject():
	level( nullptr ),
	layer( 0 ),
	outdatedBounds( false ),
	visible( true ),
	collider( nullptr )
{
}

GameObject::~GameObject()
{
	delete collider;
}

void GameObject::AddObject(GameObject* child)
{
	if ( child->level != nullptr )
		return;

	childs.push_back(child);

	child->AddToLevel(level);
}

void GameObject::RemoveObject(GameObject* child)
{
	childs.remove(child);
	child->Destroy();
}

void GameObject::SetCollider(Collider* _collider)
{
	collider = _collider;
}

void GameObject::AddToLevel( Level* _level )
{
	if ( level != nullptr || _level == nullptr )
		return;

	level = _level;
	level->AddObject( this );
}

void GameObject::Destroy()
{
	if ( level == nullptr )
		return;

	for (auto& object : childs)
		object->Destroy();

	level->RemoveObject( this );
}
void GameObject::Update( float delta )
{
	if ( outdatedBounds )
		RecalculateBounds();
}

void GameObject::SetLayer( int _layer )
{
	layer = _layer;
}

void GameObject::SetPosition(const float x, const float y)
{
	position.x = x;
	position.y = y;
	outdatedBounds = true;
}

void GameObject::SetPosition( const Point& _position )
{
	position = _position;
	outdatedBounds = true;
}

void GameObject::SetSize(const float w, const float h)
{
	size.x = w;
	size.y = h;
	outdatedBounds = true;
}

void GameObject::SetSize( const Point& _size )
{
	size = _size;
	outdatedBounds = true;
}

void GameObject::SetColor( const Color& _color )
{
	color = _color;
}

void GameObject::SetVisible( bool _visible )
{
	visible = _visible;
}

bool GameObject::Collided(const GameObject* other) const
{
	if (other == nullptr || other->collider == nullptr)
		return false;

	//const Rect& rect(collider->GetBoundsRect());
	//return rect.Intersects(bounds);
	return collider->Collides(other->collider);
}

void GameObject::RecalculateBounds()
{
	bounds.x		= position.x - size.x / 2.0f;
	bounds.y		= position.y - size.y / 2.0f;
	bounds.width	= size.x;
	bounds.height	= size.y;

	if (collider)
	{
		collider->SetPosition(position);
		collider->SetSize(size);
	}
}

void GameObject::Draw(Renderer* renderer)
{
	if ( !visible )
		return;

	renderer->DrawQuad(bounds, color);

	Renderer::LinesArray lines;
	lines.push_back(Point(bounds.x, bounds.y));
	lines.push_back(Point(bounds.x + bounds.width, bounds.y));
	lines.push_back(Point(bounds.x + bounds.width, bounds.y + bounds.height));
	lines.push_back(Point(bounds.x, bounds.y + bounds.height));
	lines.push_back(Point(bounds.x, bounds.y));
	renderer->DrawLines(lines, color);
	//{
	//	Renderer::Line line;
	//	line.start = Point(bounds.x + bounds.width, bounds.y + bounds.height);
	//	line.end = Point(bounds.x, bounds.y + bounds.height);
	//	list.push_back(line);
	//}

	//{
	//	Renderer::Line line;
	//	line.start = Point(bounds.x + bounds.width, bounds.y + bounds.height);
	//	line.end = Point(bounds.x + bounds.width, bounds.y);
	//	list.push_back(line);
	//}

	//{
	//	Renderer::Line line;
	//	line.start = Point(bounds.x, bounds.y);
	//	line.end = Point(bounds.x + bounds.width, bounds.y);
	//	list.push_back(line);
	//}

	//{
	//	Renderer::Line line;
	//	line.start = Point(bounds.x, bounds.y + bounds.height);
	//	line.end = Point(bounds.x, bounds.y);
	//	list.push_back(line);
	//}

	//renderer->DrawLines(list,color);
}