#pragma once
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"

class DebugDrawer;
class Dispacher;

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

    void AddCollisionObject(btCollisionObject*);

    void AddRigidBody(btRigidBody* rigidbody);

    void EnableDebugDraw();

    void AddConstraint(btTypedConstraint*, bool disableColisionBetwenLinke = true);

private:

    // broadface for early out test
    btBroadphaseInterface* broadphase;
    // configuration
    btDefaultCollisionConfiguration* collisionConfiguration;
    //dispacher
    Dispacher* dispatcher;
    // solver
    btSequentialImpulseConstraintSolver* solver;
    // drawer
    DebugDrawer* debugDrawer;

    // world instance
    btDiscreteDynamicsWorld* dynamicsWorld;

    void InitBulletWorld();

    void Cleanup();
};