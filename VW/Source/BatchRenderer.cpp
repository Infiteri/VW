#include "BatchRenderer.h"
#include "Core/Logger.h"
#include "Renderer.h"
#include "Shader/ShaderSystem.h"
#include "Texture/TextureSystem.h"
#include <glad/glad.h>

namespace VW
{
    BatchRenderer::BatchRenderer(u64 maxInstances) : m_MaxInstances(maxInstances)
    {
        m_InstanceBuffer = new Buffer(BufferType::Vertex, BufferUsage::Dynamic);
        InstanceData *temp = new InstanceData[maxInstances];
        m_InstanceBuffer->SetData(temp, sizeof(InstanceData) * maxInstances);
        delete[] temp;

        m_InstanceLayout.Stride = sizeof(InstanceData);
        m_InstanceLayout.Attributes = {
            {6, 0, 4, false},   // Row0
            {7, 16, 4, false},  // Row1
            {8, 32, 4, false},  // Row2
            {9, 48, 4, false},  // Row3
            {10, 64, 4, false}, // Color
            {11, 80, 2, true},  // AlbedoHandle (uvec2)
            {12, 88, 2, true},  // NormalHandle (uvec2)
        };
    }

    BatchRenderer::~BatchRenderer()
    {
        delete m_InstanceBuffer;
    }

    void BatchRenderer::Begin()
    {
        m_Batches.clear();
    }

    void BatchRenderer::Submit(Mesh *mesh, const Matrix4 &transform, Material *material,
                               Shader *shader)
    {
        if (!material)
            return;

        if (!shader)
            shader = ShaderSystem::GetEngineShader("Object.glsl");

        // check total size across all batches
        u64 totalItems = 0;
        for (auto &[s, items] : m_Batches)
            totalItems += items.size();

        if (totalItems >= m_MaxInstances)
            Flush();

        InstanceData data;
        data.Transform = transform;
        data.Material.Color = material->GetColor().Normalized();
        data.Material.AlbedoHandle =
            material->GetAlbedoID() != 0
                ? TextureSystem::GetTextureHandle(material->GetAlbedoID())
                : TextureSystem::GetTextureHandle(TextureSystem::GetDefaultTextureID());
        data.Material.NormalHandle =
            material->GetNormalID() != 0
                ? TextureSystem::GetTextureHandle(material->GetNormalID())
                : TextureSystem::GetTextureHandle(TextureSystem::GetDefaultTextureID());

        m_Batches[shader].push_back({mesh, data});
        Renderer::_GetState().Stats.ItemsSubmited++;
    }

    void BatchRenderer::Flush()
    {
        for (auto &[shader, items] : m_Batches)
        {
            if (items.empty() || !shader)
                continue;

            shader->Use();
            RendererUtils::CoreUniformsToShader(shader);

            std::unordered_map<Mesh *, std::vector<InstanceData>> meshGroups;
            for (auto &[mesh, data] : items)
            {
                meshGroups[mesh].push_back(data);
            }

            for (auto &[mesh, instances] : meshGroups)
            {

                mesh->Bind();
                m_InstanceBuffer->Bind();
                glBufferSubData(GL_ARRAY_BUFFER, 0, instances.size() * sizeof(InstanceData),
                                instances.data());

                if (m_LinkedMeshes.find(mesh) == m_LinkedMeshes.end())
                {
                    mesh->GetVAO()->AddVertexBuffer(m_InstanceBuffer, m_InstanceLayout,
                                                    VertexInputRate::Instance);
                    m_LinkedMeshes.insert(mesh);
                }

                glDrawElementsInstanced(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT,
                                        nullptr, (GLsizei)instances.size());
                Renderer::_GetState().Stats.DrawCalls++;
            }
        }

        m_Batches.clear();
    }

    void BatchRenderer::End()
    {
        Flush();
    }

} // namespace VW
