#pragma once
#include "Levels/BaseLevel.h"
#include <string>

class Factory;
class GameApp;
class Player;
class Chunk;
class FastNoiseLite;

const float HardMode = 1.425f;
const float NormalMode = 1.625f;
const float EasyMode = 1.7375f;
const float Hardest = 1.375f;

const float speedIncrease = 0.25f;
const float chanceDecrease = 0.005f;

const int MaxChunks = 6;

enum Difficulty
{
	Easy,
	Normal,
	Hard
};

class InfiniteLevel : public Level
{
public:
	InfiniteLevel(GameApp*, Factory* factory, Difficulty difficulty);
	virtual ~InfiniteLevel();
	virtual std::vector<BaseObject*> GetObjects() override;
	virtual void Initialize() override;
	virtual void Tick(float) override;
	virtual bool UpdatePhysics() { return true; }
	virtual bool RenderSkybox() { return true; }
	virtual void UpdateUI(float) override;
protected:
	bool freeCam = false;

	bool cameraState = false;
	void SetupGenerator();
	std::string GenerateName();

	float* GenerateChunk();

	void UpdateChunks();

	int FurthestChunk();

	int LastChunk();

	float* EmptyChunk();

	Player* player;

	Chunk* chunks[MaxChunks];

	int playerLastChunk;

	FastNoiseLite* noise;

	float perlinDistance = 0;

	float worleyDistance = 0;

	float chance = 0;

	char inputText[10] = "";

	float floatValue = 10.5f;
	int intValue = 50;
};