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

    private:
        Scene *m_Scene;
    };
} // namespace VW
