#include "DirectionalLight.h"

namespace VW
{
    DirectionalLight::DirectionalLight()
    {
        m_Direction = Vector3(0.0f, -1.0f, 0.0f);
    }

    DirectionalLight::~DirectionalLight()
    {
    }

    void DirectionalLight::SetDirection(const Vector3 &direction)
    {
        m_Direction = direction;
    }
} // namespace VW
