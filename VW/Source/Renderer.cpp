#include "Renderer.h"
#include "Buffer/Buffer.h"
#include "Buffer/Framebuffer.h"
#include "Buffer/VertexArray.h"
#include "Camera/OrthographicCamera.h"
#include "Camera/PerspectiveCamera.h"
#include "Core/Logger.h"
#include "Math/Math.h"
#include "Shader/Shader.h"

#include <glad/glad.h>

namespace VW
{
    static Renderer::State s_State;
    static VertexArray *s_VAO = nullptr;
    static Buffer *s_VertexBuffer = nullptr;
    static Buffer *s_IndexBuffer = nullptr;
    static Shader *s_Shader = nullptr;
    static u32 s_IndexCount = 0;

    static PerspectiveCamera pers;

    struct Vertex
    {
        f32 x, y, z;
        f32 r, g, b;
    };

    void Renderer::Init()
    {
        VW_LOG_ADD_CATEGORY("vwrn", "Renderer");
        VW_LOG(Info, "vwrn", "Initializing...");

        gladLoadGL();

        s_Shader = new Shader("Shader.glsl");

#define S 1
        Vertex vertices[] = {
            {-S, S, 0.0f, 1.0f, 0.0f, 0.0f},
            {S, S, 0.0f, 0.0f, 1.0f, 0.0f},
            {S, -S, 0.0f, 0.0f, 0.0f, 1.0f},
            {-S, -S, 0.0f, 1.0f, 1.0f, 0.0f},
        };

        u32 indices[] = {0, 1, 2, 0, 2, 3};
        s_IndexCount = 6;

        s_VAO = new VertexArray();

        s_VertexBuffer = new Buffer(BufferType::Vertex, BufferUsage::Static);
        s_VertexBuffer->SetData(vertices, sizeof(vertices));

        s_IndexBuffer = new Buffer(BufferType::Index, BufferUsage::Static);
        s_IndexBuffer->SetData(indices, sizeof(indices));

        VertexLayout layout;
        layout.Stride = sizeof(Vertex);

        layout.Attributes.push_back({0, 0, 3, false});
        layout.Attributes.push_back({1, 3 * sizeof(f32), 3, false});

        s_VAO->AddVertexBuffer(s_VertexBuffer, layout, VertexInputRate::Vertex);
        s_VAO->SetIndexBuffer(s_IndexBuffer);

        s_State.Screen.Init();

        pers.SetPosition({0, 0, 10});
        pers.SetOrientation(Quaternion::FromEulerAngles(0.5, 0.6, 0));
    }

    void Renderer::Shutdown()
    {
        delete s_VAO;
        delete s_VertexBuffer;
        delete s_IndexBuffer;
        delete s_Shader;
        s_VAO = nullptr;
        s_VertexBuffer = nullptr;
        s_IndexBuffer = nullptr;
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
        pers.Resize(w, h);
    }

    void Renderer::BeginFrame()
    {
        s_State.Screen.Begin();
    }

    void Renderer::Render()
    {
        Vector3 forward = pers.Forward() * 0.1f;
        pers.SetPosition(pers.GetPosition() + forward);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        s_Shader->Use();
        s_Shader->Mat4(pers.GetProjection(), "uProj");
        s_Shader->Mat4(pers.GetView(), "uView");
        s_VAO->Bind();
        glDrawElements(GL_TRIANGLES, s_IndexCount, GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::EndFrame()
    {
        s_State.Screen.End();
    }
} // namespace VW
