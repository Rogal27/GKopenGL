#pragma once

#include "light.h"
#include "lightmodel.h"

class Shader;

using namespace glm;

class PointLight : public Light
{
protected:
	vec3 position;

	float constant;
	float linear;
	float quadratic;
public:
	PointLight(std::string model_path, float _constant = 1.0f, float _linear = 0.04f, float _quadratic = 0.016f, vec3 _color = vec3(1.0f, 1.0f, 1.0f), vec3 _ambient = vec3(0.005f, 0.005f, 0.005f), vec3 _diffuse = vec3(0.8f, 0.8f, 0.8f), vec3 _specular = vec3(1.0f, 1.0f, 1.0f));

	void setPosition(vec3 _position);
	void setPosition(float x, float y, float z);

	void setConstant(float _constant);
	void setLinear(float _linear);
	void setQuadratic(float _quadratic);

	vec3 getPosition();
	float getConstant();
	float getLinear();
	float getQuadratic();

	virtual void setShaderUniforms(Shader& s, int& dirLights, int& pointLights, int& spotLights) override;
	virtual void Draw(Shader& shader) override;

	virtual ~PointLight() {}

	LightModel model;
};
