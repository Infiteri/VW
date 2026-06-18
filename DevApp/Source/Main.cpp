#include "Camera/CameraSystem.h"
#include "Camera/PerspectiveCamera.h"
#include "CameraController.h"
#include "Core/Entry.h"
#include "Core/Logger.h"
#include "Core/Platform.h"
#include "Mesh/MeshSystem.h"
#include "Mesh/Model.h"
#include "Mesh/ModelSystem.h"
#include "Renderer.h"
#include "Scene/Actor.h"
#include "Scene/Components.h"
#include "Scene/Scene.h"
#include "Scene/Serializer/SceneSerializer.h"

#include "Color.h"
#include "Math/Vector.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glfw/glfw3.h>
#include <imgui.h>
#include <memory>
#include <windows.h>

namespace VW
{
    static PerspectiveCamera *cam;
    static CameraController controller;
    static bool firstFrame = true;

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

    static void OnResize(GLFWwindow *window, int w, int h)
    {
        Renderer::Viewport(w, h);
    }

    class DevAppPlatform : public Platform
    {
    public:
        DevAppPlatform() = default;
        ~DevAppPlatform() = default;

        void Init() override
        {
            CameraSystem::AddCamera(
                "main", std::make_shared<PerspectiveCamera>(120, 16.f / 9, 0.001, 1000.0f));
            CameraSystem::ActivateCamera("main");
            cam = (PerspectiveCamera *)CameraSystem::GetCamera("main");

            Logger::GetSettings().FancyFormat = true;
            Logger::GetSettings().Format = "[PREFIX]: MSG";
            VW_LOG_ADD_CATEGORY("vwdp", "Dev");
            VW_INFO("vwdp", "Initializing...");

            glfwInit();
            m_Handle = glfwCreateWindow(100, 100, "DevApp", nullptr, nullptr);
            glfwMakeContextCurrent(m_Handle);
            glfwMaximizeWindow(m_Handle);
            glfwSetWindowSizeCallback(m_Handle, OnResize);
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
            return glfwWindowShouldClose(m_Handle);
        }

        void Shutdown() override
        {
            glfwDestroyWindow(m_Handle);
            glfwTerminate();
        }

        void Render() override
        {
            controller.Update(m_Handle);

            // TODO: some kind of `PreInit` instead of current `Init` and `Init` instead of first
            // frame (`Init` called after OpenGL functions are loaded)
            if (firstFrame)
            {
                firstFrame = false;
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGui::StyleColorsDark();
                ImGui_ImplGlfw_InitForOpenGL(m_Handle, true);
                ImGui_ImplOpenGL3_Init("#version 430");

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

            scene.Render();
        }

        void RenderImGui() override
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

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

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(m_Handle);
            glfwPollEvents();
        }

    private:
        GLFWwindow *m_Handle;
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
    return new VW::DevAppPlatform();
}
