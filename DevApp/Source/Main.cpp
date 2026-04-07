#include "Core/Entry.h"
#include "Core/Logger.h"
#include "Core/Platform.h"

#include <glfw/glfw3.h>
#include <windows.h>

namespace VW
{
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
