#include "Camera/PerspectiveCamera.h"
#include "Core/Entry.h"
#include "Core/Logger.h"
#include "Core/Platform.h"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Mesh/Mesh.h"
#include "Renderer.h"

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
    static Mesh *cubeMesh;
    static std::vector<RenderItem> renderItems;

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
            glfwSwapBuffers(m_Handle);
            glfwPollEvents();

            if (cubeMesh == nullptr)
            {
#define S 0.5f
                Vertex vertices[] = {
                    // Front (z = -S)
                    {Vector3{-S, -S, -S}, Vector2{0.0f, 0.0f}},
                    {Vector3{S, -S, -S}, Vector2{1.0f, 0.0f}},
                    {Vector3{S, S, -S}, Vector2{1.0f, 1.0f}},
                    {Vector3{-S, S, -S}, Vector2{0.0f, 1.0f}},

                    // Back (z = +S)
                    {Vector3{S, -S, S}, Vector2{0.0f, 0.0f}},
                    {Vector3{-S, -S, S}, Vector2{1.0f, 0.0f}},
                    {Vector3{-S, S, S}, Vector2{1.0f, 1.0f}},
                    {Vector3{S, S, S}, Vector2{0.0f, 1.0f}},

                    // Left (x = -S)
                    {Vector3{-S, -S, S}, Vector2{0.0f, 0.0f}},
                    {Vector3{-S, -S, -S}, Vector2{1.0f, 0.0f}},
                    {Vector3{-S, S, -S}, Vector2{1.0f, 1.0f}},
                    {Vector3{-S, S, S}, Vector2{0.0f, 1.0f}},

                    // Right (x = +S)
                    {Vector3{S, -S, -S}, Vector2{0.0f, 0.0f}},
                    {Vector3{S, -S, S}, Vector2{1.0f, 0.0f}},
                    {Vector3{S, S, S}, Vector2{1.0f, 1.0f}},
                    {Vector3{S, S, -S}, Vector2{0.0f, 1.0f}},

                    // Top (y = +S)
                    {Vector3{-S, S, -S}, Vector2{0.0f, 0.0f}},
                    {Vector3{S, S, -S}, Vector2{1.0f, 0.0f}},
                    {Vector3{S, S, S}, Vector2{1.0f, 1.0f}},
                    {Vector3{-S, S, S}, Vector2{0.0f, 1.0f}},

                    // Bottom (y = -S)
                    {Vector3{-S, -S, S}, Vector2{0.0f, 0.0f}},
                    {Vector3{S, -S, S}, Vector2{1.0f, 0.0f}},
                    {Vector3{S, -S, -S}, Vector2{1.0f, 1.0f}},
                    {Vector3{-S, -S, -S}, Vector2{0.0f, 1.0f}},
                };

                u32 indices[] = {
                    0,  2,  1,  0,  3,  2,  // Front
                    4,  6,  5,  4,  7,  6,  // Back
                    8,  10, 9,  8,  11, 10, // Left
                    12, 14, 13, 12, 15, 14, // Righe
                    16, 18, 17, 16, 19, 18, // Top
                    20, 22, 21, 20, 23, 22, // Bottom
                };

                VertexLayout layout;
                layout.Stride = sizeof(Vertex);
                layout.Attributes.push_back({0, 0, 3, false});
                layout.Attributes.push_back({1, 3 * sizeof(f32), 2, false});

                cubeMesh = new Mesh(vertices, sizeof(Vertex) * 25, indices, 36, layout);

                const float SPACING = 2.0f;
                const i32 CUBE_COUNT = 2000;

                for (i32 i = 0; i < CUBE_COUNT; i++)
                {
                    i32 x = i % 100 - 50;
                    i32 y = (i / 100) % 100;
                    i32 z = i / 10000;

                    RenderItem item;
                    item.Mesh = cubeMesh;
                    item.Transform = Matrix4::Translate(
                        {(float)x * SPACING, (float)y * SPACING, (float)z * SPACING});
                    item.Material.Color = Color(255.0f, 125.0f, 0.0f, 255.0f);
                    renderItems.push_back(item);
                }

                IMGUI_CHECKVERSION();
                ImGui::CreateContext();

                ImGui::StyleColorsDark();
                ImGui_ImplGlfw_InitForOpenGL(m_Handle, true);
                ImGui_ImplOpenGL3_Init("#version 330");
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
            ImGui::Text("%i %i", Renderer::GetStats().DrawCalls,
                        Renderer::GetStats().ItemsSubmited);
            ImGui::Checkbox("Writeframe", &Renderer::GetDebugSettings().RenderWireframe);
            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
