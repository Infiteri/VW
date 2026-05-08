#pragma once

#include "Base.h"
#include "Color.h"
#include "Math/Vector.h"
#include "Shader.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

namespace VW
{
    using UniformValue = std::variant<int, float, Vector2, Vector3, Vector4, Color>;
    using UniformMap = std::unordered_map<std::string, UniformValue>;

    class VW_API ShaderUniforms
    {
    public:
        ShaderUniforms() = default;
        ~ShaderUniforms() = default;

        void AddUniform(const std::string &name, const UniformValue &value);
        void RemoveUniform(const std::string &name);

        void Apply(Shader *shader);

        inline const UniformMap &GetUniforms() const
        {
            return m_Uniforms;
        };

    private:
        UniformMap m_Uniforms;
    };
} // namespace VW
