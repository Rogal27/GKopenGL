#include <iostream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "spotlight.h"
#include "light.h"
#include "shader.h"

SpotLight::SpotLight(std::string model_path, vec3 _direction, float _cutOff, float _outerCutOff, float _constant, float _linear, float _quadratic, vec3 _color, vec3 _ambient, vec3 _diffuse, vec3 _specular):
	Light(_color, _ambient, _diffuse, _specular), position(0.0f), direction(_direction), constant(_constant), linear(_linear), quadratic(_quadratic), model(model_path)
{
	setCutOff(_cutOff);
	setOuterCutOff(_outerCutOff);
	position = model.GetCenter();
}

void SpotLight::setPosition(vec3 _position)
{
	//model.Translate(_position - position);
	position = _position;
}

void SpotLight::setPosition(float x, float y, float z)
{
	setPosition(vec3(x, y, z));
}

void SpotLight::setDirection(vec3 _direction)
{
	direction = _direction;
}

void SpotLight::setDirection(float x, float y, float z)
{
	direction = vec3(x, y, z);
}
//angle in degrees
void SpotLight::setCutOff(float _cutOff)
{
	cutOff = glm::cos(glm::radians(_cutOff));
}
//angle in degrees
void SpotLight::setOuterCutOff(float _outerCutOff)
{
	outerCutOff = glm::cos(glm::radians(_outerCutOff));
}

void SpotLight::setConstant(float _constant)
{
	constant = _constant;
}
void SpotLight::setLinear(float _linear)
{
	linear = _linear;
}
void SpotLight::setQuadratic(float _quadratic)
{
	quadratic = _quadratic;
}

vec3 SpotLight::getPosition()
{
	return position;
}

vec3 SpotLight::getDirection() 
{
	return direction;
}

float SpotLight::getCutOff()
{
	return cutOff;
}

float SpotLight::getOuterCutOff()
{
	return outerCutOff;
}

float SpotLight::getConstant()
{
	return constant;
}

float SpotLight::getLinear()
{
	return linear;
}

float SpotLight::getQuadratic()
{
	return quadratic;
}

void SpotLight::setShaderUniforms(Shader& s, int& dirLights, int& pointLights, int& spotLights)
{
	std::string name = "spotLights[" + std::to_string(spotLights) + "].";
	s.setVec3(name + "position", glm::vec3(model.GetModelMatrix() * model.GetPositionMatrix() * glm::vec4(position, 1.0f)));
	s.setVec3(name + "direction", glm::normalize(glm::vec3(model.GetModelMatrix() * model.GetPositionMatrix() * glm::vec4(direction, 0.0f))));
	s.setVec3(name + "ambient", ambient);
	s.setVec3(name + "diffuse", diffuse);
	s.setVec3(name + "specular", specular);
	s.setFloat(name + "constant", constant);
	s.setFloat(name + "linear", linear);
	s.setFloat(name + "quadratic", quadratic);
	s.setFloat(name + "cutOff", cutOff);
	s.setFloat(name + "outerCutOff", outerCutOff);
	s.setVec3(name + "color", color);
	spotLights++;
}

void SpotLight::Draw(Shader& shader)
{
	shader.setVec3("lightColor", color);
	shader.setFloat("constant", constant);
	shader.setFloat("linear", linear);
	shader.setFloat("quadratic", quadratic);
	model.Draw(shader);
}
