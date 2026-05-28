#include "Camera/CameraSystem.h"
#include "Camera/PerspectiveCamera.h"
#include "CameraController.h"
#include "Core/Entry.h"
#include "Core/Logger.h"
#include "Core/Platform.h"
#include "Light/AmbientLight.h"
#include "Light/LightSystem.h"
#include "Material/Material.h"
#include "Material/MaterialSystem.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Mesh/MeshSystem.h"
#include "Mesh/Model.h"
#include "Renderer.h"
#include "Scene/Actor.h"
#include "Scene/Components.h"
#include "Scene/Scene.h"
#include "Scene/Serializer/SceneSerializer.h"
#include "Shader/ShaderSystem.h"
#include "Texture/TextureSystem.h"

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

            if (firstFrame)
            {

                // material
                Material mat;
                mat.SetAlbedoID(TextureSystem::GetTextureID("AK/textures/color.png"));
                mat.SetNormalID(TextureSystem::GetTextureID("AK/textures/normal.png"));
                mat.SetORMID(TextureSystem::GetTextureID("AK/textures/ORM.png"));
                MaterialSystem::AddMaterial("mat", mat);

                firstFrame = false;
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGui::StyleColorsDark();
                ImGui_ImplGlfw_InitForOpenGL(m_Handle, true);
                ImGui_ImplOpenGL3_Init("#version 430");

                // FIX: name of an asset that doesn't exist crashes engine, must handle

                std::unique_ptr<Actor> actor = std::make_unique<Actor>();
                actor->Start();

                // TODO: too much hassle to setup a simple mesh, all of this should be default
                auto amb = actor->AddComponent<AmbientLightComponent>();
                amb->SetIntensity(0.9f);

                // auto point = actor->AddComponent<PointLightComponent>();
                // point->SetPosition(Vector3{0, 3, 0});
                // point->SetColor(Color{0, 255, 0, 255});
                // point->SetIntensity(10);
                // point->SetRange(10);

                // auto spot = actor->AddComponent<SpotLightComponent>();
                // spot->SetPosition(Vector3{0, -3, -2});
                // spot->SetColor(Color{0, 0, 255, 255});
                // spot->SetIntensity(3);
                // spot->SetRange(100);
                // spot->SetDirection(Vector3{0.0f, 1.0f, 1.0f});

                // auto light = actor->AddComponent<DirectionalLightComponent>();
                // light->SetDirection(Vector3{0.0f, -1.0f, -1.0f});
                // light->SetColor(Color{0, 125});
                // light->SetIntensity(2.0f);
                auto m =
                    actor->AddComponent<MeshComponent>(MeshSystem::GetMesh(MeshType::Cube).get());
                m->SetTransform(Transform{});
                m->SetMaterial(MaterialSystem::GetMaterial("mat"));
                m->SetShader(ShaderSystem::GetEngineShader("Object.glsl"));

                scene.AddActor(std::move(actor));

                SceneSerializer ser(&scene);
                ser.Serialize("Scene.vwscn");
            }

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
