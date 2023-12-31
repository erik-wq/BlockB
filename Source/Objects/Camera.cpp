#include "Objects/Camera.h"
#include "Input.h"

#include <cmath>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

// constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = 0;
	Pitch = pitch;
	UpdateCameraVectors();
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
	if(direction == DOWN)
		Position -= Up * velocity;
	if (direction == UP)
		Position += Up * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset/*, GLboolean constrainPitch = true*/)
{
	if (folow) return;
	xoffset *= MouseSensitivity;
	yoffset *= -MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (Yaw >= 360)
	{
		Yaw -= 360;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	UpdateCameraVectors();
}

void Camera::ProcessInput(const MouseData& data, Input* input, float delta)
{
	if (folow) return;
	if (input->GetKey('a'))
	{
		ProcessKeyboard(LEFT, delta);
	}
	if (input->GetKey('d'))
	{
		ProcessKeyboard(RIGHT, delta);
	}
	if (input->GetKey('w'))
	{
		ProcessKeyboard(FORWARD, delta);
	}
	if (input->GetKey('s'))
	{
		ProcessKeyboard(BACKWARD, delta);
	}
}

void Camera::FollowObject(glm::vec3 objectPos, glm::vec3 ofset)
{
	Position = objectPos + ofset;

	viewMatrix = glm::lookAt(Position, objectPos, Up);

	glm::vec3 direction = glm::normalize(-ofset);

	Yaw = glm::degrees(glm::atan2(direction.z, direction.x));
	Pitch = glm::degrees(glm::asin(direction.y));

	UpdateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::UpdateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
	viewMatrix = glm::lookAt(Position, Position + Front, Up);
}