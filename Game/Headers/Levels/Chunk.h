#pragma once
#include <vector>
#include "Objects/BaseObject.h"
#include "Components/Transform.h"

const float ChunkSize = 24;

class Level;

class Chunk
{
public:
	Chunk(float* chunkData, Level* level, float);
	~Chunk();

	float GetDistnace() { return chunkDistance; };

	std::vector<BaseObject*> GetObjects() { return objects; };
private:
	void SpawnObjects(Level* level);

	void SpawnWheels(glm::vec3, Level*);

	void SpawnWall(glm::vec3, Level*);

	void SpawnDumpster(glm::vec3, Level*);

	float chunkDistance = 0;

	float* chunkData = nullptr;

	std::vector<BaseObject*> objects;
}; 