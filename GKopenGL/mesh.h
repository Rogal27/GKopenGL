#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "shader.h"

using namespace std;

enum class TextureType
{
    Diffuse,
    Specular
};

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    //// tangent
    //glm::vec3 Tangent;
    //// bitangent
    //glm::vec3 Bitangent;
};


struct Texture {
    unsigned int id;
    TextureType type;
    string path;  // we store the path of the texture to compare with other textures
};


class Mesh {
public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    /*  Functions  */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void DeleteMesh();
    void Draw(Shader& shader);
private:
    /*  Render data  */
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    /*  Functions    */
    void setupMesh();
};