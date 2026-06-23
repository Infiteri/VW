#include "SceneHierarchyPanel.h"
#include "Base.h"
#include "imgui.h"

namespace VW
{
    SceneHierarchyPanel::SceneHierarchyPanel(Scene *scene) : m_Scene(scene)
    {
    }

    void SceneHierarchyPanel::Render()
    {
        VW_CHECK(m_Scene);

        ImGui::Begin("Hierarchy");

        for (auto &actor : m_Scene->GetRoots())
            RenderNode(actor.get());

        ImGui::End();
    }

    void SceneHierarchyPanel::RenderNode(Actor *actor)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (actor->GetChildren().empty())
            flags |= ImGuiTreeNodeFlags_Leaf;

        bool open =
            ImGui::TreeNodeEx((void *)&actor->GetID(), flags, "%s", actor->GetName().c_str());
        if (open)
        {
            for (auto &child : actor->GetChildren())
                RenderNode(child.get());
                
            ImGui::TreePop();
        }
    }
} // namespace VW
