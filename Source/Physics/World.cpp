#include "Physics/World.h"
#include "Physics/DebugDrawer.h"
#include "Physics/Dispacher.h"

void BulletPhysicsWorld::Update(glm::mat4 ProjectionView, float delta)
{
    dynamicsWorld->stepSimulation(btScalar(delta), 5);

    // collision and overlaps checking
    dispatcher->CheckEvents();
}

void BulletPhysicsWorld::Render(glm::mat4 ProjectionView)
{
    dynamicsWorld->debugDrawWorld();
    debugDrawer->RenderDebug(ProjectionView);
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

void BulletPhysicsWorld::AddCollisionObject(btCollisionObject* object)
{
    dynamicsWorld->addCollisionObject(object);
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

void BulletPhysicsWorld::AddConstraint(btTypedConstraint* constraint, bool disableColisionBetwenLinke)
{
    dynamicsWorld->addConstraint(constraint, disableColisionBetwenLinke);
}

void BulletPhysicsWorld::InitBulletWorld() {
    // Broadphase
    broadphase = new btDbvtBroadphase();

    // Collision configuration
    collisionConfiguration = new btDefaultCollisionConfiguration();

    // Dispatcher
    dispatcher = new Dispacher(collisionConfiguration);

    // Solver
    solver = new btSequentialImpulseConstraintSolver;

    // Create dynamics world
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-10,0));

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