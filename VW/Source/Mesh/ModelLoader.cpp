#include "ModelLoader.h"
#include "Core/Logger.h"
#include "Material/MaterialSystem.h"
#include "Texture/TextureSystem.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>
namespace VW
{
    static std::shared_ptr<Mesh> ProcessMesh(aiMesh *mesh, const aiScene *scene)
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
                v.UV = {mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y};
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
                            std::vector<ModelSubmesh> &submeshes)
    {
        for (u32 i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            ModelSubmesh sub;
            sub.Mesh = ProcessMesh(mesh, scene);
            sub.Name = node->mName.C_Str();
            sub.LocalTransform = Matrix4(&node->mTransformation[0][0]);
            if (mesh->mMaterialIndex < scene->mNumMaterials)
            {
                aiMaterial *aiMat = scene->mMaterials[mesh->mMaterialIndex];
                aiString name;
                aiMat->Get(AI_MATKEY_NAME, name);
                sub.MaterialName = name.C_Str();
            }
            submeshes.push_back(std::move(sub));
        }
        for (u32 i = 0; i < node->mNumChildren; i++)
            ProcessNode(node->mChildren[i], scene, submeshes);
    }
    static void CreateMaterialFromAssimp(aiMaterial *aiMat)
    {
        aiString name;
        aiMat->Get(AI_MATKEY_NAME, name);
        std::string matName = name.C_Str();
        if (MaterialSystem::Exists(matName))
            return;
        Material mat;
        aiString path;
        if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            mat.SetAlbedoID(TextureSystem::GetTextureID(path.C_Str()));
        if (aiMat->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
            mat.SetNormalID(TextureSystem::GetTextureID(path.C_Str()));
        else if (aiMat->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS)
            mat.SetNormalID(TextureSystem::GetTextureID(path.C_Str()));
        // assimp doesn't have ORM directly, pack from roughness/metallic/ao
        ai_real val;
        if (aiMat->Get(AI_MATKEY_METALLIC_FACTOR, val) == AI_SUCCESS)
        {
            // no separate ORM texture, just use scalar defaults
        }
        // TODO: try loading ORM texture from naming convention or custom channel
        mat.SetORMID(TextureSystem::GetDefaultTextureID());
        MaterialSystem::AddMaterial(matName, mat);
    }

    std::shared_ptr<Model> ModelLoader::Load(const std::string &path)
    {
        return LoadWithMaterial(path, true);
    }

    std::shared_ptr<Model> ModelLoader::LoadWithMaterial(const std::string &path,
                                                         bool createMaterials)
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
        auto model = std::make_shared<Model>();
        ProcessNode(scene->mRootNode, scene, model->m_Submeshes);
        if (model->m_Submeshes.empty())
        {
            VW_ERROR("vwrn", "Model empty: %s", path.c_str());
            return nullptr;
        }
        if (createMaterials)
        {
            for (u32 i = 0; i < scene->mNumMaterials; i++)
                CreateMaterialFromAssimp(scene->mMaterials[i]);
        }
        VW_INFO("vwrn", "Loaded model '%s' (%zu submeshes)", path.c_str(),
                model->m_Submeshes.size());
        return model;
    }
} // namespace VW
