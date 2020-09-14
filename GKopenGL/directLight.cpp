#include <iostream>
#include <string>

#include "directlight.h"
#include "light.h"
#include "shader.h"

using namespace glm;

DirectLight::DirectLight(vec3 _direction, vec3 _color, vec3 _ambient, vec3 _diffuse, vec3 _specular):
	Light(_color, _ambient,_diffuse,_specular), direction(_direction), isTurnedOn(true)
{

}

void DirectLight::setDirection(vec3 _direction)
{
	direction = _direction;
}

void DirectLight::setDirection(float x, float y, float z)
{
	direction = vec3(x, y, z);
}

vec3 DirectLight::getDirection()
{
	return direction;
}

void DirectLight::setShaderUniforms(Shader& s, int& dirLights, int& pointLights, int& spotLights)
{	
	std::string name = "dirLights[" + std::to_string(dirLights) + "].";
	s.setVec3(name + "direction", direction);
	s.setVec3(name + "color", color);
	if (isTurnedOn)
	{
		s.setVec3(name + "ambient", ambient);
		s.setVec3(name + "diffuse", diffuse);
		s.setVec3(name + "specular", specular);
	}
	else
	{
		s.setVec3(name + "ambient", ambient);
		s.setVec3(name + "diffuse", glm::vec3(0.0f));
		s.setVec3(name + "specular", glm::vec3(0.0f));
	}
	dirLights++;
}
