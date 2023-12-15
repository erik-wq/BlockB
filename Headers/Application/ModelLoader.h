#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

class Model;
class ResourceManager;
class Mesh;
class Texture;

class ModelLoader
{
public:
	ModelLoader(ResourceManager* resources);

	Model LoadModel(std::string path);

	Texture LoadTexture(std::string path, std::string typeName);

	unsigned int LoadCubemap(std::vector<std::string> faces);

private:
	std::vector<Mesh> ProcessNode(aiNode* node, const aiScene* scene);

	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	unsigned int TextureFromFile(const std::string& path);

	ResourceManager* resourceManager;
};
