#pragma once

#include <vector>

#include "scene.h"

class LightenScene : public Scene
{
protected:
	vector<Light*> lights;
	int dirLightsCount;
	int pointLightsCount;
	int spotLightsCount;
	void SetUpLights();
	
public:
	LightenScene(Shader shader);
	virtual ~LightenScene();
	virtual void Draw(float width, float height, double time) override;
	void AddLight(Light* light, LightType type);
};