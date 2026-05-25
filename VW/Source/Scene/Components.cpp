#include "Components.h"
#include "Renderer.h"

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
} // namespace VW
