#pragma once

#include "Base.h"
#include "Buffer/Buffer.h"
#include "Light.h"
#include "Math/Vector.h"
#include <memory>
#include <vector>

namespace VW
{
    class VW_API LightSystem
    {
    private:
#pragma pack(push, 4)
        struct GPULight
        {
            Vector4 Position;
            Vector4 Direction;
            Vector4 Color;
            Vector4 Data;
            Vector4 Attenuation;
        };
#pragma pack(pop)
        static_assert(sizeof(GPULight) % 16 == 0, "GPULight MUST be 16 multiple");

    public:
        struct State
        {
            std::vector<std::shared_ptr<Light>> Lights;
            std::vector<GPULight> GPULights;

            std::unique_ptr<Buffer> LightBuffer = nullptr;
            u32 MaxLights = 1024;
            bool GPUDirty = true;
        };

    public:
        LightSystem() = delete;
        ~LightSystem() = delete;

        static void Init();
        static void Shutdown();

        static u32 GetLightCount();

        static void AddLight(std::shared_ptr<Light> light);

        static void UpdateGPUData();
        static void Bind(u32 spot = 0);

        static void LightUpdated();
    };
} // namespace VW
