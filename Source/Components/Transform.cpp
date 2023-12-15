#include "Components/Transform.h"

void Transform::UpdateMatrices()
{
    rotationRad = glm::radians(rotationDeg);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationRad.x, glm::vec3(1.0f, 0.0f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), rotationRad.y, glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::rotate(glm::mat4(1.0f), rotationRad.z, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);

    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    glm::vec3 front;
    front.x = cos(rotationRad.x) * sin(rotationRad.y);
    front.y = -sin(rotationRad.x);
    front.z = cos(rotationRad.x) * cos(rotationRad.y);
    forward = glm::normalize(front);

    right = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));;
    up = glm::normalize(glm::cross(right, front));
}
