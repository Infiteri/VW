#include <windows.h>
#include <gl/gl.h>
#include <cstdio>

static HWND  g_hWnd  = nullptr;
static HDC   g_hDC   = nullptr;
static HGLRC g_hGLRC = nullptr;
static bool  g_Run   = true;
static int   g_W, g_H;

#define LOG(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)

// ===========================================================================
// WALLPAPER LAYER DETECTION — THE CORE PROBLEM
// ===========================================================================
//
// Windows 10 desktop icon/window hierarchy:
//
//   ProgMan  (Program Manager)
//   │
//   ├─ WorkerW (WITH SHELLDLL_DefView)    ← this holds desktop icons
//   │   └─ SHELLDLL_DefView              ← icons are children of this
//   │
//   └─ WorkerW (WITHOUT SHELLDLL_DefView) ← hidden wallpaper layer,
//       sits BEHIND the icons WorkerW      THIS is where our window goes
//
// EnumWindows() walks Z-order TOP → BOTTOM.
// So the icons WorkerW appears FIRST, then the wallpaper WorkerW.
// We must find the wallpaper WorkerW that is BELOW (after) the icons one.
//
// ===========================================================================
static HWND FindWallpaperLayer()
{
    HWND progman = FindWindowA("ProgMan", nullptr);
    if (!progman) return nullptr;

    // Force Explorer to create WorkerW windows
    SendMessageTimeoutA(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);

    // State machine: 0=searching, 1=found icons, 2=capture next WorkerW
    struct State { HWND result; int phase; };
    State state{};

    EnumWindows(
        [](HWND top, LPARAM lp) -> BOOL {
            char cls[64];
            if (!GetClassNameA(top, cls, sizeof(cls))) return TRUE;
            if (strcmp(cls, "WorkerW") != 0) return TRUE;

            State* s = (State*)lp;
            HWND dv = FindWindowExA(top, nullptr, "SHELLDLL_DefView", nullptr);

            if (dv)
            {
                LOG("  Found icons WorkerW: %p", top);
                s->phase = 1;  // next WorkerW we see is the wallpaper layer
                return TRUE;   // keep enumerating
            }

            if (s->phase == 1)
            {
                // This is the wallpaper WorkerW (below icons in Z-order)
                LOG("  Found wallpaper WorkerW: %p", top);
                s->result = top;
                s->phase = 2;
                return FALSE;  // stop enumeration
            }

            return TRUE;
        },
        (LPARAM)&state);

    if (state.result)
        return state.result;

    // Fallback
    LOG("  No wallpaper WorkerW found, using ProgMan");
    return progman;
}

// ===========================================================================
static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_SIZE && g_hDC && g_hGLRC)
    {
        int w = LOWORD(lParam);
        int h = HIWORD(lParam);
        if (w && h)
        {
            wglMakeCurrent(g_hDC, g_hGLRC);
            glViewport(0, 0, w, h);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            float a = (float)w / (float)h;
            glOrtho(-a, a, -1, 1, -1, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            LOG("Resized to %dx%d", w, h);
        }
        return 0;
    }
    if (msg == WM_CLOSE) { g_Run = false; return 0; }
    if (msg == WM_ERASEBKGND) return 1;
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

// ===========================================================================
static void DrawFrame()
{
    static int frame = 0;
    frame++;

    float bg = (frame % 120 < 60) ? 0.0f : 0.15f;
    glClearColor(bg, 0, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
        glColor3f(1, 0, 0);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.5f, -0.5f);
        glVertex2f( 0.5f,  0.5f);
        glVertex2f(-0.5f,  0.5f);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3f(0, 1, 0);
        glVertex2f(-0.3f,  0.6f);
        glVertex2f( 0.3f,  0.6f);
        glVertex2f( 0.0f,  0.0f);
    glEnd();

    SwapBuffers(g_hDC);

    if (frame % 60 == 0) LOG("frame=%d", frame);
}

// ===========================================================================
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    g_W = GetSystemMetrics(SM_CXSCREEN);
    g_H = GetSystemMetrics(SM_CYSCREEN);
    LOG("Screen %dx%d", g_W, g_H);

    // -- Find wallpaper layer --
    HWND parent = FindWallpaperLayer();
    if (!parent) { LOG("No wallpaper parent"); return 1; }

    // Make the wallpaper WorkerW visible
    LOG("Parent visible=%d", IsWindowVisible(parent));
    ShowWindow(parent, SW_SHOW);

    // -- Register --
    WNDCLASSEXW wc{};
    wc.cbSize        = sizeof wc;
    wc.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = L"WGL";
    wc.hbrBackground = nullptr;

    if (!RegisterClassExW(&wc)) { LOG("RegisterClass FAILED"); return 1; }

    // -- Create CHILD of wallpaper WorkerW --
    g_hWnd = CreateWindowExW(
        WS_EX_NOACTIVATE | WS_EX_TOOLWINDOW,
        L"WGL", L"WALLPAPER",
        WS_CHILD | WS_VISIBLE,
        0, 0, g_W, g_H,
        parent, nullptr, hInst, nullptr);

    if (!g_hWnd) { LOG("CreateWindow err=%lu", GetLastError()); return 1; }

    LOG("HWND=%p parent=%p", g_hWnd, parent);
    LOG("Our window visible=%d", IsWindowVisible(g_hWnd));

    ShowWindow(g_hWnd, SW_SHOW);
    UpdateWindow(g_hWnd);

    // Place at BOTTOM of parent's Z-order (behind everything else in parent)
    SetWindowPos(g_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

    // -- OpenGL --
    g_hDC = GetDC(g_hWnd);
    if (!g_hDC) { LOG("No DC"); return 1; }

    PIXELFORMATDESCRIPTOR pfd{};
    pfd.nSize      = sizeof pfd;
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat(g_hDC, &pfd);
    if (!pf) { LOG("ChoosePixelFormat FAILED"); return 1; }
    SetPixelFormat(g_hDC, pf, &pfd);

    g_hGLRC = wglCreateContext(g_hDC);
    if (!g_hGLRC) { LOG("wglCreateContext FAILED"); return 1; }
    wglMakeCurrent(g_hDC, g_hGLRC);
    LOG("OpenGL ready");

    glViewport(0, 0, g_W, g_H);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    float asp = (float)g_W / (float)g_H;
    glOrtho(-asp, asp, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    // -- Loop --
    LOG("=== RENDER LOOP ===");

    MSG msg;
    while (g_Run)
    {
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) { g_Run = false; break; }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        DrawFrame();
        Sleep(16);
    }

    LOG("=== DONE ===");
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(g_hGLRC);
    ReleaseDC(g_hWnd, g_hDC);
    DestroyWindow(g_hWnd);
    UnregisterClassW(L"WGL", hInst);
    FreeConsole();
    return 0;
}
