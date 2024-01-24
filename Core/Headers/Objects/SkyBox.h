#pragma once

#if defined(__arm__) || defined(__aarch64__)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#else
#include "glad/glad.h"
#endif

#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"

class Shader;

class Skybox
{
public:
	Skybox(GLuint cubeMap, Shader* shader);
	~Skybox();
    void Render(glm::mat4 projectionView);
private:
    GLuint texture;
    Shader* shader;

    GLuint VAO, VBO;
};