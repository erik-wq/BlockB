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

    void Update(glm::mat4 ProjectionView,float);

    void Render(glm::mat4);

    void AddCollisionObject(btCollisionShape* collisionShape, btTransform transform, btScalar mass);

    void AddRigidBody(btRigidBody* rigidbody);

    void EnableDebugDraw();

    // world instance
    btDiscreteDynamicsWorld* dynamicsWorld;
private:

    // broadface for early out test
    btBroadphaseInterface* broadphase;
    // configuration
    btDefaultCollisionConfiguration* collisionConfiguration;
    //dispacher
    btCollisionDispatcher* dispatcher;
    // solver
    btSequentialImpulseConstraintSolver* solver;
    // drawer
    DebugDrawer* debugDrawer;

    void InitBulletWorld();

    void Cleanup();
};