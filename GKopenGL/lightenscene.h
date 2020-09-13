#pragma once

#include <vector>

#include "scene.h"
#include "shader.h"

class LightenScene : public Scene
{
public:
	LightenScene();
	virtual ~LightenScene();
	virtual void Draw(const float& width, const float& height, const double& time) override;
	void AddLight(Light* light, LightType type);

protected:
	void SetUpLights();

	Shader lightShader;
	std::vector<Light*> lights;
	int dirLightsCount;
	int pointLightsCount;
	int spotLightsCount;	
};
