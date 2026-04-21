#pragma once

#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"

namespace VW
{
    struct Frustum
    {
        struct Plane
        {
            Vector3 Normal;
            float D;

            float Distance(const Vector3 &p) const
            {
                return Vector3::Dot(Normal, p) + D;
            }
        };

        Plane Planes[6];

        bool IsSphereInside(const Vector3 &center, float radius) const;
        void Extract(const Matrix4 &m);
    };

    class VW_API Camera
    {
    public:
        Camera() = default;
        virtual ~Camera() = default;

        virtual Matrix4 GetProjection() = 0;
        Matrix4 GetView();

        void SetPosition(const Vector3 &pos);
        inline Vector3 GetPosition()
        {
            return m_Pos;
        };

        void SetOrientation(const Quaternion &quat);
        inline Quaternion GetOrientation()
        {
            return m_Orientation;
        }

        inline Vector3 Forward() const
        {
            return m_Orientation.RotateVector(Vector3(0, 0, -1));
        }

        inline Vector3 Right() const
        {
            return m_Orientation.RotateVector(Vector3(1, 0, 0));
        }

        virtual void Resize(float width, float height) = 0;

    protected:
        Vector3 m_Pos;
        Quaternion m_Orientation;
        Matrix4 m_View, m_Proj;
        bool m_Dirty = false;
    };
} // namespace VW
