#include "ShaderUniforms.h"
#include <type_traits>
#include <variant>

namespace VW
{
    void ShaderUniforms::AddUniform(const std::string &name, const UniformValue &value)
    {
        if (m_Uniforms.find(name) != m_Uniforms.end())
            return;

        m_Uniforms[name] = value;
    }

    void ShaderUniforms::RemoveUniform(const std::string &name)
    {
        m_Uniforms.erase(name);
    }

    UniformValue &ShaderUniforms::GetUniform(const std::string &name)
    {
        return m_Uniforms[name];
    }

    void ShaderUniforms::Apply(Shader *shader)
    {
        if (!shader)
            return;

        for (const auto &[name, value] : m_Uniforms)
        {
            std::visit(
                [&](auto &&arg)
                {
                    using T = std::decay_t<decltype(arg)>;

#define CE_UNI_HELPER(t, func) \
    if constexpr (std::is_same_v<T, t>) \
    { \
        shader->func(arg, name.c_str()); \
    }

                    CE_UNI_HELPER(int, Int)               //
                    else CE_UNI_HELPER(float, Float)      //
                        else CE_UNI_HELPER(Vector2, Vec2) //
                        else CE_UNI_HELPER(Vector3, Vec3) //
                        else CE_UNI_HELPER(Vector4, Vec4) //
                        else CE_UNI_HELPER(Color, Color)  //
                },
                value);
        }
    }
} // namespace VW
