#pragma once

#include "Shader/Shader.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Base.h"

namespace VW
{
    class VW_API ShaderSystem
    {
    public:
        struct State
        {
            std::unordered_map<std::string, std::unique_ptr<Shader>> Shaders;
        };

    public:
        ShaderSystem() = delete;
        ~ShaderSystem() = delete;

        static void Init();
        static void Shutdown();

        static Shader *GetShader(const std::string &path);
        static Shader *GetEngineShader(const std::string &path);
        static Shader *LoadShader(const std::string &path);
        static void LoadShaders(const std::vector<std::string> &path);
        static void DeleteShader(const std::string &path);
    };
} // namespace VW
