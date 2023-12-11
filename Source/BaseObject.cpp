#include "BaseObject.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <LinearMath/btMotionState.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

void BaseObject::Update()
{
	if (rigidBody)
	{
		btTransform l_transform;
		rigidBody->getMotionState()->getWorldTransform(l_transform);
		btVector3 position = l_transform.getOrigin();

		float x = position.getX();
		float y = position.getY();
		float z = position.getZ();

		transform.SetPosition(glm::vec3(x,y,z));
	}
}
