#pragma once

#include "Base.h"
#include "Core/Logger.h"
#include "Core/Platform.h"

extern VW::Platform *VW::InitPlatform();

int main(int argc, const char **argv)
{
    VW::Logger::Init();

    VW::Platform *platform = VW::InitPlatform();
    if (!platform)
        return -1;

    platform->Init();

    while (!platform->ShouldShutdown())
    {
        platform->Render();
    }

    platform->Shutdown();
    VW::Logger::Shutdown();

    delete platform;
    return 0;
}
