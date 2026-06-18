#pragma once

#include "Camera/PerspectiveCamera.h"
#include <glfw/glfw3.h>

namespace VW
{
    class Window
    {
    public:
        Window();
        ~Window();

        bool ShouldClose();
        void SwapBuffers();

        inline GLFWwindow *GetHandle()
        {
            return m_Handle;
        }

    private:
        GLFWwindow *m_Handle;

        static void _OnResize(GLFWwindow *window, int w, int h);
    };
} // namespace VW
