#pragma once

#include <vector>

#include "scene.h"
#include "shader.h"

class LightenScene : public Scene
{
public:
	std::vector<Light*> lights;

	LightenScene();
	virtual ~LightenScene();
	virtual void Draw(const float& width, const float& height, const double& time) override;
	void AddLight(Light* light, LightType type);
protected:
	void SetUpLights();

	Shader lightShader;
	int dirLightsCount;
	int pointLightsCount;
	int spotLightsCount;	
};
