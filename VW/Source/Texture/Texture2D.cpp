#include "Texture2D.h"
#include <glad/glad.h>

namespace VW
{
    Texture2D::Texture2D()
    {
        m_ID = 0;
        m_Width = 0;
        m_Height = 0;
        m_Channels = 0;
        m_Path = "";
    }

    Texture2D::~Texture2D()
    {
        Destroy();
    }

    void Texture2D::Bind() const
    {
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }

    void Texture2D::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::Load()
    {
        if (m_ID != 0)
            Destroy();

        glGenTextures(1, &m_ID);
        Bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        u8 whitePixel[4] = {0, 255, 255, 255};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);

        Unbind();
    }

    void Texture2D::Destroy()
    {
        glDeleteTextures(1, &m_ID);
        m_ID = 0;
        m_Width = 0;
        m_Height = 0;
        m_Channels = 0;
        m_Path = "";
    }

    void Texture2D::Load(const std::string &path)
    {
    }

    Texture2D::Texture2D(Texture2D &&other) noexcept : Texture()
    {
        m_ID = other.m_ID;
        m_Width = other.m_Width;
        m_Height = other.m_Height;
        m_Channels = other.m_Channels;
        m_Path = std::move(other.m_Path);

        other.m_ID = 0;
        other.m_Width = 0;
        other.m_Height = 0;
        other.m_Channels = 0;
    }

    Texture2D &Texture2D::operator=(Texture2D &&other) noexcept
    {
        if (this != &other)
        {
            Destroy();

            m_ID = other.m_ID;
            m_Width = other.m_Width;
            m_Height = other.m_Height;
            m_Channels = other.m_Channels;
            m_Path = std::move(other.m_Path);

            other.m_ID = 0;
            other.m_Width = 0;
            other.m_Height = 0;
            other.m_Channels = 0;
        }
        return *this;
    }
} // namespace VW
