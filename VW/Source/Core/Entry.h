#pragma once

#include "Base.h"
#include "Core/Logger.h"
#include "Core/Platform.h"
#include "Renderer.h"

extern VW::Platform *VW::InitPlatform();

int main(int argc, const char **argv)
{
    VW::Logger::Init();

    VW::Platform *platform = VW::InitPlatform();
    if (!platform)
        return -1;

    platform->Init();
    VW::Renderer::Init();

    while (!platform->ShouldShutdown())
    {
        VW::Renderer::BeginFrame();
        VW::Renderer::Render();
        platform->Render(); /// todo: rename from Render to something more meaniningful
        VW::Renderer::EndFrame();

        platform->RenderImGui();
    }

    platform->Shutdown();
    VW::Renderer::Shutdown();
    VW::Logger::Shutdown();

    delete platform;
    return 0;
}
