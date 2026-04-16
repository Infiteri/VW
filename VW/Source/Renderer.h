#pragma once

#include "Base.h"
#include "Camera/Camera.h"
#include "GPUScreen.h"

namespace VW
{
    class VW_API Renderer
    {
    public:
        struct Viewport
        {
            f32 Width, Height;
        };

        struct State
        {
            struct Viewport Viewport;
            GPUScreen Screen;
            Camera *Cam;
        };

    public:
        Renderer() = delete;
        ~Renderer() = delete;

        static void Init();
        static void Shutdown();

        static void Viewport(f32 w, f32 h);

        static void UseCamera(Camera *cam);

        static void BeginFrame();
        static void Render();
        static void EndFrame();
    };
} // namespace VW
