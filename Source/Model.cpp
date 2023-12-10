#include "Model.h"
#include <iostream>
#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Model::Model(std::vector<Mesh> mesh)
{
	meshes = mesh;
}

Model::~Model()
{

}

void Model::Draw(Shader* shader)
{
	for (auto& mesh : meshes)
		mesh.Draw(shader);
}
