#include "MeshSystem.h"
#include <math.h>

namespace VW
{
    static MeshSystem::State s_State;

    static void LoadCubeMesh()
    {
#define S 0.5f
        Vertex vertices[] = {
            // Front (z = -S)  Normal(0,0,-1)
            {Vector3{-S, -S, -S}, Vector2{0, 0}, Vector3{0, 0, -1}, Vector3{1, 0, 0},
             Vector3{0, -1, 0}},
            {Vector3{S, -S, -S}, Vector2{1, 0}, Vector3{0, 0, -1}, Vector3{1, 0, 0},
             Vector3{0, -1, 0}},
            {Vector3{S, S, -S}, Vector2{1, 1}, Vector3{0, 0, -1}, Vector3{1, 0, 0},
             Vector3{0, -1, 0}},
            {Vector3{-S, S, -S}, Vector2{0, 1}, Vector3{0, 0, -1}, Vector3{1, 0, 0},
             Vector3{0, -1, 0}},
            // Back (z = +S)  Normal(0,0,1)
            {Vector3{S, -S, S}, Vector2{0, 0}, Vector3{0, 0, 1}, Vector3{-1, 0, 0},
             Vector3{0, -1, 0}},
            {Vector3{-S, -S, S}, Vector2{1, 0}, Vector3{0, 0, 1}, Vector3{-1, 0, 0},
             Vector3{0, -1, 0}},
            {Vector3{-S, S, S}, Vector2{1, 1}, Vector3{0, 0, 1}, Vector3{-1, 0, 0},
             Vector3{0, -1, 0}},
            {Vector3{S, S, S}, Vector2{0, 1}, Vector3{0, 0, 1}, Vector3{-1, 0, 0},
             Vector3{0, -1, 0}},
            // Left (x = -S)  Normal(-1,0,0)
            {Vector3{-S, -S, S}, Vector2{0, 0}, Vector3{-1, 0, 0}, Vector3{0, 0, -1},
             Vector3{0, -1, 0}},
            {Vector3{-S, -S, -S}, Vector2{1, 0}, Vector3{-1, 0, 0}, Vector3{0, 0, -1},
             Vector3{0, -1, 0}},
            {Vector3{-S, S, -S}, Vector2{1, 1}, Vector3{-1, 0, 0}, Vector3{0, 0, -1},
             Vector3{0, -1, 0}},
            {Vector3{-S, S, S}, Vector2{0, 1}, Vector3{-1, 0, 0}, Vector3{0, 0, -1},
             Vector3{0, -1, 0}},
            // Right (x = +S)  Normal(1,0,0)
            {Vector3{S, -S, -S}, Vector2{0, 0}, Vector3{1, 0, 0}, Vector3{0, 0, 1},
             Vector3{0, -1, 0}},
            {Vector3{S, -S, S}, Vector2{1, 0}, Vector3{1, 0, 0}, Vector3{0, 0, 1},
             Vector3{0, -1, 0}},
            {Vector3{S, S, S}, Vector2{1, 1}, Vector3{1, 0, 0}, Vector3{0, 0, 1},
             Vector3{0, -1, 0}},
            {Vector3{S, S, -S}, Vector2{0, 1}, Vector3{1, 0, 0}, Vector3{0, 0, 1},
             Vector3{0, -1, 0}},
            // Top (y = +S)  Normal(0,1,0)
            {Vector3{-S, S, -S}, Vector2{0, 0}, Vector3{0, 1, 0}, Vector3{1, 0, 0},
             Vector3{0, 0, 1}},
            {Vector3{S, S, -S}, Vector2{1, 0}, Vector3{0, 1, 0}, Vector3{1, 0, 0},
             Vector3{0, 0, 1}},
            {Vector3{S, S, S}, Vector2{1, 1}, Vector3{0, 1, 0}, Vector3{1, 0, 0}, Vector3{0, 0, 1}},
            {Vector3{-S, S, S}, Vector2{0, 1}, Vector3{0, 1, 0}, Vector3{1, 0, 0},
             Vector3{0, 0, 1}},
            // Bottom (y = -S)  Normal(0,-1,0)
            {Vector3{-S, -S, S}, Vector2{0, 0}, Vector3{0, -1, 0}, Vector3{1, 0, 0},
             Vector3{0, 0, -1}},
            {Vector3{S, -S, S}, Vector2{1, 0}, Vector3{0, -1, 0}, Vector3{1, 0, 0},
             Vector3{0, 0, -1}},
            {Vector3{S, -S, -S}, Vector2{1, 1}, Vector3{0, -1, 0}, Vector3{1, 0, 0},
             Vector3{0, 0, -1}},
            {Vector3{-S, -S, -S}, Vector2{0, 1}, Vector3{0, -1, 0}, Vector3{1, 0, 0},
             Vector3{0, 0, -1}},
        };

        u32 indices[] = {
            0,  2,  1,  0,  3,  2,  4,  6,  5,  4,  7,  6,  8,  10, 9,  8,  11, 10,
            12, 14, 13, 12, 15, 14, 16, 18, 17, 16, 19, 18, 20, 22, 21, 20, 23, 22,
        };

        VertexLayout layout;
        layout.Stride = sizeof(Vertex);
        layout.Attributes = {
            {0, offsetof(Vertex, Position), 3, false},  {1, offsetof(Vertex, UV), 2, false},
            {2, offsetof(Vertex, Normal), 3, false},    {3, offsetof(Vertex, Tangent), 3, false},
            {4, offsetof(Vertex, Bitangent), 3, false},
        };

        s_State.BuiltinMeshes[MeshType::Cube] =
            std::make_shared<Mesh>(vertices, sizeof(vertices), indices, 36, layout);
    }

    static void LoadSphereMesh()
    {
        const int stacks = 16;
        const int slices = 16;
        const float R = 0.5f;

        std::vector<Vertex> vertices;
        std::vector<u32> indices;

        for (int i = 0; i <= stacks; i++)
        {
            float phi = M_PI * i / stacks;
            for (int j = 0; j <= slices; j++)
            {
                float theta = 2 * M_PI * j / slices;

                Vector3 pos = {R * sin(phi) * cos(theta), R * cos(phi), R * sin(phi) * sin(theta)};
                Vector2 uv = {(float)j / slices, (float)i / stacks};
                Vector3 normal = {pos.x / R, pos.y / R, pos.z / R};
                Vector3 tangent = {-sin(theta), 0, cos(theta)};
                Vector3 bitangent = {cos(phi) * cos(theta), -sin(phi), cos(phi) * sin(theta)};

                vertices.push_back({pos, uv, normal, tangent, bitangent});
            }
        }

        for (int i = 0; i < stacks; i++)
        {
            for (int j = 0; j < slices; j++)
            {
                u32 a = i * (slices + 1) + j;
                u32 b = a + slices + 1;
                indices.push_back(a);
                indices.push_back(b);
                indices.push_back(a + 1);
                indices.push_back(a + 1);
                indices.push_back(b);
                indices.push_back(b + 1);
            }
        }

        VertexLayout layout;
        layout.Stride = sizeof(Vertex);
        layout.Attributes = {
            {0, offsetof(Vertex, Position), 3, false},  {1, offsetof(Vertex, UV), 2, false},
            {2, offsetof(Vertex, Normal), 3, false},    {3, offsetof(Vertex, Tangent), 3, false},
            {4, offsetof(Vertex, Bitangent), 3, false},
        };

        s_State.BuiltinMeshes[MeshType::Sphere] =
            std::make_shared<Mesh>(vertices.data(), vertices.size() * sizeof(Vertex),
                                   indices.data(), indices.size(), layout);
    }

    static void LoadPlaneMesh()
    {
        Vertex vertices[] = {
            {Vector3{-0.5f, 0, -0.5f}, Vector2{0, 0}, Vector3{0, 1, 0}, Vector3{1, 0, 0},
             Vector3{0, 0, 1}},
            {Vector3{0.5f, 0, -0.5f}, Vector2{1, 0}, Vector3{0, 1, 0}, Vector3{1, 0, 0},
             Vector3{0, 0, 1}},
            {Vector3{0.5f, 0, 0.5f}, Vector2{1, 1}, Vector3{0, 1, 0}, Vector3{1, 0, 0},
             Vector3{0, 0, 1}},
            {Vector3{-0.5f, 0, 0.5f}, Vector2{0, 1}, Vector3{0, 1, 0}, Vector3{1, 0, 0},
             Vector3{0, 0, 1}},
        };
        u32 indices[] = {0, 2, 1, 0, 3, 2};

        VertexLayout layout;
        layout.Stride = sizeof(Vertex);
        layout.Attributes = {
            {0, offsetof(Vertex, Position), 3, false},  {1, offsetof(Vertex, UV), 2, false},
            {2, offsetof(Vertex, Normal), 3, false},    {3, offsetof(Vertex, Tangent), 3, false},
            {4, offsetof(Vertex, Bitangent), 3, false},
        };
        s_State.BuiltinMeshes[MeshType::Plane] =
            std::make_shared<Mesh>(vertices, sizeof(vertices), indices, 6, layout);
    }

    static void LoadCylinderMesh()
    {
        const int slices = 16;
        const float R = 0.5f;
        const float H = 0.5f;

        std::vector<Vertex> vertices;
        std::vector<u32> indices;

        // sides
        for (int i = 0; i <= slices; i++)
        {
            float theta = 2 * M_PI * i / slices;
            float x = cos(theta), z = sin(theta);
            Vector3 normal = {x, 0, z};
            Vector3 tangent = {-sin(theta), 0, cos(theta)};
            Vector3 bitangent = {0, 1, 0};
            float u = (float)i / slices;

            vertices.push_back(
                {Vector3{x * R, -H, z * R}, Vector2{u, 0}, normal, tangent, bitangent});
            vertices.push_back(
                {Vector3{x * R, H, z * R}, Vector2{u, 1}, normal, tangent, bitangent});
        }

        for (int i = 0; i < slices; i++)
        {
            u32 a = i * 2, b = a + 1, c = a + 2, d = a + 3;
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(b);
            indices.push_back(d);
            indices.push_back(c);
        }

        // top cap
        u32 topCenter = vertices.size();
        vertices.push_back({Vector3{0, H, 0}, Vector2{0.5f, 0.5f}, Vector3{0, 1, 0},
                            Vector3{1, 0, 0}, Vector3{0, 0, 1}});
        u32 topStart = vertices.size();
        for (int i = 0; i <= slices; i++)
        {
            float theta = 2 * M_PI * i / slices;
            float x = cos(theta), z = sin(theta);
            vertices.push_back({Vector3{x * R, H, z * R}, Vector2{x * 0.5f + 0.5f, z * 0.5f + 0.5f},
                                Vector3{0, 1, 0}, Vector3{1, 0, 0}, Vector3{0, 0, 1}});
        }
        for (int i = 0; i < slices; i++)
        {
            indices.push_back(topCenter);
            indices.push_back(topStart + i + 1);
            indices.push_back(topStart + i);
        }

        // bottom cap
        u32 botCenter = vertices.size();
        vertices.push_back({Vector3{0, -H, 0}, Vector2{0.5f, 0.5f}, Vector3{0, -1, 0},
                            Vector3{1, 0, 0}, Vector3{0, 0, -1}});
        u32 botStart = vertices.size();
        for (int i = 0; i <= slices; i++)
        {
            float theta = 2 * M_PI * i / slices;
            float x = cos(theta), z = sin(theta);
            vertices.push_back({Vector3{x * R, -H, z * R},
                                Vector2{x * 0.5f + 0.5f, z * 0.5f + 0.5f}, Vector3{0, -1, 0},
                                Vector3{1, 0, 0}, Vector3{0, 0, -1}});
        }
        for (int i = 0; i < slices; i++)
        {
            indices.push_back(botCenter);
            indices.push_back(botStart + i);
            indices.push_back(botStart + i + 1);
        }

        VertexLayout layout;
        layout.Stride = sizeof(Vertex);
        layout.Attributes = {
            {0, offsetof(Vertex, Position), 3, false},  {1, offsetof(Vertex, UV), 2, false},
            {2, offsetof(Vertex, Normal), 3, false},    {3, offsetof(Vertex, Tangent), 3, false},
            {4, offsetof(Vertex, Bitangent), 3, false},
        };
        s_State.BuiltinMeshes[MeshType::Cylinder] =
            std::make_shared<Mesh>(vertices.data(), vertices.size() * sizeof(Vertex),
                                   indices.data(), indices.size(), layout);
    }

    // TODO: figure out a way different toruses can have different parameters
    static void LoadTorusMesh()
    {
        const int rings = 24;
        const int sides = 16;
        const float R = 0.35f; // major radius
        const float r = 0.15f; // minor radius

        std::vector<Vertex> vertices;
        std::vector<u32> indices;

        for (int i = 0; i <= rings; i++)
        {
            float phi = 2 * M_PI * i / rings;
            float cosPhi = cos(phi), sinPhi = sin(phi);

            for (int j = 0; j <= sides; j++)
            {
                float theta = 2 * M_PI * j / sides;
                float cosTheta = cos(theta), sinTheta = sin(theta);

                Vector3 pos = {(R + r * cosTheta) * cosPhi, r * sinTheta,
                               (R + r * cosTheta) * sinPhi};
                Vector3 normal = {cosTheta * cosPhi, sinTheta, cosTheta * sinPhi};
                Vector3 tangent = {-sinPhi, 0, cosPhi};
                Vector3 bitangent = {-sinTheta * cosPhi, cosTheta, -sinTheta * sinPhi};
                Vector2 uv = {(float)i / rings, (float)j / sides};

                vertices.push_back({pos, uv, normal, tangent, bitangent});
            }
        }

        for (int i = 0; i < rings; i++)
        {
            for (int j = 0; j < sides; j++)
            {
                u32 a = i * (sides + 1) + j;
                u32 b = a + sides + 1;
                indices.push_back(a);
                indices.push_back(a + 1);
                indices.push_back(b);
                indices.push_back(b);
                indices.push_back(a + 1);
                indices.push_back(b + 1);
            }
        }

        VertexLayout layout;
        layout.Stride = sizeof(Vertex);
        layout.Attributes = {
            {0, offsetof(Vertex, Position), 3, false},  {1, offsetof(Vertex, UV), 2, false},
            {2, offsetof(Vertex, Normal), 3, false},    {3, offsetof(Vertex, Tangent), 3, false},
            {4, offsetof(Vertex, Bitangent), 3, false},
        };
        s_State.BuiltinMeshes[MeshType::Torus] =
            std::make_shared<Mesh>(vertices.data(), vertices.size() * sizeof(Vertex),
                                   indices.data(), indices.size(), layout);
    }

    void MeshSystem::Init()
    {
        // load builtin meshes
        LoadCubeMesh();
        LoadSphereMesh();
        LoadPlaneMesh();
        LoadCylinderMesh();
        LoadTorusMesh();
    }

    void MeshSystem::Shutdown()
    {
    }

    std::shared_ptr<Mesh> MeshSystem::GetMesh(MeshType type)
    {
        if (s_State.BuiltinMeshes.find(type) == s_State.BuiltinMeshes.end())
            return nullptr;

        return s_State.BuiltinMeshes[type];
    }

} // namespace VW
