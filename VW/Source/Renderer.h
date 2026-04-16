#pragma once

#include "Base.h"
#include "Camera/Camera.h"
#include "Color.h"
#include "GPUScreen.h"
#include "Math/Matrix.h"
#include "Mesh/Mesh.h"
#include <vector>

namespace VW
{

    struct Material
    {
        Color Color;
    };

    struct MaterialGPU
    {
        Color Color;
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
        };

    public:
        Renderer() = delete;
        ~Renderer() = delete;

        static void Init();
        static void Shutdown();

        static void Viewport(f32 w, f32 h);

        static void UseCamera(Camera *cam);

        static void Submit(const RenderItem &item);

        static void BeginFrame();
        static void Render();
        static void EndFrame();
    };
} // namespace VW
