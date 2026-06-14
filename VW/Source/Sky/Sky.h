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

        inline Color GetColor() const
        {
            return m_ColorData.Color;
        };

        inline const CubemapTexture::Configuration &GetSkyboxConfig() const
        {
            return m_SkyboxData.Config;
        };

        inline const std::string &GetShaderPath() const
        {
            return m_ShaderData.Path;
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
            CubemapTexture::Configuration Config;
        } m_SkyboxData;

        struct
        {
            Shader *Shader;
            std::string Path;
            ShaderUniforms Uniforms;
        } m_ShaderData;

    private:
        void _EnsureVA();
    };
} // namespace VW
