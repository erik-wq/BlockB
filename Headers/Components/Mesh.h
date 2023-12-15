#pragma once

#include "glm/common.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id = -1;
	std::string type;
	std::string path;
};
class Shader;

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		std::vector<Texture> textures);
	void Draw(Shader* shader);
protected:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
private:
	// render data
	unsigned int VAO, VBO, EBO;
	void setupMesh();

};

