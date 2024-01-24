#pragma once
#include <vector>
#include <string>

struct SpawnParams;

class GameApp;
class Factory;
class BaseObject;
class Camera;
class Player;
class BulletPhysicsWorld;
class btCollisionObject;
class MenuState;
class Shader;

class Level
{
public:
	Level(GameApp* game, Factory* factory);
	virtual ~Level();

	virtual void Tick(float);

	virtual std::vector<BaseObject*> GetObjects() { return objects; };

	Camera* GetCamera() { return camera; };

	void SetPhysicsWorld(BulletPhysicsWorld* world);

	// adds to all objects of level
	BaseObject* SpawnObject(const SpawnParams& params);

	// spawn only player
	Player* SpawnPlayer(const SpawnParams& params);

	void ChangeUIState(MenuState* state);

	virtual bool UpdatePhysics() { return false; }

	virtual void Initialize();

	void RemoveCollisionObject(btCollisionObject*);

	virtual void SetLevelLights(Shader*) {};

	virtual bool RenderSkybox() { return false; }

	void CloseApplication();

	std::vector<std::string> GetDependencies() { return dependencies; };

	GameApp* GetGameInstance() { return gameInstance; };

	virtual std::vector<BaseObject*> GetLights() { return std::vector<BaseObject*>(); };
protected:
	virtual void UpdateUI(float);

	virtual void SpawnCamera();

	GameApp* gameInstance;

	BulletPhysicsWorld* physicsWorld;

	Camera* camera;

	Factory* factory;

	MenuState* menuState = nullptr;

	// all objects in map
	std::vector<BaseObject*> objects;

	std::vector<std::string> dependencies;
};