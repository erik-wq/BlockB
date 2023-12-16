#include "GameApp.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "Physics/World.h"
#include "Physics/CustomMotionState.h"

#include "Input.h"

#include "Components/Transform.h"

#include "Application/ResourceManager.h"
#include "Application/Renderer.h"

#include "Objects/BaseObject.h"
#include "Objects/Player.h"
#include "Objects/Camera.h"

#include "Application/Factory.h"
#include "Objects/Player.h"

GameApp* GameApp::instance = nullptr;

GameApp::GameApp()
{
	windowManager = new WindowManager(800, 600);

	input = new Input();
	
	DisplayState* state = windowManager->GetDisplayState();
	input->root = windowManager->GetRootWindow();
	input->Init(windowManager->GetCurrentDisplay(), windowManager->GetRootWindow());
	
	resources = new ResourceManager();
	
	mainCam = new Camera(glm::vec3(1,3,15));
	mainCam->screenHeight = state->height;
	mainCam->screenWidth = state->width;

	worldInstance = new BulletPhysicsWorld();

	factory = new Factory(resources, worldInstance);

	// spawning testing floor will be replaced by tiles
	SpawnColliderParams colliderParams;
	colliderParams.extends = glm::vec3(10, 2, 10);
	colliderParams.shape = Box;
	colliderParams.type = Collision;

	SpawnParams spawnParams;
	spawnParams.colliderParams = colliderParams;

	floor = factory->SpawnObject(spawnParams);
}

void GameApp::Initialize()
{
	renderer = new Renderer(mainCam);
	SpawnPlayer();
	object->SetCameraFolow(mainCam, glm::vec3(0, 4, -10));
}

void GameApp::SpawnPlayer()
{
	SpawnColliderParams colliderParams;
	colliderParams.mass = 1;
	colliderParams.extends = glm::vec3(2, 1, 0);
	colliderParams.shape = Capsule;
	colliderParams.type = RigidBody;

	Transform tran;
	tran.SetPosition(glm::vec3(5, 12, 0));

	SpawnParams spawnParams;
	spawnParams.colliderOfset = glm::vec3(0, 2, 0);
	spawnParams.colliderParams = colliderParams;
	spawnParams.modelPath = "Madara_Uchiha.obj";
	spawnParams.transform = tran;

	object = factory->SpawmPlayer(spawnParams);
}

GameApp::~GameApp()
{
	delete windowManager;
	delete input;
	delete resources;
	delete mainCam;
	delete worldInstance;
	delete object;
	delete renderer;
	delete factory;
	delete floor;
}

GameApp* GameApp::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new GameApp();
		instance->Initialize();
	}
	return instance;
}

void GameApp::Tick(float delta)
{
	object->HandleInput(input);
	worldInstance->Update(mainCam->GetProjection() * mainCam->GetViewMatrix(), delta);
	MouseData data = input->GetMouseData();
	object->Update(delta);
	worldInstance->Render(mainCam->GetProjection() * mainCam->GetViewMatrix());
}

void GameApp::Render()
{
	renderer->Render();
}
