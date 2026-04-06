#pragma once

#include "Base.h"
#include <optional>
#include <vector>

namespace VW
{
    enum class TextureFormat : u32
    {
        RGBA8,
        RGB8,
        Depth24,
        Depth32F,
        RGBA16F,
        RGBA32F
    };

    enum class TextureTarget : u32
    {
        Texture2D,
        TextureCubemap,
        Texture2DArray
    };

    struct AttachmentDesc
    {
        TextureFormat Format;
        TextureTarget Target;
        bool Multisampled;
        u32 Samples;
    };

    struct FramebufferDesc
    {
        u32 Width, Height;
        std::vector<AttachmentDesc> ColorAttachments;
        std::optional<AttachmentDesc> DepthAttachment, StencilAttachment;
    };

    class Framebuffer
    {
    public:
        Framebuffer(const FramebufferDesc &desc);
        ~Framebuffer();

        Framebuffer(const Framebuffer &) = delete;
        Framebuffer &operator=(const Framebuffer &) = delete;
        Framebuffer(Framebuffer &&other) noexcept;
        Framebuffer &operator=(Framebuffer &&other) noexcept;

        void Bind() const;
        void Unbind() const;
        void Resize(u32 width, u32 height);

        u32 GetColorAttachmentID(u32 index = 0) const;
        u32 GetDepthAttachmentID() const;
        bool IsComplete() const;

        inline u32 GetWidth() const
        {
            return m_Width;
        };

        inline u32 GetHeight() const
        {
            return m_Height;
        };

        inline u32 GetSampleCount() const
        {
            return m_SampleCount;
        };

        inline u32 GetID() const
        {
            return m_ID;
        };

    private:
        u32 m_ID = 0, m_Width, m_Height, m_SampleCount = 1;
        std::vector<u32> m_ColorTextures;
        u32 m_DepthTexture = 0, m_StencilTexture = 0;
        FramebufferDesc m_Desc;

    private:
        void _CreateAttachment(u32 attachment, const AttachmentDesc &desc);
        void _CreateMultisampleAttachment(u32 attachment, const AttachmentDesc &desc);
        void _CreateCubemapAttachment(u32 attachment, const AttachmentDesc &desc);
        void _BuildAttachments();
    };
} // namespace VW
