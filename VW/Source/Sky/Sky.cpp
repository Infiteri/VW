#include "Sky.h"
#include "Camera/Camera.h"
#include "Camera/CameraSystem.h"
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
        s->Vec3(c->GetPosition(), "uCamPos");
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
            auto camera = CameraSystem::GetActiveCamera();
            auto shader = ShaderSystem::GetEngineShader("Cubemap.glsl");
            if (!camera || !shader)
                return;

            shader->Use();
            _UploadCameraToShader(camera, shader);
            m_SkyboxData.Texture.Use();
            shader->Int(0, "uSkybox");

            VA->Bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        break;

        case SkyMode::Shader:
        {
            auto shader = m_ShaderData.Shader;
            auto camera = CameraSystem::GetActiveCamera();
            if (!shader || !camera)
                return;

            shader->Use();

            // TODO: maybe make a function so that other shaders may get core uniforms uploaded
            shader->Float(Renderer::GetDeltaTime(), "uDeltaTime");
            shader->Float(Renderer::GetTime(), "uTime");

            _UploadCameraToShader(camera, shader);
            m_ShaderData.Uniforms.Apply(shader);

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

    void Sky::SetShaderMode(const std::string &path)
    {
        m_Mode = SkyMode::Shader;

        m_ShaderData.Shader = ShaderSystem::GetShader(path);
    }

} // namespace VW
