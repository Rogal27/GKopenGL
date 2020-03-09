#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

using namespace glm;

class Light
{
protected:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 color;

public:
	Light(vec3 _color = vec3(1.0f, 1.0f, 1.0f), vec3 _ambient = vec3(0.05f, 0.05f, 0.05f), vec3 _diffuse = vec3(0.8f, 0.8f, 0.8f), vec3 _specular = vec3(1.0f, 1.0f, 1.0f));

	void setAmbient(vec3 _ambient);
	void setAmbient(float x, float y, float z);
	void setDiffuse(vec3 _diffuse);
	void setDiffuse(float x, float y, float z);
	void setSpecular(vec3 _specular);
	void setSpecular(float x, float y, float z);

	void setColor(vec3 _color);
	void setColor(float x, float y, float z);

	vec3 getAmbient();
	vec3 getDiffuse();
	vec3 getSpecular();
	vec3 getColor();

	virtual void setShaderUniforms(Shader& s, int& dirLights, int& pointLights, int& spotLights) = 0;

	virtual ~Light() {}
};