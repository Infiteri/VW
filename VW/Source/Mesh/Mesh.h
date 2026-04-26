#pragma once

#include "Base.h"
#include "Buffer/Buffer.h"
#include "Buffer/VertexArray.h"
#include "Math/Vector.h"
#include <memory>

namespace VW
{
    struct Vertex
    {
        Vector3 Position;
        Vector2 UV;
        Vector3 Normal;
        Vector3 Tangent;
        Vector3 Bitangent;
    };

    class VW_API Mesh
    {
    public:
        Mesh(const Vertex *vertices, u32 vertexSize, u32 *indices, u32 indexCount,
             const VertexLayout &layout);
        ~Mesh();

        void Bind() const;

        inline u32 GetIndexCount() const
        {
            return m_IndexCount;
        }

        inline VertexArray *GetVAO()
        {
            return m_VAO.get();
        };

        inline const Vector3 &GetBoundsCenter()
        {
            return m_BoundsCenter;
        };

        inline float GetBoundsRadius()
        {
            return m_BoundRadius;
        };

        void AddInstanceBuffer(const Buffer *buffer, const VertexLayout &layout);

    private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<Buffer> m_VBO, m_EBO;

        u32 m_IndexCount;

        Vector3 m_BoundsCenter;
        float m_BoundRadius;
    };
}; // namespace VW
