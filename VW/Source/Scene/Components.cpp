#include "Components.h"
#include "Core/Logger.h"
#include "Light/LightSystem.h"
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

    DirectionalLightComponent::DirectionalLightComponent()
    {
        m_Light = std::make_shared<DirectionalLight>();

        m_Light->SetDirection({0, -1, 0});
        m_Light->SetIntensity(10);
    }

    DirectionalLightComponent::~DirectionalLightComponent()
    {
        LightSystem::RemoveLight(m_Light);
    }

    void DirectionalLightComponent::SetDirection(const Vector3 &direction)
    {
        m_Light->SetDirection(direction);
        LightSystem::LightUpdated();
    }

    const Vector3 &DirectionalLightComponent::GetDirection() const
    {
        return m_Light->GetDirection();
    }

    void DirectionalLightComponent::SetColor(const Color &color)
    {
        m_Light->SetColor(color);
        LightSystem::LightUpdated();
    }

    const Color &DirectionalLightComponent::GetColor() const
    {
        return m_Light->GetColor();
    }

    void DirectionalLightComponent::SetIntensity(float intensity)
    {
        m_Light->SetIntensity(intensity);
        LightSystem::LightUpdated();
    }

    float DirectionalLightComponent::GetIntensity() const
    {
        return m_Light->GetIntensity();
    }

    void DirectionalLightComponent::Start()
    {
        LightSystem::AddLight(m_Light);
        LightSystem::LightUpdated();
    }

} // namespace VW
