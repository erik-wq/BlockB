#include "Application/GameApp.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "App/WindowSystem.h"
#include "Physics/World.h"

#include "Components/Transform.h"

#include "App/ResourceManager.h"
#include "Application/Renderer.h"
#include "App/InputSystem.h"

#include "Objects/BaseObject.h"
#include "Objects/Player.h"
#include "Objects/Camera.h"

#include "Application/Factory.h"
#include "Objects/Player.h"

#include "Levels/BaseLevel.h"
#include "Levels/InfiniteLevel.h"
#include "Levels/LightsLevel.h"
#include "Levels/Menu.h"
#include <atomic>

std::atomic<bool> loaded = false;

GameApp::GameApp(InputSystem* input, WindowSystem* window) :
	inputManager(input),
	windowManager(window)
{

	resources = new ResourceManager();
	worldInstance = new BulletPhysicsWorld(resources->GetShader("debug"));
	factory = new Factory(resources, worldInstance);
	Initialize();
}

void GameApp::UpdateCamera(Camera* cam)
{
	ScreenSize screen = windowManager->GetScreenSize();
	cam->screenHeight = screen.height;
	cam->screenWidth = screen.widht;
	renderer->ChangeCamera(cam);
}

void GameApp::UpdateLevel(NewLevel levelType)
{
	newLevel = nullptr;

	switch (levelType)
	{
		case Maim_Menu:
			newLevel = new MenuLevel(this, factory);
			break;
		case Level_Easy:
			newLevel = new InfiniteLevel(this, factory, Easy);
			break;
		case Level_Normal:
			newLevel = new InfiniteLevel(this, factory, Normal);
			break;
		case Level_Hard:
			// newLevel = new InfiniteLevel(this, factory, Hard);
			newLevel = new LightLevel(this, factory);
			break;
		default:
			break;
	}
	
	currentProgress = 0;

	std::vector<std::string> dependencies = newLevel->GetDependencies();
	depndencyCount = static_cast<int>(dependencies.size());

	loadHandle = std::bind(&GameApp::OnModelLoaded, this, std::placeholders::_1);
	resources->OnModelLoaded.subscribe(loadHandle);

	loadingThread = std::thread(&GameApp::Loding, this, dependencies);;
	loadingNewLevel = true;
}

void GameApp::OnModelLoaded(Model*)
{
	loadededCount++;
	std::cout << "Loaded: " << loadededCount << " / " << depndencyCount << "\n";
	currentProgress = static_cast<float>(loadededCount) / static_cast<float>(depndencyCount);
}

void GameApp::Initialize()
{
	renderer = new Renderer(resources);
	// level = new InfiniteLevel(this, factory, Easy);
	level = new MenuLevel(this, factory);
	SetupLevel();
}

void GameApp::Loding(std::vector<std::string> dependencies)
{
	// loop over all dependencies
	for(auto& dependency : dependencies)
	{
		std::cout << dependency << "\n";
		if (resources->GetModel(dependency))
		{
			continue;
		}

		resources->GetModel(dependency);
	}
	loaded = true;
}

void GameApp::SetupLevel()
{
	level->SetPhysicsWorld(worldInstance);
	level->Initialize();
	renderer->SetLevel(level);
}

GameApp::~GameApp()
{
	delete resources;
	delete worldInstance;
	delete renderer;
	delete factory;
	delete level;
	delete newLevel;
}

void GameApp::Tick(float delta)
{
	inputManager->ProcessInput();
	if (level->UpdatePhysics())
	{
		worldInstance->Update(delta);
	}
	level->Tick(delta);
	// worldInstance->Render(renderer->GetProjectionViewMatrix());
	// std::cout << "Tick\n";
}

void GameApp::Render()
{
	renderer->Render();
	windowManager->SwapBuffers();

	if (loadingNewLevel)
	{
		if (loaded) {
			loadingThread.join();
			loadingNewLevel = false;
			resources->OnModelLoaded.unsubscribe(loadHandle);
			loaded = false;
			resources->PushToGPU();
			delete level;
			level = newLevel;
			newLevel = nullptr;
			loaded = false;
			SetupLevel();
			std::cout << "Loading finished\n";
		}
	}
}
