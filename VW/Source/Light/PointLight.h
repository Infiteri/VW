#pragma once

#include "Light.h"
#include "Math/Vector.h"

namespace VW
{
    class VW_API PointLight : public Light
    {
    public:
        PointLight();
        ~PointLight();

        virtual LightType GetLightType() const override
        {
            return LightType::Point;
        }

        void SetPosition(const Vector3 &position);

        const Vector3 &GetPosition() const
        {
            return m_Position;
        }

        void SetRange(float range);

        float GetRange() const
        {
            return m_Range;
        }

    private:
        Vector3 m_Position;
        float m_Range = 10.0f;
    };
} // namespace VW
