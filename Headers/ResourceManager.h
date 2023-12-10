#pragma once

#include <memory>
#include <unordered_map>
#include <string>

// for loading models
#include "ModelLoader.h"
#include "Mesh.h"
#include "Model.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	std::shared_ptr<Model> GetModel(std::string path);
	std::shared_ptr<Texture> GetTexture(std::string path);
private:
	ModelLoader* loader;

	// all owned pointers, change to normal pointers. 
	// Only using shared pointers when returning values from unorded map to other objects
	// change from model to list of meshes
	std::unordered_map<std::string, std::shared_ptr<Model>> models;
	// maybe separate textures based on usage
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};