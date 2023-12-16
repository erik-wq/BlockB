#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "Components/Transform.h"

class ResourceManager;
class BaseObject;
class Player;
class BulletPhysicsWorld;

enum ColliderShape
{
	NoShape,
	Box,
	Sphere,
	Capsule,
	Cylinder
};

enum CollisionType
{
	NoType,
	Ghost,
	Collision,
	RigidBody
};

struct SpawnColliderParams
{
	// type of collider
	ColliderShape shape = NoShape;
	// type of collision
	CollisionType type = NoType;
	// extends
	glm::vec3 extends = glm::vec3(0);
	// applies only to rigidBody
	float mass = 0;
};

// for spawning objets
struct SpawnParams
{
	char* modelPath = "";
	// transform of object
	Transform transform = Transform();
	// collider ofset
	glm::vec3 colliderOfset = glm::vec3(0);
	// collider information
	SpawnColliderParams colliderParams;
};

class Factory
{
public:
	Factory(ResourceManager*, BulletPhysicsWorld*);
	~Factory();

	Player* SpawmPlayer(const SpawnParams& params);

	BaseObject* SpawnObject(const SpawnParams& params);

	btCollisionObject* SpawnCollider(const SpawnColliderParams&, const glm::vec3);
private:
	btTransform CalculateColliderTransform(const glm::vec3 pos);

	btRigidBody* SpawnRigidBody(btCollisionShape* shape, float mass);

	btCollisionObject* SpawnCollisionObject(btCollisionShape* shape, bool ghost);

	btCollisionShape* SpawnCollisionShape(ColliderShape type, const glm::vec3 extends);

	ResourceManager* resources;
	BulletPhysicsWorld* world;
};