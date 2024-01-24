#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

class Model;
class ResourceManager;
class Mesh;
struct Texture;
struct Vertex;

class ModelLoader
{
public:
	ModelLoader(ResourceManager* resources);

	Model* LoadModel(std::string path);
	unsigned int LoadCubemap(std::vector<std::string> faces);

	void GenerateTexture(Texture& text);

	unsigned char* TextureFromFile(const std::string& path, Texture& text);

private:
	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene, Model* model);

	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

	void SetVertexBoneDataToDefault(Vertex& vertex);
	std::vector<Mesh*> ProcessNode(aiNode* node, const aiScene* scene, Model* model, bool Skeletal);

	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, Model* model, bool skeletal);

	ResourceManager* resourceManager;
};
