#pragma once

#include "Base.h"
#include "Light/AmbientLight.h"
#include "Light/DirectionalLight.h"
#include "Light/Light.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Material/Material.h"
#include "Math/Transform.h"
#include "Mesh/Mesh.h"
#include "Mesh/Model.h"
#include "Renderer.h"
#include <memory>

namespace VW
{
    class Actor;
    class Component
    {
    public:
        Component() {};
        virtual ~Component() {};

        virtual void Start()
        {
        }
        virtual void Render()
        {
        }
        virtual void Update()
        {
        }
        virtual void Stop()
        {
        }

        virtual inline Actor *GetOwner() const
        {
            return m_Owner;
        }

    protected:
        Actor *m_Owner;
        friend class Actor;
    };

    // TODO: more constructors

    class VW_API MeshComponent : public Component
    {
    public:
        MeshComponent(Mesh *mesh);
        ~MeshComponent();

        void SetMesh(Mesh *mesh);
        void SetMaterial(Material *material);
        void SetShader(Shader *shader);
        void SetDeltaTransform(const Transform &transform);

        inline Mesh *GetMesh() const
        {
            return m_Item.Mesh;
        }

        inline Material *GetMaterial() const
        {
            return m_Item.Material;
        }

        inline const Transform &GetDeltaTransform() const
        {
            return m_Delta;
        }

        void Render();

    private:
        RenderItem m_Item;
        Transform m_Delta;
    };

    class VW_API ModelComponent : public Component
    {
    public:
        ModelComponent(Model *model);
        ~ModelComponent();

        void SetTransform(const Transform &transform);

        void Render();

    private:
        Model *m_Model;
        Transform m_Transform;
    };

    class VW_API AmbientLightComponent : public Component
    {
    public:
        AmbientLightComponent();
        ~AmbientLightComponent();

        void SetColor(const Color &color);
        const Color &GetColor() const;

        void SetIntensity(float intensity);
        float GetIntensity() const;

        void Start();

    private:
        std::shared_ptr<AmbientLight> m_Light;
    };

    class VW_API DirectionalLightComponent : public Component
    {
    public:
        DirectionalLightComponent();
        ~DirectionalLightComponent();

        void SetDirection(const Vector3 &direction);
        const Vector3 &GetDirection() const;

        void SetColor(const Color &color);
        const Color &GetColor() const;

        void SetIntensity(float intensity);
        float GetIntensity() const;

        void Start();

    private:
        // TODO: really worth thinking about so much smart_ptr stuff, a lot of code relies on unique
        // and smart pointers (mesh system, shader system, ...)
        // TODO: while, yes, opengl related need to be loaded later, Lights are a data structure so
        // no point in doing extra allocations
        // TODO: not that speed is noticable, while yes an extra allocation is done, the overhead is
        // negligible + it only happens once per component
        // TODO: still worth thinking about the amount of smart pointers
        std::shared_ptr<DirectionalLight> m_Light;
    };

    class VW_API PointLightComponent : public Component
    {
    public:
        PointLightComponent();
        ~PointLightComponent();

        void SetColor(const Color &color);
        const Color &GetColor() const;

        void SetIntensity(float intensity);
        float GetIntensity() const;

        // TODO: note that setting the position of the light isn't supposed to work this way, lights
        // should take the actors transform in world space, for now this is a quick fix, also for
        // now there is no kind of parenting, therefore world space is used and local space is the
        // same
        // TODO 2: do i want parenting in general for actors?
        void SetPosition(const Vector3 &position);
        const Vector3 &GetPosition() const;

        void SetRange(float range);
        float GetRange() const;

        void Start();

    private:
        std::shared_ptr<PointLight> m_Light;
    };

    // TODO: position reflects the same issues as PointLightComponent above, fixing
    // PointLightComponent should mean a fix here too
    class VW_API SpotLightComponent : public Component
    {
    public:
        SpotLightComponent();
        ~SpotLightComponent();

        void SetColor(const Color &color);
        const Color &GetColor() const;

        void SetIntensity(float intensity);
        float GetIntensity() const;

        void SetPosition(const Vector3 &position);
        const Vector3 &GetPosition() const;

        void SetDirection(const Vector3 &direction);
        const Vector3 &GetDirection() const;

        void SetRange(float range);
        float GetRange() const;

        void SetInnerConeAngle(float angle);
        float GetInnerConeAngle() const;

        void SetOuterConeAngle(float angle);
        float GetOuterConeAngle() const;

        void Start();

    private:
        std::shared_ptr<SpotLight> m_Light;
    };
} // namespace VW
