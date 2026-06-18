#include "Window.h"
#include "Renderer.h"

namespace VW
{
    Window::Window()
    {
        glfwInit();
        m_Handle = glfwCreateWindow(100, 100, "Editor", nullptr, nullptr);
        glfwMakeContextCurrent(m_Handle);
        glfwMaximizeWindow(m_Handle);
        glfwSetWindowSizeCallback(m_Handle, _OnResize);
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_Handle);
        glfwTerminate();
    }

    bool Window::ShouldClose()
    {
        return glfwWindowShouldClose(m_Handle);
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(m_Handle);
        glfwPollEvents();
    }

    void Window::_OnResize(GLFWwindow *window, int w, int h)
    {
        Renderer::Viewport(w, h);
    }
} // namespace VW
