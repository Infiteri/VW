#pragma once

#include "Buffer/Buffer.h"
#include "Buffer/Framebuffer.h"
#include "Buffer/VertexArray.h"
#include "Shader/Shader.h"
#include <memory>
namespace VW
{
    class GPUScreen
    {
    public:
        GPUScreen(); // TODO: if multiple monitor setup works, this might take a configuration, dont
                     // worry for now
        ~GPUScreen();

        void Init();
        void Begin();
        void End();
        void Resize(f32 w, f32 h);

    private:
        std::unique_ptr<Framebuffer> m_FB;
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<Buffer> m_VBO, m_EBO;
        std::unique_ptr<Shader> m_SS; // TODO: from ShaderSystem when in place
        u32 m_IndexCount = 0;
    };
} // namespace VW
