#include "Renderer.h"
#include "GameApp.h"
#include "glm/ext/matrix_transform.hpp"
#include <memory>
#include "BaseObject.h"

// possible move to game app claas
Renderer::Renderer()
{
	GameApp* game = GameApp::GetInstance();
	WindowManager* windowSystem = game->GetCurrentWindowSystem();
	state = windowSystem->GetDisplayState();
	cam = game->GetCurrentCamera();
	resources = game->GetResources();
	shader = new Shader("../../../Resources/Shaders/default.vert", "../../../Resources/Shaders/default.frag");
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
	// std::shared_ptr<Model> model = resources->GetModel("../../../Resources/Models/Madara_Uchiha.obj");
	BaseObject* model = GameApp::GetInstance()->object;

	model->Update();

	glm::mat4 projectionView = cam->GetProjection() * cam->GetViewMatrix();
	glm::mat4 projection = cam->GetProjection();
	glm::mat4 view = cam->GetViewMatrix();
	 
	glm::mat4 modelMat = model->transform.GetModelMatrix();

	glm::mat4 MVP = projection * view * modelMat;

	shader->use();
	shader->setMat4("MVP", MVP);
	// calculate culling

	// execute culling and draw calls
	model->model->Draw(shader);

	// model->transform.SetPosition(pos + glm::vec3(0.002f, 0, 0));

	glFlush();
	eglSwapBuffers(state->display, state->surface);

}
