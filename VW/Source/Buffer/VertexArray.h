#pragma once

#include "Base.h"
#include "Buffer.h"
#include <vector>

namespace VW
{
    enum class VertexInputRate
    {
        Vertex,
        Instance
    };

    struct VertexAttribute
    {
        u32 Location, Offset, Size;
        bool IsLong = false;
    };

    struct VertexLayout
    {
        std::vector<VertexAttribute> Attributes;
        u32 Stride;
    };

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind();
        void Unbind();

        void AddVertexBuffer(const Buffer *buffer, const VertexLayout &layout,
                             VertexInputRate inputRate);
        void SetIndexBuffer(const Buffer *buffer);

        inline u32 GetID() const
        {
            return m_ID;
        };

    private:
        u32 m_ID;
    };
} // namespace VW
