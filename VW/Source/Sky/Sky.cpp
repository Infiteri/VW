#include "Sky.h"
#include "Camera/Camera.h"
#include "Core/Logger.h"
#include "Renderer.h"
#include "Shader/ShaderSystem.h"
#include <glad/glad.h>

namespace VW
{
    static float cubeVertices[] = {
        -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,

        -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f,
        -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

        1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
        1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,

        -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f,
    };

    Sky::Sky()
    {
        SetColorMode({});
    }

    Sky::~Sky()
    {
    }

    static inline void _UploadCameraToShader(Camera *c, Shader *s)
    {
        if (!c || !s)
            return;

        s->Mat4(c->GetProjection(), "uProj");
        Matrix4 view = c->GetView();
        view.data[12] = view.data[13] = view.data[14] = 0;
        s->Mat4(view, "uView");
    }

    void Sky::Render()
    {
        _EnsureVA();
        if (!VA)
            return; // TODO: some warning would be nice, no reason to trigger tho

        glClearColor(1, 0, 0, 1); // debug color, if sky is <- this color, something is wrong
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(false);

        switch (m_Mode)
        {
        default:
        {
            VW_LOG(Error, "vwrn", "Unknown sky mode in Sky::Render");
            SetColorMode({255, 0, 0, 255});
        }
        break;

        case SkyMode::Color:
        {
            Color c = m_ColorData.Color.Normalized();
            glClearColor(c.r, c.g, c.b, c.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        break;

        case SkyMode::Skybox:
        {
            // TODO: CAMERASYSTEM + SHADERSYSTEM
            auto camera = Renderer::GetActiveCamera();
            if (!m_SkyboxData.Shader)
            {
                m_SkyboxData.Shader = ShaderSystem::GetEngineShader("Cubemap.glsl");
            }
            if (!camera)
                return;

            m_SkyboxData.Shader->Use();
            _UploadCameraToShader(camera, m_SkyboxData.Shader);
            m_SkyboxData.Texture.Use();
            m_SkyboxData.Shader->Int(0, "uSkybox");

            VA->Bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        break;
        }
    }

    void Sky::_EnsureVA()
    {
        if (VA)
            return;

        VA = std::make_unique<VertexArray>();
        auto vb = new Buffer(BufferType::Vertex, BufferUsage::Static); // todo: holy this sucks

        vb->SetData(cubeVertices, sizeof(cubeVertices));
        VA->AddVertexBuffer(vb,
                            {
                                .Attributes = {{0, 0, 3, false}},
                                .Stride = 3 * sizeof(float),
                            },
                            VertexInputRate::Vertex);
        VA->Bind();
        VA->Unbind();
    }

    void Sky::SetColorMode(const Color &color)
    {
        m_Mode = SkyMode::Color;
        m_ColorData.Color = color;
    }

    void Sky::SetSkyboxMode(const CubemapTexture::Configuration &config)
    {
        m_Mode = SkyMode::Skybox;

        m_SkyboxData.Texture.Destroy();
        m_SkyboxData.Texture.Load(config);
    }

} // namespace VW
