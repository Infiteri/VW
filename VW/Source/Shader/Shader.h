#pragma once

#include "Base.h"
#include "Math/Matrix.h"
#include <string>
#include <unordered_map>
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
        void Use();
        bool IsLoaded() const;

        void Int(int i, const char *name);
        void Mat4(const Matrix4 &n, const char *name);

        inline u32 GetID() const
        {
            return m_ID;
        };

    private:
        u32 m_ID;
        std::unordered_map<std::string, u32> m_Locations;

    private:
        int _ComposeSources(const std::string &path, std::string &vs, std::string &fs);
        u32 _LoadShader(const std::string &str, u32 type);
        u32 _GetUniform(const char *name);
    };
} // namespace VW
