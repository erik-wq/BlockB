#include "Objects/Player.h"
#include "Objects/Camera.h"
#include "App/InputSystem.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <btBulletDynamicsCommon.h>
#include "Animations/Animator.h"
#include "Animations/Animation.h"
#include "Components/Shader.h"
#include "App/ResourceManager.h"

Player::Player(const Transform& trans, btRigidBody* collider, glm::vec3 colliderOfset, Model* model, Layer objectLayer) : BaseObject(trans, collider, colliderOfset, model, objectLayer)
{
	rigidBody = collider;
}

Player::~Player()
{
	delete animator;
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
	animator->UpdateAnimation(delta);
	return;
	if (camera)
	{
		UpdateOfset();
		MoveCamera();
	}

	btVector3 velocity = rigidBody->getLinearVelocity();
	velocity.setZ(speed);
	rigidBody->setLinearVelocity(velocity);
	transform.UpdateMatrices();
}

void Player::HandleInput(InputSystem* Input)
{
	return;
	btVector3 velocity = rigidBody->getLinearVelocity();
	if (Input->GetKey('a'))
	{
		float x = transform.GetRightVector().x * -20.f;
		velocity.setX(x);
	}
	if (Input->GetKey('d'))
	{
		float x = transform.GetRightVector().x * 20.f;
		velocity.setX(x);
	}
	if (!Input->GetKey('d') && !Input->GetKey('a'))
	{
		velocity.setX(0);
	}

	rigidBody->setLinearVelocity(velocity);

	if (Input->GetKey('w') && onGround)
	{
		rigidBody->applyCentralForce(ConverToBullet(transform.GetUpVector() * 150.0f));
	}
}

void Player::BindInput(InputSystem* input)
{
	inputHandle = std::bind(&Player::HandleInput, this, std::placeholders::_1);
	input->inputEvent.subscribe(inputHandle);
}

void Player::UnBindInput(InputSystem* input)
{
	input->inputEvent.unsubscribe(inputHandle);
}

void Player::OnCollisionEnter(BaseObject* object)
{
	if (object->GetLayer() == GROUND)
	{
		onGround = true;
	}
}

void Player::OnCollisionExit(BaseObject* object)
{
	if (object->GetLayer() == GROUND)
	{
		onGround = false;
	}
}

void Player::OnOverlapEnter(BaseObject* object)
{
	if (object->GetLayer() == OBSTICLE)
	{
		std::cout << "game over\n";
		// stop level and show game over screen
	}
}

void Player::GetResources(ResourceManager* resource)
{
	if (!model) return;
	anim = resource->GetAnimation(model->GetPath());// new Animation("../../../Game/Resources/Models/Running.dae", model);
	animator = new Animator(anim);
}

void Player::UpdateAnimation(Shader* shader)
{
	std::vector<glm::mat4> transforms = animator->GetFinalBoneMatrices();
	for (int i = 0; i < (int)transforms.size(); ++i)
		shader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
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
	glm::vec4 updatedOffset = rotationMatrix * glm::vec4(originalOfset, 1.0f);

	// Extract the updated offset vector
	cameraOfset = glm::vec3(updatedOffset);
}
