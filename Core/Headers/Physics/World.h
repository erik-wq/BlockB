#pragma once
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"

class DebugDrawer;
class Dispacher;
class Shader;

class BulletPhysicsWorld {
public:
    BulletPhysicsWorld(Shader* debugShader) {
        InitBulletWorld(debugShader);
    }

    ~BulletPhysicsWorld() {
        Cleanup();
    }

    void Update(float);

    void Render(glm::mat4);

    void AddCollisionObject(btCollisionShape* collisionShape, btTransform transform, btScalar mass);

    void AddCollisionObject(btCollisionObject*);

    void AddRigidBody(btRigidBody* rigidbody);

    void AddConstraint(btTypedConstraint*, bool disableColisionBetwenLinke = true);

    void RemoveCollisionObject(btCollisionObject*);

    void RemoveRigidBody(btRigidBody*);
    
    void RemoveConstraint(btTypedConstraint*);
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

    void InitBulletWorld(Shader* debugShader);

    void Cleanup();
};