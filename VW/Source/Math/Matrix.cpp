#include "Matrix.h"
#include <cmath>

namespace VW
{
    Matrix4::Matrix4()
    {
        for (int i = 0; i < 16; i++)
            data[i] = (i == 0 || i == 5 || i == 10 || i == 15) ? 1 : 0;
    }

    Matrix4::Matrix4(float *data)
    {
        for (int i = 0; i < 16; i++)
            this->data[i] = data[i];
    }

    Matrix4::Matrix4(const Matrix4 &other)
    {
        for (int i = 0; i < 16; i++)
            data[i] = other.data[i];
    }

    Matrix4 Matrix4::operator*(const Matrix4 &other) const
    {
        return Multiply(*this, other);
    }

    Matrix4 Matrix4::Perspective(float fov, float aspect, float nearPlane, float farPlane)
    {
        Matrix4 m;
        float f = 1.0f / std::tanf(fov * 0.5f);
        m.data[0] = f / aspect;
        m.data[5] = f;
        m.data[8] = 0;
        m.data[9] = 0;
        m.data[12] = 0;
        m.data[13] = 0;
        m.data[2] = 0;
        m.data[3] = 0;
        m.data[6] = 0;
        m.data[7] = 0;
        m.data[10] = (farPlane + nearPlane) / (nearPlane - farPlane);
        m.data[11] = -1;
        m.data[14] = (2 * farPlane * nearPlane) / (nearPlane - farPlane);
        m.data[15] = 0;
        return m;
    }

    Matrix4 Matrix4::Ortho(float left, float right, float top, float bottom, float nearPlane, float farPlane)
    {
        Matrix4 m;

        float lr = 1.0f / (right - left);
        float bt = 1.0f / (top - bottom);
        float nf = 1.0f / (nearPlane - farPlane);

        m.data[0] = 2.0f * lr;
        m.data[5] = 2.0f * bt;
        m.data[10] = 2.0f * nf;

        m.data[12] = -(right + left) * lr;
        m.data[13] = -(top + bottom) * bt;
        m.data[14] = (farPlane + nearPlane) * nf;

        return m;
    }

    Matrix4 Matrix4::Multiply(const Matrix4 &a, const Matrix4 &b)
    {
        Matrix4 m;

        // Column 0
        m.data[0] = a.data[0] * b.data[0] + a.data[4] * b.data[1] + a.data[8] * b.data[2] +
                    a.data[12] * b.data[3];
        m.data[1] = a.data[1] * b.data[0] + a.data[5] * b.data[1] + a.data[9] * b.data[2] +
                    a.data[13] * b.data[3];
        m.data[2] = a.data[2] * b.data[0] + a.data[6] * b.data[1] + a.data[10] * b.data[2] +
                    a.data[14] * b.data[3];
        m.data[3] = a.data[3] * b.data[0] + a.data[7] * b.data[1] + a.data[11] * b.data[2] +
                    a.data[15] * b.data[3];

        // Column 1
        m.data[4] = a.data[0] * b.data[4] + a.data[4] * b.data[5] + a.data[8] * b.data[6] +
                    a.data[12] * b.data[7];
        m.data[5] = a.data[1] * b.data[4] + a.data[5] * b.data[5] + a.data[9] * b.data[6] +
                    a.data[13] * b.data[7];
        m.data[6] = a.data[2] * b.data[4] + a.data[6] * b.data[5] + a.data[10] * b.data[6] +
                    a.data[14] * b.data[7];
        m.data[7] = a.data[3] * b.data[4] + a.data[7] * b.data[5] + a.data[11] * b.data[6] +
                    a.data[15] * b.data[7];

        // Column 2
        m.data[8] = a.data[0] * b.data[8] + a.data[4] * b.data[9] + a.data[8] * b.data[10] +
                    a.data[12] * b.data[11];
        m.data[9] = a.data[1] * b.data[8] + a.data[5] * b.data[9] + a.data[9] * b.data[10] +
                    a.data[13] * b.data[11];
        m.data[10] = a.data[2] * b.data[8] + a.data[6] * b.data[9] + a.data[10] * b.data[10] +
                     a.data[14] * b.data[11];
        m.data[11] = a.data[3] * b.data[8] + a.data[7] * b.data[9] + a.data[11] * b.data[10] +
                     a.data[15] * b.data[11];

        // Column 3
        m.data[12] = a.data[0] * b.data[12] + a.data[4] * b.data[13] + a.data[8] * b.data[14] +
                     a.data[12] * b.data[15];
        m.data[13] = a.data[1] * b.data[12] + a.data[5] * b.data[13] + a.data[9] * b.data[14] +
                     a.data[13] * b.data[15];
        m.data[14] = a.data[2] * b.data[12] + a.data[6] * b.data[13] + a.data[10] * b.data[14] +
                     a.data[14] * b.data[15];
        m.data[15] = a.data[3] * b.data[12] + a.data[7] * b.data[13] + a.data[11] * b.data[14] +
                     a.data[15] * b.data[15];

        return m;
    }

    Matrix4 Matrix4::Invert(const Matrix4 &a)
    {
        Matrix4 m;

        float a00 = a.data[0];
        float a01 = a.data[1];
        float a02 = a.data[2];
        float a03 = a.data[3];
        float a10 = a.data[4];
        float a11 = a.data[5];
        float a12 = a.data[6];
        float a13 = a.data[7];
        float a20 = a.data[8];
        float a21 = a.data[9];
        float a22 = a.data[10];
        float a23 = a.data[11];
        float a30 = a.data[12];
        float a31 = a.data[13];
        float a32 = a.data[14];
        float a33 = a.data[15];
        float b00 = a00 * a11 - a01 * a10;
        float b01 = a00 * a12 - a02 * a10;
        float b02 = a00 * a13 - a03 * a10;
        float b03 = a01 * a12 - a02 * a11;
        float b04 = a01 * a13 - a03 * a11;
        float b05 = a02 * a13 - a03 * a12;
        float b06 = a20 * a31 - a21 * a30;
        float b07 = a20 * a32 - a22 * a30;
        float b08 = a20 * a33 - a23 * a30;
        float b09 = a21 * a32 - a22 * a31;
        float b10 = a21 * a33 - a23 * a31;
        float b11 = a22 * a33 - a23 * a32;
        float det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

        det = 1.0f / det;

        m.data[0] = (a11 * b11 - a12 * b10 + a13 * b09) * det;
        m.data[1] = (a02 * b10 - a01 * b11 - a03 * b09) * det;
        m.data[2] = (a31 * b05 - a32 * b04 + a33 * b03) * det;
        m.data[3] = (a22 * b04 - a21 * b05 - a23 * b03) * det;
        m.data[4] = (a12 * b08 - a10 * b11 - a13 * b07) * det;
        m.data[5] = (a00 * b11 - a02 * b08 + a03 * b07) * det;
        m.data[6] = (a32 * b02 - a30 * b05 - a33 * b01) * det;
        m.data[7] = (a20 * b05 - a22 * b02 + a23 * b01) * det;
        m.data[8] = (a10 * b10 - a11 * b08 + a13 * b06) * det;
        m.data[9] = (a01 * b08 - a00 * b10 - a03 * b06) * det;
        m.data[10] = (a30 * b04 - a31 * b02 + a33 * b00) * det;
        m.data[11] = (a21 * b02 - a20 * b04 - a23 * b00) * det;
        m.data[12] = (a11 * b07 - a10 * b09 - a12 * b06) * det;
        m.data[13] = (a00 * b09 - a01 * b07 + a02 * b06) * det;
        m.data[14] = (a31 * b01 - a30 * b03 - a32 * b00) * det;
        m.data[15] = (a20 * b03 - a21 * b01 + a22 * b00) * det;

        return m;
    }

    Matrix4 Matrix4::Translate(const Vector3 &position)
    {
        Matrix4 m;

        m.data[12] = position.x;
        m.data[13] = position.y;
        m.data[14] = position.z;

        return m;
    }

    Matrix4 Matrix4::RotateX(float x)
    {
        Matrix4 m;

        float c = std::cos(x);
        float s = std::sin(x);

        m.data[5] = c;
        m.data[6] = s;
        m.data[9] = -s;
        m.data[10] = c;

        return m;
    }

    Matrix4 Matrix4::RotateY(float y)
    {
        Matrix4 m;

        float c = std::cos(y);
        float s = std::sin(y);

        m.data[0] = c;
        m.data[2] = s;
        m.data[8] = -s;
        m.data[10] = c;

        return m;
    }

    Matrix4 Matrix4::RotateZ(float z)
    {
        Matrix4 m;

        float c = std::cos(z);
        float s = std::sin(z);

        m.data[0] = c;
        m.data[1] = s;
        m.data[4] = -s;
        m.data[5] = c;

        return m;
    }

    Matrix4 Matrix4::RotateZYX(const Vector3 &vec)
    {
        return Matrix4::RotateZ(vec.z) * (Matrix4::RotateY(vec.y) * Matrix4::RotateX(vec.x));
    }

    Matrix4 Matrix4::RotateXYZ(const Vector3 &vec)
    {
        return Matrix4::RotateX(vec.x) * (Matrix4::RotateY(vec.y) * Matrix4::RotateZ(vec.z));
    }

    Matrix4 Matrix4::Scale(const Vector3 &scale)
    {
        Matrix4 m;

        m.data[0] = scale.x;
        m.data[5] = scale.y;
        m.data[10] = scale.z;

        return m;
    }

    Vector3 Matrix4::Forward(const Matrix4 &mat)
    {
        Vector3 vec;

        vec.x = -mat.data[8];
        vec.y = -mat.data[9];
        vec.z = -mat.data[10];

        return vec;
    }

    Vector3 Matrix4::Right(const Matrix4 &mat)
    {
        Vector3 vec;

        vec.x = mat.data[0];
        vec.y = mat.data[1];
        vec.z = mat.data[2];

        return vec;
    }

    Matrix4 Matrix4::Transpose(const Matrix4 &matrix)
    {
        Matrix4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                result.data[i * 4 + j] = matrix.data[j * 4 + i];

        return result;
    }
} // namespace VW
