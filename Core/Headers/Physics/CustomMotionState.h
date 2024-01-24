#pragma once
#include <LinearMath/btMotionState.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

class CustomMotionState : public btDefaultMotionState {
public:
    CustomMotionState(const btTransform& initialTransform)
        : btDefaultMotionState(initialTransform) {}
};
