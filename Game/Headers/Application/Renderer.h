#pragma once
#if defined(__arm__) || defined(__aarch64__)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#else
#include "glad/glad.h"
#endif

#include "Components/Model.h"

#include "Components/Mesh.h"

class Shader;
class Camera;
class ResourceManager;
struct DisplayState;
class Skybox;
class Level;

class Renderer
{
public:
	Renderer(ResourceManager* resorces, Camera* newCam = nullptr);
	~Renderer();
	void Render();

	void ChangeCamera(Camera* newCamera);

	void SetLevel(Level*);

	glm::mat4 GetProjectionViewMatrix();
private:
	void RenderLevel(glm::mat4);

	Camera* cam = nullptr;

	ResourceManager* resources = nullptr;

	Skybox* skybox = nullptr;

	Shader* shader = nullptr;

	Shader* lightShader = nullptr;

	Level* level = nullptr;
};