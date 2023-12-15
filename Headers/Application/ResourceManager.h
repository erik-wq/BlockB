#pragma once

#include <memory>
#include <unordered_map>
#include <string>

// for loading models
#include "Components/Model.h"
#include <vector>

class ModelLoader;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	// retuns normal pointers
	Model* GetModel(std::string path);

	// used only by model loader
	Texture GetTexture(std::string path, std::string type);

	/// create loader for textures or move to model loader?
	unsigned int LoadCubemap(std::vector<std::string> faces);
private:
	ModelLoader* loader;

	// all owned pointers, change to normal pointers. 
	// Only using shared pointers when returning values from unorded map to other objects
	// change from model to list of meshes
	std::unordered_map<std::string, Model> models;
	// maybe separate textures based on usage, store only textures not texture pointers
	std::unordered_map<std::string, Texture> textures;
};