#include "Common.h"
#include "Block.h"

#include "Level.h"
#include "Collider.h"

using namespace FlappyBird;

const float kBlockHoleMin = 380.0f;
const float kBlockHoleMax = 390.0f;

Block::Block():
	topBlock(nullptr),
	bottomBlock(nullptr)
{
	spawnProgress = 0.f;
	holePos = random(0.3f,0.7f);
	holeSize = random(kBlockHoleMin, kBlockHoleMax);

	//SetColor( Color( 0.1f, color.r, color.g, color.b) );
	SetVisible( false );
}

Block::~Block()
{
	level->DestroyBlock( this );
}

void Block::AddToLevel(Level* level)
{
	GameObject::AddToLevel(level);
	
	topBlock = new GameObject();
	bottomBlock = new GameObject();

	topBlock->SetLayer(GetLayer());
	bottomBlock->SetLayer(GetLayer());

	topBlock->AddToLevel(level);
	bottomBlock->AddToLevel(level);

	color = Color(1.0f, 0.12f, 0.45f, 0.79f);

	topBlock->SetColor(color);
	bottomBlock->SetColor(color);

	Point size = GetSize();

	topBlock->SetSize(Point(size.x, size.y / 2));
	bottomBlock->SetSize(Point(size.x, size.y / 2));

	topBlock->SetCollider(new BoxCollider());
	bottomBlock->SetCollider(new BoxCollider());
}

void Block::Destroy()
{
	GameObject::Destroy();
	topBlock->Destroy();
	bottomBlock->Destroy();
}

void Block::Update( float delta )
{
	GameObject::Update( delta );

	spawnProgress = clamp( spawnProgress + delta, 0.f, 1.f );
	
	Point size = GetSize();
	Point position = GetPosition();

	float center = holePos * size.y;

	float x = position.x;
	float y = 0;

	y = size.y * holePos - size.y / 4  - holeSize / 2 * (2.f - spawnProgress);
	bottomBlock->SetPosition( Point( x, y ) );

	y = size.y * holePos + size.y / 4 + holeSize / 2 * (2.f - spawnProgress);
	topBlock->SetPosition( Point( x, y ) );
}

bool Block::Collided(const GameObject* other) const
{
	return topBlock->Collided(other) || bottomBlock->Collided(other);
}