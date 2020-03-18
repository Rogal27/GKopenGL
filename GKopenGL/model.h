#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <iostream>

#include "stb_image.h"

#include "mesh.h"
#include "shader.h"



using namespace std;

class Model
{
public:
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;

    //bool gammaCorrection;

    Model(string path);
    ~Model();
    void Draw(Shader& shader);
private:
    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
    unsigned int TextureFromFile(const char* path, const string& directory);
};