#pragma once

#include "GameObject.h"

namespace FlappyBird
{
	class Bird : public GameObject
	{
	public:
		Bird();
		virtual ~Bird();
	
		virtual void AddToLevel(Level* _level) override;

		virtual void Update(float delta) override;
		virtual void Draw(Renderer* renderer) override;

		void Restart();

		void Flap();
		void Start();
		void Destroy();

		void PlayStunAnim();

		void SetFlyPosition(float x, float y);

	private:
		GameObject* starts[5];
		float flapSpeed;
		bool frezed;
		bool started;
		bool stunned;
		Point flyPosition;
		Point cntPosition;
		Point oldPosition;
		AnimTimer anim;
		AnimTimer dieAnim;
		float angle;
	};
}

