#include "Components.h"
#include "Core/Logger.h"
#include "Light/Light.h"
#include "Light/LightSystem.h"
#include "Material/MaterialSystem.h"
#include "Renderer.h"
#include "Shader/ShaderSystem.h"
#include <winnls.h>

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

    // TODO: DRY, move all lights to another file, move components to a folder, fix all issues that
    // are known

    AmbientLightComponent::AmbientLightComponent()
    {
        m_Light = std::make_shared<AmbientLight>();
        m_Light->SetColor({255, 255, 255, 255});
        m_Light->SetIntensity(1.0f);
    }

    AmbientLightComponent::~AmbientLightComponent()
    {
        LightSystem::RemoveLight(m_Light);
    }

    void AmbientLightComponent::SetColor(const Color &color)
    {
        m_Light->SetColor(color);
        LightSystem::LightUpdated();
    }

    const Color &AmbientLightComponent::GetColor() const
    {
        return m_Light->GetColor();
    }

    void AmbientLightComponent::SetIntensity(float intensity)
    {
        m_Light->SetIntensity(intensity);
        LightSystem::LightUpdated();
    }

    float AmbientLightComponent::GetIntensity() const
    {
        return m_Light->GetIntensity();
    }

    void AmbientLightComponent::Start()
    {
        LightSystem::AddLight(m_Light);
        LightSystem::LightUpdated();
    }

    DirectionalLightComponent::DirectionalLightComponent()
    {
        m_Light = std::make_shared<DirectionalLight>();

        m_Light->SetDirection({0, -1, 0});
        m_Light->SetIntensity(0.5);
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

    PointLightComponent::PointLightComponent()
    {
        m_Light = std::make_shared<PointLight>();
    }

    PointLightComponent::~PointLightComponent()
    {
        LightSystem::RemoveLight(m_Light);
        LightSystem::LightUpdated();
    }

    void PointLightComponent::SetColor(const Color &color)
    {
        m_Light->SetColor(color);
        LightSystem::LightUpdated();
    }

    const Color &PointLightComponent::GetColor() const
    {
        return m_Light->GetColor();
    }

    void PointLightComponent::SetIntensity(float intensity)
    {
        m_Light->SetIntensity(intensity);
        LightSystem::LightUpdated();
    }

    float PointLightComponent::GetIntensity() const
    {
        return m_Light->GetIntensity();
    }

    // FIX: maybe call "set delta" as in this delta position will be added to the actors world
    // position
    // NOTE: keep track of old position, check if position is new, if its new then update
    // the light position and notify the light system, this will happen in render
    void PointLightComponent::SetPosition(const Vector3 &position)
    {
        m_Light->SetPosition(position);
        LightSystem::LightUpdated();
    }

    const Vector3 &PointLightComponent::GetPosition() const
    {
        return m_Light->GetPosition();
    }

    void PointLightComponent::SetRange(float range)
    {
        m_Light->SetRange(range);
        LightSystem::LightUpdated();
    }

    float PointLightComponent::GetRange() const
    {
        return m_Light->GetRange();
    }

    void PointLightComponent::Start()
    {
        LightSystem::AddLight(m_Light);
        LightSystem::LightUpdated();
    }

    SpotLightComponent::SpotLightComponent()
    {
        m_Light = std::make_shared<SpotLight>();
    }

    SpotLightComponent::~SpotLightComponent()
    {
        LightSystem::RemoveLight(m_Light);
        LightSystem::LightUpdated();
    }

    void SpotLightComponent::SetColor(const Color &color)
    {
        m_Light->SetColor(color);
        LightSystem::LightUpdated();
    }

    const Color &SpotLightComponent::GetColor() const
    {
        return m_Light->GetColor();
    }

    void SpotLightComponent::SetIntensity(float intensity)
    {
        m_Light->SetIntensity(intensity);
        LightSystem::LightUpdated();
    }

    float SpotLightComponent::GetIntensity() const
    {
        return m_Light->GetIntensity();
    }

    void SpotLightComponent::SetPosition(const Vector3 &position)
    {
        m_Light->SetPosition(position);
        LightSystem::LightUpdated();
    }

    const Vector3 &SpotLightComponent::GetPosition() const
    {
        return m_Light->GetPosition();
    }

    void SpotLightComponent::SetDirection(const Vector3 &direction)
    {
        m_Light->SetDirection(direction);
        LightSystem::LightUpdated();
    }

    const Vector3 &SpotLightComponent::GetDirection() const
    {
        return m_Light->GetDirection();
    }

    void SpotLightComponent::SetRange(float range)
    {
        m_Light->SetRange(range);
        LightSystem::LightUpdated();
    }

    float SpotLightComponent::GetRange() const
    {
        return m_Light->GetRange();
    }

    void SpotLightComponent::SetInnerConeAngle(float angle)
    {
        m_Light->SetInnerConeAngle(angle);
        LightSystem::LightUpdated();
    }

    float SpotLightComponent::GetInnerConeAngle() const
    {
        return m_Light->GetInnerConeAngle();
    }

    void SpotLightComponent::SetOuterConeAngle(float angle)
    {
        m_Light->SetOuterConeAngle(angle);
        LightSystem::LightUpdated();
    }

    float SpotLightComponent::GetOuterConeAngle() const
    {
        return m_Light->GetOuterConeAngle();
    }

    void SpotLightComponent::Start()
    {
        LightSystem::AddLight(m_Light);
        LightSystem::LightUpdated();
    }
} // namespace VW
