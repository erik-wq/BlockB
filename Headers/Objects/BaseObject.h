#pragma once
#include "Components/Transform.h"

#include <bullet/btBulletCollisionCommon.h>

class Model;
class Input;

class BaseObject
{
public:
	BaseObject(const Transform& trans = Transform(), btCollisionObject* collider = nullptr, glm::vec3 colliderOfset = glm::vec3(0), Model* model = nullptr);
	
	virtual ~BaseObject() {}
	virtual void Update(float);

	// for input handling
	virtual void HandleInput(Input*) {};

	Model* GetModel() { return model; }

	Transform transform;

	virtual void OnCollisionEnter(BaseObject*) { printf("collision entered\n"); };

	virtual void OnCollisionExit(BaseObject*) { printf("collision exited\n"); };

	virtual void OnOverlapEnter(BaseObject*) { printf("overlap entered\n"); };

	virtual void OnOverlapExit(BaseObject*) { printf("overlap exited\n"); };
protected:
	btTransform GetBulletTransform();
	btVector3 ConverToBullet(glm::vec3) const;
	Model* model = nullptr;
	btCollisionObject* collisionObject = nullptr;
	btVector3 colliderOfset = btVector3(0,0,0);
};