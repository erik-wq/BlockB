#include "Levels/LightsLevel.h"
#include "Application/SpawnData.h"
#include "Objects/Camera.h"
#include "Application/GameApp.h"
#include "Objects/BaseObject.h"
#include "Components/Shader.h"
#include "imgui.h"

LightLevel::LightLevel(GameApp* game, Factory* factory) : Level(game, factory)
{
	dependencies = { "testingCube.obj", "Cube.obj", "Dumpster.obj", "Floor.obj" };
}

LightLevel::~LightLevel()
{
	camera->UnBindInput(gameInstance->GetCurrentInput());
}

void LightLevel::Initialize()
{
	camera = new Camera(glm::vec3(0, 2, 10), glm::vec3(0.0f, 1.0f, 0.0f), 270);
	gameInstance->UpdateCamera(camera);

	Transform tran;
	tran.SetPosition(glm::vec3(0, 0, 0));
	tran.UpdateMatrices();

	SpawnParams spawnParams;
	spawnParams.modelPath = "testingCube.obj";
	spawnParams.transform = tran;

	objects.push_back(SpawnObject(spawnParams));

    Transform wall;
    wall.SetPosition(glm::vec3(2, 2, -2));
    wall.SetScale(glm::vec3(10 , 10, 10));
    wall.RotateX(90);
    wall.UpdateMatrices();

    spawnParams.modelPath = "Floor.obj";
    spawnParams.transform = wall;

    // objects.push_back(SpawnObject(spawnParams));

	Transform tranLight1;
	tranLight1.SetPosition(lighPositions[0]);
	tranLight1.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	tranLight1.UpdateMatrices();

	spawnParams.modelPath = "Cube.obj";
	spawnParams.transform = tranLight1;

	lights.push_back(SpawnObject(spawnParams));

	Transform tranLight2;
	tranLight2.SetPosition(lighPositions[1]);
	tranLight2.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	tranLight2.UpdateMatrices();

	spawnParams.modelPath = "Cube.obj";
	spawnParams.transform = tranLight2;

	lights.push_back(SpawnObject(spawnParams));

	Transform tranLight3;
	tranLight3.SetPosition(lighPositions[2]);
	tranLight3.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	tranLight3.UpdateMatrices();

	spawnParams.modelPath = "Cube.obj";
	spawnParams.transform = tranLight3;

	lights.push_back(SpawnObject(spawnParams));

    Transform tranLight4;
    tranLight4.SetPosition(lighPositions[3]);
    tranLight4.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
    tranLight4.UpdateMatrices();

    spawnParams.modelPath = "Cube.obj";
    spawnParams.transform = tranLight4;

    lights.push_back(SpawnObject(spawnParams));

	camera->BindInput(gameInstance->GetCurrentInput());
}

void LightLevel::UpdateUI(float)
{
    ImGui::Begin("Light Level");

    ImGui::SliderFloat("world ambient", &worldSettings[0], 0, 1);
    ImGui::SliderFloat("world difuse", &worldSettings[1], 0, 1);
    ImGui::SliderFloat("world spectacular", &worldSettings[2], 0, 1);
    
    ImGui::SliderFloat("ambient 1", &ambientStrength[0], 0, 1);
    ImGui::SliderFloat("ambient 2", &ambientStrength[1], 0, 1);
    ImGui::SliderFloat("ambient 3", &ambientStrength[2], 0, 1);
    ImGui::SliderFloat("ambient 4", &ambientStrength[3], 0, 1);

    ImGui::SliderFloat("difuse 1", &diffuseStrength[0], 0, 1);
    ImGui::SliderFloat("difuse 2", &diffuseStrength[1], 0, 1);
    ImGui::SliderFloat("difuse 3", &diffuseStrength[2], 0, 1);
    ImGui::SliderFloat("difuse 4", &diffuseStrength[3], 0, 1);

    ImGui::SliderFloat("spectacular 1", &specularStrength[0], 0, 1);
    ImGui::SliderFloat("spectacular 2", &specularStrength[1], 0, 1);
    ImGui::SliderFloat("spectacular 3", &specularStrength[2], 0, 1);
    ImGui::SliderFloat("spectacular 4", &specularStrength[3], 0, 1);

    ImGui::SliderFloat("outer", &outer, 13, 17.5f);
    ImGui::SliderFloat("inner", &inner, 7.5f, 13);
    ImGui::SliderFloat("ambient", &point[0], 0, 1);
    ImGui::SliderFloat("difuse", &point[1], 0, 1);
    ImGui::SliderFloat("spectacular", &point[2], 0, 1);

    ImGui::End();
}

void LightLevel::SetLevelLights(Shader* shader)
{
    shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader->setVec3("dirLight.ambient", worldSettings[0], worldSettings[0], worldSettings[0]);
    shader->setVec3("dirLight.diffuse", worldSettings[1], worldSettings[1], worldSettings[1]);
    shader->setVec3("dirLight.specular", worldSettings[2], worldSettings[2], worldSettings[2]);
    // point light 1
    shader->setVec3("pointLights[0].position", lighPositions[0]);
    shader->setVec3("pointLights[0].ambient", ambientStrength[0], ambientStrength[0], ambientStrength[0]);
    shader->setVec3("pointLights[0].diffuse", diffuseStrength[0], diffuseStrength[0], diffuseStrength[0]);
    shader->setVec3("pointLights[0].specular", specularStrength[0], specularStrength[0], specularStrength[0]);
    shader->setFloat("pointLights[0].constant", 1.0f);
    shader->setFloat("pointLights[0].linear", 0.09f);
    shader->setFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    shader->setVec3("pointLights[1].position", lighPositions[1]);
    shader->setVec3("pointLights[1].ambient", ambientStrength[1], ambientStrength[1], ambientStrength[1]);
    shader->setVec3("pointLights[1].diffuse", diffuseStrength[1], diffuseStrength[1], diffuseStrength[1]);
    shader->setVec3("pointLights[1].specular", specularStrength[1], specularStrength[1], specularStrength[1]);
    shader->setFloat("pointLights[1].constant", 1.0f);
    shader->setFloat("pointLights[1].linear", 0.09f);
    shader->setFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    shader->setVec3("pointLights[2].position", lighPositions[2]);
    shader->setVec3("pointLights[2].ambient", ambientStrength[2], ambientStrength[2], ambientStrength[2]);
    shader->setVec3("pointLights[2].diffuse", diffuseStrength[2], diffuseStrength[2], diffuseStrength[2]);
    shader->setVec3("pointLights[2].specular", specularStrength[2], specularStrength[2], specularStrength[2]);
    shader->setFloat("pointLights[2].constant", 1.0f);
    shader->setFloat("pointLights[2].linear", 0.09f);
    shader->setFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    shader->setVec3("pointLights[3].position", lighPositions[3]);
    shader->setVec3("pointLights[3].ambient", ambientStrength[3], ambientStrength[3], ambientStrength[3]);
    shader->setVec3("pointLights[3].diffuse", diffuseStrength[3], diffuseStrength[3], diffuseStrength[3]);
    shader->setVec3("pointLights[3].specular", specularStrength[3], specularStrength[3], specularStrength[3]);
    shader->setFloat("pointLights[3].constant", 1.0f);
    shader->setFloat("pointLights[3].linear", 0.09f);
    shader->setFloat("pointLights[3].quadratic", 0.032f);
    // spotLight
    shader->setVec3("spotLight.position", glm::vec3(0,2,8));
    shader->setVec3("spotLight.direction", glm::vec3(0,0,-1));
    shader->setVec3("spotLight.ambient", point[0], point[0], point[0]);
    shader->setVec3("spotLight.diffuse", point[1], point[1], point[1]);
    shader->setVec3("spotLight.specular", point[2], point[2], point[2]);
    shader->setFloat("spotLight.constant", 1.0f);
    shader->setFloat("spotLight.linear", 0.09f);
    shader->setFloat("spotLight.quadratic", 0.032f);
    shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(inner)));
    shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(outer)));
}
