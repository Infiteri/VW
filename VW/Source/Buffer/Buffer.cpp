#include "Buffer.h"

#include <glad/glad.h>

namespace VW
{

    static GLenum ToGLTarget(BufferType t)
    {
        switch (t)
        {
        case BufferType::Vertex:
            return GL_ARRAY_BUFFER;
        case BufferType::Index:
            return GL_ELEMENT_ARRAY_BUFFER;
        case BufferType::SSBO:
            return GL_SHADER_STORAGE_BUFFER;
        case BufferType::Uniform:
            return GL_UNIFORM_BUFFER;
        case BufferType::Indirect:
            return GL_DRAW_INDIRECT_BUFFER;
        default:
            return GL_ARRAY_BUFFER;
        }
    }

    static GLenum ToGLUsage(BufferUsage u)
    {
        switch (u)
        {
        case BufferUsage::Static:
            return GL_STATIC_DRAW;
        case BufferUsage::Dynamic:
            return GL_DYNAMIC_DRAW;
        case BufferUsage::Stream:
            return GL_STREAM_DRAW;
        default:
            return GL_STATIC_DRAW;
        }
    }

    Buffer::Buffer(BufferType type, BufferUsage usage)
        : m_Size(0), m_Stride(0), m_Type(type), m_Usage(usage)
    {
        glGenBuffers(1, &m_ID);
    }

    Buffer::~Buffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void Buffer::SetData(const void *data, u32 size)
    {
        Bind();

        m_Size = size;

        glBufferData(ToGLTarget(m_Type), size, data, ToGLUsage(m_Usage));
    }

    void Buffer::UpdateData(const void *data, u32 size, u32 offset)
    {
        Bind();

        if (offset + size > m_Size)
        {
            SetData(data, size);
            return;
        }

        glBufferSubData(ToGLTarget(m_Type), offset, size, data);
    }

    void Buffer::Bind() const
    {
        glBindBuffer(ToGLTarget(m_Type), m_ID);
    }

    void Buffer::Unbind() const
    {
        glBindBuffer(ToGLTarget(m_Type), 0);
    }

} // namespace VW
