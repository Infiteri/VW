#pragma once

#include "Camera/Camera.h"
#include <glfw/glfw3.h>

namespace VW
{
    class CameraController
    {
    public:
        CameraController(Camera *target = nullptr);
        ~CameraController() = default;

        void SetCamera(Camera *target);
        void Update(GLFWwindow *window);

    private:
        Camera *m_Target;

        bool m_MouseLocked = false;
        bool m_PreviousRightButton = false;
        double m_LastMouseX = 0, m_LastMouseY = 0;

        float m_RotationSpeed = 0.05f;
        float m_FastSpeed = 0.6f;
        float m_MoveSpeed = 0.4f;
        float m_SlowSpeed = 0.1f;
        float m_Sensitivity = 0.001f;
        float m_Pitch = 0.0f;
        float m_Yaw = 0.0f;
    };
} // namespace VW
