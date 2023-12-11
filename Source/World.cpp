#include "World.h"
#include "DebugDrawer.h"

void BulletPhysicsWorld::Update(glm::mat4 ProjectionView, float)
{
    dynamicsWorld->stepSimulation(1.f / 60.f, 5);
    dynamicsWorld->debugDrawWorld();
    debugDrawer->RenderDebug(ProjectionView);

    int numManifolds = dispatcher->getNumManifolds();
    for (int i = 0; i < numManifolds; i++) {
        btPersistentManifold* contactManifold = dispatcher->getManifoldByIndexInternal(i);

        // Get the two colliding objects
        const btCollisionObject* objA = contactManifold->getBody0();
        const btCollisionObject* objB = contactManifold->getBody1();

        // Process collision information here
        int numContacts = contactManifold->getNumContacts();
        if (numContacts > 0) {
            printf("Collision detected between objects\n");
           //  std::cout << "Object A: " << objA->getUserPointer() << ", ";
           //  std::cout << "Object B: " << objB->getUserPointer() << std::endl;
        }
    }
}

void BulletPhysicsWorld::AddCollisionObject(btCollisionShape* collisionShape, btTransform transform, btScalar mass) {
    btVector3 localInertia(0, 0, 0);

    if (mass != 0.0)
        collisionShape->calculateLocalInertia(mass, localInertia);

    btMotionState* motionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShape, localInertia);
    btRigidBody* rigidBody = new btRigidBody(rbInfo);

    dynamicsWorld->addRigidBody(rigidBody);
}

void BulletPhysicsWorld::AddRigidBody(btRigidBody* rigidbody)
{
    if (dynamicsWorld)
    {
        dynamicsWorld->addRigidBody(rigidbody);
    }
}

void BulletPhysicsWorld::EnableDebugDraw() {
    // Create debug drawer
    debugDrawer = new DebugDrawer();
    dynamicsWorld->setDebugDrawer(debugDrawer);
}

void BulletPhysicsWorld::InitBulletWorld() {
    // Broadphase
    broadphase = new btDbvtBroadphase();

    // Collision configuration
    collisionConfiguration = new btDefaultCollisionConfiguration();

    // Dispatcher
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    // Solver
    solver = new btSequentialImpulseConstraintSolver;

    // Create dynamics world
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-1,0));

    EnableDebugDraw();
}

void BulletPhysicsWorld::Cleanup() {
    // Delete collision objects and shapes
    for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; --i) {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState()) {
            delete body->getMotionState();
        }
        dynamicsWorld->removeCollisionObject(obj);
        delete obj;
    }

    // Delete dynamics world components
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}