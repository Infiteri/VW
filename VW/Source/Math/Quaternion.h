#pragma once

#include "Base.h"
#include "Vector.h"

namespace VW
{
    class Matrix4;

    class VW_API Quaternion
    {
    public:
        float x, y, z, w;

        Quaternion();
        Quaternion(float x, float y, float z, float w);
        Quaternion(const Vector3 &axis, float angle);
        Quaternion(const Quaternion &other);
        ~Quaternion() = default;

        void Set(float x, float y, float z, float w);
        void Set(const Quaternion &other);

        Quaternion operator*(const Quaternion &other) const;

        Quaternion &operator+=(const Quaternion &other);
        Quaternion &operator-=(const Quaternion &other);
        Quaternion &operator*=(const Quaternion &other);
        Quaternion &operator*=(float scalar);
        Quaternion &operator/=(float scalar);

        bool operator==(const Quaternion &other) const;
        bool operator!=(const Quaternion &other) const;

        Quaternion Normalized() const;
        void Normalize();

        Quaternion Conjugated() const;
        void Conjugate();

        float Length() const;
        float LengthSquared() const;

        float Dot(const Quaternion &other) const;

        Vector3 RotateVector(const Vector3 &v) const;

        Matrix4 GetMatrix() const;

        static Quaternion Identity();
        static Quaternion FromEulerAngles(float pitch, float yaw, float roll);
    };
} // namespace VW
