#include "Camera/Camera.h"
#include "Camera/CameraSystem.h"
#include "Camera/PerspectiveCamera.h"
#include "CameraController.h"
#include "Core/Entry.h"
#include "Core/Logger.h"
#include "Core/Platform.h"
#include "Light/DirectionalLight.h"
#include "Light/LightSystem.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Material/Material.h"
#include "Material/MaterialSystem.h"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Mesh/Mesh.h"
#include "Mesh/MeshSystem.h"
#include "Renderer.h"
#include "Shader/ShaderSystem.h"
#include "Texture/TextureSystem.h"

#include <algorithm>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glfw/glfw3.h>
#include <imgui.h>
#include <memory>
#include <windows.h>

// TODO: todos: add a material system, fix models being represent as 1 mesh (includes writing a
// 'mtl' file parser, a.k.a custom .obj and .mlt parser), add missing textures to materials
// (metallic and roughness)

namespace VW
{
    static PerspectiveCamera *cam;
    static CameraController controller;
    static bool firstFrame = true;
    static std::vector<RenderItem> renderItems;

    static i32 s_GridSize = 2;
    static float s_Spacing = 15.5f;
    static bool s_RebuildGrid = true;

    // lights
    static std::shared_ptr<DirectionalLight> s_DirLight;
    static std::shared_ptr<SpotLight> s_SpotLight;
    static std::shared_ptr<PointLight> s_PointLight;

    // imgui state mirrors
    static float s_DirColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    static float s_DirDirection[3] = {-1.5f, -1.0f, -1.0f};
    static float s_DirIntensity = 4.0f;

    static float s_SpotColor[4] = {1.0f, 1.0f, 0.0f, 1.0f};
    static float s_SpotPos[3] = {0.0f, 5.0f, 0.0f};
    static float s_SpotDir[3] = {0.0f, -1.0f, 0.0f};
    static float s_SpotIntensity = 2.0f;
    static float s_SpotRange = 20.0f;
    static float s_SpotInner = 20.0f;
    static float s_SpotOuter = 35.0f;

    static float s_PointColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    static float s_PointPos[3] = {.2f, 0.0f, 0.0f};
    static float s_PointIntensity = 5.0f;
    static float s_PointRange = 30.0f;

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
                    item.Mesh = MeshSystem::GetMesh("a.obj").get();
                    item.Transform = Matrix4::Translate({(float)x * s_Spacing, (float)y * s_Spacing,
                                                         (float)z * s_Spacing}) *
                                     Matrix4::Scale({5, 5, 5});
                    item.Shader = x % 2 != 0 ? ShaderSystem::GetShader("Object2.glsl") : nullptr;
                    renderItems.push_back(item);
                }
        s_RebuildGrid = false;
    }

    static void InitLights()
    {
        s_DirLight = std::make_shared<DirectionalLight>();
        s_DirLight->SetColor({255, 255, 255, 255});
        s_DirLight->SetIntensity(s_DirIntensity);
        s_DirLight->SetDirection({s_DirDirection[0], s_DirDirection[1], s_DirDirection[2]});
        LightSystem::AddLight(s_DirLight);

        s_SpotLight = std::make_shared<SpotLight>();
        s_SpotLight->SetColor({255, 255, 0, 255});
        s_SpotLight->SetIntensity(s_SpotIntensity);
        s_SpotLight->SetPosition({s_SpotPos[0], s_SpotPos[1], s_SpotPos[2]});
        s_SpotLight->SetDirection({s_SpotDir[0], s_SpotDir[1], s_SpotDir[2]});
        s_SpotLight->SetRange(s_SpotRange);
        s_SpotLight->SetInnerConeAngle(s_SpotInner);
        s_SpotLight->SetOuterConeAngle(s_SpotOuter);
        LightSystem::AddLight(s_SpotLight);

        s_PointLight = std::make_shared<PointLight>();
        s_PointLight->SetColor({255, 255, 255, 255});
        s_PointLight->SetIntensity(s_PointIntensity);
        s_PointLight->SetPosition({s_PointPos[0], s_PointPos[1], s_PointPos[2]});
        s_PointLight->SetRange(s_PointRange);
        LightSystem::AddLight(s_PointLight);
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

                MeshSystem::LoadModel("a.obj", "AK/source/AK47.glb");
                InitLights();
            }

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

            // Directional light
            ImGui::Separator();
            ImGui::Text("Directional Light");
            if (ImGui::ColorEdit4("Dir Color", s_DirColor))
                s_DirLight->SetColor({s_DirColor[0] * 255, s_DirColor[1] * 255, s_DirColor[2] * 255,
                                      s_DirColor[3] * 255});
            if (ImGui::DragFloat3("Dir Direction", s_DirDirection, 0.01f, -1.0f, 1.0f))
                s_DirLight->SetDirection({s_DirDirection[0], s_DirDirection[1], s_DirDirection[2]});
            if (ImGui::DragFloat("Dir Intensity", &s_DirIntensity, 0.01f, 0.0f, 10.0f))
                s_DirLight->SetIntensity(s_DirIntensity);

            // Spot light
            ImGui::Separator();
            ImGui::Text("Spot Light");
            if (ImGui::ColorEdit4("Spot Color", s_SpotColor))
                s_SpotLight->SetColor({s_SpotColor[0] * 255, s_SpotColor[1] * 255,
                                       s_SpotColor[2] * 255, s_SpotColor[3] * 255});
            if (ImGui::DragFloat3("Spot Position", s_SpotPos, 0.1f))
                s_SpotLight->SetPosition({s_SpotPos[0], s_SpotPos[1], s_SpotPos[2]});
            if (ImGui::DragFloat3("Spot Direction", s_SpotDir, 0.01f, -1.0f, 1.0f))
                s_SpotLight->SetDirection({s_SpotDir[0], s_SpotDir[1], s_SpotDir[2]});
            if (ImGui::DragFloat("Spot Intensity", &s_SpotIntensity, 0.01f, 0.0f, 10.0f))
                s_SpotLight->SetIntensity(s_SpotIntensity);
            if (ImGui::DragFloat("Spot Range", &s_SpotRange, 0.1f, 0.0f, 200.0f))
                s_SpotLight->SetRange(s_SpotRange);
            if (ImGui::DragFloat("Inner Angle", &s_SpotInner, 0.5f, 0.0f, 90.0f))
                s_SpotLight->SetInnerConeAngle(s_SpotInner);
            if (ImGui::DragFloat("Outer Angle", &s_SpotOuter, 0.5f, 0.0f, 90.0f))
                s_SpotLight->SetOuterConeAngle(s_SpotOuter);

            ImGui::Separator();
            ImGui::Text("Point Light");
            if (ImGui::ColorEdit4("Point Color", s_PointColor))
                s_PointLight->SetColor({s_PointColor[0] * 255, s_PointColor[1] * 255,
                                        s_PointColor[2] * 255, s_PointColor[3] * 255});
            if (ImGui::DragFloat3("Point Position", s_PointPos, 0.1f))
                s_PointLight->SetPosition({s_PointPos[0], s_PointPos[1], s_PointPos[2]});
            if (ImGui::DragFloat("Point Intensity", &s_PointIntensity, 0.01f, 0.0f, 10.0f))
                s_PointLight->SetIntensity(s_PointIntensity);
            if (ImGui::DragFloat("Point Range", &s_PointRange, 0.1f, 0.0f, 200.0f))
                s_PointLight->SetRange(s_PointRange);

            LightSystem::LightUpdated();

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
