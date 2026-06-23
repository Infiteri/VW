#pragma once

#include "Panel.h"
#include "Scene/Actor.h"
#include "Scene/Scene.h"
namespace VW
{
    class SceneHierarchyPanel : public Panel
    {
    public:
        SceneHierarchyPanel(Scene *scene);
        ~SceneHierarchyPanel() = default;

        void Render() override;

        void RenderNode(Actor *actor);

        inline Actor *GetSelected() const
        {
            return m_Selected;
        }

    private:
        Scene *m_Scene;
        Actor *m_Selected = nullptr;
    };
} // namespace VW
