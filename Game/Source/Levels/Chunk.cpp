#include "Levels/Chunk.h"
#include "Levels/BaseLevel.h"
#include "Application/SpawnData.h"

Chunk::Chunk(float* chunkData, Level* level, float distance) : 
	chunkDistance(distance),
	chunkData(chunkData)
{
	SpawnObjects(level);
}

Chunk::~Chunk()
{
	for (int i = 0; i < int(objects.size()); i++)
	{
		delete objects[i];
	}
	delete chunkData;
}

void Chunk::SpawnObjects(Level* level)
{
	SpawnColliderParams colliderParams;
	colliderParams.extends = glm::vec3(12, 0.5f, 12);
	colliderParams.shape = Box;
	colliderParams.type = Collision;
	colliderParams.layer = GROUND;

	glm::vec3 pos = glm::vec3(0, 0.5f, chunkDistance);
	Transform floorTrans;
	floorTrans.SetScale(glm::vec3(12, 0, 12));
	floorTrans.SetPosition(pos);
	SpawnParams spawnParams;
	spawnParams.colliderParams = colliderParams;
	spawnParams.transform = floorTrans;
	spawnParams.colliderOfset = glm::vec3(0, 0.5f , 0);
	//spawnParams.modelPath = "Floor.obj";

	objects.push_back(level->SpawnObject(spawnParams));
	objects[0]->SetSynchronizationState(TransformSynch);
	objects[0]->Update(0);
	objects[0]->transform.RotateY(90);

	colliderParams = SpawnColliderParams();
	colliderParams.extends = glm::vec3(1, 4, 12);
	colliderParams.shape = Box;
	colliderParams.type = Collision;
	colliderParams.layer = NO;

	Transform first;
	first.position = pos + glm::vec3(-(ChunkSize / 2 + 0.5f), 4, 0);
	spawnParams = SpawnParams();
	spawnParams.colliderParams = colliderParams;
	spawnParams.transform = first;

	objects.push_back(level->SpawnObject(spawnParams));

	Transform second;
	second.position = pos + glm::vec3((ChunkSize / 2 + 0.5f), 4, 0);
	spawnParams = SpawnParams();
	spawnParams.colliderParams = colliderParams;
	spawnParams.transform = second;

	objects.push_back(level->SpawnObject(spawnParams));


	float corner = -ChunkSize / 2;
	
	int count = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (chunkData[count])
			{
				glm::vec3 objectPos = pos;
				objectPos.x += corner + i * 4 + 2;
				objectPos.z += corner + j * 4 + 2;

				int random = rand() % 4;
				switch (random)
				{
				case 0:
					SpawnWheels(objectPos, level);
					break;
				case 1:
					SpawnWall(objectPos, level);
					break;
				case 2:
					SpawnDumpster(objectPos, level);
					break;
				case 3:
					break;
				}

			}
			count++;
		}
	}
}

void Chunk::SpawnWheels(glm::vec3 position, Level* level)
{
	SpawnColliderParams colliderParams;
	colliderParams.layer = OBSTICLE;
	colliderParams.type = Ghost;
	colliderParams.shape = Box;
	colliderParams.extends = glm::vec3(0.8f, 1.4f, 0.4f);

	Transform trans;
	trans.RotateY(90);
	trans.SetScale(glm::vec3(2.5f, 2.5f, 2.5f));
	position.y = 0.4f;
	trans.SetPosition(position);
	SpawnParams spawnParams;
	spawnParams.colliderParams = colliderParams;
	spawnParams.transform = trans;
	//spawnParams.modelPath = "Nismo_LMGT4_Wheels.obj";

	objects.push_back(level->SpawnObject(spawnParams));
}

void Chunk::SpawnWall(glm::vec3 position, Level* level)
{
	SpawnColliderParams colliderParams;
	colliderParams.layer = OBSTICLE;
	colliderParams.type = Ghost;
	colliderParams.shape = Box;
	colliderParams.extends = glm::vec3(2.3, 2.5, 0.5f);

	Transform trans;
	trans.RotateY(90);
	trans.SetScale(glm::vec3(2.5f, 2.5f, 2.5f));
	position.y = 0.5f;
	trans.SetPosition(position);
	SpawnParams spawnParams;
	spawnParams.colliderParams = colliderParams;
	spawnParams.transform = trans;
	//spawnParams.modelPath = "Barier.obj";

	objects.push_back(level->SpawnObject(spawnParams));
}

void Chunk::SpawnDumpster(glm::vec3 position, Level* level)
{
	SpawnColliderParams colliderParams;
	colliderParams.layer = OBSTICLE;
	colliderParams.type = Ghost;
	colliderParams.shape = Box;
	colliderParams.extends = glm::vec3(2.1f, 3, 1);

	Transform trans;
	trans.RotateY(90);
	trans.SetScale(glm::vec3(2.5f, 2.5f, 2.5f));
	position.y = 0.5f;
	trans.SetPosition(position);
	SpawnParams spawnParams;
	spawnParams.colliderParams = colliderParams;
	spawnParams.transform = trans;
	//spawnParams.modelPath = "Dumpster.obj";

	objects.push_back(level->SpawnObject(spawnParams));
}
