#include "GPUScreen.h"
#include <glad/glad.h>

namespace VW
{

    GPUScreen::GPUScreen()
    {
    }

    GPUScreen::~GPUScreen()
    {
    }

    void GPUScreen::Init()
    {
        m_SS = std::make_unique<Shader>("Screen.glsl");

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

        m_FB = std::make_unique<Framebuffer>(desc);

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

        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<Buffer>(BufferType::Vertex, BufferUsage::Static);
        m_EBO = std::make_unique<Buffer>(BufferType::Index, BufferUsage::Static);
        m_VBO->SetData(screenVertices, sizeof(screenVertices));
        m_EBO->SetData(quadIndices, sizeof(quadIndices));

        VertexLayout screenLayout;
        screenLayout.Stride = sizeof(ScreenVertex);
        screenLayout.Attributes.push_back({0, 0, 2, false});
        screenLayout.Attributes.push_back({1, 2 * sizeof(float), 2, false});

        m_VAO->AddVertexBuffer(m_VBO.get(), screenLayout, VertexInputRate::Vertex);
        m_VAO->SetIndexBuffer(m_EBO.get());

        m_FB->Unbind();
    }

    void GPUScreen::Begin()
    {
        m_FB->Bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
    }

    void GPUScreen::End()
    {
        m_FB->Unbind();

        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        m_SS->Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_FB->GetColorAttachmentID(0));
        m_SS->Int(0, "uSceneColor");
        m_VAO->Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    void GPUScreen::Resize(f32 w, f32 h)
    {
        if (!m_FB)
            return;
        m_FB->Resize(w, h);
    }
} // namespace VW
