#pragma once
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include "WindowManager.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Model.h"

class Shader;

class Renderer
{
public:
	Renderer();
	void Render();
	void SetCamera(Camera* newCam) { cam = newCam; }
private:
	DisplayState* state;
	Camera* cam;
	ResourceManager* resources;
	Model model;
	Shader* shader;
};