#pragma once
#include "Components/Transform.h"

#include <btBulletCollisionCommon.h>

class Model;
class InputSystem;
class Level;
class ResourceManager;

enum SynchState
{
	NoSynch, // no synch at all
	PhysicsSynch, // synch position with collider
	TransformSynch // synch collider based on transform
};

// used when collision happens or overlap
enum Layer
{
	NO,
	GROUND,
	COLLECTIABLE,
	PLAYER,
	ENEMY,
	OBSTICLE
};


class BaseObject
{
public:
	BaseObject(const Transform& trans = Transform(), btCollisionObject* collider = nullptr, glm::vec3 colliderOfset = glm::vec3(0), Model* model = nullptr, Layer objectLayer = NO);
	virtual ~BaseObject();
	
	virtual void Update(float);

	void SetLevel(Level* level) { this->levelInstance = level; };

	Model* GetModel() { return model; }

	btCollisionObject* GetCollider() { return collisionObject; }

	void SetSynchronizationState(SynchState state) { synch = state; }

	Transform transform;

	virtual void OnCollisionEnter(BaseObject*) {  };

	virtual void OnCollisionExit(BaseObject*) {  };

	virtual void OnOverlapEnter(BaseObject*) {  };

	virtual void OnOverlapExit(BaseObject*) {  };

	virtual void GetResources(ResourceManager*) {};

	virtual void BindInput(InputSystem*) {};
	virtual void UnBindInput(InputSystem*) {};

	Layer GetLayer() const { return layer; };
protected:
	// synchronize transform with physics collider
	void SynchWithPhysics();
	// synchronize physics collider with transform values
	void SynchPhysics();
	btTransform GetBulletTransform();
	btVector3 ConverToBullet(glm::vec3) const;
	SynchState synch = NoSynch;
	Model* model = nullptr;
	btCollisionObject* collisionObject = nullptr;
	btVector3 colliderOfset = btVector3(0,0,0);
	Level* levelInstance;
	Layer layer = NO;
};