#pragma once
#include "Model.h"
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

	BaseObject* object;
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
};