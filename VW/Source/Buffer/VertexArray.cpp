#include "VertexArray.h"
#include "Buffer.h"

#include <glad/glad.h>

namespace VW
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_ID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_ID);
    }

    void VertexArray::Bind()
    {
        glBindVertexArray(m_ID);
    }

    void VertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    void VertexArray::AddVertexBuffer(const Buffer *buffer, const VertexLayout &layout,
                                      VertexInputRate inputRate)
    {
        Bind();
        buffer->Bind();

        for (const auto &attr : layout.Attributes)
        {
            glEnableVertexAttribArray(attr.Location);

            // TODO: awful
            if (attr.IsLong)
            {
                glVertexAttribLPointer(attr.Location, 1, GL_UNSIGNED_INT64_ARB, layout.Stride,
                                       (const void *)(uintptr_t)attr.Offset);
            }
            else
            {
                glVertexAttribPointer(attr.Location, attr.Size, GL_FLOAT, GL_FALSE, layout.Stride,
                                      (const void *)(uintptr_t)attr.Offset);
            }

            if (inputRate == VertexInputRate::Instance)
                glVertexAttribDivisor(attr.Location, 1);
        }
    }

    void VertexArray::SetIndexBuffer(const Buffer *buffer)
    {
        Bind();
        buffer->Bind();
    }

} // namespace VW
