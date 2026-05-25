#pragma once

#include "Base.h"
#include "Material/Material.h"
#include "Math/Transform.h"
#include "Mesh/Mesh.h"
#include "Renderer.h"

namespace VW
{
    class Actor;
    class Component
    {
    public:
        Component() {};
        virtual ~Component() {};

        virtual void Start() {}
        virtual void Render() {}
        virtual void Update() {}
        virtual void Stop() {}

        virtual inline Actor *GetOwner() const
        {
            return m_Owner;
        }

    protected:
        Actor *m_Owner;
        friend class Actor;
    };

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
} // namespace VW
