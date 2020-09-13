#pragma once

#include "light.h"

class Shader;

class DirectLight : public Light
{
protected:
	glm::vec3 direction;

public:
	DirectLight(glm::vec3 _direction, glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 _ambient = glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3 _diffuse = glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3 _specular = glm::vec3(1.0f, 1.0f, 1.0f));

	void setDirection(glm::vec3 _direction);
	void setDirection(float x, float y, float z);	

	glm::vec3 getDirection();

	virtual void setShaderUniforms(Shader& s, int& dirLights, int& pointLights, int& spotLights) override;

	virtual ~DirectLight() {}
};
