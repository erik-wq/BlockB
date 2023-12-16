#include "Application/Factory.h"
#include "Physics/CustomMotionState.h"
#include "Physics/World.h"
#include "Application/ResourceManager.h"
#include "Objects/Player.h"

Factory::Factory(ResourceManager* resource, BulletPhysicsWorld* dynamicWorld) :
	resources(resource),
	world(dynamicWorld)
{
}

Factory::~Factory()
{
}

Player* Factory::SpawmPlayer(const SpawnParams& params)
{
	btCollisionObject* collider = SpawnCollider(params.colliderParams, params.transform.position);

	btRigidBody* rigidBody = dynamic_cast<btRigidBody*>(collider);
	if (!rigidBody)
	{
		printf("failed to pawn rigidbody for player\n");
		delete collider;
		return nullptr;
	}

	btTransform transform = CalculateColliderTransform(params.transform.position + params.colliderOfset);

	// constrain for player rotation
	btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(*rigidBody, transform, false);
	dof6->setAngularLowerLimit(btVector3(0, -SIMD_INFINITY, 0));
	dof6->setAngularUpperLimit(btVector3(0, SIMD_INFINITY, 0));
	dof6->setLinearLowerLimit(btVector3(-SIMD_INFINITY, -SIMD_INFINITY, -SIMD_INFINITY));
	dof6->setLinearUpperLimit(btVector3(SIMD_INFINITY, SIMD_INFINITY, SIMD_INFINITY));
	// disable deactivation
	rigidBody->setActivationState(DISABLE_DEACTIVATION);

	// add to physics world
	world->AddConstraint(dof6);
	world->AddRigidBody(rigidBody);

	Model* model = resources->GetModel(params.modelPath);

	Player* player = new Player(params.transform, rigidBody, params.colliderOfset, model);
	rigidBody->setUserPointer(player);
	return player;
}

BaseObject* Factory::SpawnObject(const SpawnParams& params)
{
	btCollisionObject* collider = SpawnCollider(params.colliderParams, params.transform.position);
	if (collider)
	{
		// object may not have collider
		btTransform transform = CalculateColliderTransform(params.transform.position + params.colliderOfset);
		world->AddCollisionObject(collider);
	}

	Model* model = nullptr;
	
	if (params.modelPath != "")
	{
		model = resources->GetModel(params.modelPath); 
		if (!model)
		{
			printf("failed to load model path %s\n", params.modelPath);
		}
	}

	BaseObject* object = new BaseObject(params.transform, collider, params.colliderOfset, model);;
	if (collider)
	{
		collider->setUserPointer(object);
	}
	return object;
}

btTransform Factory::CalculateColliderTransform(glm::vec3 pos)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	// posible to add custom rotation later
	transform.setRotation(btQuaternion(0, 0, 0, 1));
	return transform;
}

btCollisionObject* Factory::SpawnCollider(const SpawnColliderParams& params,const glm::vec3 position)
{
	if (params.type == NoType || params.shape == NoShape)
	{
		return nullptr;
	}
	btCollisionShape* shape = SpawnCollisionShape(params.shape, params.extends);
	if (!shape) return nullptr;
	btCollisionObject* object = nullptr;
	if (params.type == RigidBody)
	{
		object = SpawnRigidBody(shape, params.mass);
	}
	else
	{
		object = SpawnCollisionObject(shape, params.type == Ghost);
	}

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(position.x, position.y, position.z));
	transform.setRotation(btQuaternion(0, 0, 0, 1));

	object->setWorldTransform(transform);

	return object;
}

btRigidBody* Factory::SpawnRigidBody(btCollisionShape* shape ,float mass)
{
	btTransform transform;
	transform.setIdentity();

	btVector3 localInertia = btVector3(0, 0, 0);
	shape->calculateLocalInertia(mass, localInertia);
	btMotionState* motionState = new CustomMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(1, motionState, shape, localInertia);
	return new btRigidBody(rbInfo);
}

btCollisionObject* Factory::SpawnCollisionObject(btCollisionShape* shape, bool ghost)
{
	btCollisionObject* object = new btCollisionObject();
	object->setCollisionShape(shape);
	if (ghost)
	{
		// if object is ghost set flags
		object->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
	return object;
}

btCollisionShape* Factory::SpawnCollisionShape(ColliderShape type, const glm::vec3 extends)
{
	btVector3 bulletExtend = btVector3(extends.x, extends.y, extends.z);
	switch (type)
	{
	case Box:
	{
		btBoxShape* Box = new btBoxShape(bulletExtend);
		return Box;
	}
	break;
	case Capsule:
	{
		btCapsuleShape* Capsule = new btCapsuleShape(bulletExtend.getX(), bulletExtend.getY());
		return Capsule;
	}
	break;

	case Sphere:
	{
		btSphereShape* Sphere = new btSphereShape(bulletExtend.getY());
		return Sphere;
	}
	break;
	case Cylinder:
	{
		btCylinderShape* Cylinder = new btCylinderShape(bulletExtend);
		return Cylinder;
	}
	break;
	default:
	{
		return nullptr;
	}
	}
}
