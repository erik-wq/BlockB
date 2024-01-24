#include "Application/Renderer.h"

#include "Objects/Camera.h"
#include "Objects/SkyBox.h"
#include "Objects/BaseObject.h"

#include "Components/Shader.h"

#include "App/ResourceManager.h"

#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <memory>

#if defined(__arm__) || defined(__aarch64__)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#else
#include "glad/glad.h"
#endif

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "Levels/BaseLevel.h"
#include "Levels/LightsLevel.h"
#include <vector>
#include "Objects/Player.h"

// possible move to game app claas
Renderer::Renderer(ResourceManager* resorces, Camera* newCam) : cam(newCam)
{
	std::vector<std::string> faces =
	{
		"right.jpg","left.jpg","top.jpg","bottom.jpg","front.jpg","back.jpg"
	};
	GLuint cubeMap = resorces->LoadCubemap(faces);

	resources = resorces;

	// lightShader = resources->GetShader("LightCube");
	// shader = resources->GetShader("Lights");
	// shader = resources->GetShader("default");
	shader = resources->GetShader("Skeletal");
	Shader* skyboxShader = resources->GetShader("skybox");

	skybox = new Skybox(cubeMap, skyboxShader);
}

Renderer::~Renderer()
{
	delete skybox;
}

void Renderer::Render()
{
	if (!cam)
	{
		// remove if never triggered, now for safety
		ImGui::EndFrame();
		return;
	}

	glm::mat4 projection = cam->GetProjection();
	glm::mat4 view = cam->GetViewMatrix();

	if (level)
	{
		RenderLevel(projection * view);
	}

	if (level->RenderSkybox())
	{
		glm::mat4 camTranslation = glm::mat4(1);
		camTranslation = glm::translate(camTranslation, cam->Position);
		skybox->Render(projection * view * camTranslation);;
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::ChangeCamera(Camera* newCamera)
{
	if (newCamera)
	{
		cam = newCamera;
	}
}

void Renderer::SetLevel(Level* newLevel)
{
	level = newLevel;
	shader->use();
	level->SetLevelLights(shader);
	shader->setFloat("material.shininess", 32.0f);
}

glm::mat4 Renderer::GetProjectionViewMatrix()
{
	if (!cam) return glm::mat4(1);
	return cam->GetProjection() * cam->GetViewMatrix();
}

void Renderer::RenderLevel(glm::mat4 projectionView)
{
	shader->use();
	shader->setMat4("projectionView", projectionView);
	// shader->setVec3("viewPos", cam->Position);

	// loop over level dependecies
	for (std::string modelPath : level->GetDependencies())
	{
		if (Model* model = resources->GetModel(modelPath))
		{ 
			model->Draw(shader);
		}
	}
}
