#include "Components.h"
#include "Material/MaterialSystem.h"
#include "Renderer.h"
#include "Shader/ShaderSystem.h"

namespace VW
{
    MeshComponent::MeshComponent(Mesh *mesh)
    {
        m_Item.Mesh = mesh;
    }

    MeshComponent::~MeshComponent()
    {
    }

    void MeshComponent::SetMesh(Mesh *mesh)
    {
        m_Item.Mesh = mesh;
    }

    void MeshComponent::SetMaterial(Material *material)
    {
        m_Item.Material = material;
    }

    void MeshComponent::SetShader(Shader *shader)
    {
        m_Item.Shader = shader;
    }

    void MeshComponent::SetTransform(const Transform &transform)
    {
        m_Item.Transform = transform.GetMatrix();
    }

    void MeshComponent::Render()
    {
        Renderer::Submit(m_Item);
    }

    ModelComponent::ModelComponent(Model *model) : m_Model(model)
    {
    }

    ModelComponent::~ModelComponent()
    {
    }

    void ModelComponent::Render()
    {
        if (!m_Model)
            return;

        for (const auto &sm : m_Model->GetSubmeshes())
        {
            // TODO: empty names shouldn't happen, but handle it by setting the default material
            if (!sm.Mesh || sm.MaterialName.empty())
                return;

            RenderItem item;
            item.Mesh = sm.Mesh.get();
            item.Material = MaterialSystem::GetMaterial(sm.MaterialName);
            item.Shader = ShaderSystem::GetEngineShader("Object.glsl");
            Renderer::Submit(item);
        }
    }
} // namespace VW
