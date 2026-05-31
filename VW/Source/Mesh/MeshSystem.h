#pragma once

#include "Base.h"
#include "Mesh/Mesh.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace VW
{
    class VW_API MeshSystem
    {
    public:
        struct State
        {
            std::unordered_map<MeshType, std::shared_ptr<Mesh>> BuiltinMeshes;
        };

    public:
        MeshSystem() = delete;
        ~MeshSystem() = delete;

        static void Init();
        static void Shutdown();

        static std::shared_ptr<Mesh> GetMesh(MeshType type);
    };
} // namespace VW
