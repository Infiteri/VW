#pragma once

#include "Base.h"

namespace VW
{
    class VW_API Vector2
    {
    public:
        float x, y;

        Vector2();
        Vector2(const Vector2 &other);
        Vector2(float x, float y);
        ~Vector2() = default;

        Vector2 operator+(const Vector2 &other) const;
        Vector2 operator-(const Vector2 &other) const;
        Vector2 operator*(float scalar) const;
        Vector2 operator/(float scalar) const;
        Vector2 operator*(const Vector2 &other) const;
        Vector2 operator-() const;

        Vector2 &operator+=(const Vector2 &other);
        Vector2 &operator-=(const Vector2 &other);
        Vector2 &operator*=(float scalar);
        Vector2 &operator/=(float scalar);
        Vector2 &operator*=(const Vector2 &other);

        bool operator==(const Vector2 &other) const;
        bool operator!=(const Vector2 &other) const;

        float &operator[](int index);
        const float &operator[](int index) const;

        void Set(float x, float y);

        float Length() const;
        float LengthSquared() const;
        Vector2 Normalized() const;
        Vector2 &Normalize();

        float Dot(const Vector2 &other) const;
        static float Dot(const Vector2 &a, const Vector2 &b);

        float Distance(const Vector2 &other) const;
        float DistanceSquared(const Vector2 &other) const;
        static float Distance(const Vector2 &a, const Vector2 &b);

        Vector2 Lerp(const Vector2 &other, float t) const;
        static Vector2 Lerp(const Vector2 &a, const Vector2 &b, float t);

        Vector2 Min(const Vector2 &other) const;
        Vector2 Max(const Vector2 &other) const;
        static Vector2 Min(const Vector2 &a, const Vector2 &b);
        static Vector2 Max(const Vector2 &a, const Vector2 &b);

        static Vector2 Zero();
        static Vector2 One();
        static Vector2 Up();
        static Vector2 Down();
        static Vector2 Left();
        static Vector2 Right();
    };

    class VW_API Vector3
    {
    public:
        float x, y, z;

        Vector3();
        Vector3(const Vector3 &other);
        Vector3(float x, float y, float z);
        ~Vector3() = default;

        Vector3 operator+(const Vector3 &other) const;
        Vector3 operator-(const Vector3 &other) const;
        Vector3 operator*(float scalar) const;
        Vector3 operator/(float scalar) const;
        Vector3 operator*(const Vector3 &other) const;
        Vector3 operator-() const;

        Vector3 &operator+=(const Vector3 &other);
        Vector3 &operator-=(const Vector3 &other);
        Vector3 &operator*=(float scalar);
        Vector3 &operator/=(float scalar);
        Vector3 &operator*=(const Vector3 &other);

        bool operator==(const Vector3 &other) const;
        bool operator!=(const Vector3 &other) const;

        float &operator[](int index);
        const float &operator[](int index) const;

        void Set(float x, float y, float z);

        float Length() const;
        float LengthSquared() const;
        Vector3 Normalized() const;
        Vector3 &Normalize();

        float Dot(const Vector3 &other) const;
        static float Dot(const Vector3 &a, const Vector3 &b);

        Vector3 Cross(const Vector3 &other) const;
        static Vector3 Cross(const Vector3 &a, const Vector3 &b);

        float Distance(const Vector3 &other) const;
        float DistanceSquared(const Vector3 &other) const;
        static float Distance(const Vector3 &a, const Vector3 &b);

        Vector3 Lerp(const Vector3 &other, float t) const;
        static Vector3 Lerp(const Vector3 &a, const Vector3 &b, float t);

        Vector3 Reflect(const Vector3 &normal) const;
        static Vector3 Reflect(const Vector3 &v, const Vector3 &normal);

        Vector3 Min(const Vector3 &other) const;
        Vector3 Max(const Vector3 &other) const;
        static Vector3 Min(const Vector3 &a, const Vector3 &b);
        static Vector3 Max(const Vector3 &a, const Vector3 &b);

        float MaxComponent() const;
        float MinComponent() const;

        static Vector3 Zero();
        static Vector3 One();
        static Vector3 Up();
        static Vector3 Down();
        static Vector3 Left();
        static Vector3 Right();
        static Vector3 Forward();
        static Vector3 Back();
    };

    class VW_API Vector4
    {
    public:
        float x, y, z, w;

        Vector4();
        Vector4(const Vector4 &other);
        Vector4(float x, float y, float z, float w);
        Vector4(const Vector3 &xyz, float w);
        explicit Vector4(const Vector3 &xyz);
        ~Vector4() = default;

        Vector4 operator+(const Vector4 &other) const;
        Vector4 operator-(const Vector4 &other) const;
        Vector4 operator*(float scalar) const;
        Vector4 operator/(float scalar) const;
        Vector4 operator*(const Vector4 &other) const;
        Vector4 operator-() const;

        Vector4 &operator+=(const Vector4 &other);
        Vector4 &operator-=(const Vector4 &other);
        Vector4 &operator*=(float scalar);
        Vector4 &operator/=(float scalar);
        Vector4 &operator*=(const Vector4 &other);

        bool operator==(const Vector4 &other) const;
        bool operator!=(const Vector4 &other) const;

        float &operator[](int index);
        const float &operator[](int index) const;

        void Set(float x, float y, float z, float w);
        void Set(const Vector3 &xyz, float w);

        float Length() const;
        float LengthSquared() const;
        Vector4 Normalized() const;
        Vector4 &Normalize();

        float Dot(const Vector4 &other) const;
        static float Dot(const Vector4 &a, const Vector4 &b);

        float Distance(const Vector4 &other) const;
        float DistanceSquared(const Vector4 &other) const;
        static float Distance(const Vector4 &a, const Vector4 &b);

        Vector4 Lerp(const Vector4 &other, float t) const;
        static Vector4 Lerp(const Vector4 &a, const Vector4 &b, float t);

        Vector3 ToVector3() const;
        Vector3 ToPosition() const;

        Vector4 Min(const Vector4 &other) const;
        Vector4 Max(const Vector4 &other) const;
        static Vector4 Min(const Vector4 &a, const Vector4 &b);
        static Vector4 Max(const Vector4 &a, const Vector4 &b);

        float MaxComponent() const;
        float MinComponent() const;

        static Vector4 Zero();
        static Vector4 One();
        static Vector4 IdentityPoint();
        static Vector4 IdentityVector();
    };
} // namespace VW
