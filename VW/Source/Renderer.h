#pragma once

#include "Base.h"
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
        };

    public:
        Renderer() = delete;
        ~Renderer() = delete;

        static void Init();
        static void Shutdown();

        static void Viewport(f32 w, f32 h);

        static void BeginFrame();
        static void Render();
        static void EndFrame();
    };
} // namespace VW
