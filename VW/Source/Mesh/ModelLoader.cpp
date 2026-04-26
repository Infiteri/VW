#include "ModelLoader.h"
#include "Core/Logger.h"
#include "fast_obj.h"
#include <cstring>
#include <vector>

namespace VW
{
    struct VertexKey
    {
        u32 p, n, t;
        bool operator==(const VertexKey &other) const
        {
            return p == other.p && n == other.n && t == other.t;
        }
    };

    struct KeyHash
    {
        size_t operator()(const VertexKey &key) const
        {
            return key.p ^ (key.n << 10) ^ (key.t << 20);
        }
    };

    std::shared_ptr<Mesh> ModelLoader::LoadObj(const std::string &path)
    {
        fastObjMesh *mesh = fast_obj_read(path.c_str());
        if (!mesh)
        {
            VW_ERROR("vwrn", "Failed to load model: %s", path.c_str());
            return nullptr;
        }

        std::vector<Vertex> vertices;
        std::vector<u32> indices;
        vertices.reserve(mesh->face_count * 3);
        indices.reserve(mesh->face_count * 3);

        std::unordered_map<VertexKey, u32, KeyHash> uniqueVertices;
        uniqueVertices.reserve(mesh->face_count * 3);

        u32 index_offset = 0;
        for (u32 i = 0; i < mesh->face_count; i++)
        {
            u32 face_vertices = mesh->face_vertices[i];
            if (face_vertices < 3)
            {
                index_offset += face_vertices;
                continue;
            }

            for (u32 j = 1; j < face_vertices - 1; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    u32 vertexIndex = (k == 0) ? 0 : (k == 1) ? j : j + 1;
                    fastObjIndex idx = mesh->indices[index_offset + vertexIndex];
                    VertexKey key = {idx.p, idx.n, idx.t};

                    auto it = uniqueVertices.find(key);
                    if (it != uniqueVertices.end())
                    {
                        indices.push_back(it->second);
                    }
                    else
                    {
                        Vertex vertex = {};

                        if (idx.p < mesh->position_count)
                        {
                            vertex.Position.x = mesh->positions[idx.p * 3 + 0];
                            vertex.Position.y = mesh->positions[idx.p * 3 + 1];
                            vertex.Position.z = mesh->positions[idx.p * 3 + 2];
                        }

                        if (idx.t < mesh->texcoord_count)
                        {
                            vertex.UV.x = mesh->texcoords[idx.t * 2 + 0];
                            vertex.UV.y = 1.0f - mesh->texcoords[idx.t * 2 + 1];
                        }

                        if (idx.n < mesh->normal_count)
                        {
                            vertex.Normal.x = mesh->normals[idx.n * 3 + 0];
                            vertex.Normal.y = mesh->normals[idx.n * 3 + 1];
                            vertex.Normal.z = mesh->normals[idx.n * 3 + 2];
                        }

                        u32 newIndex = (u32)vertices.size();
                        vertices.push_back(vertex);
                        uniqueVertices[key] = newIndex;
                        indices.push_back(newIndex);
                    }
                }
            }
            index_offset += face_vertices;
        }

        // tangent + bitangent
        for (size_t i = 0; i + 2 < indices.size(); i += 3)
        {
            Vertex &v0 = vertices[indices[i]];
            Vertex &v1 = vertices[indices[i + 1]];
            Vertex &v2 = vertices[indices[i + 2]];

            Vector3 deltaPos1 = v1.Position - v0.Position;
            Vector3 deltaPos2 = v2.Position - v0.Position;
            Vector2 deltaUV1 = v1.UV - v0.UV;
            Vector2 deltaUV2 = v2.UV - v0.UV;

            float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y + 0.00001f);
            Vector3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
            Vector3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

            v0.Tangent += tangent;
            v1.Tangent += tangent;
            v2.Tangent += tangent;
            v0.Bitangent += bitangent;
            v1.Bitangent += bitangent;
            v2.Bitangent += bitangent;
        }

        for (auto &v : vertices)
        {
            if (v.Tangent.LengthSquared() > 0.0f)
                v.Tangent = v.Tangent.Normalized();
            if (v.Bitangent.LengthSquared() > 0.0f)
                v.Bitangent = v.Bitangent.Normalized();
        }

        // clamp bad indices
        for (size_t i = 0; i < indices.size(); i++)
            if (indices[i] >= vertices.size())
                indices[i] = 0;

        VertexLayout layout;
        layout.Stride = sizeof(Vertex);
        layout.Attributes = {
            {0, offsetof(Vertex, Position), 3, false},  {1, offsetof(Vertex, UV), 2, false},
            {2, offsetof(Vertex, Normal), 3, false},    {3, offsetof(Vertex, Tangent), 3, false},
            {4, offsetof(Vertex, Bitangent), 3, false},
        };

        fast_obj_destroy(mesh);

        if (vertices.empty() || indices.empty())
        {
            VW_ERROR("vwrn", "Model empty");
            return nullptr;
        }

        return std::make_shared<Mesh>(vertices.data(), vertices.size() * sizeof(Vertex),
                                      indices.data(), indices.size(), layout);
    }
} // namespace VW
