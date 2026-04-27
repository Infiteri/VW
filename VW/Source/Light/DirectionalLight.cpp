#include "DirectionalLight.h"

namespace VW
{
    DirectionalLight::DirectionalLight()
    {
        m_Direction = Vector3(0.0f, -1.0f, 0.0f);
        SetColor(Color(1.0f, 1.0f, 1.0f));
        SetIntensity(1.0f);
    }

    DirectionalLight::~DirectionalLight()
    {
    }

    void DirectionalLight::SetDirection(const Vector3 &direction)
    {
        m_Direction = direction.Normalized();
    }
} // namespace VW
