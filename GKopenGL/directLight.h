#pragma once

#include "light.h"

class Shader;

using namespace glm;

class DirectLight : public Light
{
protected:
	vec3 direction;

public:
	DirectLight(vec3 _direction, vec3 _color = vec3(1.0f, 1.0f, 1.0f), vec3 _ambient = vec3(0.05f, 0.05f, 0.05f), vec3 _diffuse = vec3(0.8f, 0.8f, 0.8f), vec3 _specular = vec3(1.0f, 1.0f, 1.0f));

	void setDirection(vec3 _direction);
	void setDirection(float x, float y, float z);	

	vec3 getDirection();

	virtual void setShaderUniforms(Shader& s, int& dirLights, int& pointLights, int& spotLights) override;

	virtual ~DirectLight() {}
};