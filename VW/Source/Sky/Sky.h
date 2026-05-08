#pragma once

#include "Base.h"
#include "Buffer/VertexArray.h"
#include "Color.h"
#include "Shader/Shader.h"
#include "Shader/ShaderUniforms.h"
#include "Texture/CubemapTexture.h"
#include <memory>
#include <string>

namespace VW
{
    enum class SkyMode
    {
        Color,
        Skybox,
        Shader
    };

    class VW_API Sky
    {
    public:
        Sky();
        ~Sky();

        void Render();

        void SetColorMode(const Color &color);
        void SetSkyboxMode(const CubemapTexture::Configuration &config);
        void SetShaderMode(const std::string &path);

        inline SkyMode GetMode() const
        {
            return m_Mode;
        };

        inline ShaderUniforms &GetShaderUniforms()
        {
            return m_ShaderData.Uniforms;
        };

    private:
        SkyMode m_Mode;
        std::unique_ptr<VertexArray> VA;

        struct
        {
            Color Color;
        } m_ColorData;

        struct
        {
            CubemapTexture Texture;
        } m_SkyboxData;

        struct
        {
            Shader *Shader;
            ShaderUniforms Uniforms;
        } m_ShaderData;

    private:
        void _EnsureVA();
    };
} // namespace VW
