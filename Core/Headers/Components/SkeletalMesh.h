#pragma once
#include "Mesh.h"

class ResourceManager;

class SkeletalMesh : public Mesh
{
public:
	SkeletalMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
		std::vector<Texture> textures);
protected:
	virtual void SetupMesh() override;
};