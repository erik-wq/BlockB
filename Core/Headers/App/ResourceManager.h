#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include "Components/Mesh.h"

// for loading models
#include "Components/Model.h"
#include <vector>
#include "Event.h"
#include "Animations/Animation.h"

class ModelLoader;
class Shader;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	Model* GetModel(std::string path);

	Shader* GetShader(std::string path);

	Texture GetTexture(std::string path);

	Animation* GetAnimation(std::string path);

	bool IsTextureLoaded(std::string path);

	void LoadTexture(std::string path, std::string type);

	unsigned int LoadCubemap(std::vector<std::string> faces);

	void PushToGPU();

	EventManager<Model*> OnModelLoaded;
private:
	ModelLoader* loader;

	std::unordered_map<std::string, Model*> models;
	
	std::unordered_map<std::string, Texture> textures;

	std::unordered_map<std::string, Shader*> shaders;

	std::unordered_map<std::string,	Animation*> animations;

	std::vector<Texture> loadedTextures;
};