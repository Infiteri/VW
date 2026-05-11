#include "PointLight.h"

namespace VW
{
    PointLight::PointLight() : m_Position(0, 0, 0), m_Range(10.0f)
    {
    }

    PointLight::~PointLight()
    {
    }

    void PointLight::SetPosition(const Vector3 &position)
    {
        m_Position = position;
    }

    void PointLight::SetRange(float range)
    {
        m_Range = range;
    }
} // namespace VW
