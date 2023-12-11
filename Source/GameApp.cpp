#include "GameApp.h"
#include "Input.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ModelLoader.h"
#include "Shader.h"
#include "World.h"
#include "BaseObject.h"
#include "CustomMotionState.h"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>


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

	object = new BaseObject();
	object->model = resources->GetModel("../../../Resources/Models/Madara_Uchiha.obj").get();


	btCollisionShape* capsule = new btCapsuleShape(2,1);
	// shape = new btSphereShape(1);

	btTransform transform;
	transform.setIdentity();
	// changing this value break moving of object
	transform.setOrigin(btVector3(1,6,0));
	transform.setRotation(btQuaternion(0,0,0,1));

	btVector3 localInertia = btVector3(0, 0, 0);
	capsule->calculateLocalInertia(1, localInertia);
	btMotionState* motionState = new CustomMotionState(transform, object);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(1, motionState, capsule, localInertia);
	btRigidBody* rigidBody = new btRigidBody(rbInfo);

	worldInstance->AddRigidBody(rigidBody);
	
	object->rigidBody = rigidBody;
	object->shape = capsule;

	// floor
	shape = new btBoxShape(btVector3(10,2,10));

	btTransform secondtransform;
	secondtransform.setIdentity();
	secondtransform.setOrigin(btVector3(0, -2, 0));
	secondtransform.setRotation(btQuaternion(0, 0, 0, 1));

	worldInstance->AddCollisionObject(shape, secondtransform, 0);
}

GameApp::~GameApp()
{
	delete windowManager;
	delete input;
	delete resources;
	delete mainCam;
	delete worldInstance;
	delete object;
}

GameApp* GameApp::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new GameApp();
	}
	return instance;
}

void GameApp::Tick(float delta)
{
	// object->Update();
	worldInstance->Update(mainCam->GetProjection() * mainCam->GetViewMatrix(), delta);
	MouseData data = input->GetMouseData();
	mainCam->ProcessInput(data, input, delta);
}
