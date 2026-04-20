#pragma once

#include "Base.h"
#include "Camera/Camera.h"
#include "Color.h"
#include "GPUScreen.h"
#include "Math/Matrix.h"
#include "Mesh/Mesh.h"
#include "RenderDebug.h"
#include <vector>

namespace VW
{

    struct Material
    {
        Color Color;
        u64 AlbedoHandle;
    };

    struct MaterialGPU
    {
        Color Color;
        u64 AlbedoHandle = 0;
    };

    struct RenderItem
    {
        Mesh *Mesh;
        Matrix4 Transform;
        Material Material;
    };

    struct InstanceData
    {
        Matrix4 Transform;
        MaterialGPU Material;
    };

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

            std::vector<RenderItem> RenderQueue;
            class BatchRenderer *Batch;

            RendererStats Stats;
            RendererDebugSettings Debug;
        };

    public:
        Renderer() = delete;
        ~Renderer() = delete;

        static void Init();
        static void Shutdown();

        static void Viewport(f32 w, f32 h);

        static void UseCamera(Camera *cam);

        static void Submit(RenderItem &item);

        static const RendererStats &GetStats();
        static RendererDebugSettings &GetDebugSettings();

        static void BeginFrame();
        static void Render();
        static void EndFrame();

    private:
        friend class BatchRenderer;
        static State &_GetState();
    };

} // namespace VW
