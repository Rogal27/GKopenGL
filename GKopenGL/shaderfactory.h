#pragma once

#include "shader.h"

enum class ShaderType
{
    phong,
    gouraud,
    light,
    simpleModel
};

class ShaderFactory
{
public:
    static ShaderFactory& getInstance();

private:
    ShaderFactory();

public:
    ShaderFactory(ShaderFactory const&) = delete;
    void operator=(ShaderFactory const&) = delete;

private:

    Shader phongShader;
    Shader gouraudShader;
    Shader lightShader;
    Shader simpleModel;

public:
    Shader getShader(ShaderType type);
};
