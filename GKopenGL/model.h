#pragma once

#include <assimp/scene.h>

#include <vector>
#include <iostream>

#include <glm/glm.hpp>

class Mesh;
struct Texture;
class Shader;
enum class TextureType;

class Model
{
private:
    glm::mat4 modelMatrix;
    glm::mat3 normalMatrix;
    glm::mat4 positionMatrix;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory);

public:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    bool IsFloor;

    //bool gammaCorrection;

    Model(std::string path);
    Model(Model* model);
    ~Model();
    void Draw(Shader& shader);
    void SetModelMatrix(glm::mat4 model);
    void SetPositionMatrix(glm::mat4 position);
    glm::mat4 GetModelMatrix();
    glm::mat3 GetNormalMatrix();
    glm::mat4 GetPositionMatrix();
    void Translate(glm::vec3 vector);
    void Scale(glm::vec3 scale);
    void Rotate(float angle, glm::vec3 axis);
    glm::vec3 GetFirstPoint();
};
