#include "Components/Model.h"
#include "Components/Shader.h"
#include "Objects/BaseObject.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Objects/Player.h"

Model::Model(std::vector<Mesh*> mesh)
{
	meshes = mesh;
}

Model::~Model()
{

}

void Model::Draw(Shader* shader)
{
	if (instances.size() == 0)
	{
		return;
	}

	if (Player* player = (Player*)(instances[0]))
	{
		player->UpdateAnimation(shader);
		shader->setMat4("model", player->transform.GetModelMatrix());
	}

	for (Mesh* mesh : meshes)
	{
		mesh->BindVAO();
		mesh->BindTextures(shader);
		mesh->Draw(shader);
		mesh->UnBindTextures();
	}
}

void Model::DrawInstanced(Shader* shader)
{
	if (instances.size() == 0)
	{
		return;
	}

	UpdateMatricies();
	UpdateInstancesBuffer();

	if (Player* player = (Player*)(instances[0]))
	{
		player->UpdateAnimation(shader);
	}

	for (Mesh* mesh : meshes)
	{
		mesh->BindVAO();
		mesh->BindTextures(shader);
		mesh->DrawInstanced(static_cast<unsigned int>(instancesMatrices.size()));
		mesh->UnBindTextures();
	}
}

void Model::PushToGPU(ResourceManager* resources)
{
	// glGenBuffers(1, &instancesBuffer);
	for (Mesh* mesh : meshes)
	{
		mesh->PushToGPU(resources);
		mesh->BindVAO();

		/*
		glBindBuffer(GL_ARRAY_BUFFER, instancesBuffer);
		
		// creating a buffer for mat4
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		*/
	}
}

std::vector<glm::vec3> Model::GetAllVertices()
{
	std::vector<glm::vec3> allPostiions;
	// loop through all meshes and get all vertices
	for (Mesh* mesh : meshes)
	{
		std::vector<glm::vec3> vertices = mesh->GetVertices();
		allPostiions.insert(allPostiions.end(), vertices.begin(), vertices.end());
	}
	return allPostiions;
}

void Model::AddInstance(BaseObject* object)
{
	instances.push_back(object);
	instancesMatrices.push_back(object->transform.GetModelMatrix());
	UpdateInstancesBuffer();
}

void Model::RemoveInstance(BaseObject* object)
{
	for (auto it = instances.begin(); it != instances.end();) {
		if (*it == object) {
			it = instances.erase(it);
			break;
		}
		else {
			++it;
		}
	}

	instancesMatrices.pop_back();
	UpdateMatricies();

	if (instances.size() != 0)
	{
		UpdateInstancesBuffer();
	}
}

void Model::UpdateInstancesBuffer()
{
	for (Mesh* mesh : meshes)
	{
		mesh->BindVAO();

		glBindBuffer(GL_ARRAY_BUFFER, instancesBuffer);
		glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(glm::mat4), &instancesMatrices[0], GL_STATIC_DRAW);
		glBindVertexArray(0);
	}
}

void Model::UpdateMatricies()
{
	for (int i = 0; i < (int)instances.size(); i++)
	{
		instancesMatrices[i] = instances[i]->transform.GetModelMatrix();
	}
}
