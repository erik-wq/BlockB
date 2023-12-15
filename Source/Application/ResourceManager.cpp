#include "Application/ResourceManager.h"
#include "Application/ModelLoader.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

#include "stb_image.h"

const char* modelsDir = "../../../Resources/Models/";
const char* textureDir = "../../../Resources/Textures/";
const char* ShaderDir = "../../../Resources/Shaders/";

ResourceManager::ResourceManager()
{
	loader = new ModelLoader(this);
}

ResourceManager::~ResourceManager()
{
	// free all resources
	delete loader;
}

Model* ResourceManager::GetModel(std::string path)
{
	std::unordered_map<std::string, Model>::iterator loaded = models.find(path);
	if (loaded != models.end())
	{
		return &loaded->second;
	}

	// load and add new model
	std::string fullPath = std::string(modelsDir);
	fullPath.append(path);
	Model model = loader->LoadModel(fullPath);

	if (!model.IsValid())
	{
		return nullptr;
	}

	models.insert(std::make_pair(path, model));
	return &models.find(path)->second;
}

Texture ResourceManager::GetTexture(std::string path, std::string type)
{
	std::unordered_map<std::string, Texture>::iterator loaded = textures.find(path);
	if (loaded != textures.end())
	{
		return loaded->second;
	}
	// load and add new texture
	std::string fullPath = std::string(textureDir);
	fullPath.append(path);
	Texture texture = loader->LoadTexture(fullPath, type);
	if (!texture.id == -1)
	{
		printf("missing texture %s\n", path);
		return Texture();
	}

	textures.insert(std::make_pair(path, texture));
	return textures.find(path)->second;
}

unsigned int ResourceManager::LoadCubemap(std::vector<std::string> faces)
{
	std::vector<std::string> paths;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		std::string fullpath = std::string(textureDir);
		fullpath.append(faces[i]);
		paths.push_back(fullpath);
	}

	return loader->LoadCubemap(paths);
}
