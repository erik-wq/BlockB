#pragma once
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

#include "Components/Model.h"

#include "Components/Mesh.h"

class Shader;
class Camera;
class ResourceManager;
struct DisplayState;
class Skybox;

class Renderer
{
public:
	Renderer(Camera* newCam = nullptr);
	~Renderer();
	void Render();
private:
	DisplayState* state;
	Camera* cam;
	ResourceManager* resources;
	Shader* shader;
	Shader* shaderSuare;
	Skybox* skybox;
};