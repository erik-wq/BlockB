#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <functional>

struct InputEvent;
class InputSystem;
struct MouseData;

enum Camera_Movement {
    NONE,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    bool folow = true;
    glm::mat4 viewMatrix;

    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    int screenHeight = 0;
    int screenWidth = 0;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() const
    {
        return viewMatrix;
    }

    // change widht and screen to updated resize event
    glm::mat4 GetProjection() const
    {
        return glm::perspective(glm::radians(Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    }
    void ProcessMouseMovement(float xoffset, float yoffset/*, GLboolean constrainPitch = true*/);

    void HandleInput(InputSystem*);

    void BindInput(InputSystem*);
    void UnBindInput(InputSystem*);

    void FollowObject(glm::vec3 objectPos, glm::vec3 ofset);
private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors();

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

    std::function<void(InputSystem*)> inputHandle;
};