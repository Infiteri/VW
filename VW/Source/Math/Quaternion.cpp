#include "Quaternion.h"
#include "Matrix.h"
#include <cmath>
#include <cfloat>

namespace VW
{
    Quaternion::Quaternion() : x(0), y(0), z(0), w(1)
    {
    }

    Quaternion::Quaternion(const Vector3 &axis, float angle)
    {
        float half = 0.5f * angle;
        float s = std::sin(half);
        w = std::cos(half);
        x = s * axis.x;
        y = s * axis.y;
        z = s * axis.z;
    }

    Quaternion::Quaternion(float x, float y, float z, float w)
    {
        Set(x, y, z, w);
    }

    Quaternion::Quaternion(const Quaternion &other)
    {
        Set(other);
    }

    void Quaternion::Set(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    void Quaternion::Set(const Quaternion &other)
    {
        Set(other.x, other.y, other.z, other.w);
    }

    Quaternion Quaternion::operator*(const Quaternion &other) const
    {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z);
    }

    Quaternion &Quaternion::operator+=(const Quaternion &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    Quaternion &Quaternion::operator-=(const Quaternion &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    Quaternion &Quaternion::operator*=(const Quaternion &other)
    {
        *this = *this * other;
        return *this;
    }

    Quaternion &Quaternion::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    Quaternion &Quaternion::operator/=(float scalar)
    {
        float inv = 1.0f / scalar;
        x *= inv;
        y *= inv;
        z *= inv;
        w *= inv;
        return *this;
    }

    bool Quaternion::operator==(const Quaternion &other) const
    {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    bool Quaternion::operator!=(const Quaternion &other) const
    {
        return !(*this == other);
    }

    float Quaternion::Length() const
    {
        return std::sqrt(LengthSquared());
    }

    float Quaternion::LengthSquared() const
    {
        return w * w + x * x + y * y + z * z;
    }

    void Quaternion::Normalize()
    {
        float len = Length();
        if (len < FLT_EPSILON)
        {
            Set(1, 0, 0, 0);
            return;
        }
        float inv = 1.0f / len;
        w *= inv;
        x *= inv;
        y *= inv;
        z *= inv;
    }

    Quaternion Quaternion::Normalized() const
    {
        Quaternion n(*this);
        n.Normalize();
        return n;
    }

    void Quaternion::Conjugate()
    {
        x = -x;
        y = -y;
        z = -z;
    }

    Quaternion Quaternion::Conjugated() const
    {
        return Quaternion(-x, -y, -z, w);
    }

    float Quaternion::Dot(const Quaternion &other) const
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    Vector3 Quaternion::RotateVector(const Vector3 &v) const
    {
        // q * (0, v) * q^-1, assuming normalized q so q^-1 = conjugate
        Quaternion qv(v.x, v.y, v.z, 0);
        Quaternion result = *this * qv * Conjugated();
        return Vector3(result.x, result.y, result.z);
    }

    Matrix4 Quaternion::GetMatrix() const
    {
        Quaternion n = Normalized();

        Matrix4 m;
        float xx = n.x * n.x;
        float yy = n.y * n.y;
        float zz = n.z * n.z;
        float xy = n.x * n.y;
        float xz = n.x * n.z;
        float yz = n.y * n.z;
        float wx = n.w * n.x;
        float wy = n.w * n.y;
        float wz = n.w * n.z;

        m.data[0] = 1 - 2 * (yy + zz);
        m.data[1] = 2 * (xy + wz);
        m.data[2] = 2 * (xz - wy);
        m.data[3] = 0;

        m.data[4] = 2 * (xy - wz);
        m.data[5] = 1 - 2 * (xx + zz);
        m.data[6] = 2 * (yz + wx);
        m.data[7] = 0;

        m.data[8] = 2 * (xz + wy);
        m.data[9] = 2 * (yz - wx);
        m.data[10] = 1 - 2 * (xx + yy);
        m.data[11] = 0;

        m.data[12] = 0;
        m.data[13] = 0;
        m.data[14] = 0;
        m.data[15] = 1;

        return m;
    }

    Quaternion Quaternion::Identity()
    {
        return Quaternion(0, 0, 0, 1);
    }

    Quaternion Quaternion::FromEulerAngles(float pitch, float yaw, float roll)
    {
        float cy = std::cos(yaw * 0.5f);
        float sy = std::sin(yaw * 0.5f);
        float cp = std::cos(pitch * 0.5f);
        float sp = std::sin(pitch * 0.5f);
        float cr = std::cos(roll * 0.5f);
        float sr = std::sin(roll * 0.5f);

        Quaternion q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = cy * sp * cr + sy * cp * sr;
        q.y = -cy * sp * sr + sy * cp * cr;
        q.z = -sy * sp * cr + cy * cp * sr;
        return q;
    }
} // namespace VW
