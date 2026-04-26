#include "MeshSystem.h"
#include "Mesh/ModelLoader.h"

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

    void MeshSystem::Init()
    {
        // load builtin meshes
        LoadCubeMesh();
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

    std::shared_ptr<Mesh> MeshSystem::GetMesh(const std::string &name)
    {
        if (s_State.ModelMeshes.find(name) == s_State.ModelMeshes.end())
            return nullptr;

        return s_State.ModelMeshes[name];
    }

    std::shared_ptr<Mesh> MeshSystem::LoadModel(const std::string &name, const std::string &path)
    {
        if (s_State.ModelMeshes.find(name) != s_State.ModelMeshes.end())
        {
            // TODO: warn
            return s_State.ModelMeshes[name];
        }

        // TODO: check if real mesh
        auto mesh = ModelLoader::LoadObj(path);
        s_State.ModelMeshes[name] = mesh;

        return s_State.ModelMeshes[name];
    }

} // namespace VW
