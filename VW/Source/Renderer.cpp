#include "Renderer.h"
#include "Buffer/Buffer.h"
#include "Buffer/Framebuffer.h"
#include "Buffer/VertexArray.h"
#include "Core/Logger.h"
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

    static Framebuffer *s_FB;
    static VertexArray *s_ScreenVAO;
    static Buffer *s_VBO, *s_EBO;
    static Shader *s_SS;

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

        Vertex vertices[] = {
            {-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f},
            {0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
            {0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f},
            {-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f},
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

        s_SS = new Shader("Screen.glsl");

        // screen buffer setup
        FramebufferDesc desc;
        desc.Width = 1920;
        desc.Height = 1080;

        AttachmentDesc colorDesc;
        colorDesc.Target = TextureTarget::Texture2D;
        colorDesc.Format = TextureFormat::RGBA8;
        colorDesc.Multisampled = false;
        desc.ColorAttachments.push_back(colorDesc);

        AttachmentDesc depthDesc;
        depthDesc.Target = TextureTarget::Texture2D;
        depthDesc.Format = TextureFormat::Depth24;
        depthDesc.Multisampled = false;
        desc.DepthAttachment = depthDesc;

        s_FB = new Framebuffer(desc);

        struct ScreenVertex
        {
            f32 x, y, u, v;
        };

        ScreenVertex screenVertices[] = {
            {-1.0f, 1.0f, 0.0f, 1.0f},  // top-left
            {1.0f, 1.0f, 1.0f, 1.0f},   // top-right
            {1.0f, -1.0f, 1.0f, 0.0f},  // bottom-right
            {-1.0f, -1.0f, 0.0f, 0.0f}, // bottom-left
        };
        u32 quadIndices[] = {0, 1, 2, 0, 2, 3};

        s_ScreenVAO = new VertexArray();
        s_VBO = new Buffer(BufferType::Vertex, BufferUsage::Static);
        s_EBO = new Buffer(BufferType::Index, BufferUsage::Static);
        s_VBO->SetData(screenVertices, sizeof(screenVertices));
        s_EBO->SetData(quadIndices, sizeof(quadIndices));

        VertexLayout screenLayout;
        screenLayout.Stride = sizeof(ScreenVertex);
        screenLayout.Attributes.push_back({0, 0, 2, false});
        screenLayout.Attributes.push_back({1, 2 * sizeof(float), 2, false});

        s_ScreenVAO->AddVertexBuffer(s_VBO, screenLayout, VertexInputRate::Vertex);
        s_ScreenVAO->SetIndexBuffer(s_EBO);

        s_FB->Unbind();
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

        // todo: resize framebuffer
        s_State.Viewport = {w, h};
        glViewport(0, 0, w, h);
    }

    void Renderer::BeginFrame()
    {
    }

    void Renderer::Render()
    {
        //  3     // ====== PASS 1: Render scene to framebuffer ======
        //  4     s_FB->Bind();
        //  5     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //  6     glEnable(GL_DEPTH_TEST);
        //  7
        //  8     s_Shader->Use();
        //  9     s_VAO->Bind();
        // 10     glDrawElements(GL_TRIANGLES, s_IndexCount, GL_UNSIGNED_INT, nullptr);
        // 11
        // 12     s_FB->Unbind();
        // 13
        // 14     // ====== PASS 2: Blit framebuffer texture to screen ======
        // 15     glClear(GL_COLOR_BUFFER_BIT);
        // 16     glDisable(GL_DEPTH_TEST);
        // 17
        // 18     // You'll need a screen shader pointer — assuming you added it:
        // 19     s_ScreenShader->Use();
        // 20
        // 21     glActiveTexture(GL_TEXTURE0);
        // 22     glBindTexture(GL_TEXTURE_2D, s_FB->GetColorAttachmentID(0));
        // 23     // Set the sampler uniform to texture unit 0
        // 24     glUniform1i(glGetUniformLocation(s_ScreenShader->GetID(), "uSceneColor"), 0);
        // 25
        // 26     s_ScreenVAO->Bind();
        // 27     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        s_FB->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        s_Shader->Use();
        s_VAO->Bind();
        glDrawElements(GL_TRIANGLES, s_IndexCount, GL_UNSIGNED_INT, nullptr);

        s_FB->Unbind();

        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        s_SS->Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, s_FB->GetColorAttachmentID(0));
        glUniform1i(glGetUniformLocation(s_SS->GetID(), "uSceneColor"), 0);
        s_ScreenVAO->Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::EndFrame()
    {
    }
} // namespace VW
