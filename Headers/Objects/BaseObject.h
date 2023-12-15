#pragma once
#include "Components/Transform.h"

#include <bullet/btBulletCollisionCommon.h>

class Model;
class Input;

class BaseObject
{
public:
	BaseObject(const Transform& trans = Transform(), Model* model = nullptr, btCollisionShape* shape = nullptr, btRigidBody* rigidbody = nullptr);
	
	virtual ~BaseObject() {}
	virtual void Update(float);

	// possible need to add which collision object it was
	virtual void OnCollisionStarted() {};

	virtual void OnCollisionEnded() {};

	// for input handling
	virtual void HandleInput(Input*) {};

	Model* GetModel() { return model; }

	Transform transform;

protected:
	btTransform GetBulletTransform();
	btVector3 ConverToBullet(glm::vec3) const;
	Model* model = nullptr;
	btCollisionShape* shape = nullptr;
	btRigidBody* rigidBody = nullptr;
	btVector3 colliderOfset = btVector3(0,0,0);
};