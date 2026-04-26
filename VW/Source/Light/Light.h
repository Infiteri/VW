#pragma once

#include "Base.h"
#include "Color.h"

namespace VW
{
    enum class LightType
    {
        Ambient,
        Directional,
        Spot
    };

    class VW_API Light
    {
    public:
        Light() = default;
        virtual ~Light() = default;

        inline float GetIntensity()
        {
            return m_Intensity;
        };
        inline Color &GetColor()
        {
            return m_Color;
        };

        void SetIntensity(float intensity);
        void SetColor(const Color &color);

        virtual LightType GetLightType() const = 0;

    protected:
        Color m_Color;
        float m_Intensity;
    };
} // namespace VW
