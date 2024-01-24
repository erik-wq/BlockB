#pragma once
#include "Objects/BaseObject.h"
#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <functional>

class InputSystem;
class Camera;
class btRigidBody;
class Animator;
class Animation;
class Shader;
class ResourceManager;

const float StartSpeed = 10;

class Player : public BaseObject
{
public:
	Player(const Transform& trans = Transform(), btRigidBody* collider = nullptr, glm::vec3 colliderOfset = glm::vec3(0), Model* model = nullptr, Layer objectLayer = NO);
	virtual ~Player();

	void SetCameraFolow(Camera* cam, glm::vec3 ofset);

	virtual void Update(float) override;

	void HandleInput(InputSystem*);

	virtual void BindInput(InputSystem*) override;
	virtual void UnBindInput(InputSystem*) override;

	virtual void OnCollisionEnter(BaseObject*) override;
	virtual void OnCollisionExit(BaseObject*) override;

	virtual void OnOverlapEnter(BaseObject*) override;

	virtual void GetResources(ResourceManager*) override;

	void SetSpeed(float newSpeed) { speed = newSpeed; }

	void modifySpeed(float delta) { speed += delta; }

	void UpdateAnimation(Shader* shader);
private:
	void MoveCamera();

	void UpdateOfset();

	std::function<void(InputSystem*)> inputHandle;  

	Camera* camera = nullptr;

	float speed = StartSpeed;

	bool onGround = false;

	// rigidbody is just casted collisionObject from BaseObject
	btRigidBody* rigidBody = nullptr;
	glm::vec3 cameraOfset = glm::vec3(0);
	glm::vec3 originalOfset = glm::vec3(0);

	Animation* anim;
	Animator* animator = nullptr;
};