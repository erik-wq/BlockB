#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

// const char* ModelFolder = "./";

class Model;
class ResourceManager;
class Mesh;
class Texture;

class ModelLoader
{
public:
	ModelLoader(ResourceManager* resources);

	Model LoadModel(std::string path);
private:
	std::vector<Mesh> ProcessNode(aiNode* node, const aiScene* scene);

	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	unsigned int TextureFromFile(const char* path, const std::string& directory);

	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	ResourceManager* resourceManager;

	std::vector<Texture> textures_loaded;
};
