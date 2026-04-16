#include "Mesh.h"

namespace VW
{
    Mesh::Mesh(const Vertex *vertices, u32 vertexSize, u32 *indices, u32 indexCount,
               const VertexLayout &layout)
        : m_IndexCount(indexCount)
    {
        m_VAO = std::make_unique<VertexArray>();

        m_VBO = std::make_unique<Buffer>(BufferType::Vertex, BufferUsage::Static);
        m_VBO->SetData(vertices, vertexSize);

        m_EBO = std::make_unique<Buffer>(BufferType::Index, BufferUsage::Static);
        m_EBO->SetData(indices, indexCount * sizeof(u32));

        m_VAO->AddVertexBuffer(m_VBO.get(), layout, VertexInputRate::Vertex);
        m_VAO->SetIndexBuffer(m_EBO.get());

        m_VAO->Unbind();

        // compute bounds
        const u32 vertexCount = vertexSize / sizeof(Vertex);
        Vector3 min = vertices[0].Position;
        Vector3 max = vertices[0].Position;

        for (u32 i = 1; i < vertexCount; ++i)
        {
            min = Vector3::Min(min, vertices[i].Position);
            max = Vector3::Max(max, vertices[i].Position);
        }

        m_BoundsCenter = (min + max) * 0.5f;

        m_BoundRadius = 0.0f;
        for (u32 i = 0; i < vertexCount; ++i)
        {
            float d = (vertices[i].Position - m_BoundsCenter).Length();
            if (d > m_BoundRadius)
                m_BoundRadius = d;
        }
    }

    Mesh::~Mesh()
    {
    }

    void Mesh::Bind() const
    {
        m_VAO->Bind();
    }

}; // namespace VW
