#include "MeshSystem.h"
#include "Mesh/ModelLoader.h"

namespace VW
{
    static MeshSystem::State s_State;

    static void LoadCubeMesh()
    {
#define S 0.5f
        Vertex vertices[] = {
            // Front (z = -S)
            {Vector3{-S, -S, -S}, Vector2{0.0f, 0.0f}},
            {Vector3{S, -S, -S}, Vector2{1.0f, 0.0f}},
            {Vector3{S, S, -S}, Vector2{1.0f, 1.0f}},
            {Vector3{-S, S, -S}, Vector2{0.0f, 1.0f}},
            // Back (z = +S)
            {Vector3{S, -S, S}, Vector2{0.0f, 0.0f}},
            {Vector3{-S, -S, S}, Vector2{1.0f, 0.0f}},
            {Vector3{-S, S, S}, Vector2{1.0f, 1.0f}},
            {Vector3{S, S, S}, Vector2{0.0f, 1.0f}},
            // Left (x = -S)
            {Vector3{-S, -S, S}, Vector2{0.0f, 0.0f}},
            {Vector3{-S, -S, -S}, Vector2{1.0f, 0.0f}},
            {Vector3{-S, S, -S}, Vector2{1.0f, 1.0f}},
            {Vector3{-S, S, S}, Vector2{0.0f, 1.0f}},
            // Right (x = +S)
            {Vector3{S, -S, -S}, Vector2{0.0f, 0.0f}},
            {Vector3{S, -S, S}, Vector2{1.0f, 0.0f}},
            {Vector3{S, S, S}, Vector2{1.0f, 1.0f}},
            {Vector3{S, S, -S}, Vector2{0.0f, 1.0f}},
            // Top (y = +S)
            {Vector3{-S, S, -S}, Vector2{0.0f, 0.0f}},
            {Vector3{S, S, -S}, Vector2{1.0f, 0.0f}},
            {Vector3{S, S, S}, Vector2{1.0f, 1.0f}},
            {Vector3{-S, S, S}, Vector2{0.0f, 1.0f}},
            // Bottom (y = -S)
            {Vector3{-S, -S, S}, Vector2{0.0f, 0.0f}},
            {Vector3{S, -S, S}, Vector2{1.0f, 0.0f}},
            {Vector3{S, -S, -S}, Vector2{1.0f, 1.0f}},
            {Vector3{-S, -S, -S}, Vector2{0.0f, 1.0f}},
        };

        u32 indices[] = {
            0,  2,  1,  0,  3,  2,  4,  6,  5,  4,  7,  6,  8,  10, 9,  8,  11, 10,
            12, 14, 13, 12, 15, 14, 16, 18, 17, 16, 19, 18, 20, 22, 21, 20, 23, 22,
        };

        VertexLayout layout;
        layout.Stride = sizeof(Vertex);
        layout.Attributes = {
            {0, offsetof(Vertex, Position), 3, false},
            {1, offsetof(Vertex, UV), 2, false},
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
