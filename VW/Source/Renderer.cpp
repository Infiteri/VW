#include "Renderer.h"
#include "Buffer/Buffer.h"
#include "Buffer/VertexArray.h"
#include "Core/Logger.h"
#include "Mesh/Mesh.h"
#include "Shader/Shader.h"

#include <glad/glad.h>

namespace VW
{
    static Renderer::State s_State;
    static Shader *s_Shader = nullptr;
    static u32 s_IndexCount = 0;
    static Mesh *mesh;

    void Renderer::Init()
    {
        VW_LOG_ADD_CATEGORY("vwrn", "Renderer");
        VW_LOG(Info, "vwrn", "Initializing...");

        gladLoadGL();

        s_Shader = new Shader("Shader.glsl");

#define S 1
        Vertex vertices[] = {
            {Vector3{-S, S, 0.0f}, Vector2{1.0f, 0.0f}},
            {Vector3{S, S, 0.0f}, Vector2{0.0f, 1.0f}},
            {Vector3{S, -S, 0.0f}, Vector2{0.0f, 0.0f}},
            {Vector3{-S, -S, 0.0f}, Vector2{1.0f, 1.0f}},
        };

        u32 indices[] = {0, 1, 2, 0, 2, 3};
        s_IndexCount = 6;

        VertexLayout layout;
        layout.Stride = sizeof(Vertex);
        layout.Attributes.push_back({0, 0, 3, false});
        layout.Attributes.push_back({1, 3 * sizeof(f32), 2, false});

        s_State.Screen.Init();

        mesh = new Mesh(vertices, sizeof(Vertex) * 4, indices, sizeof(indices) / 4, layout);
    }

    void Renderer::Shutdown()
    {
        delete s_Shader;
        s_Shader = nullptr;
        s_IndexCount = 0;
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
        s_State.Screen.Begin();
    }

    void Renderer::Render()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        s_Shader->Use();

        if (s_State.Cam)
        {
            s_Shader->Mat4(s_State.Cam->GetProjection(), "uProj");
            s_Shader->Mat4(s_State.Cam->GetView(), "uView");
        }

        mesh->Bind();
        glDrawElements(GL_TRIANGLES, s_IndexCount, GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::EndFrame()
    {
        s_State.Screen.End();
    }

    void Renderer::UseCamera(Camera *cam)
    {
        s_State.Cam = cam;
    }
} // namespace VW
