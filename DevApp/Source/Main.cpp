#include "Camera/PerspectiveCamera.h"
#include "Core/Entry.h"
#include "Core/Logger.h"
#include "Core/Platform.h"
#include "Math/Math.h"
#include "Math/Quaternion.h"
#include "Renderer.h"

#include <algorithm>
#include <glfw/glfw3.h>
#include <windows.h>

namespace VW
{
    static PerspectiveCamera cam;
    static bool s_MouseLocked = false;
    static double s_LastMouseX = 0, s_LastMouseY = 0;
    static float s_RotationSpeed = 0.05f;
    static float s_MoveSpeed = 0.1f;
    static float s_Sensitivity = 0.001f;
    static float s_Pitch = 0.0f;
    static float s_Yaw = 0.0f;

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
