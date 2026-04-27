#pragma once

#include "Light.h"
#include "Math/Vector.h"

namespace VW
{
    class VW_API DirectionalLight : public Light
    {
    public:
        DirectionalLight();
        ~DirectionalLight();

        virtual LightType GetLightType() const override
        {
            return LightType::Directional;
        }

        void SetDirection(const Vector3 &direction);
        const Vector3 &GetDirection() const
        {
            return m_Direction;
        }

    private:
        Vector3 m_Direction;
    };
} // namespace VW
