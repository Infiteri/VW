#include "Light.h"

namespace VW
{
    void Light::SetIntensity(float intensity)
    {
        m_Intensity = intensity;
    }

    void Light::SetColor(const Color &color)
    {
        m_Color = color;
    }
} // namespace VW
