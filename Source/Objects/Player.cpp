#include "Objects/Player.h"
#include "Objects/Camera.h"
#include "Input.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <btBulletDynamicsCommon.h>

Player::Player(const Transform& trans, btCollisionObject* collider, glm::vec3 colliderOfset, Model* model) : BaseObject(trans, collider, colliderOfset, model)
{
	rigidBody = dynamic_cast<btRigidBody*>(collider);
}

Player::~Player()
{
}

void Player::SetCameraFolow(Camera* cam, glm::vec3 ofset)
{
	if (!cam) return;
	camera = cam;
	originalOfset = ofset;
	cameraOfset = ofset;
	UpdateOfset();
	MoveCamera();
}

void Player::Update(float delta)
{
	BaseObject::Update(delta);
	if (camera)
	{
		UpdateOfset();
		MoveCamera();
	}
}

void Player::HandleInput(Input* Input)
{
	// change to using bullet physics instead
	if (Input->GetKey('w'))
	{
		rigidBody->applyCentralForce(ConverToBullet(transform.GetForwardVector() * 20.f));
		// transform.ModifyPosition(transform.GetForwardVector());
	}
	if (Input->GetKey('s'))
	{
		rigidBody->applyCentralForce(ConverToBullet(-transform.GetForwardVector() * 20.f));
		// transform.ModifyPosition(-transform.GetForwardVector());
	}
	if (Input->GetKey('a'))
	{
		rigidBody->applyCentralForce(ConverToBullet(-transform.GetRightVector() * 20.f));
		// transform.ModifyPosition(-transform.GetRightVector());
	}
	if (Input->GetKey('d'))
	{
		rigidBody->applyCentralForce(ConverToBullet(transform.GetRightVector() * 20.f));
		// transform.ModifyPosition(transform.GetRightVector());
	}
	if (Input->GetKey('e'))
	{
		rigidBody->applyTorque(ConverToBullet(glm::vec3(0, 5, 0)));
		// transform.ModifyRotation(glm::vec3(0, 5 ,0));
	}
	if (Input->GetKey('q'))
	{
		rigidBody->applyTorque(ConverToBullet(glm::vec3(0, -5, 0)));
		// transform.ModifyRotation(glm::vec3(0, -5, 0));
	}
	if (Input->GetKey('f'))
	{
		rigidBody->applyCentralForce(ConverToBullet(glm::vec3(0, 100, 0)));
		// transform.ModifyRotation(glm::vec3(0, -5, 0));
	}
}

void Player::MoveCamera()
{
	camera->FollowObject(transform.position, cameraOfset);
}

void Player::UpdateOfset()
{
	// Apply the same rotation to the offset vector
	glm::vec3 rotation = transform.rotationRad;
	glm::mat4 rotationMatrix = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
	// OR use quaternion
	// glm::mat4 rotationMatrix = glm::toMat4(rotationQuaternion);

	glm::vec4 updatedOffset = rotationMatrix * glm::vec4(originalOfset, 1.0f);

	// Extract the updated offset vector
	cameraOfset = glm::vec3(updatedOffset);
}
