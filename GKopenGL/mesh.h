#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "shader.h"

using namespace std;


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};


struct Texture {
    unsigned int id;
    string type;
};


class Mesh {
public:
    /*  Mesh Data  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    /*  Functions  */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    ~Mesh();
    void Draw(Shader shader);
private:
    /*  Render data  */
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    /*  Functions    */
    void setupMesh();
};