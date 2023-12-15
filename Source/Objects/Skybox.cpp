#include "Objects/SkyBox.h"

#include "GameApp.h"
#include "Components/Shader.h"
#include "Application/ResourceManager.h"

#include <vector>
#include <string>

float skyboxVertices[324] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

Skybox::Skybox()
{
	shader = new Shader("../../../Resources/Shaders/skybox.vert", "../../../Resources/Shaders/skybox.frag");
    shader->use();
    glUniform1i(glGetUniformLocation(shader->ID, "skybox"), 0);
    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	LoadSkybox();
}

Skybox::~Skybox()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VBO);
	delete shader;
}

void Skybox::Render(glm::mat4 projectionView)
{

	shader->use();
	shader->setMat4("Matrix", projectionView);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

    glDepthMask(GL_TRUE);

}

void Skybox::LoadSkybox()
{
	std::vector<std::string> faces =
	{
		"right.jpg","left.jpg","top.jpg","bottom.jpg","front.jpg","back.jpg"
	};

	GameApp* instance = GameApp::GetInstance();
	texture = instance->GetResources()->LoadCubemap(faces);
}
