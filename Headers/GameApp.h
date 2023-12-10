#pragma once
#include "Model.h"
#include "WindowManager.h"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

class Input;
class WindowManager;
class ResourceManager;
class Camera;
class Shader;
class ModelLoader;
class BulletPhysicsWorld;

class GameApp
{
public:
	~GameApp();
	static GameApp* GetInstance();

	void Tick(float);
	Input* GetCurrentInput() { return input; }
	WindowManager* GetCurrentWindowSystem() { return windowManager; }
	Camera* GetCurrentCamera() { return mainCam; }
	ResourceManager* GetResources() { return resources; }
protected:
	GameApp();

	static GameApp* instance;

	Input* input;
	WindowManager* windowManager;

	ResourceManager* resources;
	Camera* mainCam;
	BulletPhysicsWorld* worldInstance;

	// testing bullet rendering
	btCollisionShape* shape;
	btCollisionShape* Cubeshape;
};