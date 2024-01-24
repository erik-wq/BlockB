#include "Objects/BaseObject.h"

#include "Components/Model.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "Levels/BaseLevel.h"

#include <LinearMath/btMotionState.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

BaseObject::BaseObject(const Transform& trans, btCollisionObject* collider, glm::vec3 ofset, Model* model, Layer objectLayer) :
	transform(trans),
	model(model),
	collisionObject(collider),
	layer(objectLayer)
{
	colliderOfset = btVector3(ofset.x, ofset.y, ofset.z);
	if (model)
	{
		model->AddInstance(this);
	}
}

BaseObject::~BaseObject()
{
	if (model)
	{
		model->RemoveInstance(this);
	}
	if (levelInstance && collisionObject)
	{
		levelInstance->RemoveCollisionObject(collisionObject);
		btCollisionShape* collisionShape = collisionObject->getCollisionShape();
		delete collisionObject;
		delete collisionShape;
	}
#ifndef NDEBUG
	if (collisionObject && !levelInstance)
	{
		printf("Cannot remove collider and properly remove \n");
	}
#endif
}

void BaseObject::Update(float)
{
	if (synch == NoSynch) return;
	else if (synch == PhysicsSynch)
	{
		SynchWithPhysics();
	}
	else if (synch == TransformSynch)
	{
		SynchPhysics();
	}
}
void BaseObject::SynchWithPhysics()
{
	if (!collisionObject) return;
	btTransform tran = GetBulletTransform();
	transform.SetPosition(tran.getOrigin() - colliderOfset);
	float x, y, z;
	tran.getRotation().getEulerZYX(z, y, x);
	transform.SetRotation(glm::degrees(glm::vec3(x, y, z)));
}
void BaseObject::SynchPhysics()
{
	if (!collisionObject) return;
	glm::vec3 position = transform.position;
	glm::vec3 rotation = transform.rotationRad;
	glm::quat quaternionRotation = glm::quat_cast(glm::orientate3(rotation));

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(position.x, position.y, position.z) - colliderOfset);
	trans.setRotation(btQuaternion(quaternionRotation.x, quaternionRotation.y, quaternionRotation.z, quaternionRotation.w));

	collisionObject->setWorldTransform(trans);
}
btTransform BaseObject::GetBulletTransform()
{
	return collisionObject->getWorldTransform();
}

btVector3 BaseObject::ConverToBullet(glm::vec3 vec) const
{
	return btVector3(vec.x, vec.y ,vec.z);
}
