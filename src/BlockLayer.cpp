#include "Common.h"
#include "BlockLayer.h"

#include "Level.h"

using namespace FlappyBird;

BlockLayer::BlockLayer() :
	spawnOffset(0.f),
	minDistance(0),
	maxDistance(100),
	parralaxOffset(.1f),
	spawnPosition(1200)
{
}

BlockLayer::~BlockLayer()
{
}

void BlockLayer::Update( float delta )
{
	GameObject::Update( delta );
}

void BlockLayer::SetDistance(float minValue, float maxValue)
{
	minDistance = minValue;
	maxDistance = maxValue;
}

void BlockLayer::SetSize(const Point& _minSize, const Point& _maxSize)
{
	minSize = _minSize;
	maxSize = _maxSize;
}

void BlockLayer::SetParalaxOffset(float value)
{
	parralaxOffset = value;
}

void BlockLayer::SetSpawnPosition(float offset)
{
	spawnPosition = offset;
}

void BlockLayer::MoveBackground( float offsetX )
{
	float delta = offsetX * parralaxOffset;
	BlocksList::iterator it = blocks.begin();
	while(it != blocks.end())
	{
		MoveableBlock block = (*it);
		float offsetY = clamp(block.timer.GetPingPongAnim(), -0.5f, 0.5f ) * 30.f;
		Point position = block.block->GetPosition();
		position.x += delta;
		position.y = block.position.y + offsetY;
		block.block->SetPosition(position);
		if (position.x + 100 < 0)
		{
			block.block->Destroy();
			it = blocks.erase(it);
		}

		if (blocks.empty())
			break;

		it++;
	}

	if( spawnOffset < 0 )
	{
		MoveableBlock block;

		block.position = Point(spawnPosition + maxSize.x, random(0, 600));
		block.timer.SetLength(random(1.f, 8.f));

		block.block = new GameObject();
		block.block->SetPosition(block.position);
		block.block->SetSize(Point(random(minSize.x, maxSize.x), random(minSize.y, maxSize.y)));
		block.block->SetColor(GetColor());
		block.block->SetLayer(GetLayer());
		block.block->AddToLevel(level);

		blocks.push_back(block);
		spawnOffset = random(minDistance, maxDistance);
	}
	spawnOffset += delta;
}