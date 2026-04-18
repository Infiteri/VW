#include "BatchRenderer.h"
#include "Core/Logger.h"
#include <glad/glad.h>
namespace VW
{
    BatchRenderer::BatchRenderer(u64 maxInstances) : m_MaxInstances(maxInstances)
    {
        m_InstanceStorage.reserve(m_MaxInstances);
        m_InstanceBuffer = new Buffer(BufferType::Vertex, BufferUsage::Dynamic);
        m_InstanceBuffer->SetData(nullptr, sizeof(InstanceData) * m_MaxInstances);
        m_InstanceLayout.Stride = sizeof(InstanceData);
        m_InstanceLayout.Attributes = {
            {4, 0, 4, false},  // Transform Row0 (mat4 col 0)
            {5, 16, 4, false}, // Transform Row1
            {6, 32, 4, false}, // Transform Row2
            {7, 48, 4, false}, // Transform Row3
            {8, 64, 4, false}, // Color
        };
    }
    BatchRenderer::~BatchRenderer()
    {
        delete m_InstanceBuffer;
    }
    void BatchRenderer::Begin()
    {
        m_CurrentMesh = nullptr;
        m_InstanceStorage.clear();
    }
    void BatchRenderer::Submit(Mesh *mesh, const Matrix4 &transform, const MaterialGPU &material)
    {
        if ((m_CurrentMesh && mesh != m_CurrentMesh) || m_InstanceStorage.size() >= m_MaxInstances)
        {
            Flush();
        }
        m_CurrentMesh = mesh;
        InstanceData data;
        data.Transform = transform;
        data.Material.Color = material.Color.Normalized();
        m_InstanceStorage.push_back(data);
    }

    void BatchRenderer::Flush()
    {
        if (m_InstanceStorage.empty() || !m_CurrentMesh)
            return;
        m_InstanceBuffer->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_InstanceStorage.size() * sizeof(InstanceData),
                        m_InstanceStorage.data());
        if (m_LinkedMeshes.find(m_CurrentMesh) == m_LinkedMeshes.end())
        {
            m_CurrentMesh->GetVAO()->AddVertexBuffer(m_InstanceBuffer, m_InstanceLayout,
                                                     VertexInputRate::Instance);
            m_LinkedMeshes.insert(m_CurrentMesh);
        }
        m_CurrentMesh->Bind();
        glDrawElementsInstanced(GL_TRIANGLES, m_CurrentMesh->GetIndexCount(), GL_UNSIGNED_INT,
                                nullptr, (GLsizei)m_InstanceStorage.size());
        m_InstanceStorage.clear();
    }
    void BatchRenderer::End()
    {
        Flush();
    }
} // namespace VW
