#include "Vector.h"
#include <algorithm>
#include <cmath>

namespace VW
{
    Vector2::Vector2() : x(0), y(0)
    {
    }
    Vector2::Vector2(const Vector2 &other) : x(other.x), y(other.y)
    {
    }
    Vector2::Vector2(float x, float y) : x(x), y(y)
    {
    }

    Vector2 Vector2::operator+(const Vector2 &other) const
    {
        return Vector2(x + other.x, y + other.y);
    }
    Vector2 Vector2::operator-(const Vector2 &other) const
    {
        return Vector2(x - other.x, y - other.y);
    }
    Vector2 Vector2::operator*(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }
    Vector2 Vector2::operator/(float scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }
    Vector2 Vector2::operator*(const Vector2 &other) const
    {
        return Vector2(x * other.x, y * other.y);
    }
    Vector2 Vector2::operator-() const
    {
        return Vector2(-x, -y);
    }

    Vector2 &Vector2::operator+=(const Vector2 &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vector2 &Vector2::operator-=(const Vector2 &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Vector2 &Vector2::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Vector2 &Vector2::operator/=(float scalar)
    {
        float inv = 1.0f / scalar;
        x *= inv;
        y *= inv;
        return *this;
    }
    Vector2 &Vector2::operator*=(const Vector2 &other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    bool Vector2::operator==(const Vector2 &other) const
    {
        return x == other.x && y == other.y;
    }
    bool Vector2::operator!=(const Vector2 &other) const
    {
        return !(*this == other);
    }

    float &Vector2::operator[](int index)
    {
        return (&x)[index];
    }
    const float &Vector2::operator[](int index) const
    {
        return (&x)[index];
    }

    void Vector2::Set(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    float Vector2::Length() const
    {
        return std::sqrt(x * x + y * y);
    }
    float Vector2::LengthSquared() const
    {
        return x * x + y * y;
    }

    Vector2 Vector2::Normalized() const
    {
        float len = Length();
        if (len > 0)
            return *this / len;
        return *this;
    }

    Vector2 &Vector2::Normalize()
    {
        float len = Length();
        if (len > 0)
            *this /= len;
        return *this;
    }

    float Vector2::Dot(const Vector2 &other) const
    {
        return x * other.x + y * other.y;
    }
    float Vector2::Dot(const Vector2 &a, const Vector2 &b)
    {
        return a.x * b.x + a.y * b.y;
    }

    float Vector2::Distance(const Vector2 &other) const
    {
        return (*this - other).Length();
    }
    float Vector2::DistanceSquared(const Vector2 &other) const
    {
        return (*this - other).LengthSquared();
    }
    float Vector2::Distance(const Vector2 &a, const Vector2 &b)
    {
        return (a - b).Length();
    }

    Vector2 Vector2::Lerp(const Vector2 &other, float t) const
    {
        return *this * (1 - t) + other * t;
    }
    Vector2 Vector2::Lerp(const Vector2 &a, const Vector2 &b, float t)
    {
        return a * (1 - t) + b * t;
    }

    Vector2 Vector2::Min(const Vector2 &other) const
    {
        return Vector2(std::min(x, other.x), std::min(y, other.y));
    }
    Vector2 Vector2::Max(const Vector2 &other) const
    {
        return Vector2(std::max(x, other.x), std::max(y, other.y));
    }
    Vector2 Vector2::Min(const Vector2 &a, const Vector2 &b)
    {
        return Vector2(std::min(a.x, b.x), std::min(a.y, b.y));
    }
    Vector2 Vector2::Max(const Vector2 &a, const Vector2 &b)
    {
        return Vector2(std::max(a.x, b.x), std::max(a.y, b.y));
    }

    Vector2 Vector2::Zero()
    {
        return Vector2(0, 0);
    }
    Vector2 Vector2::One()
    {
        return Vector2(1, 1);
    }
    Vector2 Vector2::Up()
    {
        return Vector2(0, 1);
    }
    Vector2 Vector2::Down()
    {
        return Vector2(0, -1);
    }
    Vector2 Vector2::Left()
    {
        return Vector2(-1, 0);
    }
    Vector2 Vector2::Right()
    {
        return Vector2(1, 0);
    }

    Vector3::Vector3() : x(0), y(0), z(0)
    {
    }
    Vector3::Vector3(const Vector3 &other) : x(other.x), y(other.y), z(other.z)
    {
    }
    Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
    {
    }

    Vector3 Vector3::operator+(const Vector3 &other) const
    {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    Vector3 Vector3::operator-(const Vector3 &other) const
    {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    Vector3 Vector3::operator*(float scalar) const
    {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    Vector3 Vector3::operator/(float scalar) const
    {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }
    Vector3 Vector3::operator*(const Vector3 &other) const
    {
        return Vector3(x * other.x, y * other.y, z * other.z);
    }
    Vector3 Vector3::operator-() const
    {
        return Vector3(-x, -y, -z);
    }

    Vector3 &Vector3::operator+=(const Vector3 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    Vector3 &Vector3::operator-=(const Vector3 &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    Vector3 &Vector3::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    Vector3 &Vector3::operator/=(float scalar)
    {
        float inv = 1.0f / scalar;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }
    Vector3 &Vector3::operator*=(const Vector3 &other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    bool Vector3::operator==(const Vector3 &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
    bool Vector3::operator!=(const Vector3 &other) const
    {
        return !(*this == other);
    }

    float &Vector3::operator[](int index)
    {
        return (&x)[index];
    }
    const float &Vector3::operator[](int index) const
    {
        return (&x)[index];
    }

    void Vector3::Set(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float Vector3::Length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }
    float Vector3::LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    Vector3 Vector3::Normalized() const
    {
        float len = Length();
        if (len > 0)
            return *this / len;
        return *this;
    }

    Vector3 &Vector3::Normalize()
    {
        float len = Length();
        if (len > 0)
            *this /= len;
        return *this;
    }

    float Vector3::Dot(const Vector3 &other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }
    float Vector3::Dot(const Vector3 &a, const Vector3 &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Vector3 Vector3::Cross(const Vector3 &other) const
    {
        return Vector3(y * other.z - z * other.y, z * other.x - x * other.z,
                       x * other.y - y * other.x);
    }

    Vector3 Vector3::Cross(const Vector3 &a, const Vector3 &b)
    {
        return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    }

    float Vector3::Distance(const Vector3 &other) const
    {
        return (*this - other).Length();
    }
    float Vector3::DistanceSquared(const Vector3 &other) const
    {
        return (*this - other).LengthSquared();
    }
    float Vector3::Distance(const Vector3 &a, const Vector3 &b)
    {
        return (a - b).Length();
    }

    Vector3 Vector3::Lerp(const Vector3 &other, float t) const
    {
        return *this * (1 - t) + other * t;
    }
    Vector3 Vector3::Lerp(const Vector3 &a, const Vector3 &b, float t)
    {
        return a * (1 - t) + b * t;
    }

    Vector3 Vector3::Reflect(const Vector3 &normal) const
    {
        return *this - normal * (2 * Dot(normal));
    }

    Vector3 Vector3::Reflect(const Vector3 &v, const Vector3 &normal)
    {
        return v - normal * (2 * Dot(v, normal));
    }

    Vector3 Vector3::Min(const Vector3 &other) const
    {
        return Vector3(std::min(x, other.x), std::min(y, other.y), std::min(z, other.z));
    }

    Vector3 Vector3::Max(const Vector3 &other) const
    {
        return Vector3(std::max(x, other.x), std::max(y, other.y), std::max(z, other.z));
    }

    Vector3 Vector3::Min(const Vector3 &a, const Vector3 &b)
    {
        return Vector3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    }

    Vector3 Vector3::Max(const Vector3 &a, const Vector3 &b)
    {
        return Vector3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
    }

    float Vector3::MaxComponent() const
    {
        return std::max({x, y, z});
    }
    float Vector3::MinComponent() const
    {
        return std::min({x, y, z});
    }

    Vector3 Vector3::Zero()
    {
        return Vector3(0, 0, 0);
    }
    Vector3 Vector3::One()
    {
        return Vector3(1, 1, 1);
    }
    Vector3 Vector3::Up()
    {
        return Vector3(0, 1, 0);
    }
    Vector3 Vector3::Down()
    {
        return Vector3(0, -1, 0);
    }
    Vector3 Vector3::Left()
    {
        return Vector3(-1, 0, 0);
    }
    Vector3 Vector3::Right()
    {
        return Vector3(1, 0, 0);
    }
    Vector3 Vector3::Forward()
    {
        return Vector3(0, 0, 1);
    }
    Vector3 Vector3::Back()
    {
        return Vector3(0, 0, -1);
    }

    Vector4::Vector4() : x(0), y(0), z(0), w(0)
    {
    }
    Vector4::Vector4(const Vector4 &other) : x(other.x), y(other.y), z(other.z), w(other.w)
    {
    }
    Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
    {
    }
    Vector4::Vector4(const Vector3 &xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w)
    {
    }
    Vector4::Vector4(const Vector3 &xyz) : x(xyz.x), y(xyz.y), z(xyz.z), w(1.0f)
    {
    }

    Vector4 Vector4::operator+(const Vector4 &other) const
    {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }
    Vector4 Vector4::operator-(const Vector4 &other) const
    {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }
    Vector4 Vector4::operator*(float scalar) const
    {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }
    Vector4 Vector4::operator/(float scalar) const
    {
        return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
    }
    Vector4 Vector4::operator*(const Vector4 &other) const
    {
        return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
    }
    Vector4 Vector4::operator-() const
    {
        return Vector4(-x, -y, -z, -w);
    }

    Vector4 &Vector4::operator+=(const Vector4 &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }
    Vector4 &Vector4::operator-=(const Vector4 &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }
    Vector4 &Vector4::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }
    Vector4 &Vector4::operator/=(float scalar)
    {
        float inv = 1.0f / scalar;
        x *= inv;
        y *= inv;
        z *= inv;
        w *= inv;
        return *this;
    }
    Vector4 &Vector4::operator*=(const Vector4 &other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
        return *this;
    }

    bool Vector4::operator==(const Vector4 &other) const
    {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
    bool Vector4::operator!=(const Vector4 &other) const
    {
        return !(*this == other);
    }

    float &Vector4::operator[](int index)
    {
        return (&x)[index];
    }
    const float &Vector4::operator[](int index) const
    {
        return (&x)[index];
    }

    void Vector4::Set(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }
    void Vector4::Set(const Vector3 &xyz, float w)
    {
        this->x = xyz.x;
        this->y = xyz.y;
        this->z = xyz.z;
        this->w = w;
    }

    float Vector4::Length() const
    {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }
    float Vector4::LengthSquared() const
    {
        return x * x + y * y + z * z + w * w;
    }

    Vector4 Vector4::Normalized() const
    {
        float len = Length();
        if (len > 0)
            return *this / len;
        return *this;
    }

    Vector4 &Vector4::Normalize()
    {
        float len = Length();
        if (len > 0)
            *this /= len;
        return *this;
    }

    float Vector4::Dot(const Vector4 &other) const
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
    float Vector4::Dot(const Vector4 &a, const Vector4 &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    float Vector4::Distance(const Vector4 &other) const
    {
        return (*this - other).Length();
    }
    float Vector4::DistanceSquared(const Vector4 &other) const
    {
        return (*this - other).LengthSquared();
    }
    float Vector4::Distance(const Vector4 &a, const Vector4 &b)
    {
        return (a - b).Length();
    }

    Vector4 Vector4::Lerp(const Vector4 &other, float t) const
    {
        return *this * (1 - t) + other * t;
    }
    Vector4 Vector4::Lerp(const Vector4 &a, const Vector4 &b, float t)
    {
        return a * (1 - t) + b * t;
    }

    Vector3 Vector4::ToVector3() const
    {
        return Vector3(x, y, z);
    }
    Vector3 Vector4::ToPosition() const
    {
        if (w != 0.0f)
            return Vector3(x / w, y / w, z / w);
        return Vector3(x, y, z);
    }

    Vector4 Vector4::Min(const Vector4 &other) const
    {
        return Vector4(std::min(x, other.x), std::min(y, other.y), std::min(z, other.z),
                       std::min(w, other.w));
    }

    Vector4 Vector4::Max(const Vector4 &other) const
    {
        return Vector4(std::max(x, other.x), std::max(y, other.y), std::max(z, other.z),
                       std::max(w, other.w));
    }

    Vector4 Vector4::Min(const Vector4 &a, const Vector4 &b)
    {
        return Vector4(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z),
                       std::min(a.w, b.w));
    }

    Vector4 Vector4::Max(const Vector4 &a, const Vector4 &b)
    {
        return Vector4(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z),
                       std::max(a.w, b.w));
    }

    float Vector4::MaxComponent() const
    {
        return std::max({x, y, z, w});
    }
    float Vector4::MinComponent() const
    {
        return std::min({x, y, z, w});
    }

    Vector4 Vector4::Zero()
    {
        return Vector4(0, 0, 0, 0);
    }
    Vector4 Vector4::One()
    {
        return Vector4(1, 1, 1, 1);
    }
    Vector4 Vector4::IdentityPoint()
    {
        return Vector4(0, 0, 0, 1);
    }
    Vector4 Vector4::IdentityVector()
    {
        return Vector4(0, 0, 0, 0);
    }
} // namespace VW
