#include "ModelLoader.h"
#include "Core/Logger.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

namespace VW
{
    static std::shared_ptr<Mesh> ProcessMesh(aiMesh *mesh)
    {
        std::vector<Vertex> vertices;
        std::vector<u32> indices;
        vertices.reserve(mesh->mNumVertices);

        for (u32 i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex v = {};
            v.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

            if (mesh->HasNormals())
                v.Normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

            if (mesh->mTextureCoords[0])
            {
                v.UV = {mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y};
            }

            if (mesh->HasTangentsAndBitangents())
            {
                v.Tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
                v.Bitangent = {mesh->mBitangents[i].x, mesh->mBitangents[i].y,
                               mesh->mBitangents[i].z};
            }

            vertices.push_back(v);
        }

        for (u32 i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace &face = mesh->mFaces[i];
            for (u32 j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        VertexLayout layout;
        layout.Stride = sizeof(Vertex);
        layout.Attributes = {
            {0, offsetof(Vertex, Position), 3, false},  {1, offsetof(Vertex, UV), 2, false},
            {2, offsetof(Vertex, Normal), 3, false},    {3, offsetof(Vertex, Tangent), 3, false},
            {4, offsetof(Vertex, Bitangent), 3, false},
        };

        return std::make_shared<Mesh>(vertices.data(), vertices.size() * sizeof(Vertex),
                                      indices.data(), indices.size(), layout);
    }

    static void ProcessNode(aiNode *node, const aiScene *scene,
                            std::vector<std::shared_ptr<Mesh>> &meshes)
    {
        for (u32 i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh));
        }

        for (u32 i = 0; i < node->mNumChildren; i++)
            ProcessNode(node->mChildren[i], scene, meshes);
    }

    std::shared_ptr<Mesh> ModelLoader::LoadObj(const std::string &path)
    {
        Assimp::Importer importer;
        const aiScene *scene =
            importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                        aiProcess_CalcTangentSpace | aiProcess_GenNormals);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            VW_ERROR("vwrn", "Assimp: %s", importer.GetErrorString());
            return nullptr;
        }

        std::vector<std::shared_ptr<Mesh>> meshes;
        ProcessNode(scene->mRootNode, scene, meshes);

        if (meshes.empty())
        {
            VW_ERROR("vwrn", "Model empty: %s", path.c_str());
            return nullptr;
        }

        return meshes[0]; // return first mesh, or merge if needed
    }
} // namespace VW
