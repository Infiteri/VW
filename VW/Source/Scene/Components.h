#pragma once

#include "Base.h"
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
} // namespace VW
