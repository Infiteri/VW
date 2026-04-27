#pragma once

#include "Light.h"
#include "Math/Vector.h"

namespace VW
{
    class VW_API SpotLight : public Light
    {
    public:
        SpotLight();
        ~SpotLight();

        virtual LightType GetLightType() const override
        {
            return LightType::Spot;
        }

        void SetPosition(const Vector3 &position);
        const Vector3 &GetPosition() const
        {
            return m_Position;
        }

        void SetDirection(const Vector3 &direction);
        const Vector3 &GetDirection() const
        {
            return m_Direction;
        }

        void SetRange(float range);
        float GetRange() const
        {
            return m_Range;
        }

        void SetInnerConeAngle(float degrees);
        void SetOuterConeAngle(float degrees);
        float GetInnerConeAngle() const
        {
            return m_InnerConeAngle;
        }
        float GetOuterConeAngle() const
        {
            return m_OuterConeAngle;
        }

    private:
        Vector3 m_Position;
        Vector3 m_Direction;
        float m_Range = 10.0f;
        float m_InnerConeAngle = 30.0f;
        float m_OuterConeAngle = 45.0f;
    };
} // namespace VW
