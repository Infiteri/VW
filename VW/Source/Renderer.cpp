#include "Renderer.h"
#include "BatchRenderer.h"
#include "Core/Logger.h"
#include "Mesh/MeshSystem.h"
#include "RenderDebug.h"
#include "Shader/Shader.h"
#include "Texture/TextureSystem.h"
#include <glad/glad.h>

namespace VW
{
    static Renderer::State s_State;
    static Shader *s_Shader = nullptr;

    void Renderer::Init()
    {
        VW_LOG_ADD_CATEGORY("vwrn", "Renderer");
        VW_LOG(Info, "vwrn", "Initializing...");

        gladLoadGL();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        s_Shader = new Shader("Shader.glsl");

        s_State.Screen.Init();
        s_State.Batch = new BatchRenderer(1000);

        TextureSystem::Init();
        MeshSystem::Init();
    }

    void Renderer::Shutdown()
    {
        delete s_Shader;
        s_Shader = nullptr;
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

        if (s_State.Cam)
            s_State.Cam->Resize(w, h);
    }

    void Renderer::BeginFrame()
    {
        // reset Stats
        s_State.Stats = RendererStats();

        s_State.Screen.Begin();
        s_State.RenderQueue.clear();

        if (s_State.Cam)
        {
            Matrix4 vp = Matrix4::Multiply(s_State.Cam->GetProjection(), s_State.Cam->GetView());
            s_State.Frustum.Extract(vp);
        }
    }

    void Renderer::Render()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        s_Shader->Use();

        if (s_State.Cam)
        {
            s_Shader->Mat4(s_State.Cam->GetProjection(), "uProj");
            s_Shader->Mat4(s_State.Cam->GetView(), "uView");
        }
    }

    void Renderer::Submit(const RenderItem &item)
    {
        s_State.RenderQueue.push_back(item);
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

        s_State.Batch->Begin();
        for (const auto &item : s_State.RenderQueue)
        {

            Vector3 pos = {item.Transform.data[12], item.Transform.data[13],
                           item.Transform.data[14]};
            Vector3 worldCenter = pos + item.Mesh->GetBoundsCenter();
            float worldRadius = item.Mesh->GetBoundsRadius();
            if (s_State.Frustum.IsSphereInside(worldCenter, worldRadius))
            {
                s_State.Batch->Submit(item.Mesh, item.Transform, item.Material);
            }
        }
        s_State.Batch->End();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glDisable(GL_CULL_FACE);

        s_State.Screen.End();
    }

    void Renderer::UseCamera(Camera *cam)
    {
        s_State.Cam = cam;
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

} // namespace VW
