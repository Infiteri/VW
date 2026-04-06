#include "Core/Entry.h"
#include "Core/Logger.h"
#include "Core/Platform.h"

#include <glfw/glfw3.h>

namespace VW
{
    class DevAppPlatform : public Platform
    {
    public:
        DevAppPlatform() = default;
        ~DevAppPlatform() = default;

        void Init() override
        {
            glfwInit();
            m_Handle = glfwCreateWindow(100, 100, "DevApp", nullptr, nullptr);
            glfwMakeContextCurrent(m_Handle);
            glfwMaximizeWindow(m_Handle);
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
            glfwSwapBuffers(m_Handle);
            glfwPollEvents();
        }

    private:
        GLFWwindow *m_Handle;
    };
} // namespace VW

VW::Platform *VW::InitPlatform()
{
    return new VW::DevAppPlatform();
}
