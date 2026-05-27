#pragma once

#include "Base.h"
#include "Light/DirectionalLight.h"
#include "Material/Material.h"
#include "Math/Transform.h"
#include "Mesh/Mesh.h"
#include "Mesh/Model.h"
#include "Renderer.h"

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
        void SetTransform(const Transform &transform);

        void Render();

    private:
        RenderItem m_Item;
    };

    class VW_API ModelComponent : public Component
    {
    public:
        ModelComponent(Model *model);
        ~ModelComponent();

        void Render();

    private:
        Model *m_Model;
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
} // namespace VW
