#pragma once

#include "Base.h"
#include "Camera/Camera.h"
#include "Color.h"
#include "GPUScreen.h"
#include "Material/Material.h"
#include "Math/Matrix.h"
#include "Mesh/Mesh.h"
#include "RenderDebug.h"
#include "Sky/Sky.h"
#include <vector>

namespace VW
{
    struct MaterialGPU
    {
        Color Color;
        u64 AlbedoHandle = 0;
        u64 NormalHandle = 0;
        u64 ORMHandle = 0;
    };

    struct VW_API RenderItem
    {
        Mesh *Mesh;
        Matrix4 Transform;
        Material *Material;
        Shader *Shader;

        RenderItem();
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
            struct Frustum Frustum;

            GPUScreen Screen;

            std::vector<RenderItem> RenderQueue;
            class BatchRenderer *Batch;

            RendererStats Stats;
            RendererDebugSettings Debug;

            float Time, DeltaTime;

            class Sky *Sky;
        };

    public:
        Renderer() = delete;
        ~Renderer() = delete;

        static void Init();
        static void Shutdown();

        static void Viewport(f32 w, f32 h);

        static void Submit(const RenderItem &item);

        static const RendererStats &GetStats();
        static RendererDebugSettings &GetDebugSettings();

        static void BeginFrame();
        static void Render();
        static void EndFrame();

        static void SetSky(Sky *sky);

        // TODO: move to separate file
        static float GetDeltaTime();
        static float GetTime();

    private:
        friend class BatchRenderer;
        static State &_GetState();
    };

    namespace RendererUtils
    {
        void CoreUniformsToShader(Shader *shader);
    };

} // namespace VW
