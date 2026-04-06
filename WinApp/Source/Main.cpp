#include "Core/Entry.h"
#include "Core/Logger.h"
#include "Core/Platform.h"
#include <cstdio>
#include <windows.h>

namespace VW
{
    static HWND g_hWnd = nullptr;
    static HDC g_hDC = nullptr;
    static HGLRC g_hGLRC = nullptr;
    static bool g_Run = true;
    static int g_W = 0;
    static int g_H = 0;
    static HINSTANCE g_hInst = nullptr;

    class WindowsPlatform : public VW::Platform
    {
    public:
        ~WindowsPlatform() override
        {
            if (g_hGLRC)
            {
                wglMakeCurrent(nullptr, nullptr);
                wglDeleteContext(g_hGLRC);
                g_hGLRC = nullptr;
            }
            if (g_hDC && g_hWnd)
            {
                ReleaseDC(g_hWnd, g_hDC);
                g_hDC = nullptr;
            }
            if (g_hWnd)
            {
                DestroyWindow(g_hWnd);
                g_hWnd = nullptr;
            }
            UnregisterClassW(L"WGL", g_hInst);
            FreeConsole();
        }

        void Init() override
        {
            Logger::GetSettings().FancyFormat = true;
            Logger::GetSettings().Format = "[PREFIX]: MSG";

            VW_LOG_ADD_CATEGORY("vwwp", "Windows");
            VW_INFO("vwwp", "Initializing...");

            AllocConsole();
            freopen("CONOUT$", "w", stdout);

            g_W = GetSystemMetrics(SM_CXSCREEN);
            g_H = GetSystemMetrics(SM_CYSCREEN);

            g_hInst = GetModuleHandleA(nullptr);

            HWND parent = FindWallpaperLayer();
            if (!parent)
            {
                return;
            }

            ShowWindow(parent, SW_SHOW);

            WNDCLASSEXW wc{};
            wc.cbSize = sizeof wc;
            wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
            wc.lpfnWndProc = WndProc;
            wc.hInstance = g_hInst;
            wc.lpszClassName = L"WGL";
            wc.hbrBackground = nullptr;

            if (!RegisterClassExW(&wc))
            {
                return;
            }

            g_hWnd = CreateWindowExW(WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW, L"WGL", L"WALLPAPER",
                                     WS_CHILD | WS_VISIBLE, 0, 0, g_W, g_H, parent, nullptr,
                                     g_hInst, nullptr);

            if (!g_hWnd)
            {
                return;
            }

            ShowWindow(g_hWnd, SW_SHOW);
            UpdateWindow(g_hWnd);
            SetWindowPos(g_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

            g_hDC = GetDC(g_hWnd);
            if (!g_hDC)
            {
                return;
            }

            PIXELFORMATDESCRIPTOR pfd{};
            pfd.nSize = sizeof pfd;
            pfd.nVersion = 1;
            pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            pfd.iPixelType = PFD_TYPE_RGBA;
            pfd.cColorBits = 32;
            pfd.cDepthBits = 24;
            pfd.iLayerType = PFD_MAIN_PLANE;

            int pf = ChoosePixelFormat(g_hDC, &pfd);
            if (!pf)
            {
                return;
            }
            SetPixelFormat(g_hDC, pf, &pfd);

            g_hGLRC = wglCreateContext(g_hDC);
            if (!g_hGLRC)
            {
                return;
            }
            wglMakeCurrent(g_hDC, g_hGLRC);
        }

        void Render() override
        {
            MSG msg;
            while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    g_Run = false;
                    return;
                }
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }

            SwapBuffers(g_hDC);
        }

        void Shutdown() override
        {
        }

        bool ShouldShutdown() override
        {
            return !g_Run;
        }

    private:
        static HWND FindWallpaperLayer()
        {
            HWND progman = FindWindowA("ProgMan", nullptr);
            if (!progman)
                return nullptr;

            SendMessageTimeoutA(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);

            struct State
            {
                HWND result;
                int phase;
            };
            State state{};

            EnumWindows(
                [](HWND top, LPARAM lp) -> BOOL
                {
                    char cls[64];
                    if (!GetClassNameA(top, cls, sizeof(cls)))
                        return TRUE;
                    if (strcmp(cls, "WorkerW") != 0)
                        return TRUE;

                    State *s = (State *)lp;
                    HWND dv = FindWindowExA(top, nullptr, "SHELLDLL_DefView", nullptr);

                    if (dv)
                    {
                        s->phase = 1;
                        return TRUE;
                    }

                    if (s->phase == 1)
                    {
                        s->result = top;
                        s->phase = 2;
                        return FALSE;
                    }

                    return TRUE;
                },
                (LPARAM)&state);

            return state.result ? state.result : progman;
        }

        static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (msg == WM_SIZE && g_hDC && g_hGLRC)
            {
                int w = LOWORD(lParam);
                int h = HIWORD(lParam);
                if (w && h)
                {
                }
                return 0;
            }
            if (msg == WM_CLOSE)
            {
                g_Run = false;
                return 0;
            }
            if (msg == WM_ERASEBKGND)
                return 1;
            return DefWindowProcW(hWnd, msg, wParam, lParam);
        }
    };

} // namespace VW

VW::Platform *VW::InitPlatform()
{
    VW::Logger::Log(VW::LogLevel::Info, "", "%s %i", "hahaha", 12);
    return new VW::WindowsPlatform();
}
