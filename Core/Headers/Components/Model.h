#pragma once
#include "Mesh.h"
#include "Transform.h"
#include "Animations/Bone.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <vector>
#include <map>

class Shader;
class ResourceManager;
class BaseObject;

class Model
{
public:
	Model(){}
	Model(std::vector<Mesh*> mesh);
	void SetMeshes(std::vector<Mesh*> mesh) { meshes = mesh; }
	~Model();
	void Draw(Shader* shader);
	void DrawInstanced(Shader* shader);

	void PushToGPU(ResourceManager*);

	std::vector<glm::vec3> GetAllVertices();

	void AddInstance(BaseObject*);

	void RemoveInstance(BaseObject*);

	auto& GetBoneInfoMap() { return boneInfoMap; }
	int& GetBoneCount() { return boneCounter; }

	std::string GetPath() { return path; }

	void SetPath(std::string p) { path = p; }

	// position, rotation and scale
	Transform transform;
private:
	std::map<std::string, BoneInfo> boneInfoMap;
	int boneCounter = 0;
	void UpdateInstancesBuffer();

	void UpdateMatricies();

	unsigned int instancesBuffer;

	std::vector<BaseObject*> instances;

	std::vector<glm::mat4> instancesMatrices;

	std::string path;

	// model data
	std::vector<Mesh*> meshes;

	bool valid = false;
	
};

