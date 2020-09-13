#pragma once

#include <assimp/scene.h>

#include <vector>
#include <iostream>

#include <glm/glm.hpp>


class Mesh;
struct Texture;
class Shader;
enum class TextureType;

using namespace std;


class Model
{
private:
    glm::mat4 modelMatrix;
    glm::mat3 normalMatrix;

    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
    unsigned int TextureFromFile(const char* path, const string& directory);

public:
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;
    bool IsFloor;

    //bool gammaCorrection;

    Model(string path);
    Model(Model* model);
    ~Model();
    void Draw(Shader& shader);
    void SetModelMatrix(glm::mat4 model);
    glm::mat4 GetModelMatrix();
    glm::mat3 GetNormalMatrix();
};
