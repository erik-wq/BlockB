#include "Application/Renderer.h"

#include "GameApp.h"

#include "Objects/Player.h"
#include "Objects/Camera.h"
#include "Objects/SkyBox.h"

#include "Components/Shader.h"

#include "WindowManager.h"

#include "Application/ResourceManager.h"

#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <memory>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"

// possible move to game app claas
Renderer::Renderer(Camera* newCam) : cam(newCam)
{
	GameApp* game = GameApp::GetInstance();
	WindowManager* windowSystem = game->GetCurrentWindowSystem();
	state = windowSystem->GetDisplayState();
	resources = game->GetResources();
	shader = new Shader("../../../Resources/Shaders/default.vert", "../../../Resources/Shaders/default.frag");
	skybox = new Skybox();
}

Renderer::~Renderer()
{
	delete shader;
	delete skybox;
}

void Renderer::Render()
{
	if (!cam)
	{
		// remove if never triggered, now for safety
		return;
	}

	// get all models for rendering in level
	// create level which wiil manage all objects for rendering
	// td::shared_ptr<Model> model = resources->GetModel("../../../Resources/Models/Madara_Uchiha.obj");
	Player* model = GameApp::GetInstance()->object;

	glm::mat4 projectionView = cam->GetProjection() * cam->GetViewMatrix();
	glm::mat4 projection = cam->GetProjection();
	glm::mat4 view = cam->GetViewMatrix();

	glm::mat4 modelMat = model->transform.GetModelMatrix();

	glm::mat4 MVP = projection * view * modelMat;

	shader->use();
	shader->setMat4("MVP", MVP);

	// execute culling and draw calls
	model->GetModel()->Draw(shader);

	// render skybox
	glm::mat4 camTranslation = glm::mat4(1);
	camTranslation = glm::translate(camTranslation, cam->Position);
	skybox->Render(projection * view * camTranslation);;

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glFlush();
	eglSwapBuffers(state->display, state->surface);

}
