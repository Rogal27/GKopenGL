#pragma once

#include <vector>

#include  "model.h"
#include "light.h"
#include "shader.h"


class Scene
{
protected:
	vector<Model> models;
	Shader shader;

public:
	Scene(Shader shader);
	virtual ~Scene() {}
	void AddModel(Model& model);
	void SetShader(Shader shader);
	virtual void Draw();	
};


class LightScene : public Scene
{
protected:
	vector<Light> lights;
	int dirLightsCount;
	int pointLightsCount;
	int spotLightsCount;
	void SetUpLights();
public:
	LightScene(Shader shader);
	virtual ~LightScene() {}
	virtual void Draw() override;
	void AddLight(Light& light, LightType type);
	

};

LightScene::LightScene(Shader shader) : Scene(shader)
{
	dirLightsCount = 0;
	pointLightsCount = 0;
	spotLightsCount = 0;
}

void LightScene::SetUpLights()
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

void LightScene::AddLight(Light& light, LightType type)
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