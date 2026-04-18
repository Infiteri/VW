#include "Renderer.h"
#include "BatchRenderer.h"
#include "Buffer/Buffer.h"
#include "Buffer/VertexArray.h"
#include "Core/Logger.h"
#include "Mesh/Mesh.h"
#include "RenderDebug.h"
#include "Shader/Shader.h"

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

        s_State.Batch->Begin();
        for (const auto &item : s_State.RenderQueue)
        {
            MaterialGPU m;
            m.Color = item.Material.Color;
            s_State.Batch->Submit(item.Mesh, item.Transform, m);
        }
        s_State.Batch->End();
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

    const RendererDebugSettings &Renderer::GetDebugSettings()
    {
        return s_State.Debug;
    }

    Renderer::State &Renderer::_GetState()
    {
        return s_State;
    }

} // namespace VW
