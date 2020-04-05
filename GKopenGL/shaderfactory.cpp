#include "shaderfactory.h"


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

ShaderFactory::ShaderFactory() :
    phongShader(shaderPath::phongVertexShaderPath, shaderPath::phongFragmentShaderPath),
    gouraudShader(shaderPath::gouraudVertexShaderPath, shaderPath::gouraudFragmentShaderPath),
    lightShader(shaderPath::lightVertexShaderPath, shaderPath::lightFragmentShaderPath),
    simpleModel(shaderPath::modelVertexShaderPath, shaderPath::modelFragmentShaderPath)
{
    
}

ShaderFactory& ShaderFactory::getInstance()
{
    static ShaderFactory instance;

    return instance;
}

Shader& ShaderFactory::getShader(ShaderType type)
{
    switch (type)
    {
    case ShaderType::phong:
        return phongShader;
    case ShaderType::gouraud:
        return gouraudShader;
    case ShaderType::light:
        return lightShader;
    case ShaderType::simpleModel:
        return simpleModel;
    default:
        break;
    }
    return phongShader;
}