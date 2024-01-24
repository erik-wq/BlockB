#pragma once

#include "glm/common.hpp"
#include "glm/gtc/matrix_transform.hpp"
#if defined(__arm__) || defined(__aarch64__)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#else
#include "glad/glad.h"
#endif

#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

	int boneIDs[MAX_BONE_INFLUENCE];
	float weights[MAX_BONE_INFLUENCE];
};

struct Texture {
	int id = -1;
	std::string type;
	std::string path;

	int width, height, nrComponents;

	unsigned char* data = nullptr;
};
class Shader;
class ResourceManager;

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		std::vector<Texture> textures);
	void Draw(Shader* shader);
	virtual void PushToGPU(ResourceManager* resources);

	void BindTextures(Shader* shader);

	void UnBindTextures();

	void DrawInstanced(unsigned int size);

	void BindVAO();

	std::vector<glm::vec3> GetVertices();
protected:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// render data
	unsigned int VAO, VBO, EBO, InstanceBuffer;
	virtual void SetupMesh();

};

