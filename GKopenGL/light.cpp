#include "light.h"
#include "shader.h"

Light::Light(vec3 _color, vec3 _ambient, vec3 _diffuse, vec3 _specular):
	color(_color), ambient(_ambient), diffuse(_diffuse), specular(_specular)
{

}

void Light::setAmbient(vec3 _ambient)
{
	ambient = _ambient;
}

void Light::setAmbient(float x, float y, float z)
{
	ambient = vec3(x, y, z);
}

void Light::setDiffuse(vec3 _diffuse)
{
	diffuse = _diffuse;
}

void Light::setDiffuse(float x, float y, float z)
{
	diffuse = vec3(x, y, z);
}

void Light::setSpecular(vec3 _specular)
{
	specular = _specular;
}

void Light::setSpecular(float x, float y, float z)
{
	specular = vec3(x, y, z);
}

void Light::setColor(vec3 _color)
{
	color = _color;
}

void Light::setColor(float x, float y, float z)
{
	color = vec3(x, y, z);
}

vec3 Light::getAmbient()
{
	return ambient;
}

vec3 Light::getDiffuse()
{
	return diffuse;
}

vec3 Light::getSpecular()
{
	return specular;
}

vec3 Light::getColor()
{
	return color;
}

void Light::Draw(Shader& shader)
{
}
