#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <LinearMath/btMotionState.h>


struct Transform
{
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 rotationDeg = glm::vec3(0);
    glm::vec3 rotationRad = glm::vec3(0);
    glm::vec3 scale = glm::vec3(1);

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;

    Transform() {
        UpdateMatrices();
    }

    void UpdateMatrices();

    glm::mat4 GetModelMatrix() const
    {
        return modelMatrix;
    }

    glm::vec3 GetForwardVector() const 
    {
        return forward;
    }

    glm::vec3 GetRightVector() const
    {
        return right;
    }

    glm::vec3 GetUpVector() const
    {
        return up;
    }

    void SetPosition(const glm::vec3& newPosition) {
        position = newPosition;
        UpdateMatrices();
    }

    // in degrees
    void SetRotation(const glm::vec3& newRotation) {
        rotationDeg = newRotation;
        UpdateMatrices();
    }

    void SetScale(const glm::vec3& newScale) {
        scale = newScale;
        UpdateMatrices();
    }

    void ModifyPosition(const glm::vec3& deltaPosition) {
        position += deltaPosition;
        UpdateMatrices();
    }

    void ModifyRotation(const glm::vec3& deltaRotation) {
        rotationDeg += deltaRotation;
        UpdateMatrices();
    }

    void ModifyScale(const glm::vec3& deltaScale) {
        scale += deltaScale;
        UpdateMatrices();
    }

    void RotateX(float angle) {
        rotationDeg.x += angle;
        UpdateMatrices();
    }

    void RotateY(float angle) {
        rotationDeg.y += angle;
        UpdateMatrices();
    }

    void RotateZ(float angle) {
        rotationDeg.z += angle;
        UpdateMatrices();
    }

    void SetPosition(const btVector3& newPosition)
    {
        position = ConvertBulletToGlm(newPosition);
        UpdateMatrices();
    }

    glm::vec3 ConvertBulletToGlm(const btVector3& vector) const
    {
        return glm::vec3(vector.getX(), vector.getY(), vector.getZ());
    }
};