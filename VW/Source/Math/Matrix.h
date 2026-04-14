#pragma once

#include "Base.h"
#include "Math/Vector.h"

namespace VW
{
    class VW_API Matrix4
    {
    public:
        float data[16];

        Matrix4();
        Matrix4(float *data);
        Matrix4(const Matrix4 &other);
        ~Matrix4() = default;

        Matrix4 operator*(const Matrix4 &other) const;

        static Matrix4 Perspective(float fov, float aspect, float nearPlane, float farPlane);
        static Matrix4 Ortho(float left, float right, float top, float bottom, float nearPlane,
                             float farPlane);
        static Matrix4 Multiply(const Matrix4 &a, const Matrix4 &b);
        static Matrix4 Invert(const Matrix4 &a);
        static Matrix4 Translate(const Vector3 &position);
        static Matrix4 RotateX(float angle);
        static Matrix4 RotateY(float angle);
        static Matrix4 RotateZ(float angle);
        static Matrix4 RotateZYX(const Vector3 &euler);
        static Matrix4 RotateXYZ(const Vector3 &euler);
        static Matrix4 Scale(const Vector3 &scale);
        static Matrix4 Transpose(const Matrix4 &matrix);

        static Vector3 Forward(const Matrix4 &m);
        static Vector3 Right(const Matrix4 &m);
    };
} // namespace VW
