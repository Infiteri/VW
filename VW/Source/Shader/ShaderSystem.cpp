#include "ShaderSystem.h"
#include "Shader/Shader.h"
#include <filesystem>
#include <memory>
#include <string>

namespace VW
{
    static ShaderSystem::State s_State;

    static constexpr const char *ASSETS_PATH = "";

    void ShaderSystem::Init()
    {
        std::string paths[] = {"Cubemap.glsl", "Object.glsl", "Screen.glsl"};

        for (auto p2 : paths)
        {
            // yey std::filesystem now compiles :)
            std::filesystem::path p(ASSETS_PATH);
            p /= p2;
            LoadShader(p.string());
        }
    }

    void ShaderSystem::Shutdown()
    {
    }

    Shader *ShaderSystem::GetShader(const std::string &path)
    {
        if (s_State.Shaders.find(path) == s_State.Shaders.end())
            return LoadShader(path);

        return s_State.Shaders[path].get();
    }

    Shader *ShaderSystem::GetEngineShader(const std::string &path)
    {
        std::filesystem::path p(ASSETS_PATH);
        p /= path;
        return GetShader(p.string());
    }

    Shader *ShaderSystem::LoadShader(const std::string &path)
    {
        if (s_State.Shaders.find(path) != s_State.Shaders.end())
            return s_State.Shaders[path].get();

        auto shader = std::make_unique<Shader>(path);
        s_State.Shaders[path] = std::move(shader);
        return s_State.Shaders[path].get();
    }

    void ShaderSystem::LoadShaders(const std::vector<std::string> &path)
    {
        for (const std::string &p : path)
            LoadShader(p);
    }

    void ShaderSystem::DeleteShader(const std::string &path)
    {
        if (!GetShader(path))
            return;

        s_State.Shaders.erase(path);
    }
} // namespace VW
