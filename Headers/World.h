#pragma once
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"

class DebugDrawer;

class BulletPhysicsWorld {
public:
    BulletPhysicsWorld() {
        InitBulletWorld();
    }

    ~BulletPhysicsWorld() {
        Cleanup();
    }

    void Update(glm::mat4, float);

    void AddCollisionObject(btCollisionShape* collisionShape, btTransform transform, btScalar mass);

    void AddRigidBody(btRigidBody* rigidbody);

    void EnableDebugDraw();

private:

    btRigidBody* rigidbody;

    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    DebugDrawer* debugDrawer;

    void InitBulletWorld();

    void Cleanup();
};