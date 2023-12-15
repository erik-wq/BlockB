#include "Objects/BaseObject.h"

#include "Components/Model.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <LinearMath/btMotionState.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

BaseObject::BaseObject(const Transform& trans, Model* model, btCollisionShape* shape, btRigidBody* rigidbody) :
	transform(trans),
	model(model),
	shape(shape),
	rigidBody(rigidbody)
{
	colliderOfset = btVector3(0, 2, 0);

	btTransform colTrans;
	colTrans.setIdentity();
	// changing this value break moving of object
	btTransform transform = GetBulletTransform();
	colTrans.setOrigin(transform.getOrigin() + colliderOfset);
	colTrans.setRotation(btQuaternion(0, 0, 0, 1));

	rigidBody->setWorldTransform(colTrans);

}

void BaseObject::Update(float)
{
	if (rigidBody)
	{
		btTransform tran = GetBulletTransform();
		transform.SetPosition(tran.getOrigin() - colliderOfset);
		float x, y, z;
		tran.getRotation().getEulerZYX(z,y,x);
		transform.SetRotation(glm::degrees(glm::vec3(x,y,z)));
	}
}

btTransform BaseObject::GetBulletTransform()
{
	btTransform bulletTransform;
	rigidBody->getMotionState()->getWorldTransform(bulletTransform);
	return bulletTransform;
}

btVector3 BaseObject::ConverToBullet(glm::vec3 vec) const
{
	return btVector3(vec.x, vec.y ,vec.z);
}
