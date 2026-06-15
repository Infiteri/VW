#include "Camera/CameraSystem.h"
#include "Camera/PerspectiveCamera.h"
#include "CameraController.h"
#include "Core/Entry.h"
#include "Core/Logger.h"
#include "Core/Platform.h"
#include "Light/LightSystem.h"
#include "Material/Material.h"
#include "Material/MaterialSystem.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Mesh/MeshSystem.h"
#include "Mesh/Model.h"
#include "Mesh/ModelSystem.h"
#include "Renderer.h"
#include "Scene/Actor.h"
#include "Scene/Components.h"
#include "Scene/Scene.h"
#include "Scene/Serializer/SceneSerializer.h"
#include "Shader/ShaderSystem.h"
#include "Texture/CubemapTexture.h"
#include "Texture/TextureSystem.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <cmath>
#include <glfw/glfw3.h>
#include <imgui.h>
#include <memory>
#include <windows.h>

namespace VW
{
    static PerspectiveCamera *cam;
    static CameraController controller;
    static bool firstFrame = true;
    static std::vector<RenderItem> renderItems;

    static Scene scene;

    static i32 s_GridSize = 0;
    static float s_Spacing = 15.5f;
    static bool s_RebuildGrid = true;

    static std::shared_ptr<Model> model;

    static void OnResize(GLFWwindow *window, int w, int h)
    {
        Renderer::Viewport(w, h);
    }

    static void BuildGrid()
    {
        renderItems.clear();
        for (i32 z = 0; z < s_GridSize; z++)
            for (i32 y = 0; y < s_GridSize; y++)
                for (i32 x = 0; x < s_GridSize; x++)
                {
                    RenderItem item;

                    item.Material = MaterialSystem::GetMaterial("mat");
                    item.Mesh = MeshSystem::GetMesh(MeshType::Cube).get();
                    item.Transform = Matrix4::Translate({(float)x * s_Spacing, (float)y * s_Spacing,
                                                         (float)z * s_Spacing}) *
                                     Matrix4::Scale({5, 5, 5});
                    item.Shader = x % 2 != 0 ? ShaderSystem::GetShader("Object2.glsl") : nullptr;
                    renderItems.push_back(item);
                }
        s_RebuildGrid = false;
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
            cam->SetPosition({0, 0, 5});
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

                // FIX: name of an asset that doesn't exist crashes engine, must handle

#if 1
                // TODO: known issue, calling actor->Start() individually causes issues with
                // scene.Start(), actor->Start() will get called twice resulting in multiple
                // component Start function calls FIX: add some flag to know if Start was called and
                // return if true
                scene.GetSky().SetShaderMode("Sky.glsl");
                scene.GetSky().GetShaderUniforms().AddUniform("uColor", Color({1, 125, 255, 255}));
                scene.GetSky().GetShaderUniforms().AddUniform("uIntensity", 1.0f);
                scene.GetSky().GetShaderUniforms().AddUniform("uTiling", Vector2({4.0f, 4.0f}));
                scene.GetSky().GetShaderUniforms().AddUniform("uOffset",
                                                              Vector3({0.5f, 0.0f, 0.0f}));
                scene.GetSky().GetShaderUniforms().AddUniform("uClipPlane",
                                                              Vector4({0.0f, 1.0f, 0.0f, 10.0f}));
                scene.GetSky().GetShaderUniforms().AddUniform("uSampleCount", 8);

                ModelSystem::LoadModel("AK/source/AK47.glb", "a.obj");

                {
                    std::unique_ptr<Actor> actor = std::make_unique<Actor>();
                    actor->GetTransform().Position = Vector3{0.2f, 0.2f, 0.2f};
                    actor->GetTransform().Rotation = Vector3{45, 45, 45};
                    actor->GetTransform().Scale = Vector3{0.2f, 0.2f, 0.2f};
                    auto amb = actor->AddComponent<AmbientLightComponent>();
                    amb->SetIntensity(1.0f);
                    auto mc = actor->AddComponent<ModelComponent>(
                        ModelSystem::GetModel("AK/source/AK47.glb").get());
                    scene.AddActor(std::move(actor));
                }

                {
                    std::unique_ptr<Actor> actor = std::make_unique<Actor>();
                    auto pl = actor->AddComponent<PointLightComponent>();
                    pl->SetPosition(Vector3{3, 2, 0});
                    pl->SetColor(Color{255, 0, 0, 255});
                    pl->SetIntensity(10);
                    pl->SetRange(15);
                    scene.AddActor(std::move(actor));
                }

                {
                    std::unique_ptr<Actor> actor = std::make_unique<Actor>();
                    auto sl = actor->AddComponent<SpotLightComponent>();
                    sl->SetPosition(Vector3{0, 5, 0});
                    sl->SetColor(Color{0, 255, 255, 255});
                    sl->SetIntensity(5);
                    sl->SetRange(20);
                    sl->SetDirection(Vector3{0, -1, 0});
                    scene.AddActor(std::move(actor));
                }

                {
                    std::unique_ptr<Actor> actor = std::make_unique<Actor>();
                    actor->Start();
                    auto dl = actor->AddComponent<DirectionalLightComponent>();
                    dl->SetDirection(Vector3{0.5f, -1.0f, -0.5f});
                    dl->SetColor(Color{255, 255, 200, 255});
                    dl->SetIntensity(1.5f);
                    scene.AddActor(std::move(actor));
                }

                SceneSerializer ser(&scene);
                ser.Serialize("Scene2.vwscn");

                scene.Start();
#else
                //            scene.AddActor(std::move(actor));
                SceneSerializer ser(&scene);
                ser.Deserialize("Scene2.vwscn");
                scene.Start();
                LightSystem::LightUpdated();
#endif
            }

            // TODO:
            scene.Render();

            if (s_RebuildGrid)
                BuildGrid();

            for (auto &item : renderItems)
                Renderer::Submit(item);
        }

        void RenderImGui() override
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Stats");
            ImGui::Text("%i draw calls", Renderer::GetStats().DrawCalls);
            ImGui::Text("%i items submitted", (i32)Renderer::GetStats().ItemsSubmited);
            ImGui::Checkbox("Wireframe", &Renderer::GetDebugSettings().RenderWireframe);

            ImGui::Separator();
            ImGui::Text("Grid");
            ImGui::DragInt("Grid Size", &s_GridSize, 1.0f, 1, 1000);
            ImGui::DragFloat("Spacing", &s_Spacing, 0.1f, 0.0f, 100.0f);
            if (ImGui::Button("Rebuild"))
                s_RebuildGrid = true;

            ImGui::Separator();
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
