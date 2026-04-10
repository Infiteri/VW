#pragma once

#include "Base.h"

namespace VW
{
    enum class BufferType
    {
        Vertex,
        Index,
        SSBO,
        Uniform,
        Indirect
    };

    enum class BufferUsage
    {
        Static,
        Dynamic,
        Stream
    };

    class Buffer
    {
    public:
        Buffer(BufferType type, BufferUsage usage);
        ~Buffer();

        void Bind() const;
        void Unbind() const;

        void SetData(const void *data, u32 size);
        void UpdateData(const void *data, u32 size, u32 offset = 0);

        inline u32 GetID() const
        {
            return m_ID;
        };

    private:
        u32 m_ID;
        u32 m_Size;
        u32 m_Stride;
        BufferType m_Type;
        BufferUsage m_Usage;
    };
} // namespace VW
