#pragma once

#include "Base.h"

namespace VW
{
    struct RendererStats
    {
        int DrawCalls = 0;
        int ItemsSubmited = 0;
    };

    enum class RenderDebugMode
    {
        Full,
        UV
    };

    struct RendererDebugSettings
    {
        bool RenderWireframe = false;
        RenderDebugMode RenderMode = RenderDebugMode::Full;
    };
} // namespace VW
