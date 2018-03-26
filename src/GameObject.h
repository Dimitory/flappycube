#pragma once

namespace FlappyBird
{
	class Collider;
	class Level;
	class Renderer;
	class GameObject
	{
		typedef std::list<GameObject*> GameObjectList;
	public:
		GameObject();
		virtual ~GameObject();

		void AddObject( GameObject* child );
		void RemoveObject( GameObject* child );

		void SetCollider(Collider* collider);

		virtual void AddToLevel(Level* level);
		virtual void Destroy();
	
		virtual void Update( float delta );
		virtual void Draw(Renderer* renderer);

		virtual bool Collided(const GameObject* other) const;

		void SetLayer( int layer );
		void SetPosition(const float x, const float y);
		void SetPosition(const Point& position);
		void SetSize(const float w, const float h);
		void SetSize(const Point& size);
		void SetColor( const Color& color );
		void SetVisible( bool visible );

		int GetLayer() const { return layer; }
		Point GetPosition() const { return position; }
		Point GetSize() const { return size; }
		Rect GetBoundsRect() const { return bounds; }
		Color GetColor() const { return color; }

		bool IsVisible() const { return visible; }

	private:
		void RecalculateBounds();

	protected:
		Level* level;

	private:
		GameObjectList childs;
		Collider* collider;
		bool outdatedBounds;
		int layer;
		Point position;
		Point size;
		Rect bounds;
		Color color;
		bool visible;
	};
}

