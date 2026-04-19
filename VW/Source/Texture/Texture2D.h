#pragma once

#include "Texture/Texture.h"
#include <string>
namespace VW
{
    class Texture2D : public Texture
    {
    public:
        Texture2D();
        virtual ~Texture2D() override;

        Texture2D(const Texture2D &) = delete;
        Texture2D &operator=(const Texture2D &) = delete;
        Texture2D(Texture2D &&other) noexcept;
        Texture2D &operator=(Texture2D &&other) noexcept;

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual void Load() override;
        void Load(const std::string &path);
        virtual void Destroy() override;

        inline int GetWidth() const
        {
            return m_Width;
        };
        inline int GetHeight() const
        {
            return m_Height;
        };
        inline int GetChannels() const
        {
            return m_Channels;
        };
        inline const std::string &GetPath() const
        {
            return m_Path;
        };

    private:
        int m_Width, m_Height, m_Channels;
        std::string m_Path;
    };
} // namespace VW
