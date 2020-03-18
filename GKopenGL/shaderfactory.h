#pragma once

#include "shader.h"

namespace shaderPath
{
    const char* phongVertexShaderPath = "phongVertexShader.vert";
    const char* phongFragmentShaderPath = "phongFragmentShader.frag";
    const char* gouraudVertexShaderPath = "gouraudVertexShader.vert";
    const char* gouraudFragmentShaderPath = "gouraudFragmentShader.frag";
    const char* lightVertexShaderPath = "lightVertexShader.vert";
    const char* lightFragmentShaderPath = "lightFragmentShader.frag";
    const char* modelVertexShaderPath = "modelVertexShader.vert";
    const char* modelFragmentShaderPath = "modelFragmentShader.frag";
}

enum class ShaderType
{
    phong,
    gouraud,
    light
};

class ShaderFactory
{
public:
    static ShaderFactory& getInstance()
    {
        static ShaderFactory instance;

        return instance;
    }

private:
    ShaderFactory():
        phongShader(shaderPath::phongVertexShaderPath, shaderPath::phongFragmentShaderPath),
        gouraudShader(shaderPath::gouraudVertexShaderPath, shaderPath::gouraudFragmentShaderPath),
        lightShader(shaderPath::lightVertexShaderPath, shaderPath::lightFragmentShaderPath)
    {

    }

public:
    ShaderFactory(ShaderFactory const&) = delete;
    void operator=(ShaderFactory const&) = delete;

private:

    Shader phongShader;
    Shader gouraudShader;
    //Shader blinnShader;
    Shader lightShader;

public:
    Shader& getShader(ShaderType type)
    {
        switch (type)
        {
        case ShaderType::phong:
            return phongShader;
        case ShaderType::gouraud:
            return gouraudShader;
        case ShaderType::light:
            return lightShader;
        default:
            break;
        }
        return phongShader;
    }
};