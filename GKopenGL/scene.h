#pragma once

#include <vector>

#include  "model.h"
#include "light.h"
#include "shader.h"
#include "camera.h"

class Scene
{
protected:
	Shader shader;
	vector<Camera> cameras;
	int activeCameraIndex;

public:
	vector<Model> models;

	Scene(Shader shader);
	virtual ~Scene() {}
	void AddModel(Model& model);
	void SetShader(Shader shader);
	void AddCamera(Camera& camera);
	bool SetActiveCamera(int index);
	int GetCamerasCount();
	void SwitchCamera();
	Camera* GetActiveCamera();
	virtual void Draw(float width, float height, double time);
};

//class LightOnlyScene : public Scene
//{
//protected:
//	vector<Light> lights;
//public:
//	LightOnlyScene(Shader shader);
//	virtual ~LightOnlyScene() {}
//	void AddLight(Light& light, LightType type);
//	virtual void Draw(float width, float height, double time) override;
//};


class LightenScene : public Scene
{
protected:
	vector<Light> lights;
	int dirLightsCount;
	int pointLightsCount;
	int spotLightsCount;
	void SetUpLights();

public:
	LightenScene(Shader shader);
	virtual ~LightenScene() {}
	virtual void Draw(float width, float height, double time) override;
	void AddLight(Light& light, LightType type);
};

Scene::Scene(Shader shader) : shader(shader)
{

}

void Scene::AddModel(Model& model)
{
	models.push_back(model);
}

void Scene::SetShader(Shader shader)
{
	this->shader = shader;
}

void Scene::AddCamera(Camera& camera)
{
	if (cameras.size() == 0)
		activeCameraIndex = 0;
	cameras.push_back(camera);
}

bool Scene::SetActiveCamera(int index)
{
	if (index < 0 && index >= cameras.size())
		return false;
	activeCameraIndex = index;
	return true;
}

Camera* Scene::GetActiveCamera()
{
	if (activeCameraIndex < 0 && activeCameraIndex >= cameras.size())
		return nullptr;
	return &cameras[activeCameraIndex];
}

int Scene::GetCamerasCount()
{
	return cameras.size();
}

void Scene::SwitchCamera()
{
	if (cameras.size() == 0)
		return;
	activeCameraIndex++;
	activeCameraIndex %= cameras.size();
}

void Scene::Draw(float width, float height, double time)
{
	if (cameras.size() == 0)
		return;

	Camera* camera = GetActiveCamera();
	if (camera == nullptr)
		return;

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 normalMatrix = glm::mat4(1.0f);
	glm::mat4 projection = camera->GetProjectionMatrix(width, height);
	glm::mat4 view = camera->GetViewMatrix();

	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

	for (auto model : models)
	{
		modelMatrix = model.GetModelMatrix();
		normalMatrix = model.GetModelMatrix();

		shader.setMat4("model", modelMatrix);
		shader.setMat3("NormalMatrix", normalMatrix);

        model.Draw(shader);
	}
}

LightenScene::LightenScene(Shader shader) : Scene(shader)
{
	dirLightsCount = 0;
	pointLightsCount = 0;
	spotLightsCount = 0;
}

void LightenScene::SetUpLights()
{
	int dirLights = 0, pointLights = 0, spotLights = 0;
	shader.setInt("dirLightsCount", dirLightsCount);
	shader.setInt("pointLightsCount", pointLightsCount);
	shader.setInt("spotLightsCount", spotLightsCount);
	for (auto l : lights)
	{
		l.setShaderUniforms(shader, dirLights, pointLights, spotLights);
	}
}

void LightenScene::AddLight(Light& light, LightType type)
{
	lights.push_back(light);
	switch (type)
	{
	case LightType::Directional:
		dirLightsCount++;
		break;
	case LightType::Point:
		pointLightsCount++;
		break;
	case LightType::SpotLight:
		spotLightsCount++;
		break;
	}
}


void LightenScene::Draw(float width, float height, double time)
{
	if (cameras.size() == 0)
		return;

	Camera* camera = GetActiveCamera();
	if (camera == nullptr)
		return;

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 normalMatrix = glm::mat4(1.0f);
	glm::mat4 projection = camera->GetProjectionMatrix(width, height);
	glm::mat4 view = camera->GetViewMatrix();

	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

	SetUpLights();

	for (auto model : models)
	{
		modelMatrix = model.GetModelMatrix();
		normalMatrix = model.GetModelMatrix();

		shader.setMat4("model", modelMatrix);
		shader.setMat3("NormalMatrix", normalMatrix);

		model.Draw(shader);
	}
}
