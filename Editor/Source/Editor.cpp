#include "Camera/CameraSystem.h"
#include "Camera/PerspectiveCamera.h"
#include "CameraController.h"
#include "Core/Entry.h"
#include "Core/Logger.h"
#include "Core/Platform.h"
#include "GUI.h"
#include "Renderer.h"
#include "Scene/Scene.h"
#include "Scene/Serializer/SceneSerializer.h"
#include "Window.h"

#include "Color.h"
#include "Math/Vector.h"
#include <imgui.h>
#include <memory>
#include <windows.h>

namespace VW
{
    static PerspectiveCamera *cam;
    static CameraController controller;

    static Scene scene;

    // tunable black hole params
    static float s_BH_Intensity = 1.2f;
    static float s_BH_DiskInner = 3.0f;
    static float s_BH_DiskOuter = 10.0f;
    static float s_BH_Lensing = 1.5f;
    static float s_BH_StarDensity = 0.6f;
    static float s_BH_Mass = 1.0f;
    static int s_BH_SampleCount = 48;
    static float s_BH_Near = 0.1f;
    static float s_BH_Far = 60.0f;
    static Vector3 s_BH_Offset = {0.0f, 0.0f, 0.0f};
    static float s_BH_Color[4] = {1.0f, 0.86f, 0.7f, 1.0f};

    struct EditorState
    {
        ImVec2 LastFrameViewport;
    };
    static EditorState s_State;

    class EditorPlatform : public Platform
    {
    public:
        EditorPlatform() = default;
        ~EditorPlatform() = default;

        void PreInit() override
        {
            Logger::GetSettings().FancyFormat = true;
            Logger::GetSettings().Format = "[PREFIX]: MSG";
            VW_LOG_ADD_CATEGORY("vwed", "Editor");
            VW_INFO("vwed", "Initializing...");

            CameraSystem::AddCamera("MainEditorCamera", std::make_shared<PerspectiveCamera>(
                                                            120, 16.f / 9, 0.001, 1000.0f));
            CameraSystem::ActivateCamera("MainEditorCamera");
            cam = (PerspectiveCamera *)CameraSystem::GetCamera("MainEditorCamera");

            controller.SetCamera(cam);
            cam->SetPosition({0, 0, 15});

            auto &su = scene.GetSky().GetShaderUniforms();
            su.AddUniform("uColor", Color(255, 220, 180, 255));
            su.AddUniform("uIntensity", 1.2f);
            su.AddUniform("uTiling", Vector2(3.0f, 10.0f));
            su.AddUniform("uOffset", Vector3(0.0f, 0.0f, 0.0f));
            su.AddUniform("uClipPlane", Vector4(0.1f, 1.5f, 0.6f, 60.0f));
            su.AddUniform("uSampleCount", 48);
            su.AddUniform("uMass", 1.0f);
        }

        bool ShouldShutdown() override
        {
            return m_Window.ShouldClose();
        }

        void Shutdown() override
        {
            GUI::Shutdown();
        }

        void Init() override
        {
            GUI::Init(m_Window);

            scene.GetSky().SetShaderMode("Sky.glsl");

#if 0
                          // serialize
                          auto pa = std::make_unique<Actor>();
                          pa->SetName("Parent");
                          pa->GetTransform().Position = Vector3(0, 0, 0);
                          pa->AddComponent<MeshComponent>(MeshSystem::GetMesh(MeshType::Cube).get());

                          auto ca = std::make_unique<Actor>();
                          ca->SetName("Child");
                          ca->GetTransform().Position = Vector3(2, 0, 0);
                          ca->GetTransform().Scale = Vector3(0.1, 0.1, 0.1);
                          ModelSystem::LoadModel("a.obj", "a.obj");
                          ca->AddComponent<ModelComponent>(ModelSystem::GetModel("a.obj").get());

                          pa->AddChild(std::move(ca));
                          scene.AddActor(std::move(pa));

                          SceneSerializer ser(&scene);
                          ser.Serialize("Scene2.vwscn");
#endif

#if 1
            SceneSerializer ser(&scene);
            ser.Deserialize("Scene2.vwscn");
#endif
            scene.Start();
        }

        void Render() override
        {
            controller.Update(m_Window.GetHandle());

            scene.Render();
        }

        void Viewport()
        {
            Renderer::Viewport(s_State.LastFrameViewport.x, s_State.LastFrameViewport.y);
        }

        void RenderSceneInViewport()
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
            ImGui::Begin("Viewport");
            ImGui::PopStyleVar();

            // Update renderer viewport
            ImVec2 viewportSize = ImGui::GetContentRegionAvail();
            if (viewportSize.x != s_State.LastFrameViewport.x ||
                viewportSize.y != s_State.LastFrameViewport.y)
            {
                s_State.LastFrameViewport = viewportSize;
                Viewport();
            }

            GUI::GetDockspaceSettings().ViewportLeftTop = ImGui::GetWindowPos();
            GUI::GetDockspaceSettings().ViewportRightBottom = ImGui::GetWindowSize();

            ImGui::Image((void *)(u64)(u32)(Renderer::GetPassID(0)), viewportSize, ImVec2{0, 1},
                         ImVec2{1, 0});

            ImGui::End();
        }

        void RenderImGui() override
        {
            GUI::BeginFrame();
            GUI::BeginEditorDockspace();

            RenderSceneInViewport();

            ImGui::Begin("Black Hole");
            {
                auto &su = scene.GetSky().GetShaderUniforms();
                if (ImGui::ColorEdit3("Color", s_BH_Color))
                    su.GetUniform("uColor") = Color(s_BH_Color[0] * 255, s_BH_Color[1] * 255,
                                                    s_BH_Color[2] * 255, s_BH_Color[3] * 255);
                if (ImGui::DragFloat("Intensity", &s_BH_Intensity, 0.1f))
                    su.GetUniform("uIntensity") = s_BH_Intensity;
                if (ImGui::DragFloat("Mass", &s_BH_Mass, 0.1f))
                    su.GetUniform("uMass") = s_BH_Mass;
                if (ImGui::DragFloat("Disk Inner", &s_BH_DiskInner, 0.1f))
                    su.GetUniform("uTiling") = Vector2(s_BH_DiskInner, s_BH_DiskOuter);
                if (ImGui::DragFloat("Disk Outer", &s_BH_DiskOuter, 0.1f))
                    su.GetUniform("uTiling") = Vector2(s_BH_DiskInner, s_BH_DiskOuter);
                if (ImGui::DragFloat("Lensing", &s_BH_Lensing, 0.1f))
                    su.GetUniform("uClipPlane") =
                        Vector4(s_BH_Near, s_BH_Lensing, s_BH_StarDensity, s_BH_Far);
                if (ImGui::DragFloat("Star Density", &s_BH_StarDensity, 0.1f))
                    su.GetUniform("uClipPlane") =
                        Vector4(s_BH_Near, s_BH_Lensing, s_BH_StarDensity, s_BH_Far);
                if (ImGui::DragFloat("Near Clip", &s_BH_Near, 0.1f))
                    su.GetUniform("uClipPlane") =
                        Vector4(s_BH_Near, s_BH_Lensing, s_BH_StarDensity, s_BH_Far);
                if (ImGui::DragFloat("Far Clip", &s_BH_Far, 0.1f))
                    su.GetUniform("uClipPlane") =
                        Vector4(s_BH_Near, s_BH_Lensing, s_BH_StarDensity, s_BH_Far);
                if (ImGui::DragInt("Samples", &s_BH_SampleCount, 1))
                    su.GetUniform("uSampleCount") = s_BH_SampleCount;
                if (ImGui::DragFloat3("Offset", (float *)&s_BH_Offset, 0.1f))
                    su.GetUniform("uOffset") = s_BH_Offset;
            }
            ImGui::Separator();
            ImGui::Text("%i draw calls", Renderer::GetStats().DrawCalls);
            ImGui::Text("%i items submitted", (i32)Renderer::GetStats().ItemsSubmited);
            ImGui::Checkbox("Wireframe", &Renderer::GetDebugSettings().RenderWireframe);
            {
                auto &debug = Renderer::GetDebugSettings();
                const char *modes[] = {"Full", "UV", "Normal", "Tangent", "Bitangent"};
                int current = static_cast<int>(debug.RenderMode);
                if (ImGui::Combo("Render Mode", &current, modes, IM_ARRAYSIZE(modes)))
                    debug.RenderMode = static_cast<RenderDebugMode>(current);
            }
            ImGui::End();

            GUI::EndEditorDockspace();

            GUI::EndFrame();
            m_Window.SwapBuffers();
        }

    private:
        Window m_Window;
    };
} // namespace VW

VW::Platform *VW::InitPlatform()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE)
    {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode))
        {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
    return new VW::EditorPlatform();
}
