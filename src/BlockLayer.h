#pragma once

#include "GameObject.h"

namespace FlappyBird
{
	class BlockLayer : public GameObject
	{
		struct MoveableBlock
		{
			AnimTimer timer;
			Point position;
			GameObject* block;
		};
		typedef std::list<MoveableBlock> BlocksList;

	public:
		BlockLayer();
		virtual ~BlockLayer();

		virtual void Update(float delta) override;

		void SetDistance(float minValue, float maxValue);
		void SetSize(const Point& minSize, const Point& maxSize);
		void SetParalaxOffset(float value);
		void MoveBackground(float offset);
		void SetSpawnPosition(float offset);

	private:
		BlocksList blocks;
		float spawnOffset;
		float minDistance;
		float maxDistance;
		Point minSize;
		Point maxSize;
		float parralaxOffset;
		float spawnPosition;
	};
}

