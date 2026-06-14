#include "Renderer.h"
#include "BatchRenderer.h"
#include "Camera/CameraSystem.h"
#include "Core/Logger.h"
#include "Light/LightSystem.h"
#include "Material/MaterialSystem.h"
#include "Mesh/MeshSystem.h"
#include "RenderDebug.h"
#include "Shader/Shader.h"
#include "Shader/ShaderSystem.h"
#include "Texture/TextureSystem.h"
#include <chrono>
#include <glad/glad.h>

namespace VW
{
    static Renderer::State s_State;

    RenderItem::RenderItem()
    {
        Material = nullptr;
        Shader = nullptr;
        Mesh = nullptr;
    }

    void Renderer::Init()
    {
        VW_LOG_ADD_CATEGORY("vwrn", "Renderer");
        VW_LOG(Info, "vwrn", "Initializing...");

        gladLoadGL();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // TODO: configurable
        s_State.Screen.Init();
        s_State.Batch = new BatchRenderer(1000);

        ShaderSystem::Init();
        TextureSystem::Init();
        MaterialSystem::Init();
        MeshSystem::Init();
        LightSystem::Init();
    }

    void Renderer::Shutdown()
    {
    }

    // TODO: figure out viewport on 'true' platforms (non dev apps)
    // NOTE: note that once sized the display shouldn't change sizes on non dev apps, as the monitor
    // doesnt magically change sizes, but user might change display from landscape to portrait
    void Renderer::Viewport(f32 w, f32 h)
    {
        if (s_State.Viewport.Width == w && s_State.Viewport.Height == h)
            return;

        s_State.Viewport = {w, h};
        glViewport(0, 0, w, h);
        s_State.Screen.Resize(w, h);

        auto camera = CameraSystem::GetActiveCamera();
        if (camera)
            camera->Resize(w, h);
    }

    void Renderer::BeginFrame()
    {
        // TODO: move to some timing class
        {
            auto now = std::chrono::high_resolution_clock::now();
            static auto lastFrame = now;
            static float totalTime = 0.0f;

            float deltaTime = std::chrono::duration<float>(now - lastFrame).count();
            totalTime += deltaTime;
            lastFrame = now;

            s_State.Time = totalTime;
            s_State.DeltaTime = deltaTime;
        }

        // reset Stats
        auto camera = CameraSystem::GetActiveCamera();
        s_State.Stats = RendererStats();

        s_State.Screen.Begin();
        s_State.RenderQueue.clear();

        if (camera)
        {
            Matrix4 vp = Matrix4::Multiply(camera->GetProjection(), camera->GetView());
            s_State.Frustum.Extract(vp);
        }

        if (s_State.Sky)
            s_State.Sky->Render();
        else
        {
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }

    void Renderer::Render()
    {
        LightSystem::UpdateGPUData();
        LightSystem::Bind(3);
    }

    void Renderer::Submit(const RenderItem &item)
    {
        // TODO: more clear scaling function (some extract function would be nice)
        if (!item.Mesh || !item.Shader || !item.Material ||
            (item.Transform[0] == 0.0f && item.Transform[5] == 0.0f && item.Transform[10] == 0.0f))
            return;
        s_State.RenderQueue.push_back(item);
    }

    void Renderer::SetSky(class Sky *sky)
    {
        s_State.Sky = sky;
    }

    static Vector3 TransformPoint(const float matrix[16], const Vector3 &point)
    {
        return Vector3(matrix[0] * point.x + matrix[4] * point.y + matrix[8] * point.z + matrix[12],
                       matrix[1] * point.x + matrix[5] * point.y + matrix[9] * point.z + matrix[13],
                       matrix[2] * point.x + matrix[6] * point.y + matrix[10] * point.z +
                           matrix[14]);
    }

    void Renderer::EndFrame()
    {
        if (s_State.Debug.RenderWireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDepthMask(true);

        s_State.Batch->Begin();
        for (const auto &item : s_State.RenderQueue)
        {
            if (!item.Mesh)
                continue;
            Vector3 pos = {item.Transform.data[12], item.Transform.data[13],
                           item.Transform.data[14]};
            Vector3 worldCenter = pos + item.Mesh->GetBoundsCenter();
            float maxScale =
                std::max(item.Transform[0], std::max(item.Transform[5], item.Transform[10]));

            float worldRadius = item.Mesh->GetBoundsRadius() * maxScale;
            if (s_State.Frustum.IsSphereInside(worldCenter, worldRadius))
            {
                s_State.Batch->Submit(item.Mesh, item.Transform, item.Material, item.Shader);
            }
        }
        s_State.Batch->End();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glDisable(GL_CULL_FACE);

        s_State.Screen.End();
    }

    const RendererStats &Renderer::GetStats()
    {
        return s_State.Stats;
    }

    RendererDebugSettings &Renderer::GetDebugSettings()
    {
        return s_State.Debug;
    }

    Renderer::State &Renderer::_GetState()
    {
        return s_State;
    }

    float Renderer::GetDeltaTime()
    {
        return s_State.DeltaTime;
    }

    float Renderer::GetTime()
    {
        return s_State.Time;
    }

    void RendererUtils::CoreUniformsToShader(Shader *shader)
    {
        auto camera = CameraSystem::GetActiveCamera();
        if (!shader || !camera)
            return;

        shader->Use();
        shader->Int((int)s_State.Debug.RenderMode, "uRenderMode");
        shader->Mat4(camera->GetProjection(), "uProj");
        shader->Mat4(camera->GetView(), "uView");
        shader->Int(LightSystem::GetLightCount(), "uLightCount");

        // TODO: add time based uniforms + others
    }
} // namespace VW
