#include "ModelLoader.h"
#include "Core/Logger.h"
#include "Material/MaterialSystem.h"
#include "Texture/TextureSystem.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stb_image.h>
#include <vector>
namespace VW
{
    static std::string GetTextureDir(const std::string &modelPath)
    {
        auto pos = modelPath.find_last_of("/\\");
        if (pos != std::string::npos)
            return modelPath.substr(0, pos + 1) + "textures/";
        return "textures/";
    }

    static u32 LoadEmbeddedTexture(const aiScene *scene, const char *path,
                                    const std::string &cacheKey)
    {
        if (path[0] != '*')
            return (u32)TextureSystem::GetTextureID(path);

        int index = std::atoi(path + 1);
        if (index < 0 || index >= (int)scene->mNumTextures)
            return (u32)TextureSystem::GetDefaultTextureID();

        aiTexture *tex = scene->mTextures[index];
        if (tex->mHeight == 0)
        {
            int w, h, c;
            u8 *pixels = stbi_load_from_memory(
                reinterpret_cast<const u8 *>(tex->pcData),
                static_cast<int>(tex->mWidth),
                &w, &h, &c, 0);
            if (!pixels)
            {
                VW_WARN("vwrn", "Failed to decode embedded texture: %s", cacheKey.c_str());
                return (u32)TextureSystem::GetDefaultTextureID();
            }
            u32 id = (u32)TextureSystem::CreateTextureFromData(cacheKey, pixels, w, h, c);
            stbi_image_free(pixels);
            VW_INFO("vwrn", "Loaded embedded texture: %s", cacheKey.c_str());
            return id;
        }
        VW_WARN("vwrn", "Uncompressed embedded texture not supported: %s", cacheKey.c_str());
        return (u32)TextureSystem::GetDefaultTextureID();
    }

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
    static void CreateMaterialFromAssimp(aiMaterial *aiMat, const aiScene *scene,
                                          const std::string &textureDir)
    {
        aiString name;
        aiMat->Get(AI_MATKEY_NAME, name);
        std::string matName = name.C_Str();
        if (matName.empty() || MaterialSystem::Exists(matName))
            return;

        Material mat;
        aiString path;

        if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
        {
            std::string cacheKey = textureDir + matName + "_albedo.png";
            mat.SetAlbedoID(LoadEmbeddedTexture(scene, path.C_Str(), cacheKey));
        }

        if (aiMat->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
        {
            std::string cacheKey = textureDir + matName + "_normal.png";
            mat.SetNormalID(LoadEmbeddedTexture(scene, path.C_Str(), cacheKey));
        }
        else if (aiMat->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS)
        {
            std::string cacheKey = textureDir + matName + "_normal.png";
            mat.SetNormalID(LoadEmbeddedTexture(scene, path.C_Str(), cacheKey));
        }

        // GLB metallic-roughness packs occlusion(R), roughness(G), metallic(B)
        if (aiMat->GetTexture(aiTextureType_METALNESS, 0, &path) == AI_SUCCESS)
        {
            std::string cacheKey = textureDir + matName + "_orm.png";
            mat.SetORMID(LoadEmbeddedTexture(scene, path.C_Str(), cacheKey));
        }

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
            std::string textureDir = GetTextureDir(path);
            for (u32 i = 0; i < scene->mNumMaterials; i++)
                CreateMaterialFromAssimp(scene->mMaterials[i], scene, textureDir);
        }
        VW_INFO("vwrn", "Loaded model '%s' (%zu submeshes)", path.c_str(),
                model->m_Submeshes.size());
        return model;
    }
} // namespace VW
