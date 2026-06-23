#pragma once

#include "Window.h"
#include "imgui.h"
namespace VW
{
    namespace GUI
    {
        struct DockspaceSettings
        {
            bool Open = true;
            bool Fullscreen = true;
            ImGuiDockNodeFlags Flags = ImGuiDockNodeFlags_None;
            ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            ImVec2 ViewportLeftTop;
            ImVec2 ViewportRightBottom;
        };

        void Init(Window &window);
        void BeginFrame();
        void EndFrame();
        void Shutdown();

        void BeginEditorDockspace();
        void EndEditorDockspace();

        DockspaceSettings& GetDockspaceSettings();
    } // namespace GUI
} // namespace VW
