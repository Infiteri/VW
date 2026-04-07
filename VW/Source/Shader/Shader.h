#pragma once

#include "Base.h"
#include <string>
namespace VW
{
    class Shader
    {
    public:
        Shader();
        Shader(const std::string &filepath);
        ~Shader();

        void Load(const std::string &filepath);
        void Unload();

        bool IsLoaded() const;

        void Use();

    private:
        u32 m_ID;

    private:
        int _ComposeSources(const std::string &path, std::string &vs, std::string &fs);
        u32 _LoadShader(const std::string &str, u32 type);
    };
} // namespace VW
