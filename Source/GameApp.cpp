#include "GameApp.h"
#include "Input.h"
#include "ResourceManager.h"
#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ModelLoader.h"
#include "Shader.h"
#include "World.h"

GameApp* GameApp::instance = nullptr;

GameApp::GameApp()
{
	windowManager = new WindowManager(800, 600);

	input = new Input();
	DisplayState* state = windowManager->GetDisplayState();
	input->root = windowManager->GetRootWindow();
	input->Init(windowManager->GetCurrentDisplay(), windowManager->GetRootWindow());
	resources = new ResourceManager();
	mainCam = new Camera(glm::vec3(0,2,10));
	mainCam->screenHeight = state->height;
	mainCam->screenWidth = state->width;
	worldInstance = new BulletPhysicsWorld();

	// create base class for objects in scene -> transform, model , collision object

	shape = new btCapsuleShape(2,1);
	// shape = new btSphereShape(1);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(0,6,0));
	transform.setRotation(btQuaternion(0,0,0,1));

	worldInstance->AddCollisionObject(shape, transform, 1);

	Cubeshape = new btBoxShape(btVector3(10,2,10));

	btTransform secondtransform;
	secondtransform.setIdentity();
	secondtransform.setOrigin(btVector3(0, -2, 0));
	secondtransform.setRotation(btQuaternion(0, 0, 0, 1));

	worldInstance->AddCollisionObject(Cubeshape, secondtransform, 0);
}

GameApp::~GameApp()
{
	delete windowManager;
	delete input;
	delete resources;
	delete mainCam;
	delete worldInstance;
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
	worldInstance->Update(mainCam->GetProjection() * mainCam->GetViewMatrix(), delta);
	MouseData data = input->GetMouseData();
	mainCam->ProcessInput(data, input, delta);
}
