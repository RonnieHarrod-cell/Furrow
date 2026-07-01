#include "engine/assets/ModelLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>
#include <iostream>

namespace engine
{

    std::unique_ptr<Mesh> ModelLoader::Load(const std::string &path)
    {
        Assimp::Importer importer;

        const aiScene *scene = importer.ReadFile(
            path,
            aiProcess_Triangulate | aiProcess_GenSmoothNormals);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            throw std::runtime_error("Assimp failed to load model: " + path + " (" + importer.GetErrorString() + ")");
        }

        if (scene->mNumMeshes == 0)
        {
            throw std::runtime_error("Model file has no meshes: " + path);
        }

        // Only reading the first mesh for now - see header comment.
        aiMesh *mesh = scene->mMeshes[0];

        std::vector<Vertex> vertices;
        vertices.reserve(mesh->mNumVertices);

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

            if (mesh->HasNormals())
            {
                vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            }
            else
            {
                vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f); // fallback, shouldn't hit given GenSmoothNormals
            }

            vertices.push_back(vertex);
        }

        std::vector<unsigned int> indices;
        indices.reserve(mesh->mNumFaces * 3);
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        std::printf("Loaded model '%s': %u vertices, %u triangles\n", path.c_str(), mesh->mNumVertices, mesh->mNumFaces);

        return std::make_unique<Mesh>(vertices, indices);
    }

} // namespace engine