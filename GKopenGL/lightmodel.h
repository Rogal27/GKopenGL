#pragma once

#include <assimp/scene.h>

#include <vector>
#include <iostream>

#include <glm/glm.hpp>

#include "lightmesh.h"

class Shader;

class LightModel
{
public:
    LightMesh mesh;

    LightModel(std::string path);
    LightModel(LightModel* model);
    ~LightModel();
    void Draw(Shader& shader);
    void SetModelMatrix(glm::mat4 model);
    glm::mat4 GetModelMatrix();
    void Translate(glm::vec3 vector);
    void Scale(glm::vec3 scale);
    void Rotate(float angle, glm::vec3 axis);
    glm::vec3 GetFirstPoint();
    glm::vec3 GetCenter();

private:
    glm::mat4 modelMatrix;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    LightMesh processMesh(aiMesh* mesh, const aiScene* scene);
};
