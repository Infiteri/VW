#include "Framebuffer.h"
#include "Base.h"
#include "Core/Logger.h"
#include <glad/glad.h>

namespace VW
{
    static GLenum ToInternalFormat(TextureFormat format)
    {
        switch (format)
        {
        case TextureFormat::RGBA8:
            return GL_RGBA8;
        case TextureFormat::RGB8:
            return GL_RGB8;
        case TextureFormat::Depth24:
            return GL_DEPTH_COMPONENT24;
        case TextureFormat::Depth32F:
            return GL_DEPTH_COMPONENT32F;
        case TextureFormat::RGBA16F:
            return GL_RGBA16F;
        case TextureFormat::RGBA32F:
            return GL_RGBA32F;
        default:
            return GL_RGBA8;
        }
    }

    static GLenum ToFormat(TextureFormat format)
    {
        switch (format)
        {
        case TextureFormat::Depth24:
        case TextureFormat::Depth32F:
            return GL_DEPTH_COMPONENT;

        case TextureFormat::RGB8:
            return GL_RGB;

        default:
            return GL_RGBA;
        }
    }

    static GLenum ToType(TextureFormat format)
    {
        switch (format)
        {
        case TextureFormat::Depth32F:
        case TextureFormat::RGBA32F:
        case TextureFormat::RGBA16F:
            return GL_FLOAT;

        default:
            return GL_UNSIGNED_BYTE;
        }
    }

    static GLenum ToGLTarget(TextureTarget target)
    {
        switch (target)
        {
        case TextureTarget::Texture2D:
            return GL_TEXTURE_2D;
        case TextureTarget::TextureCubemap:
            return GL_TEXTURE_CUBE_MAP;
        case TextureTarget::Texture2DArray:
            return GL_TEXTURE_2D_ARRAY;
        default:
            return GL_TEXTURE_2D;
        }
    }

    Framebuffer::Framebuffer(const FramebufferDesc &desc)
        : m_Width(desc.Width), m_Height(desc.Height), m_Desc(desc)
    {
        glCreateFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

        _BuildAttachments();
    }

    Framebuffer::~Framebuffer()
    {
        if (m_ID)
        {
            glDeleteFramebuffers(1, &m_ID);
            glDeleteTextures(m_ColorTextures.size(), m_ColorTextures.data());
            if (m_DepthTexture)
                glDeleteTextures(1, &m_DepthTexture);
            if (m_StencilTexture)
                glDeleteTextures(1, &m_StencilTexture);
        }
    }

    Framebuffer::Framebuffer(Framebuffer &&other) noexcept
    {
        m_ID = other.m_ID;
        m_Width = other.m_Width;
        m_Height = other.m_Height;
        m_SampleCount = other.m_SampleCount;
        m_ColorTextures = std::move(other.m_ColorTextures);
        m_DepthTexture = other.m_DepthTexture;
        m_StencilTexture = other.m_StencilTexture;
        m_Desc = std::move(other.m_Desc);

        other.m_ID = 0;
        other.m_DepthTexture = 0;
        other.m_StencilTexture = 0;
    }

    Framebuffer &Framebuffer::operator=(Framebuffer &&other) noexcept
    {
        if (this != &other)
        {
            this->~Framebuffer();
            m_ID = other.m_ID;
            m_Width = other.m_Width;
            m_Height = other.m_Height;
            m_SampleCount = other.m_SampleCount;
            m_ColorTextures = std::move(other.m_ColorTextures);
            m_DepthTexture = other.m_DepthTexture;
            m_StencilTexture = other.m_StencilTexture;
            m_Desc = std::move(other.m_Desc);

            other.m_ID = 0;
            other.m_DepthTexture = 0;
            other.m_StencilTexture = 0;
        }

        return *this;
    }

    void Framebuffer::_CreateAttachment(u32 attachment, const AttachmentDesc &desc)
    {
        u32 textureID;
        u32 glTarget = ToGLTarget(desc.Target);
        glCreateTextures(glTarget, 1, &textureID);
        glTextureStorage2D(textureID, 1, ToInternalFormat(desc.Format), m_Width, m_Height);
        glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glNamedFramebufferTexture(m_ID, attachment, textureID, 0);

        if (attachment >= GL_COLOR_ATTACHMENT0 && attachment < GL_COLOR_ATTACHMENT0 + 16)
            m_ColorTextures.push_back(textureID);
        else if (attachment == GL_DEPTH_ATTACHMENT)
            m_DepthTexture = textureID;
        else if (attachment == GL_STENCIL_ATTACHMENT)
            m_StencilTexture = textureID;
    }

    void Framebuffer::_CreateMultisampleAttachment(u32 attachment, const AttachmentDesc &desc)
    {
        u32 textureID;
        glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &textureID);
        glTextureStorage2DMultisample(textureID, desc.Samples, ToInternalFormat(desc.Format),
                                      m_Width, m_Height, GL_FALSE);

        glNamedFramebufferTexture(m_ID, attachment, textureID, 0);

        if (attachment >= GL_COLOR_ATTACHMENT0 && attachment < GL_COLOR_ATTACHMENT0 + 16)
            m_ColorTextures.push_back(textureID);
        else if (attachment == GL_DEPTH_ATTACHMENT)
            m_DepthTexture = textureID;
        else if (attachment == GL_STENCIL_ATTACHMENT)
            m_StencilTexture = textureID;
    }

    void Framebuffer::_CreateCubemapAttachment(u32 attachment, const AttachmentDesc &desc)
    {
        u32 textureID;
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &textureID);
        glTextureStorage2D(textureID, 1, ToInternalFormat(desc.Format), m_Width, m_Height);

        glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        for (u32 face = 0; face < 6; ++face)
        {
            glNamedFramebufferTextureLayer(m_ID, attachment, textureID, 0, face);
        }

        if (attachment >= GL_COLOR_ATTACHMENT0 && attachment < GL_COLOR_ATTACHMENT0 + 16)
            m_ColorTextures.push_back(textureID);
        else if (attachment == GL_DEPTH_ATTACHMENT)
            m_DepthTexture = textureID;
        else if (attachment == GL_STENCIL_ATTACHMENT)
            m_StencilTexture = textureID;
    }

    void Framebuffer::_BuildAttachments()
    {
        u32 sampleCount = 0;
        for (u32 i = 0; i < m_Desc.ColorAttachments.size(); i++)
        {
            const auto &att = m_Desc.ColorAttachments[i];
            if (att.Multisampled && sampleCount == 0)
                sampleCount = att.Samples;

            GLenum target = ToGLTarget(att.Target);
            if (att.Target == TextureTarget::TextureCubemap)
                _CreateCubemapAttachment(GL_COLOR_ATTACHMENT0 + i, att);
            else if (att.Multisampled)
                _CreateMultisampleAttachment(GL_COLOR_ATTACHMENT0 + i, att);
            else
                _CreateAttachment(GL_COLOR_ATTACHMENT0 + i, att);
        }

        if (m_Desc.DepthAttachment)
        {
            const auto &att = *m_Desc.DepthAttachment;
            if (att.Multisampled && sampleCount == 0)
                sampleCount = att.Samples;

            if (att.Multisampled)
                _CreateMultisampleAttachment(GL_DEPTH_ATTACHMENT, att);
            else
                _CreateAttachment(GL_DEPTH_ATTACHMENT, att);
        }

        if (m_Desc.StencilAttachment)
        {
            const auto &att = *m_Desc.StencilAttachment;
            if (att.Multisampled)
                _CreateMultisampleAttachment(GL_STENCIL_ATTACHMENT, att);
            else
                _CreateAttachment(GL_STENCIL_ATTACHMENT, att);
        }

        m_SampleCount = sampleCount > 0 ? sampleCount : 1;

        if (!m_Desc.ColorAttachments.empty())
        {
            std::vector<GLenum> buffers(m_Desc.ColorAttachments.size());
            for (u32 i = 0; i < buffers.size(); i++)
                buffers[i] = GL_COLOR_ATTACHMENT0 + i;
            glNamedFramebufferDrawBuffers(m_ID, buffers.size(), buffers.data());
        }

        if (!IsComplete())
        {
            VW_LOG(Error, "vwrn", "Framebuffer incomplete");
            glDeleteFramebuffers(1, &m_ID);
            m_ID = 0;
        }
    }

    bool Framebuffer::IsComplete() const
    {
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

    void Framebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
        glViewport(0, 0, m_Width, m_Height);
    }

    void Framebuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::Resize(u32 width, u32 height)
    {
        if (width == m_Width && height == m_Height)
            return;

        m_Width = width;
        m_Height = height;
        m_Desc.Width = width;
        m_Desc.Height = height;

        glDeleteTextures(m_ColorTextures.size(), m_ColorTextures.data());
        if (m_DepthTexture)
            glDeleteTextures(1, &m_DepthTexture);
        if (m_StencilTexture)
            glDeleteTextures(1, &m_StencilTexture);
        m_ColorTextures.clear();
        m_DepthTexture = 0;
        m_StencilTexture = 0;

        glDeleteFramebuffers(1, &m_ID);
        glCreateFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

        _BuildAttachments();
    }

    u32 Framebuffer::GetColorAttachmentID(u32 index) const
    {
        if (index < 0 || index >= m_ColorTextures.size())
            return 0;

        return m_ColorTextures[index];
    }

    u32 Framebuffer::GetDepthAttachmentID() const
    {
        return m_DepthTexture;
    }

} // namespace VW
