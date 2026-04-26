#include "LightSystem.h"
#include "Core/Logger.h"
#include "Math/Math.h"
#include <cmath>
#include <glad/glad.h>
#include <memory>

namespace VW
{
    static LightSystem::State s_State;

    void LightSystem::Init()
    {
        s_State.Lights.reserve(s_State.MaxLights);
        s_State.GPULights.reserve(s_State.MaxLights);

        s_State.LightBuffer = std::make_unique<Buffer>(BufferType::SSBO, BufferUsage::Dynamic);
        s_State.LightBuffer->SetData(nullptr, sizeof(GPULight) * s_State.MaxLights);
    }

    void LightSystem::Shutdown()
    {
    }

    void LightSystem::AddLight(std::shared_ptr<Light> light)
    {
        s_State.Lights.push_back(light);
        s_State.GPUDirty = true;
    }

    u32 LightSystem::GetLightCount()
    {
        return s_State.Lights.size();
    }

    void LightSystem::UpdateGPUData()
    {
        if (!s_State.GPUDirty)
            return;

        s_State.GPULights.clear();

        for (auto &light : s_State.Lights)
        {

            if (s_State.GPULights.size() >= s_State.MaxLights)
            {
                VW_WARN("vwrn", "Max lights reached (%u), some lights will not be rendered",
                        s_State.MaxLights);
                break;
            }

            GPULight gpuLight = {};
            gpuLight.Color = Vector4(light->GetColor().r / 255, light->GetColor().g / 255,
                                     light->GetColor().b / 255, light->GetIntensity());

            switch (light->GetLightType())
            {
            case LightType::Ambient:
                gpuLight.Data.x = 0.0f;
                break;
                // case LightType::Directional:
                // {
                //     auto dirLight = std::static_pointer_cast<DirectionalLight>(light);
                //     Vector3 dir = dirLight->GetDirection();
                //     gpuLight.Data.x = 1.0f;
                //     gpuLight.Direction = Vector4(dir.x, dir.y, dir.z, 0.0f);
                //     break;
                // }
                //
                // case LightType::Spot:
                // {
                //     auto spotLight = std::static_pointer_cast<SpotLight>(light);
                //     Vector3 pos = spotLight->GetPosition();
                //     Vector3 dir = spotLight->GetDirection();
                //     float range = spotLight->GetRange();
                //     float innerAngle = spotLight->GetInnerConeAngle();
                //     float outerAngle = spotLight->GetOuterConeAngle();
                //
                //     gpuLight.Data.x = 3.0f; // type 3
                //
                //     gpuLight.Position = Vector4(pos.x, pos.y, pos.z, range);
                //
                //     float outerCos = cosf(outerAngle * CE_DEG_TO_RAD);
                //     gpuLight.Direction = Vector4(dir.x, dir.y, dir.z, outerCos);
                //
                //     float innerCos = cosf(innerAngle * CE_DEG_TO_RAD);
                //     gpuLight.Data.y = innerCos;
                //
                //     gpuLight.Attenuation = Vector4(1.0f, 4.5f / range, 75.0f / (range * range),
                //     0.0f);
                //
                //     break;
                // }

            default:
                continue;
            }

            s_State.GPULights.push_back(gpuLight);
        }

        if (!s_State.GPULights.empty())
        {
            s_State.LightBuffer->UpdateData(s_State.GPULights.data(),
                                            sizeof(GPULight) * s_State.GPULights.size(), 0);
        }

        s_State.GPUDirty = false;
    }

    void LightSystem::Bind(u32 spot)
    {
        if (s_State.LightBuffer)
        {
            s_State.LightBuffer->Bind();
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, spot, s_State.LightBuffer->GetID());
        }
    }

    void LightSystem::LightUpdated()
    {
        s_State.GPUDirty = true;
    }

} // namespace VW
