#pragma once

#include "Texture.h"
#include <string>

namespace VW
{
    class VW_API CubemapTexture : public Texture
    {
    public:
        struct Configuration
        {
            std::string Left, Right, Top, Bottom, Front, Back;
        };

    public:
        CubemapTexture();
        virtual ~CubemapTexture() override;

        CubemapTexture(const CubemapTexture &) = delete;
        CubemapTexture &operator=(const CubemapTexture &) = delete;
        CubemapTexture(CubemapTexture &&other) noexcept;
        CubemapTexture &operator=(CubemapTexture &&other) noexcept;

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual void Load() override;
        void Load(const Configuration &config);
        virtual void Destroy() override;

        void Use();
    };
} // namespace VW
