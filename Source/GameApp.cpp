#include "GameApp.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "Physics/World.h"
#include "Physics/CustomMotionState.h"

#include "Input.h"

#include "Components/Transform.h"

#include "Application/ResourceManager.h"
#include "Application/Renderer.h"

#include "Objects/BaseObject.h"
#include "Objects/Player.h"
#include "Objects/Camera.h"


GameApp* GameApp::instance = nullptr;

GameApp::GameApp()
{
	windowManager = new WindowManager(800, 600);

	input = new Input();
	DisplayState* state = windowManager->GetDisplayState();
	input->root = windowManager->GetRootWindow();
	input->Init(windowManager->GetCurrentDisplay(), windowManager->GetRootWindow());
	resources = new ResourceManager();
	mainCam = new Camera(glm::vec3(1,3,15));
	mainCam->screenHeight = state->height;
	mainCam->screenWidth = state->width;
	worldInstance = new BulletPhysicsWorld();

	// create base class for objects in scene -> transform, model , collision object

	btCollisionShape* capsule = new btCapsuleShape(2,1);
	// shape = new btSphereShape(1);


	btTransform transform;
	transform.setIdentity();
	// changing this value break moving of object
	transform.setOrigin(btVector3(1,6,0));
	transform.setRotation(btQuaternion(0,0,0,1));

	btVector3 localInertia = btVector3(0, 0, 0);
	capsule->calculateLocalInertia(1, localInertia);
	btMotionState* motionState = new CustomMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(1, motionState, capsule, localInertia);
	btRigidBody* rigidBody = new btRigidBody(rbInfo);
	// disable deactivation for player for player
	rigidBody->setActivationState(DISABLE_DEACTIVATION);

	btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(*rigidBody, transform, false);
	bool bLimitAngularMotion = true;
	if (bLimitAngularMotion) {
		dof6->setAngularLowerLimit(btVector3(0, -SIMD_INFINITY, 0));
		dof6->setAngularUpperLimit(btVector3(0, SIMD_INFINITY, 0));
	}

	dof6->setLinearLowerLimit(btVector3(-SIMD_INFINITY, -SIMD_INFINITY, -SIMD_INFINITY));
	dof6->setLinearUpperLimit(btVector3(SIMD_INFINITY, SIMD_INFINITY, SIMD_INFINITY));


	// add the constraint to the world
	worldInstance->dynamicsWorld->addConstraint(dof6, true);

	worldInstance->AddRigidBody(rigidBody);

	Model* model = resources->GetModel("Madara_Uchiha.obj");
	
	object = new Player(Transform(),
		model,
		capsule,
		rigidBody,
		mainCam
	);

	// floor
	shape = new btBoxShape(btVector3(10,2,10));

	btTransform secondtransform;
	secondtransform.setIdentity();
	secondtransform.setOrigin(btVector3(0, -2, 0));
	secondtransform.setRotation(btQuaternion(0, 0, 0, 1));

	worldInstance->AddCollisionObject(shape, secondtransform, 0);

}

void GameApp::Initialize()
{
	renderer = new Renderer(mainCam);
}

GameApp::~GameApp()
{
	delete windowManager;
	delete input;
	delete resources;
	delete mainCam;
	delete worldInstance;
	delete object;
	delete renderer;
}

GameApp* GameApp::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new GameApp();
		instance->Initialize();
	}
	return instance;
}

void GameApp::Tick(float delta)
{
	object->HandleInput(input);
	worldInstance->Update(mainCam->GetProjection() * mainCam->GetViewMatrix(), delta);
	MouseData data = input->GetMouseData();
	object->Update(delta);
	worldInstance->Render(mainCam->GetProjection() * mainCam->GetViewMatrix());
}

void GameApp::Render()
{
	renderer->Render();
}
