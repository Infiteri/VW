#pragma once

#include "Base.h"

namespace VW
{
    class VW_API Renderer
    {
    public:
        struct State
        {
        };

    public:
        Renderer() = delete;
        ~Renderer() = delete;

        static void Init();
        static void Shutdown();

        static void BeginFrame();
        static void Render();
        static void EndFrame();
    };
} // namespace VW
