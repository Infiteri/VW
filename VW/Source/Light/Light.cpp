#include "Light.h"

namespace VW
{
    Light::Light() : m_Intensity(1.0), m_Color({})
    {
    }

    void Light::SetIntensity(float intensity)
    {
        m_Intensity = intensity;
    }

    void Light::SetColor(const Color &color)
    {
        m_Color = color;
    }
} // namespace VW
