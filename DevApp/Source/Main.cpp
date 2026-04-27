#include "Camera/PerspectiveCamera.h"
#include "Core/Entry.h"
#include "Core/Logger.h"
#include "Core/Platform.h"
#include "Light/DirectionalLight.h"
#include "Light/LightSystem.h"
#include "Light/SpotLight.h"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Mesh/Mesh.h"
#include "Mesh/MeshSystem.h"
#include "Renderer.h"
#include "Texture/TextureSystem.h"

#include <algorithm>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_opengl3_loader.h>
#include <glfw/glfw3.h>
#include <imgui.h>
#include <windows.h>

namespace VW
{
    static PerspectiveCamera cam;
    static bool s_MouseLocked = false;
    static double s_LastMouseX = 0, s_LastMouseY = 0;
    static float s_RotationSpeed = 0.05f;
    static float s_MoveSpeed = 0.5f;
    static float s_Sensitivity = 0.001f;
    static float s_Pitch = 0.0f;
    static float s_Yaw = 0.0f;
    static bool firstFrame = true;
    static std::vector<RenderItem> renderItems;

    static i32 s_GridSize = 1;
    static float s_Spacing = 15.5f;
    static bool s_RebuildGrid = true;

    // lights
    static std::shared_ptr<DirectionalLight> s_DirLight;
    static std::shared_ptr<SpotLight> s_SpotLight;

    // imgui state mirrors
    static float s_DirColor[4] = {0.0f, 1.0f, 1.0f, 1.0f};
    static float s_DirDirection[3] = {-0.5f, -1.0f, -0.5f};
    static float s_DirIntensity = 1.0f;

    static float s_SpotColor[4] = {1.0f, 1.0f, 0.0f, 1.0f};
    static float s_SpotPos[3] = {0.0f, 5.0f, 0.0f};
    static float s_SpotDir[3] = {0.0f, -1.0f, 0.0f};
    static float s_SpotIntensity = 2.0f;
    static float s_SpotRange = 20.0f;
    static float s_SpotInner = 20.0f;
    static float s_SpotOuter = 35.0f;

    static void CameraMovement(GLFWwindow *window)
    {
        bool rotate = false;

        if (s_MouseLocked)
        {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            float dx = (float)(mouseX - s_LastMouseX);
            float dy = (float)(mouseY - s_LastMouseY);
            s_LastMouseX = mouseX;
            s_LastMouseY = mouseY;
            s_Yaw -= dx * s_Sensitivity;
            s_Pitch -= dy * s_Sensitivity;
            s_Pitch = std::clamp(s_Pitch, -1.55f, 1.55f);
            rotate = true;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            s_Pitch += s_RotationSpeed;
            s_Pitch = std::clamp(s_Pitch, -1.55f, 1.55f);
            rotate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            s_Pitch -= s_RotationSpeed;
            s_Pitch = std::clamp(s_Pitch, -1.55f, 1.55f);
            rotate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            s_Yaw += s_RotationSpeed;
            rotate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            s_Yaw -= s_RotationSpeed;
            rotate = true;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            cam.SetOrientation(cam.GetOrientation() *
                               Quaternion(Vector3(0, 1, 0), -s_RotationSpeed));
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            cam.SetOrientation(cam.GetOrientation() *
                               Quaternion(Vector3(0, 1, 0), s_RotationSpeed));

        if (rotate)
            cam.SetOrientation(Quaternion(Vector3(0, 1, 0), s_Yaw) *
                               Quaternion(Vector3(1, 0, 0), s_Pitch));

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cam.SetPosition(cam.GetPosition() + cam.Forward() * s_MoveSpeed);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cam.SetPosition(cam.GetPosition() - cam.Forward() * s_MoveSpeed);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cam.SetPosition(cam.GetPosition() - cam.Right() * s_MoveSpeed);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cam.SetPosition(cam.GetPosition() + cam.Right() * s_MoveSpeed);
    }

    static void OnMouseButton(GLFWwindow *window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            if (action == GLFW_PRESS)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                s_MouseLocked = true;
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                s_LastMouseX = x;
                s_LastMouseY = y;
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                s_MouseLocked = false;
            }
        }
    }

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
                    item.Material.AlbedoID =
                        x % 2 != 0 ? 0 : TextureSystem::GetTextureID("1-akane.jpg");
                    item.Material.NormalID =
                        x % 2 != 0 ? 0 : TextureSystem::GetTextureID("normal.png");
                    item.Mesh = MeshSystem::GetMesh(MeshType::Cube).get();
                    item.Transform = Matrix4::Translate(
                        {(float)x * s_Spacing, (float)y * s_Spacing, (float)z * s_Spacing});
                    renderItems.push_back(item);
                }
        s_RebuildGrid = false;
    }

    static void InitLights()
    {
        s_DirLight = std::make_shared<DirectionalLight>();
        s_DirLight->SetColor({0, 255, 255, 255});
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
    }

    class DevAppPlatform : public Platform
    {
    public:
        DevAppPlatform() = default;
        ~DevAppPlatform() = default;

        void Init() override
        {
            Logger::GetSettings().FancyFormat = true;
            Logger::GetSettings().Format = "[PREFIX]: MSG";
            VW_LOG_ADD_CATEGORY("vwdp", "Dev");
            VW_INFO("vwdp", "Initializing...");

            glfwInit();
            m_Handle = glfwCreateWindow(100, 100, "DevApp", nullptr, nullptr);
            glfwMakeContextCurrent(m_Handle);
            glfwMaximizeWindow(m_Handle);
            glfwSetWindowSizeCallback(m_Handle, OnResize);
            glfwSetMouseButtonCallback(m_Handle, OnMouseButton);
            cam.SetPosition({0, 0, 5});
            Renderer::UseCamera(&cam);
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
            CameraMovement(m_Handle);

            if (firstFrame)
            {
                firstFrame = false;
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGui::StyleColorsDark();
                ImGui_ImplGlfw_InitForOpenGL(m_Handle, true);
                ImGui_ImplOpenGL3_Init("#version 430");

                MeshSystem::LoadModel("model", "a.obj");
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
