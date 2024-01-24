#include "Levels/BaseLevel.h"
#include "Application/Factory.h"
#include "Objects/Camera.h"
#include "App/ResourceManager.h"
#include "Application/GameApp.h"

#include "Physics/World.h"
#include "Objects/BaseObject.h"
#include "Objects/Player.h"
#include "App/WindowSystem.h"
#include "UI/MenuState.h"

Level::Level(GameApp* game, Factory* factory) :
	gameInstance(game),
	factory(factory)
{

}

Level::~Level()
{
	delete camera;

	InputSystem* input = gameInstance->GetCurrentInput();
	// clear all objects from vetor;
	for (BaseObject* object : objects)
	{
		physicsWorld->RemoveCollisionObject(object->GetCollider());
		object->UnBindInput(input);
		delete object;
	}
}

void Level::Tick(float delta)
{
	for (BaseObject* object : objects)
	{
		object->Update(delta);
	}
	UpdateUI(delta);
}

void Level::Initialize()
{
	SpawnCamera();
}

void Level::RemoveCollisionObject(btCollisionObject* object)
{
	physicsWorld->RemoveCollisionObject(object);
}
void Level::SetPhysicsWorld(BulletPhysicsWorld* world)
{
	physicsWorld = world;
}

BaseObject* Level::SpawnObject(const SpawnParams& params)
{
	if (factory)
	{
		BaseObject* object = factory->SpawnObject(params);
		object->SetLevel(this);
		object->GetResources(gameInstance->GetResources());
		return object;
	}
	return nullptr;
}

Player* Level::SpawnPlayer(const SpawnParams& params)
{
	if (factory)
	{
		Player* player = factory->SpawnPlayer(params);
		player->BindInput(gameInstance->GetCurrentInput());
		player->SetLevel(this);
		player->GetResources(gameInstance->GetResources());
		return player;
	}
	return nullptr;
}

void Level::UpdateUI(float dt)
{
	if (!menuState)
	{
		return;
	}
	ScreenSize screen = gameInstance->GetWindowSystem()->GetScreenSize();

	menuState->UpdateUI(float(screen.widht), float(screen.height), dt);
}
void Level::ChangeUIState(MenuState* state)
{
	delete menuState;
	menuState = state;
}

void Level::CloseApplication()
{
	gameInstance->SetEndApplication();
}

void Level::SpawnCamera()
{
	camera = new Camera(glm::vec3(0, 0, -10));
	gameInstance->UpdateCamera(camera);
}
