#pragma once
#include "Mesh.h"
#include "Transform.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <vector>

class Shader;

class Model
{
public:
	Model(){}
	Model(std::vector<Mesh> mesh);
	~Model();
	void Draw(Shader* shader);

	bool IsValid() { return valid; }

	// position, rotation and scale
	Transform transform;
private:

	// model data
	std::vector<Mesh> meshes;

	bool valid = false;
	
};

