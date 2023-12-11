#pragma once
#include "Model.h"
#include "Transform.h"
#include <bullet/btBulletCollisionCommon.h>

class BaseObject
{
public:
	void Update();

	Model* GetModel() { return model; }
	Transform* GetTransform() { return &transform; }

	Transform transform;
	Model* model;
	btCollisionShape* shape;
	btRigidBody* rigidBody;
};