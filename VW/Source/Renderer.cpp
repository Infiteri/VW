#include "Renderer.h"
#include "Core/Logger.h"
#include "Shader/Shader.h"

#include <glad/glad.h>

namespace VW
{
    void Renderer::Init()
    {
        VW_LOG_ADD_CATEGORY("vwrn", "Renderer");
        VW_LOG(Info, "vwrn", "Initializing...");

        gladLoadGL();

        Shader shader("Shader.glsl");
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
