#pragma once
#include "BaseObject.h"
#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"

class Input;
class Camera;

class Player : public BaseObject
{
public:
	Player(const Transform& trans = Transform(), Model* model = nullptr, btCollisionShape* shape = nullptr, btRigidBody* rigidbody = nullptr, Camera* cam = nullptr);
	virtual ~Player();

	virtual void Update(float) override;

	virtual void HandleInput(Input*) override;
private:
	void MoveCamera();

	void UpdateOfset();

	Camera* camera;

	glm::vec3 cameraOfset;
	glm::vec3 originalOfset;
};