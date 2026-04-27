#include "SpotLight.h"

namespace VW
{
    SpotLight::SpotLight()
    {
        m_Direction = Vector3(0.0f, -1.0f, 0.0f);
        SetColor(Color(1.0f, 1.0f, 1.0f));
        SetIntensity(1.0f);
    }

    SpotLight::~SpotLight()
    {
    }

    void SpotLight::SetPosition(const Vector3 &position)
    {
        m_Position = position;
    }

    void SpotLight::SetDirection(const Vector3 &direction)
    {
        m_Direction = direction.Normalized();
    }

    void SpotLight::SetRange(float range)
    {
        m_Range = range;
    }

    void SpotLight::SetInnerConeAngle(float degrees)
    {
        m_InnerConeAngle = degrees;
        if (m_InnerConeAngle > m_OuterConeAngle)
            m_InnerConeAngle = m_OuterConeAngle;
    }

    void SpotLight::SetOuterConeAngle(float degrees)
    {
        m_OuterConeAngle = degrees;
        if (m_OuterConeAngle < m_InnerConeAngle)
            m_OuterConeAngle = m_InnerConeAngle;
    }
} // namespace VW
