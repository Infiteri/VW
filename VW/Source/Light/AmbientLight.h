#pragma once

#include "Light.h"

namespace VW
{
    class VW_API AmbientLight : public Light
    {
    public:
        AmbientLight();
        ~AmbientLight() = default;

        virtual LightType GetLightType() const override
        {
            return LightType::Ambient;
        }
    };
} // namespace VW
