#pragma once
#include "Components/Model.h"
#include <thread>
#include <functional>

enum NewLevel
{
	Maim_Menu,
	Level_Easy,
	Level_Normal,
	Level_Hard,
};

class InputSystem;
class WindowSystem;
class ResourceManager;
class Camera;
class Shader;
class ModelLoader;
class BulletPhysicsWorld;
class BaseObject;
class Renderer;
class Factory;
class Player;
class Level;

class GameApp
{
public:
	GameApp(InputSystem* input, WindowSystem* window);
	~GameApp();

	void Tick(float);
	void Render();
	InputSystem* GetCurrentInput() { return inputManager; }

	WindowSystem* GetWindowSystem() { return windowManager; }

	void UpdateCamera(Camera* cam);

	void UpdateLevel(NewLevel);

	void SetEndApplication() { EndApplication = true; }

	bool GetEndApplication() { return EndApplication; }

	void OnModelLoaded(Model* model);

	float GetProgress() { return currentProgress; };

	ResourceManager* GetResources() { return resources; };

protected:
	void Initialize();

	void Loding(std::vector<std::string>);

	void SetupLevel();

	std::function<void(Model*)> loadHandle;

	// input system
	InputSystem* inputManager = nullptr;
	// window system
	WindowSystem* windowManager = nullptr;
	// render class
	Renderer* renderer = nullptr;
	// resources manager
	ResourceManager* resources = nullptr;
	// bullet world class wrapper
	BulletPhysicsWorld* worldInstance = nullptr;
	// factory for spawning
	Factory* factory = nullptr;

	Level* level = nullptr;

	Level* newLevel = nullptr;

	bool EndApplication = false;

	bool loadingNewLevel = false;

	int depndencyCount = 0;
	int loadededCount = 0;

	float currentProgress = 0;

	std::thread loadingThread;
};