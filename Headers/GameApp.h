#pragma once
#include "Components/Model.h"
#include "WindowManager.h"

class Input;
class WindowManager;
class ResourceManager;
class Camera;
class Shader;
class ModelLoader;
class BulletPhysicsWorld;
class BaseObject;
class btCollisionShape;
class Renderer;

class GameApp
{
public:
	~GameApp();
	static GameApp* GetInstance();

	void Tick(float);
	void Render();
	Input* GetCurrentInput() { return input; }
	WindowManager* GetCurrentWindowSystem() { return windowManager; }
	ResourceManager* GetResources() { return resources; }

	BaseObject* object;
protected:
	GameApp();
	void Initialize();

	// instance refernce
	static GameApp* instance;

	// input system
	Input* input;
	// window system
	WindowManager* windowManager;
	// render class
	Renderer* renderer;
	// resources manager
	ResourceManager* resources;
	// main camera
	Camera* mainCam;
	// bullet world class wrapper
	BulletPhysicsWorld* worldInstance;

	// testing bullet rendering, remove
	btCollisionShape* shape;
};