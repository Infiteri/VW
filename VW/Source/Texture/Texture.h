#pragma once

#include "Base.h"

namespace VW
{
    class Texture
    {
    public:
        Texture() = default;
        virtual ~Texture() = default;

        virtual void Load() = 0;
        virtual void Destroy() = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        inline u32 GetID() const
        {
            return m_ID;
        }

    protected:
        u32 m_ID;
    };
} // namespace VW
