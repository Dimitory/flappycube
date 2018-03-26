#pragma once

#include "GameObject.h"

namespace FlappyBird
{
	class Block : public GameObject
	{
	public:
		Block();
		virtual ~Block();

		virtual void AddToLevel(Level* _level) override;

		virtual bool Collided(const GameObject* other) const override;

		virtual void Destroy() override;
		virtual void Update( float delta ) override;

	private:
		GameObject* topBlock;
		GameObject* bottomBlock;
		float holePos;
		float holeSize;
		float spawnProgress;
		Color color;
	};
}

