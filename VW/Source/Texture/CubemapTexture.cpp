#include "CubemapTexture.h"
#include "Core/Logger.h"
#include "Resource/Image.h"
#include <glad/glad.h>
#include <string>

namespace VW
{
    CubemapTexture::CubemapTexture()
    {
        m_ID = 0;
    }

    CubemapTexture::~CubemapTexture()
    {
        Destroy();
    }

    void CubemapTexture::Bind() const
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
    }

    void CubemapTexture::Unbind() const
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void CubemapTexture::Load()
    {
        if (m_ID != 0)
            Destroy();

        glGenTextures(1, &m_ID);
        Bind();

        u8 white[4] = {255, 255, 255, 255};

        for (int i = 0; i < 6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, 1, 1, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, white);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        Unbind();
    }

    void CubemapTexture::Load(const Configuration &config)
    {
        if (m_ID != 0)
            Destroy();

        glGenTextures(1, &m_ID);
        Bind();

        const char *paths[] = {
            config.Right.c_str(),  // POSITIVE_X
            config.Left.c_str(),   // NEGATIVE_X
            config.Top.c_str(),    // POSITIVE_Y
            config.Bottom.c_str(), // NEGATIVE_Y
            config.Front.c_str(),  // POSITIVE_Z
            config.Back.c_str()    // NEGATIVE_Z
        };

        GLenum targets[] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                            GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                            GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

        for (int i = 0; i < 6; i++)
        {
            Image image(paths[i]);
            if (!image.IsValid())
            {
                VW_ERROR("vwrn", "Failed to load image '%s'", paths[i]);
                Destroy();
                Load();
                return;
            }

            GLenum format = image.GetChannels() == 4 ? GL_RGBA : GL_RGB;
            GLenum internalFormat = image.GetChannels() == 4 ? GL_RGBA8 : GL_RGB8;

            glTexImage2D(targets[i], 0, internalFormat, image.GetWidth(), image.GetHeight(), 0,
                         format, GL_UNSIGNED_BYTE, image.GetData());
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        GLint swizzleRGBA[] = {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA};
        glTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_RGBA, swizzleRGBA);

        Unbind();
    }

    void CubemapTexture::Destroy()
    {
        glDeleteTextures(1, &m_ID);
    }

    void CubemapTexture::Use()
    {
        glActiveTexture(GL_TEXTURE0);
        Bind();
    }
} // namespace VW
