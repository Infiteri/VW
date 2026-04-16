#pragma once

#include "Base.h"

namespace VW
{
    class VW_API Color
    {
    public:
        float r, g, b, a;

        Color(float r = 255.0f, float g = 255.0f, float b = 255.0f, float a = 255.0f);
        ~Color() = default;

        Color(const Color &other) = default;
        Color &operator=(const Color &other) = default;

        Color Normalized() const
        {
            return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
        }

        Color operator+() const
        {
            return *this;
        }
        Color operator-() const
        {
            return Color(-r, -g, -b, -a);
        }

        Color operator+(const Color &rhs) const
        {
            return Color(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
        }
        Color operator-(const Color &rhs) const
        {
            return Color(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a);
        }
        Color operator*(const Color &rhs) const
        {
            return Color(r * rhs.r, g * rhs.g, b * rhs.b, a * rhs.a);
        }
        Color operator/(const Color &rhs) const
        {
            return Color(r / rhs.r, g / rhs.g, b / rhs.b, a / rhs.a);
        }

        Color &operator+=(const Color &rhs)
        {
            r += rhs.r;
            g += rhs.g;
            b += rhs.b;
            a += rhs.a;
            return *this;
        }
        Color &operator-=(const Color &rhs)
        {
            r -= rhs.r;
            g -= rhs.g;
            b -= rhs.b;
            a -= rhs.a;
            return *this;
        }
        Color &operator*=(const Color &rhs)
        {
            r *= rhs.r;
            g *= rhs.g;
            b *= rhs.b;
            a *= rhs.a;
            return *this;
        }
        Color &operator/=(const Color &rhs)
        {
            r /= rhs.r;
            g /= rhs.g;
            b /= rhs.b;
            a /= rhs.a;
            return *this;
        }

        Color operator+(float v) const
        {
            return Color(r + v, g + v, b + v, a + v);
        }
        Color operator-(float v) const
        {
            return Color(r - v, g - v, b - v, a - v);
        }
        Color operator*(float v) const
        {
            return Color(r * v, g * v, b * v, a * v);
        }
        Color operator/(float v) const
        {
            return Color(r / v, g / v, b / v, a / v);
        }

        Color &operator+=(float v)
        {
            r += v;
            g += v;
            b += v;
            a += v;
            return *this;
        }
        Color &operator-=(float v)
        {
            r -= v;
            g -= v;
            b -= v;
            a -= v;
            return *this;
        }
        Color &operator*=(float v)
        {
            r *= v;
            g *= v;
            b *= v;
            a *= v;
            return *this;
        }
        Color &operator/=(float v)
        {
            r /= v;
            g /= v;
            b /= v;
            a /= v;
            return *this;
        }

        bool operator==(const Color &rhs) const
        {
            return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
        }
        bool operator!=(const Color &rhs) const
        {
            return !(*this == rhs);
        }

        void Clamp()
        {
            r = (r < 0) ? 0 : (r > 255) ? 255 : r;
            g = (g < 0) ? 0 : (g > 255) ? 255 : g;
            b = (b < 0) ? 0 : (b > 255) ? 255 : b;
            a = (a < 0) ? 0 : (a > 255) ? 255 : a;
        }
    };
} // namespace VW
