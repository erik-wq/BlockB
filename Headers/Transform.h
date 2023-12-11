#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Transform
{
public:
    glm::vec3 position = glm::vec3(0, 0, 0);
    // rotation sroed in degrees
    glm::vec3 rotationDeg = glm::vec3(0);
    glm::vec3 rotationRad = glm::vec3(0);
    glm::vec3 scale = glm::vec3(1);

    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    glm::mat4 scaleMatrix = glm::mat4(1.0f);

    glm::mat4 translationMatrix = glm::mat4(1.0f);
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    Transform() {
        UpdateMatrices();
    }

    void UpdateMatrices() {
        translationMatrix = glm::translate(glm::mat4(1.0f), position);

        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    }

    glm::mat4 GetModelMatrix()
    {
        return modelMatrix;
    }

    void SetPosition(const glm::vec3& newPosition) {
        position = newPosition;
        UpdateMatrices();
    }

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
};