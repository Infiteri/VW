#pragma once

#include "Base.h"
#include "Mesh/Model.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace VW
{
    class VW_API ModelSystem
    {
    public:
        struct State
        {
            std::unordered_map<std::string, std::shared_ptr<Model>> Models;
        };

    public:
        ModelSystem() = delete;
        ~ModelSystem() = delete;

        static void Init();
        static void Shutdown();

        static std::shared_ptr<Model> GetModel(const std::string &name);
        static std::shared_ptr<Model> LoadModel(const std::string &name, const std::string &path);
    };
} // namespace VW
