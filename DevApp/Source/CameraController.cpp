#include "CameraController.h"
#include <algorithm>

namespace VW
{
    CameraController::CameraController(Camera *target) : m_Target(target)
    {
    }

    void CameraController::SetCamera(Camera *target)
    {
        m_Target = target;
        m_Pitch = 0.0f;
        m_Yaw = 0.0f;
        m_MouseLocked = false;
        m_PreviousRightButton = false;
    }

    void CameraController::Update(GLFWwindow *window)
    {
        if (!m_Target)
            return;

        bool rotate = false;
        float speed = m_MoveSpeed;

        // right-click to lock/unlock mouse
        int rightButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
        if (rightButton == GLFW_PRESS && !m_PreviousRightButton)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            m_MouseLocked = true;
            glfwGetCursorPos(window, &m_LastMouseX, &m_LastMouseY);
        }
        else if (rightButton == GLFW_RELEASE && m_PreviousRightButton)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_MouseLocked = false;
        }
        m_PreviousRightButton = (rightButton == GLFW_PRESS);

        if (m_MouseLocked)
        {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            float dx = (float)(mouseX - m_LastMouseX);
            float dy = (float)(mouseY - m_LastMouseY);
            m_LastMouseX = mouseX;
            m_LastMouseY = mouseY;
            m_Yaw -= dx * m_Sensitivity;
            m_Pitch -= dy * m_Sensitivity;
            m_Pitch = std::clamp(m_Pitch, -1.55f, 1.55f);
            rotate = true;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            speed = m_SlowSpeed;

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            speed = m_FastSpeed;

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            m_Pitch += m_RotationSpeed;
            m_Pitch = std::clamp(m_Pitch, -1.55f, 1.55f);
            rotate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            m_Pitch -= m_RotationSpeed;
            m_Pitch = std::clamp(m_Pitch, -1.55f, 1.55f);
            rotate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            m_Yaw += m_RotationSpeed;
            rotate = true;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            m_Yaw -= m_RotationSpeed;
            rotate = true;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            m_Target->SetOrientation(
                m_Target->GetOrientation() * Quaternion(Vector3(0, 1, 0), -m_RotationSpeed));
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            m_Target->SetOrientation(
                m_Target->GetOrientation() * Quaternion(Vector3(0, 1, 0), m_RotationSpeed));

        if (rotate)
            m_Target->SetOrientation(Quaternion(Vector3(0, 1, 0), m_Yaw) *
                                     Quaternion(Vector3(1, 0, 0), m_Pitch));

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            m_Target->SetPosition(m_Target->GetPosition() + m_Target->Forward() * speed);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            m_Target->SetPosition(m_Target->GetPosition() - m_Target->Forward() * speed);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            m_Target->SetPosition(m_Target->GetPosition() - m_Target->Right() * speed);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            m_Target->SetPosition(m_Target->GetPosition() + m_Target->Right() * speed);
    }
} // namespace VW
