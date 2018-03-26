#pragma once

namespace FlappyBird
{
	class Scene
	{
		typedef std::list<GameObject*>	GameObjectsList;
		typedef std::list<Block*>		BlocksList;

	public:
		Scene();
		virtual ~Scene();
	
		virtual void Update( float delta );
		virtual void Draw();

		virtual void Pause() = 0;
		virtual void Resume() = 0;

	protected:
		friend class GameObject;

		void AddObject( GameObject* object );
		void RemoveObject( GameObject* object );

	private:
		GameObjectsList gameObjects;
		BlocksList blocks;
	};
}

