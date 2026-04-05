#pragma once

#include "Base.h"

namespace VW
{
    // every platform inherits this
    class VW_API Platform
    {
    public:
        Platform() {};
        virtual ~Platform() {};

        virtual void Init() {};
        virtual void Render() {};
        virtual void Shutdown() {};
        virtual bool ShouldShutdown()
        {
            return true;
        };
    };

    // every platform implementation (i.e. windows or developer) will implement this function
    // the entry point is handled by the engine
    Platform *InitPlatform();
} // namespace VW
