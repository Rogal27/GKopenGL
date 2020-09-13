#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "lightmodel.h"
#include "shader.h"

using namespace std;

LightModel::LightModel(string path)
{
    modelMatrix = glm::mat4(1.0f);

    loadModel(path);
}

LightModel::LightModel(LightModel* model)
{
    this->modelMatrix = model->modelMatrix;
    this->mesh = model->mesh;
}

LightModel::~LightModel()
{
    mesh.DeleteMesh();
}

void LightModel::Draw(Shader& shader)
{
    shader.setMat4("model", modelMatrix);

    mesh.Draw(shader);
}

void LightModel::SetModelMatrix(glm::mat4 model)
{
    modelMatrix = model;
}

glm::mat4 LightModel::GetModelMatrix()
{
    return modelMatrix;
}

void LightModel::Translate(glm::vec3 vector)
{
    SetModelMatrix(glm::translate(modelMatrix, vector));
}

void LightModel::Scale(glm::vec3 scale)
{
    SetModelMatrix(glm::scale(modelMatrix, scale));
}

void LightModel::Rotate(float angle, glm::vec3 axis)
{
    SetModelMatrix(glm::rotate(modelMatrix, angle, axis));
}

glm::vec3 LightModel::GetFirstPoint()
{
    return mesh.vertices[0];
}

glm::vec3 LightModel::GetCenter()
{
    const auto get_minmax = [&](auto selector) {
        return std::minmax_element(mesh.vertices.rbegin(), mesh.vertices.rend(), [selector](const auto& p1, const auto& p2) { return selector(p1) < selector(p2); });
    };

    const auto [xMin, xMax] = get_minmax([](auto p) { return p.x; });
    const auto [yMin, yMax] = get_minmax([](auto p) { return p.y; });
    const auto [zMin, zMax] = get_minmax([](auto p) { return p.z; });

    return glm::vec3(xMax->x - xMin->x, yMax->y - yMin->y, zMax->z - zMin->z);    
}

void LightModel::loadModel(string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }

    processNode(scene->mRootNode, scene);
}

void LightModel::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->mesh = processMesh(mesh, scene);
        break;
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

LightMesh LightModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    vector<glm::vec3> vertices;
    vector<unsigned int> indices;

    // Walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 vector;
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;

        vertices.push_back(vector);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
   
    return LightMesh(vertices, indices);
}
