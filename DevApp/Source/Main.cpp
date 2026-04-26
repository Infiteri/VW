#include "Camera/PerspectiveCamera.h"
#include "Core/Entry.h"
#include "Core/Logger.h"
#include "Core/Platform.h"
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
        {
            Quaternion quat(Vector3(0, 1, 0), -s_RotationSpeed);
            cam.SetOrientation(cam.GetOrientation() * quat);
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            Quaternion quat(Vector3(0, 1, 0), s_RotationSpeed);
            cam.SetOrientation(cam.GetOrientation() * quat);
        }

        if (rotate)
        {
            Quaternion pitchQuat(Vector3(1, 0, 0), s_Pitch);
            Quaternion yawQuat(Vector3(0, 1, 0), s_Yaw);
            cam.SetOrientation(yawQuat * pitchQuat);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            auto forward = cam.Forward() * s_MoveSpeed;
            cam.SetPosition(cam.GetPosition() + forward);
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            auto forward = cam.Forward() * s_MoveSpeed;
            cam.SetPosition(cam.GetPosition() - forward);
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            auto forward = cam.Right() * s_MoveSpeed;
            cam.SetPosition(cam.GetPosition() - forward);
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            auto forward = cam.Right() * s_MoveSpeed;
            cam.SetPosition(cam.GetPosition() + forward);
        }
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

        i32 count = s_GridSize * s_GridSize * s_GridSize;
        i32 idx = 0;

        for (i32 z = 0; z < s_GridSize; z++)
        {
            for (i32 y = 0; y < s_GridSize; y++)
            {
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
                    idx++;
                }
            }
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
                ImGui_ImplOpenGL3_Init("#version 330");

                MeshSystem::LoadModel("model", "a.obj");
            }

            if (s_RebuildGrid)
            {
                BuildGrid();
            }

            for (auto &item : renderItems)
            {
                Renderer::Submit(item);
            }
        }

        void RenderImGui() override
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Stats");
            ImGui::Text("%i draw calls", Renderer::GetStats().DrawCalls);
            ImGui::Text("%i rendered cubes", (i32)Renderer::GetStats().ItemsSubmited);
            ImGui::Checkbox("Wireframe", &Renderer::GetDebugSettings().RenderWireframe);
            ImGui::Separator();
            ImGui::Text("Grid Settings");
            ImGui::DragInt("Grid Size", &s_GridSize, 1.0f, 1, 1000);
            ImGui::DragFloat("Spacing", &s_Spacing, 0.1f, 0.0f, 100.0f);
            if (ImGui::Button("Rebuild"))
            {
                s_RebuildGrid = true;
            }

            // dropdown
            {

                auto &debug = Renderer::GetDebugSettings();

                const char *modes[] = {"Full", "UV", "Normal", "Tangent", "Bitangent"};

                int current = static_cast<int>(debug.RenderMode);

                if (ImGui::Combo("Render Mode", &current, modes, IM_ARRAYSIZE(modes)))
                {
                    debug.RenderMode = static_cast<RenderDebugMode>(current);
                }
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
    // allows nice logger colors in the GetConsoleMode
    // removed from Logger.cpp to keep VW platform independent
    // NOTE: some code as in WinApp main
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
