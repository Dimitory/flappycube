#pragma once

#include "GameObject.h"

namespace FlappyBird
{
	class Score : public GameObject
	{
		struct WatchBlock
		{
			Rect rect;
			bool show;
		};
	public:
		Score();
		virtual ~Score();

		virtual void AddToLevel(Level* level) override;
		virtual void Draw(Renderer* renderer) override;

		void SetValue(int value);
	private:
		WatchBlock blocks[70];
	};
}

