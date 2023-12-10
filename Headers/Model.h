#pragma once
#include <string>
#include <vector>
#include "Mesh.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "Transform.h"

class Model
{
public:
	Model() {};
	Model(std::vector<Mesh> mesh);
	~Model();
	void Draw(Shader* shader);

	// position, rotation and scale
	Transform transform;
private:

	// model data
	std::vector<Mesh> meshes;

	// Shader* shader;
	
};

