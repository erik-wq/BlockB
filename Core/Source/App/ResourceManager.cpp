#include "App/ResourceManager.h"
#include "App/ModelLoader.h"

#if defined(__arm__) || defined(__aarch64__)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#else
#include "glad/glad.h"
#endif

#include "stb_image.h"
#include "Components/Shader.h"
#include "AssimpHelper.h"

#if defined(__arm__) || defined(__aarch64__)
const char* modelsDir = "../../../Game/Resources/Models/";
const char* textureDir = "../../../Game/Resources/Textures/";
const char* ShaderDir = "../../../Game/Resources/Shaders/Rasberry/";
#else
const char* modelsDir = "../Game/Resources/Models/";
const char* textureDir = "../Game/Resources/Textures/";
const char* ShaderDir = "../Game/Resources/Shaders/Windows/";
#endif

ResourceManager::ResourceManager()
{
	loader = new ModelLoader(this);
}

ResourceManager::~ResourceManager()
{
	for (std::unordered_map<std::string, Model*>::iterator it = models.begin(); it != models.end(); ++it)
	{
		delete it->second;
	}
	models.clear();
	for (std::unordered_map<std::string, Shader*>::iterator it = shaders.begin(); it != shaders.end(); ++it)
	{
		delete it->second;
	}
	shaders.clear();

	for (std::unordered_map<std::string, Animation*>::iterator it = animations.begin(); it != animations.end(); ++it)
	{
		delete it->second;
	}
	animations.clear();

	delete loader;
}

Model* ResourceManager::GetModel(std::string path)
{
	std::unordered_map<std::string, Model*>::iterator loaded = models.find(path);
	if (loaded != models.end())
	{
		return loaded->second;
	}

	// load and add new model
	std::string fullPath = std::string(modelsDir);
	fullPath.append(path);
	Model* model = loader->LoadModel(fullPath);

	if (!model)
	{
		return nullptr;
	}

	if (AssimpHelper::IsSkeletalModel(path))
	{
		Animation* animation = new Animation(fullPath, model);
		animations.insert(std::make_pair(path, animation));
	}

	model->SetPath(path);
	OnModelLoaded.triggerEvent(model);
	models.insert(std::make_pair(path, model));
	return models.find(path)->second;
}

Shader* ResourceManager::GetShader(std::string path)
{
	std::unordered_map<std::string, Shader*>::iterator loaded = shaders.find(path);
	if (loaded != shaders.end())
	{
		return loaded->second;
	}

	// load and add new texture
	std::string fullPath = std::string(ShaderDir);
	fullPath.append(path);
	std::string frag = fullPath.c_str();
	std::string vert = fullPath.c_str();

	frag.append(".frag");
	vert.append(".vert");

	Shader* shader = new Shader(vert.c_str(), frag.c_str());
	shaders.insert(std::make_pair(path, shader));
	return shaders.find(path)->second;
}

Texture ResourceManager::GetTexture(std::string path)
{
	std::unordered_map<std::string, Texture>::iterator loaded = textures.find(path);
	if (loaded != textures.end())
	{
		return loaded->second;
	}
	// search loaded textures vector based on path
	for (std::vector<Texture>::iterator it = loadedTextures.begin(); it != loadedTextures.end(); ++it)
	{
		if (it->path == path)
		{
			return *it;
		}
	}

	return Texture();
}

Animation* ResourceManager::GetAnimation(std::string path)
{
	std::unordered_map<std::string, Animation*>::iterator loaded = animations.find(path);
	if (loaded != animations.end())
	{
		return loaded->second;
	}
	return nullptr;
}

bool ResourceManager::IsTextureLoaded(std::string path)
{
	// search loaded textures
	for (std::vector<Texture>::iterator it = loadedTextures.begin(); it != loadedTextures.end(); ++it)
	{
		if (it->path == path)
		{
			return true;
		}
	}

	std::unordered_map<std::string, Texture>::iterator loaded = textures.find(path);
	if (loaded != textures.end())
	{
		return true;
	}
	return false;
}

void ResourceManager::LoadTexture(std::string path, std::string type)
{
	// load and add new texture
	std::string fullPath = std::string(textureDir);
	fullPath.append(path);
	Texture texture;
	texture.path = path;
	texture.type = type;
	loader->TextureFromFile(fullPath, texture);
	if (texture.data)
	{
		loadedTextures.push_back(texture);
	}
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

void ResourceManager::PushToGPU()
{
	// itarate over all loaded textures and push them to gpu
	for (std::vector<Texture>::iterator it = loadedTextures.begin(); it != loadedTextures.end(); ++it)
	{
		Texture texture = *it;
		loader->GenerateTexture(texture);
		textures.insert(std::make_pair(texture.path, texture));
	}

	loadedTextures.clear();

	// itarate over all loaded models and push them to gpu
	for (std::unordered_map<std::string, Model*>::iterator it = models.begin(); it != models.end(); ++it)
	{
		Model* model = it->second;
		model->PushToGPU(this);
	}

}
