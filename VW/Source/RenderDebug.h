#pragma once

#include "Base.h"

namespace VW
{
    struct RendererStats
    {
        int DrawCalls = 0;
        int ItemsSubmited = 0;
    };

    struct RendererDebugSettings
    {
        bool RenderWireframe = false;
    };
} // namespace VW
