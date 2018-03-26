#include "Common.h"
#include "Level.h"

#include "Bird.h"
#include "Block.h"
#include "BlockLayer.h"
#include "Collider.h"
#include "Renderer.h"
#include "Score.h"

using namespace FlappyBird;

const float kMinBlockDistance = 350.f;
const float kMaxBlockDistance = 660.f;
const float kDefaultSpeed = 240.f;
const float kBlockW = 120.f;

enum
{
	ELayer_Background = 0,
	ELayer_Blocks,
	ELayer_Player,
	ELayer_Foreground,
	ELayer_Overlay,
	ELayer_Menu,
	ELayer_Max
};

Level::Level() :
	speed(kDefaultSpeed),
	score( 0 ),
	distance( 0.f ),
	nextBlockDistance( 0.f ),
	paused( false ),
	bird( nullptr ),
	floor( nullptr ),
	ceiling( nullptr ),
	background0(nullptr),
	background1(nullptr),
	size(600,800)
{
	bird = new Bird();
	bird->SetLayer( ELayer_Player );
	bird->SetColor(Color(1.0f, 1.f, 1.0f, 1.f));
	bird->AddToLevel(this);

	background0 = new BlockLayer();
	background0->AddToLevel(this);
	background0->SetVisible(false);
	background0->SetDistance(20, 200);
	background0->SetSize(Point(100, 200), Point(200, 600));
	background0->SetColor(Color(1.0f, 0.0f, 0.10f, 0.35f));
	background0->SetParalaxOffset(0.7f);

	background1 = new BlockLayer();
	background1->AddToLevel(this);
	background1->SetVisible(false);
	background1->SetDistance(20, 200);
	background1->SetSize(Point(100, 200), Point(200, 600));
	background1->SetColor(Color(1.0f, 0.0f, 0.01f, 0.15f));
	background1->SetParalaxOffset(0.8f);

	foreground = new BlockLayer();
	foreground->AddToLevel(this);
	foreground->SetVisible(false);
	foreground->SetDistance(200, 800);
	foreground->SetSize(Point(40, 10), Point(80, 15));
	foreground->SetColor(Color(1.0f, 0.0f, 0.15f, 0.29f));
	foreground->SetLayer(ELayer_Foreground);
	foreground->SetParalaxOffset(7.5f);
	
	Color floorColor = Color(1.0f, 0.02f, 0.25f, 0.49f);

	floor = new GameObject();
	floor->SetSize(size.x, 10.f);
	floor->SetPosition(size.x / 2, 800.f);
	floor->SetColor(floorColor);
	floor->SetLayer(ELayer_Overlay);
	floor->AddToLevel(this);
	floor->SetCollider(new BoxCollider());

	ceiling = new GameObject();
	ceiling->SetSize(size.x, 10.f);
	ceiling->SetPosition(size.x / 2, 0);
	ceiling->SetColor(floorColor);
	ceiling->SetLayer(ELayer_Overlay);
	ceiling->AddToLevel(this);
	ceiling->SetCollider(new BoxCollider());

	Color digitalColor = Color(1.0f, 0.5f, 0.5f, 0.9f);

	scoreMenu = new Score();
	scoreMenu->SetPosition(size.x/2,size.y/2-100);
	scoreMenu->SetColor(digitalColor);
	scoreMenu->SetLayer(ELayer_Overlay);
	scoreMenu->AddToLevel(this);

	pause[0] = new GameObject();
	pause[0]->SetSize(50, 200);
	pause[0]->SetColor(digitalColor);
	pause[0]->SetLayer(ELayer_Menu);
	pause[0]->SetVisible(false);
	pause[0]->AddToLevel(this);

	pause[1] = new GameObject();
	pause[1]->SetSize(50, 200);
	pause[1]->SetColor(digitalColor);
	pause[1]->SetLayer(ELayer_Menu);
	pause[1]->SetVisible(false);
	pause[1]->AddToLevel(this);

	StartScene( EScene::PreGameScene );
}

Level::~Level()
{
	// TODO:
	std::list<GameObject*> aliveObjects = gameObjects;
	for (auto& object : aliveObjects)
		object->Destroy();

	for (auto& object : deletedObjects)
		delete object;
	deletedObjects.clear();
}

void Level::Resize(float width, float height)
{
	size.x = width;
	size.y = height;

	floor->SetSize(size.x, 10.f);
	floor->SetPosition(size.x / 2, 800.f);

	ceiling->SetSize(size.x, 10.f);
	ceiling->SetPosition(size.x / 2, 0);

	scoreMenu->SetPosition(size.x / 2, size.y / 2 - 100);

	if (scene == EScene::EndCutscene)
	{
		bird->SetPosition(size.x / 2, size.y / 2 + 100);
	}
	else if (scene == EScene::PreGameScene)
	{
		bird->SetPosition(size.x / 2, size.y / 2);
	}

	pause[0]->SetPosition(size.x / 2 - 40, size.y / 2);
	pause[1]->SetPosition(size.x / 2 + 40, size.y / 2);

	background0->SetSpawnPosition(size.x);
	background1->SetSpawnPosition(size.x);
	foreground->SetSpawnPosition(size.x);
}


void Level::Update( float delta )
{
	if( paused )
		return;

	for (auto& object : gameObjects)
		object->Update( delta );

	for (auto& object : deletedObjects)
		delete object;
	deletedObjects.clear();

	float dx = speed * delta;

	switch (scene)
	{

	case EScene::StartCutscene:
	{

	}
	break;

	case EScene::PreGameScene:
	{
		background0->MoveBackground(-dx);
		background1->MoveBackground(-dx);
		foreground->MoveBackground(-dx);
	}
	break;

	case EScene::GameScene:
	{
		background0->MoveBackground(-dx);
		background1->MoveBackground(-dx);
		foreground->MoveBackground(-dx);

		for (auto block : blocks)
		{
			auto position = block->GetPosition();
			block->SetPosition(position.x - dx, position.y);

			if (block->Collided(bird) && block->GetLayer() == ELayer_Player)
			{
				StartScene(EScene::EndCutscene);
				return;
			}

			if (block->GetBoundsRect().x + block->GetBoundsRect().width < 0)
			{
				block->Destroy();
			}
		}

		if (bird->Collided(floor) || bird->Collided(ceiling))
		{
			StartScene(EScene::EndCutscene);
			return;
		}

		if (distance > nextBlockDistance)
		{
			Point position = Point(size.x + kBlockW / 2, size.y / 2);
			Point blockSize = Point(kBlockW, size.y);

			Block* block = new Block();
			block->SetPosition(position);
			block->SetSize(blockSize);
			block->SetLayer(ELayer_Player);
			block->AddToLevel(this);
			blocks.push_back(block);

			nextBlockDistance = distance + random(kMinBlockDistance, kMaxBlockDistance);
		}

		distance += dx;
	}
	break;

	case EScene::EndCutscene:
	{
		
	}
	break;

	default:
		break;
	}
	
	
}

void Level::StartScene( EScene _scene )
{
	scene = _scene;
	paused = false;
	if (scene == EScene::GameScene)
	{
		Restart();
		bird->Start();
	}
	else if (scene == EScene::EndCutscene)
	{
		bird->PlayStunAnim();
		bird->SetPosition( size.x/2, size.y/2 + 100);
		bird->SetSize( 200, 200 );
	}
	else if (scene == EScene::PreGameScene)
	{
		bird->SetSize(Point(80.f, 80.f));
		bird->SetPosition(size.x / 2, size.y / 2);
		bird->SetFlyPosition(100, size.y / 2);
		bird->Restart();
		for (auto block : blocks)
			block->Destroy();
		blocks.clear();
		distance = 0;
		nextBlockDistance = 0;
	}
}

void Level::Draw( Renderer* renderer )
{
	for (int i = 0; i < ELayer_Max; i++)
	{
		if (i == ELayer_Background)
			renderer->SetLineWidth(10.05f);
		else if (i == ELayer_Foreground)
			renderer->SetLineWidth(15.01f);
		else if (i == ELayer_Overlay)
			renderer->SetLineWidth(40.1f);
		else if (i == ELayer_Player)
			renderer->SetLineWidth(15.04f);
		else if(i == ELayer_Menu)
			renderer->SetLineWidth(25.f);
		else
			renderer->SetLineWidth(100.f);
		for (auto& object : gameObjects)
		{
			if ( object->GetLayer() == i )
				object->Draw( renderer );
		}
	}
}

void Level::DestroyBlock(Block* block)
{
	blocks.remove(block);
	if (scene == EScene::GameScene)
	{
		score++;
		scoreMenu->SetValue(score);
		speed = kDefaultSpeed * log(score+1);
	}
}

void Level::Restart()
{
	distance = 0.0f;
	blocks.clear();
	score = 0;
	speed = kDefaultSpeed;
	scoreMenu->SetValue(score);
	paused = false;
}

void Level::SetSize( Point _size )
{
	size = _size;
}

void Level::Pause()
{
	if (scene == EScene::GameScene)
	{
		paused = true;
		pause[0]->SetVisible(true);
		pause[1]->SetVisible(true);
	}
}

void Level::Resume()
{
	paused = false;
	pause[0]->SetVisible(false);
	pause[1]->SetVisible(false);
}

void Level::KeyDown( int key )
{
}

void Level::KeyUp( int key )
{
}

void Level::MouseMove( float x, float y )
{
}

void Level::MouseDown( float x, float y )
{
	if (scene == EScene::PreGameScene)
	{
		StartScene(EScene::GameScene);
	}
	else if (scene == EScene::GameScene)
	{
		if ( paused )
			Resume();
		else
			bird->Flap();
	}
	else if (scene == EScene::EndCutscene)
	{
		StartScene(EScene::PreGameScene);
	}
}

void Level::MouseUp( float x, float y )
{
}

void Level::AddObject( GameObject* object )
{
	gameObjects.push_back(object);
}

void Level::RemoveObject( GameObject* object )
{
	gameObjects.remove(object);
	deletedObjects.push_back(object);
}