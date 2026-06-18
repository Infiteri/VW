#pragma once

#include "Window.h"
namespace VW
{
    namespace GUI
    {
        void Init(Window& window);
        void BeginFrame();
        void EndFrame();
        void Shutdown();
    } // namespace GUI
} // namespace VW
