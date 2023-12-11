#pragma once
#include <LinearMath/btMotionState.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include "BaseObject.h"

class CustomMotionState : public btDefaultMotionState {
public:
    CustomMotionState(const btTransform& initialTransform, BaseObject* owner)
        : btDefaultMotionState(initialTransform), owner(owner) {}

private:
    BaseObject* owner; 
};
