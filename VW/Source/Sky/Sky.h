#pragma once

#include "Base.h"
#include "Buffer/VertexArray.h"
#include "Color.h"
#include "Shader/Shader.h"
#include "Texture/CubemapTexture.h"
#include <memory>

namespace VW
{
    enum class SkyMode
    {
        Color,
        Skybox
    };

    class VW_API Sky
    {
    public:
        Sky();
        ~Sky();

        void Render();

        void SetColorMode(const Color &color);
        void SetSkyboxMode(const CubemapTexture::Configuration &config);

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
            Shader *Shader;
        } m_SkyboxData;

    private:
        void _EnsureVA();
    };
} // namespace VW
