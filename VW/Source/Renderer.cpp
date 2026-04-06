#include "Renderer.h"

#include <glad/glad.h>

namespace VW
{
    void Renderer::Init()
    {
        gladLoadGL();
    }

    void Renderer::Shutdown()
    {
    }

    void Renderer::BeginFrame()
    {
    }

    void Renderer::Render()
    {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::EndFrame()
    {
    }
} // namespace VW
