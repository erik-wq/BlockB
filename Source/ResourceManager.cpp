#include "ResourceManager.h"

const char* modelsDir = "../../../Resources/Models/";
const char* textureDir = "../../../Resources/Textures/";
const char* ShaderDir = "../../../Resources/Shaders/";

ResourceManager::ResourceManager()
{
	std::string path = std::string(modelsDir);
	path.append("Madara_Uchiha.obj");
	loader = new ModelLoader(this);
	Model mod = loader->LoadModel(path);
	models.insert(std::make_pair(path, std::make_shared<Model>(mod)));
}

ResourceManager::~ResourceManager()
{
	std::unordered_map<std::string, std::shared_ptr<Model>>::iterator itterato = models.begin();
	delete itterato->second.get();
	delete loader;
}

std::shared_ptr<Model> ResourceManager::GetModel(std::string path)
{
	std::unordered_map<std::string, std::shared_ptr<Model>>::iterator loaded = models.find(path);
	if (loaded != models.end())
	{
		return loaded->second;
	}
	// load and add new model
	return std::shared_ptr<Model>();
}

std::shared_ptr<Texture> ResourceManager::GetTexture(std::string path)
{
	std::unordered_map<std::string, std::shared_ptr<Texture>>::iterator loaded = textures.find(path);
	if (loaded != textures.end())
	{
		return loaded->second;
	}
	// load and add new texture
	return std::shared_ptr<Texture>();
}
