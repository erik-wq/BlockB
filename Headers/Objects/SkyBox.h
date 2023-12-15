#pragma once

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"

class Shader;

class Skybox
{
public:
	Skybox();
	~Skybox();
    void Render(glm::mat4 projectionView);
private:
	void LoadSkybox();
    GLuint texture;
    Shader* shader;

    GLuint VAO, VBO;
};