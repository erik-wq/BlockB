#include "Levels/InfiniteLevel.h"
#include "Application/SpawnData.h"

#include "Objects/Player.h"
#include "Levels/Chunk.h"
#include "FastNoiseLite.h"
#include <iostream>
#include <random>
#include <vector>
#include "Objects/Camera.h"
#include "Application/GameApp.h"
#include "App/InputSystem.h"

#if defined(__arm__) || defined(__aarch64__)
#include <chrono>
#endif
#include <imgui.h>

std::vector<std::string> consonants = { "b", "c", "d", "f", "g", "h", "j", "k", "l", "m",
										  "n", "p", "r", "s", "t", "th", "v", "x", "z" };
std::vector<std::string> vowels = { "a", "au", "e", "i", "o", "ou", "u", "y" };

InfiniteLevel::InfiniteLevel(GameApp* game, Factory* factory, Difficulty difficulty) : Level(game, factory)
{
	switch (difficulty)
	{
		case Easy:
			chance = EasyMode;
			break;
		case Normal:
			chance = NormalMode;
			break;
		case Hard:
			chance = HardMode;
			break;
	}

	dependencies = { "Running.dae" /*, "Nismo_LMGT4_Wheels.obj", "Barier.obj", "Dumpster.obj", "Floor.obj"*/};
}

InfiniteLevel::~InfiniteLevel()
{
	delete player;
	for (int i = 0; i < MaxChunks; i++)
	{
		delete chunks[i];
	}
}

std::vector<BaseObject*> InfiniteLevel::GetObjects()
{
	// loop over chunks get all objects and merge them into one vector
	std::vector<BaseObject*> result = objects;
	for (int i = 0; i < MaxChunks; i++)
	{
		std::vector<BaseObject*> chunkObjects = chunks[i]->GetObjects();
		result.insert(result.end(), chunkObjects.begin(), chunkObjects.end());
	}
	return result;
}

void InfiniteLevel::Initialize()
{
	Level::Initialize();

	SetupGenerator();

	SpawnColliderParams colliderParams;
	colliderParams.mass = 100;
	colliderParams.extends = glm::vec3(0.5f, 3, 0);
	colliderParams.shape = Capsule;
	colliderParams.type = RigidBody;
	colliderParams.layer = PLAYER;

	Transform tran;
	tran.SetPosition(glm::vec3(0, 4, -10));
	tran.SetRotation(glm::vec3(0, 90, 0));
	tran.SetScale(glm::vec3(2));
	tran.UpdateMatrices();

	SpawnParams spawnParams;
	spawnParams.colliderOfset = glm::vec3(0, 2, 0);
	spawnParams.colliderParams = colliderParams;
	spawnParams.modelPath = "Running.dae";
	spawnParams.transform = tran;
	player = SpawnPlayer(spawnParams);

	objects.push_back(player);

	player->SetCameraFolow(camera, glm::vec3(0, 4.5f, -12.5f));

	chunks[0] = new Chunk(EmptyChunk(), this, 0);

	for (int i = 1; i < MaxChunks; i++)
	{
		chunks[i] = new Chunk(EmptyChunk(), this, ChunkSize * i);
	}

	playerLastChunk = 0;
}

void InfiniteLevel::Tick(float delta)
{
	Level::Tick(delta);

	int chunk = int(player->transform.position.z / ChunkSize);
	if (playerLastChunk != chunk)
	{
		playerLastChunk = chunk;
		UpdateChunks();
	}
}

void InfiniteLevel::UpdateUI(float)
{
	ImGui::Begin("Checkbox Example");

	// Slider for a floating-point value
	ImGui::SliderFloat("speed", &floatValue, 2.5f, 20);
	player->SetSpeed(floatValue);

	ImGui::Checkbox("Enable Feature", &freeCam);

	ImGui::InputText("Enter Text", inputText, sizeof(inputText));

	ImGui::End();

	// Check if the checkbox state has changed
	if (cameraState != freeCam) {
		std::cout << "Checkbox state changed!" << std::endl;
		cameraState = freeCam;
		if (freeCam)
		{
			camera->BindInput(gameInstance->GetCurrentInput());
		}
		else
		{
			camera->UnBindInput(gameInstance->GetCurrentInput());
		}
	}
}

void InfiniteLevel::SetupGenerator()
{
	unsigned int time = 0;

#if defined(__arm__) || defined(__aarch64__)
	auto current_time = std::chrono::system_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(current_time.time_since_epoch());

	time = static_cast<unsigned int>(duration.count());
#else
	time = static_cast<unsigned int>(std::time(nullptr));
#endif

	noise = new FastNoiseLite(time);
	std::srand(time);

	perlinDistance = static_cast<float>(std::rand() % 1001);

	worleyDistance = static_cast<float>(std::rand() % 1001);
}

std::string InfiniteLevel::GenerateName()
{
	std::string name = "";
	int length = rand() % 3 + 2;
	for (int i = 0; i < length; i++) {
		name += (consonants[rand() % consonants.size()] + vowels[rand() % vowels.size()]);
	}

	name[0] = char(toupper(name[0]));
#ifndef NDEBUG
	std::cout << name << std::endl;
#endif
	return name;
}

float* InfiniteLevel::GenerateChunk()
{
	float* chunkData = new float[36];

	for (int i = 0; i < 36; i++)
	{
		chunkData[i] = 0;
	}

	int count = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			noise->SetNoiseType(FastNoiseLite::NoiseType_Perlin);
			float p = noise->GetNoise(perlinDistance + 10.0f / 6 * j, perlinDistance + 10.0f / 6 * i);

			noise->SetNoiseType(FastNoiseLite::NoiseType_Cellular);
			float d = noise->GetNoise(worleyDistance + 10.0f / 6 * j, worleyDistance + 10.0f / 6 * i);

			float f = (1 - abs(p) * abs(d));
			float rand = static_cast<float>(std::rand()) / RAND_MAX + 1;

			if (f * rand > chance)
				chunkData[count] = 1;

			count += 1;
		}

		perlinDistance += 10.0f / 6;
		worleyDistance += 10.0f / 6 * 0.5f;
	}

#ifndef NDEBUG
	count = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			std::cout << chunkData[count] << " , ";
			count += 1;
		}
		std::cout << "\n";
	}
	std::cout << "\ncomplete chunk\n";
#endif

	return chunkData;
}

void InfiniteLevel::UpdateChunks()
{
	player->modifySpeed(speedIncrease);
	chance -= chanceDecrease;
	int last = LastChunk();
	float newDist = chunks[FurthestChunk()]->GetDistnace();
	delete chunks[last];
	chunks[last] = new Chunk(GenerateChunk(), this, newDist + ChunkSize);
}

int InfiniteLevel::FurthestChunk()
{
	int result = 0;
	float dist = chunks[0]->GetDistnace();
	for (int i = 1; i < MaxChunks; i++)
	{
		float localDist = chunks[i]->GetDistnace();
		if (localDist > dist)
		{
			dist = localDist;
			result = i;
		}
	}
	return result;
}

int InfiniteLevel::LastChunk()
{
	int result = 0;
	float dist = chunks[0]->GetDistnace();
	for (int i = 1; i < MaxChunks; i++)
	{
		float localDist = chunks[i]->GetDistnace();
		if (localDist < dist)
		{
			dist = localDist;
			result = i;
		}
	}
	return result;
}

float* InfiniteLevel::EmptyChunk()
{
	float* data = new float[36];
	for (int i = 0; i < 36; i++)
	{
		data[i] = 0;
	}
	return data;
}
