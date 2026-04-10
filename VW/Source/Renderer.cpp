#include "Renderer.h"
#include "Buffer/Buffer.h"
#include "Core/Logger.h"
#include "Shader/Shader.h"

#include <glad/glad.h>

namespace VW
{
    static Buffer *s_VertexBuffer = nullptr;
    static Shader *s_Shader = nullptr;

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

        Vertex vertices[] = {{0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f},
                             {0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
                             {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f}};

        s_VertexBuffer = new Buffer(BufferType::Vertex, BufferUsage::Static);
        s_VertexBuffer->SetData(vertices, sizeof(vertices));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(f32)));
    }

    void Renderer::Shutdown()
    {
        delete s_Shader;
        delete s_VertexBuffer;
        s_Shader = nullptr;
        s_VertexBuffer = nullptr;
    }

    void Renderer::BeginFrame()
    {
    }

    void Renderer::Render()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        s_Shader->Use();
        s_VertexBuffer->Bind();

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void Renderer::EndFrame()
    {
    }
} // namespace VW
