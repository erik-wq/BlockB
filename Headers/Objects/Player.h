#pragma once
#include "BaseObject.h"
#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"

class Input;
class Camera;

class Player : public BaseObject
{
public:
	Player(const Transform& trans = Transform(), btCollisionObject* collider = nullptr, glm::vec3 colliderOfset = glm::vec3(0), Model* model = nullptr);
	virtual ~Player();

	void SetCameraFolow(Camera* cam, glm::vec3 ofset);

	virtual void Update(float) override;

	virtual void HandleInput(Input*) override;
private:
	void MoveCamera();

	void UpdateOfset();

	Camera* camera;

	// rigidbody is just casted collisionObject from BaseObject
	btRigidBody* rigidBody;

	glm::vec3 cameraOfset;
	glm::vec3 originalOfset;
};