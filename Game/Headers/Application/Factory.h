#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "SpawnData.h"

class ResourceManager;
class BaseObject;
class Player;
class BulletPhysicsWorld;
class btCollisionObject;
class btTransform;
class btRigidBody;
class btCollisionShape;


class Factory
{
public:
	Factory(ResourceManager*, BulletPhysicsWorld*);
	~Factory();

	Player* SpawnPlayer(const SpawnParams& params);

	BaseObject* SpawnObject(const SpawnParams& params);

	btCollisionObject* SpawnCollider(const SpawnColliderParams&, const glm::vec3, std::string convex = "");

private:

	btTransform CalculateColliderTransform(const glm::vec3 pos);

	btRigidBody* SpawnRigidBody(btCollisionShape* shape, float mass);

	btCollisionObject* SpawnCollisionObject(btCollisionShape* shape, bool ghost);

	btCollisionShape* SpawnCollisionShape(ColliderShape type, const glm::vec3 extends , std::string convex = "");

	btCollisionShape* SpawnConvex(std::string convex);

	ResourceManager* resources;
	BulletPhysicsWorld* world;
};