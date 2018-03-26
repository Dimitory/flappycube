#pragma once

namespace FlappyBird
{
	class Bird;
	class Block;
	class GameObject;
	class BlockLayer;
	class Renderer;
	class Score;

	class Level
	{
		enum class EScene
		{
			StartCutscene,
			PreGameScene,
			GameScene,
			EndCutscene
		};

		typedef std::list<GameObject*>	GameObjectsList;
		typedef std::list<Block*>		BlocksList;

	public:
		Level();
		~Level();
	
		void Update( float delta );
		void Draw(Renderer* renderer);

		void Resize(float width, float height);
		void Pause();
		void Resume();

		void Restart();

		void KeyDown( int key );
		void KeyUp( int key );
		void MouseMove( float x, float y );
		void MouseDown( float x, float y );
		void MouseUp( float x, float y );

		void SetSize( Point size );

		void DestroyBlock(Block* block);

	private:
		void StartScene( EScene scene );

	protected:
		friend class GameObject;

		void AddObject( GameObject* object );
		void RemoveObject( GameObject* object );

	private:
		Bird* bird;
		GameObject* floor;
		GameObject* ceiling;
		GameObject* left;
		GameObject* top;
		GameObject* pause[2];
		Score* scoreMenu;
		GameObjectsList gameObjects;
		GameObjectsList deletedObjects;
		BlockLayer* background0;
		BlockLayer* background1;
		BlockLayer* foreground;
		BlocksList blocks;	
		float nextBlockDistance;
		float distance;
		float speed;
		bool paused;
		EScene scene;
		Point size;
		int score;
	};
}

