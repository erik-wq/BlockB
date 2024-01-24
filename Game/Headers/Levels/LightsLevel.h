#pragma once
#include "BaseLevel.h"
#include <vector>
#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"

class LightLevel : public Level
{
public:
	LightLevel(GameApp* game, Factory* factory);
	virtual ~LightLevel();
	virtual void Initialize() override;
	virtual void UpdateUI(float) override;
	virtual std::vector<BaseObject*> GetLights() override { return lights; };

	virtual bool RenderSkybox() override { return true; }

	virtual void SetLevelLights(Shader*) override;
protected:
	std::vector<BaseObject*> lights;

	glm::vec3 lighPositions[4] = {
		glm::vec3(-1, 4, -1.5f),
		glm::vec3(3, -2, 0),
		glm::vec3(-2, -1.5, 0),
		glm::vec3(2, 4, -1)
	};

	float ambientStrength[4] = {
		0.05f,
		0.05f,
		0.05f,
		0.05f
	};
	
	float diffuseStrength[4] = {
		0.2f,
		0.2f,
		0.2f,
		0.2f
	};

	float specularStrength[4] = {
		1,
		1,
		1,
		1
	};

	float worldSettings[3] = {
		0.05f,
		0.4f,
		0.5f
	};

	float point[3] = {
		0.0f,
		1.0f,
		0.5f
	};

	float inner = 12.5f;
	float outer = 15.0f;


};