#include <iostream>
#include <string>

#include "pointlight.h"
#include "light.h"
#include "shader.h"

PointLight::PointLight(vec3 _position, float _constant, float _linear, float _quadratic, vec3 _color, vec3 _ambient, vec3 _diffuse, vec3 _specular):
	Light(_color, _ambient, _diffuse, _specular), position(_position), constant(_constant), linear(_linear), quadratic(_quadratic)
{

}

void PointLight::setPosition(vec3 _position)
{
	position = _position;
}

void PointLight::setPosition(float x, float y, float z)
{
	position = vec3(x, y, z);
}

void PointLight::setConstant(float _constant)
{
	constant = _constant;
}
void PointLight::setLinear(float _linear)
{
	linear = _linear;
}
void PointLight::setQuadratic(float _quadratic)
{
	quadratic = _quadratic;
}

vec3 PointLight::getPosition()
{
	return position;
}

float PointLight::getConstant()
{
	return constant;
}

float PointLight::getLinear()
{
	return linear;
}

float PointLight::getQuadratic()
{
	return quadratic;
}

void PointLight::setShaderUniforms(Shader& s, int& dirLights, int& pointLights, int& spotLights)
{	
	std::string name = "pointLights[" + std::to_string(pointLights) + "].";
	s.setVec3(name + "position", position);
	s.setVec3(name + "ambient", ambient);
	s.setVec3(name + "diffuse", diffuse);
	s.setVec3(name + "specular", specular);
	s.setFloat(name + "constant", constant);
	s.setFloat(name + "linear", linear);
	s.setFloat(name + "quadratic", quadratic);
	s.setVec3(name + "color", color);
	pointLights++;
}