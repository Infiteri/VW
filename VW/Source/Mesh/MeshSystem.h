#pragma once

#include "Base.h"
#include "Mesh/Mesh.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace VW
{
    enum class MeshType
    {
        Cube
    };

    class VW_API MeshSystem
    {
    public:
        struct State
        {
            std::unordered_map<MeshType, std::shared_ptr<Mesh>> BuiltinMeshes;
            std::unordered_map<std::string, std::shared_ptr<Mesh>> ModelMeshes;
        };

    public:
        MeshSystem() = delete;
        ~MeshSystem() = delete;

        static void Init();
        static void Shutdown();

        static std::shared_ptr<Mesh> GetMesh(MeshType type);
        static std::shared_ptr<Mesh> GetMesh(const std::string &name);

        static std::shared_ptr<Mesh> LoadModel(const std::string &name, const std::string &path);
    };
} // namespace VW
