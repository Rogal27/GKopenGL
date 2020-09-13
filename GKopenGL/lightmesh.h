#pragma once

#include <glm/glm.hpp>
#include <vector>

class Shader;

using namespace std;

class LightMesh {
public:
    vector<glm::vec3> vertices;
    vector<unsigned int> indices;

    LightMesh() : VAO(0), VBO(0), EBO(0) {};
    LightMesh(vector<glm::vec3> vertices, vector<unsigned int> indices);
    void DeleteMesh();
    void Draw(Shader& shader);
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    void setupMesh();
};
