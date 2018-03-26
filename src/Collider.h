#pragma once

namespace FlappyBird
{
	struct Box
	{
		Point size;
		Point position;
		float rotation;
	};

	struct Circle
	{
		float radius;
		Point position;
	};

	// TODO: enum type
	class Collider
	{
	public:
		Collider();
		virtual ~Collider();

		bool Collides(const Collider* other) const;

		void SetPosition(const Point& position);
		void SetSize(const Point& size);

	protected:
		Box* box;
		Circle* circle;
	};

	class BoxCollider: public Collider
	{
	public:
		BoxCollider();
	};

	class SphereCollider: public Collider
	{
	public:
		SphereCollider();
	};
}

