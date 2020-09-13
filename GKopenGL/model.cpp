#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>

#include "model.h"
#include "mesh.h"
#include "shader.h"

#include "stb_image.h"

using namespace std;

namespace texturesPath
{
    const char* EmptySpecularDirectoryPath = "models/textures";
    const char* EmptySpecularFileName = "empty_specular.jpg";
}

Model::Model(string path)
{
    modelMatrix = glm::mat4(1.0f);
    normalMatrix = glm::mat4(1.0f);
    IsFloor = false;
    loadModel(path);
}

Model::Model(Model* model)
{
    this->modelMatrix = model->modelMatrix;
    this->normalMatrix = model->normalMatrix;
    this->meshes = model->meshes;
    this->directory = model->directory;
    this->textures_loaded = model->textures_loaded;
    this->IsFloor = model->IsFloor;
}

Model::~Model()
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].DeleteMesh();
}

void Model::Draw(Shader& shader)
{
    if (IsFloor)
    {
        int scale = 5;
        float size = 2.0f;
        glm::mat4 modelMatrixClone = modelMatrix;
        for (int x = -10; x < 10; x++)
        {
            for (int y = -10; y < 10; y++)
            {
                modelMatrixClone = modelMatrix;
                
                modelMatrixClone = glm::scale(modelMatrixClone, glm::vec3(scale, scale, scale));
                modelMatrixClone = glm::translate(modelMatrixClone, glm::vec3(x * size, 0.0f, y * size));

                shader.setMat4("model", modelMatrixClone);
                shader.setMat3("NormalMatrix", glm::mat3(glm::transpose(glm::inverse(modelMatrixClone))));

                for (unsigned int i = 0; i < meshes.size(); i++)
                    meshes[i].Draw(shader);
            }
        }        
    }
    else
    {
        shader.setMat4("model", modelMatrix);
        shader.setMat3("NormalMatrix", normalMatrix);

        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
}

void Model::loadModel(string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('\\'));

    processNode(scene->mRootNode, scene);
}

void Model::SetModelMatrix(glm::mat4 model)
{
    modelMatrix = model;
    normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
}

glm::mat4 Model::GetModelMatrix()
{
    return modelMatrix;
}

glm::mat3 Model::GetNormalMatrix()
{
    return normalMatrix;
}

void Model::Translate(glm::vec3 vector)
{
    SetModelMatrix(glm::translate(modelMatrix, vector));
}

void Model::Scale(glm::vec3 scale)
{
    SetModelMatrix(glm::scale(modelMatrix, scale));
}

void Model::Rotate(float angle ,glm::vec3 axis)
{
    SetModelMatrix(glm::rotate(modelMatrix, angle, axis));
}

glm::vec3 Model::GetFirstPoint()
{
    return meshes[0].vertices[0].Position;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // Walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        // texture coordinates
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        //// tangent
        //vector.x = mesh->mTangents[i].x;
        //vector.y = mesh->mTangents[i].y;
        //vector.z = mesh->mTangents[i].z;
        //vertex.Tangent = vector;
        //// bitangent
        //vector.x = mesh->mBitangents[i].x;
        //vector.y = mesh->mBitangents[i].y;
        //vector.z = mesh->mBitangents[i].z;
        //vertex.Bitangent = vector;
        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular);
    if (specularMaps.size() == 0)
    {
        bool skip = false;
        string str = string(texturesPath::EmptySpecularDirectoryPath) + "/" + string(texturesPath::EmptySpecularFileName);
        for (size_t i = 0; i < textures_loaded.size(); i++)
        {
            if (textures_loaded[i].path.compare(str) == 0)
            {
                textures.push_back(textures_loaded[i]);
                skip = true;
                break;
            }
        }
        if (skip == false)
        {
            Texture texture;
            texture.id = TextureFromFile(texturesPath::EmptySpecularFileName, texturesPath::EmptySpecularDirectoryPath);
            texture.type = TextureType::Specular;
            texture.path = str;
            specularMaps.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    //// 3. normal maps
    //std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    //textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    //// 4. height maps
    //std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    //textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName)
{
    vector<Texture> textures;
    std::cout << "Loading textures...";
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    std::cout << "Done" << endl;
    return textures;
}

unsigned int Model::TextureFromFile(const char* path, const string& directory)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    cout << "loading texture from file: " + filename << endl;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        cout << "Texture failed to load at path: " << path << endl;
        stbi_image_free(data);
    }

    return textureID;
}
