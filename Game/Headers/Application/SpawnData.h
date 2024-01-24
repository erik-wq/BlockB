#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Components/Transform.h"
#include <string>
#include "Objects/BaseObject.h"

enum ColliderShape
{
	NoShape,
	Box,
	Sphere,
	Capsule,
	Cylinder,
	Convex
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

	Layer layer = NO;
};

// for spawning objets
struct SpawnParams
{
	std::string modelPath = "";
	// transform of object
	Transform transform = Transform();
	// collider ofset
	glm::vec3 colliderOfset = glm::vec3(0);
	// collider information
	SpawnColliderParams colliderParams;
};