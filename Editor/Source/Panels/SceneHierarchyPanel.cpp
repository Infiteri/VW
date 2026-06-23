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

        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Create Actor"))
                m_Scene->CreateActor("Actor");
            ImGui::EndPopup();
        }

        for (auto &actor : m_Scene->GetRoots())
            RenderNode(actor.get());

        // Drop zone to deparent (make root)
        float availY = ImGui::GetContentRegionAvail().y;
        ImGui::InvisibleButton("DropRoot", ImVec2(-1, availY > 1.0f ? availY : 1.0f));
        if (ImGui::BeginDragDropTarget())
        {
            auto payload = ImGui::AcceptDragDropPayload("ACTOR");
            if (payload)
            {
                Actor *dragged = *(Actor **)payload->Data;
                if (dragged)
                    m_Scene->Reparent(dragged, nullptr);
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::End();
    }

    void SceneHierarchyPanel::RenderNode(Actor *actor)
    {
        VW_CHECK(actor);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (actor->GetChildren().empty())
            flags |= ImGuiTreeNodeFlags_Leaf;

        if (actor == m_Selected)
            flags |= ImGuiTreeNodeFlags_Selected;

        bool open =
            ImGui::TreeNodeEx((void *)&actor->GetID(), flags, "%s", actor->GetName().c_str());

        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("ACTOR", &actor, sizeof(Actor *));
            ImGui::Text("%s", actor->GetName().c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            auto payload = ImGui::AcceptDragDropPayload("ACTOR");
            if (payload)
            {
                Actor *dragged = *(Actor **)payload->Data;
                if (dragged != actor)
                    m_Scene->Reparent(dragged, actor);
            }
            ImGui::EndDragDropTarget();
        }

        if (ImGui::IsItemClicked())
            m_Selected = actor;

        bool deleted = false;

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Create Child"))
                actor->AddChild(std::make_unique<Actor>("Child"));

            if (ImGui::MenuItem("Delete"))
            {
                m_Scene->DestroyActor(actor);
                if (m_Selected == actor)
                    m_Selected = nullptr;
                deleted = true;
            }

            ImGui::EndPopup();
        }

        if (open)
        {
            if (!deleted)
                for (auto &child : actor->GetChildren())
                    RenderNode(child.get());

            ImGui::TreePop();
        }
    }
} // namespace VW
