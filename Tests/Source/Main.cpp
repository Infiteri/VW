#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Math/Vector.h"
#include "doctest/doctest.h"

using namespace VW;

TEST_CASE("Vector2 Default Constructor")
{
    Vector2 v;
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(0.0f));
}

TEST_CASE("Vector2 Copy Constructor")
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(a);
    CHECK(b.x == doctest::Approx(1.0f));
    CHECK(b.y == doctest::Approx(2.0f));
}

TEST_CASE("Vector2 Parameterized Constructor")
{
    Vector2 v(3.0f, 4.0f);
    CHECK(v.x == doctest::Approx(3.0f));
    CHECK(v.y == doctest::Approx(4.0f));
}

TEST_CASE("Vector2 Addition Operator")
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(3.0f, 4.0f);
    Vector2 result = a + b;
    CHECK(result.x == doctest::Approx(4.0f));
    CHECK(result.y == doctest::Approx(6.0f));
}

TEST_CASE("Vector2 Subtraction Operator")
{
    Vector2 a(5.0f, 6.0f);
    Vector2 b(3.0f, 4.0f);
    Vector2 result = a - b;
    CHECK(result.x == doctest::Approx(2.0f));
    CHECK(result.y == doctest::Approx(2.0f));
}

TEST_CASE("Vector2 Scalar Multiplication Operator")
{
    Vector2 v(2.0f, 3.0f);
    Vector2 result = v * 3.0f;
    CHECK(result.x == doctest::Approx(6.0f));
    CHECK(result.y == doctest::Approx(9.0f));
}

TEST_CASE("Vector2 Scalar Division Operator")
{
    Vector2 v(6.0f, 9.0f);
    Vector2 result = v / 3.0f;
    CHECK(result.x == doctest::Approx(2.0f));
    CHECK(result.y == doctest::Approx(3.0f));
}

TEST_CASE("Vector2 Component-wise Multiplication Operator")
{
    Vector2 a(2.0f, 3.0f);
    Vector2 b(4.0f, 5.0f);
    Vector2 result = a * b;
    CHECK(result.x == doctest::Approx(8.0f));
    CHECK(result.y == doctest::Approx(15.0f));
}

TEST_CASE("Vector2 Negation Operator")
{
    Vector2 v(1.0f, -2.0f);
    Vector2 result = -v;
    CHECK(result.x == doctest::Approx(-1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
}

TEST_CASE("Vector2 Addition Assignment Operator")
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(3.0f, 4.0f);
    a += b;
    CHECK(a.x == doctest::Approx(4.0f));
    CHECK(a.y == doctest::Approx(6.0f));
}

TEST_CASE("Vector2 Subtraction Assignment Operator")
{
    Vector2 a(5.0f, 6.0f);
    Vector2 b(3.0f, 4.0f);
    a -= b;
    CHECK(a.x == doctest::Approx(2.0f));
    CHECK(a.y == doctest::Approx(2.0f));
}

TEST_CASE("Vector2 Scalar Multiplication Assignment Operator")
{
    Vector2 v(2.0f, 3.0f);
    v *= 3.0f;
    CHECK(v.x == doctest::Approx(6.0f));
    CHECK(v.y == doctest::Approx(9.0f));
}

TEST_CASE("Vector2 Scalar Division Assignment Operator")
{
    Vector2 v(6.0f, 9.0f);
    v /= 3.0f;
    CHECK(v.x == doctest::Approx(2.0f));
    CHECK(v.y == doctest::Approx(3.0f));
}

TEST_CASE("Vector2 Component-wise Multiplication Assignment Operator")
{
    Vector2 a(2.0f, 3.0f);
    Vector2 b(4.0f, 5.0f);
    a *= b;
    CHECK(a.x == doctest::Approx(8.0f));
    CHECK(a.y == doctest::Approx(15.0f));
}

TEST_CASE("Vector2 Equality Operator")
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(1.0f, 2.0f);
    Vector2 c(3.0f, 4.0f);
    CHECK(a == b);
    CHECK_FALSE(a == c);
}

TEST_CASE("Vector2 Inequality Operator")
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(1.0f, 2.0f);
    Vector2 c(3.0f, 4.0f);
    CHECK_FALSE(a != b);
    CHECK(a != c);
}

TEST_CASE("Vector2 Subscript Operator")
{
    Vector2 v(1.0f, 2.0f);
    CHECK(v[0] == doctest::Approx(1.0f));
    CHECK(v[1] == doctest::Approx(2.0f));
    v[0] = 5.0f;
    CHECK(v[0] == doctest::Approx(5.0f));
}

TEST_CASE("Vector2 Const Subscript Operator")
{
    const Vector2 v(1.0f, 2.0f);
    CHECK(v[0] == doctest::Approx(1.0f));
    CHECK(v[1] == doctest::Approx(2.0f));
}

TEST_CASE("Vector2 Set Method")
{
    Vector2 v;
    v.Set(3.0f, 4.0f);
    CHECK(v.x == doctest::Approx(3.0f));
    CHECK(v.y == doctest::Approx(4.0f));
}

TEST_CASE("Vector2 Length")
{
    Vector2 v(3.0f, 4.0f);
    CHECK(v.Length() == doctest::Approx(5.0f));
}

TEST_CASE("Vector2 LengthSquared")
{
    Vector2 v(3.0f, 4.0f);
    CHECK(v.LengthSquared() == doctest::Approx(25.0f));
}

TEST_CASE("Vector2 Normalized")
{
    Vector2 v(3.0f, 4.0f);
    Vector2 result = v.Normalized();
    CHECK(result.x == doctest::Approx(0.6f));
    CHECK(result.y == doctest::Approx(0.8f));
}

TEST_CASE("Vector2 Normalized Zero Vector")
{
    Vector2 v(0.0f, 0.0f);
    Vector2 result = v.Normalized();
    CHECK(result.x == doctest::Approx(0.0f));
    CHECK(result.y == doctest::Approx(0.0f));
}

TEST_CASE("Vector2 Normalize In-Place")
{
    Vector2 v(3.0f, 4.0f);
    Vector2 &result = v.Normalize();
    CHECK(&result == &v);
    CHECK(v.x == doctest::Approx(0.6f));
    CHECK(v.y == doctest::Approx(0.8f));
}

TEST_CASE("Vector2 Normalize Zero Vector")
{
    Vector2 v(0.0f, 0.0f);
    v.Normalize();
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(0.0f));
}

TEST_CASE("Vector2 Dot Product Instance")
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(3.0f, 4.0f);
    float result = a.Dot(b);
    CHECK(result == doctest::Approx(11.0f));
}

TEST_CASE("Vector2 Dot Product Static")
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(3.0f, 4.0f);
    float result = Vector2::Dot(a, b);
    CHECK(result == doctest::Approx(11.0f));
}

TEST_CASE("Vector2 Distance Instance")
{
    Vector2 a(0.0f, 0.0f);
    Vector2 b(3.0f, 4.0f);
    float result = a.Distance(b);
    CHECK(result == doctest::Approx(5.0f));
}

TEST_CASE("Vector2 DistanceSquared Instance")
{
    Vector2 a(0.0f, 0.0f);
    Vector2 b(3.0f, 4.0f);
    float result = a.DistanceSquared(b);
    CHECK(result == doctest::Approx(25.0f));
}

TEST_CASE("Vector2 Distance Static")
{
    Vector2 a(0.0f, 0.0f);
    Vector2 b(3.0f, 4.0f);
    float result = Vector2::Distance(a, b);
    CHECK(result == doctest::Approx(5.0f));
}

TEST_CASE("Vector2 Lerp Instance")
{
    Vector2 a(0.0f, 0.0f);
    Vector2 b(10.0f, 20.0f);
    Vector2 result = a.Lerp(b, 0.5f);
    CHECK(result.x == doctest::Approx(5.0f));
    CHECK(result.y == doctest::Approx(10.0f));
}

TEST_CASE("Vector2 Lerp Static")
{
    Vector2 a(0.0f, 0.0f);
    Vector2 b(10.0f, 20.0f);
    Vector2 result = Vector2::Lerp(a, b, 0.5f);
    CHECK(result.x == doctest::Approx(5.0f));
    CHECK(result.y == doctest::Approx(10.0f));
}

TEST_CASE("Vector2 Lerp t=0")
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(3.0f, 4.0f);
    Vector2 result = Vector2::Lerp(a, b, 0.0f);
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
}

TEST_CASE("Vector2 Lerp t=1")
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(3.0f, 4.0f);
    Vector2 result = Vector2::Lerp(a, b, 1.0f);
    CHECK(result.x == doctest::Approx(3.0f));
    CHECK(result.y == doctest::Approx(4.0f));
}

TEST_CASE("Vector2 Min Instance")
{
    Vector2 a(1.0f, 5.0f);
    Vector2 b(3.0f, 2.0f);
    Vector2 result = a.Min(b);
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
}

TEST_CASE("Vector2 Max Instance")
{
    Vector2 a(1.0f, 5.0f);
    Vector2 b(3.0f, 2.0f);
    Vector2 result = a.Max(b);
    CHECK(result.x == doctest::Approx(3.0f));
    CHECK(result.y == doctest::Approx(5.0f));
}

TEST_CASE("Vector2 Min Static")
{
    Vector2 a(1.0f, 5.0f);
    Vector2 b(3.0f, 2.0f);
    Vector2 result = Vector2::Min(a, b);
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
}

TEST_CASE("Vector2 Max Static")
{
    Vector2 a(1.0f, 5.0f);
    Vector2 b(3.0f, 2.0f);
    Vector2 result = Vector2::Max(a, b);
    CHECK(result.x == doctest::Approx(3.0f));
    CHECK(result.y == doctest::Approx(5.0f));
}

TEST_CASE("Vector2 Zero Static")
{
    Vector2 v = Vector2::Zero();
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(0.0f));
}

TEST_CASE("Vector2 One Static")
{
    Vector2 v = Vector2::One();
    CHECK(v.x == doctest::Approx(1.0f));
    CHECK(v.y == doctest::Approx(1.0f));
}

TEST_CASE("Vector2 Up Static")
{
    Vector2 v = Vector2::Up();
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(1.0f));
}

TEST_CASE("Vector2 Down Static")
{
    Vector2 v = Vector2::Down();
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(-1.0f));
}

TEST_CASE("Vector2 Left Static")
{
    Vector2 v = Vector2::Left();
    CHECK(v.x == doctest::Approx(-1.0f));
    CHECK(v.y == doctest::Approx(0.0f));
}

TEST_CASE("Vector2 Right Static")
{
    Vector2 v = Vector2::Right();
    CHECK(v.x == doctest::Approx(1.0f));
    CHECK(v.y == doctest::Approx(0.0f));
}

TEST_CASE("Vector3 Default Constructor")
{
    Vector3 v;
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(0.0f));
    CHECK(v.z == doctest::Approx(0.0f));
}

TEST_CASE("Vector3 Copy Constructor")
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(a);
    CHECK(b.x == doctest::Approx(1.0f));
    CHECK(b.y == doctest::Approx(2.0f));
    CHECK(b.z == doctest::Approx(3.0f));
}

TEST_CASE("Vector3 Parameterized Constructor")
{
    Vector3 v(3.0f, 4.0f, 5.0f);
    CHECK(v.x == doctest::Approx(3.0f));
    CHECK(v.y == doctest::Approx(4.0f));
    CHECK(v.z == doctest::Approx(5.0f));
}

TEST_CASE("Vector3 Addition Operator")
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 result = a + b;
    CHECK(result.x == doctest::Approx(5.0f));
    CHECK(result.y == doctest::Approx(7.0f));
    CHECK(result.z == doctest::Approx(9.0f));
}

TEST_CASE("Vector3 Subtraction Operator")
{
    Vector3 a(5.0f, 6.0f, 7.0f);
    Vector3 b(3.0f, 4.0f, 5.0f);
    Vector3 result = a - b;
    CHECK(result.x == doctest::Approx(2.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(2.0f));
}

TEST_CASE("Vector3 Scalar Multiplication Operator")
{
    Vector3 v(2.0f, 3.0f, 4.0f);
    Vector3 result = v * 3.0f;
    CHECK(result.x == doctest::Approx(6.0f));
    CHECK(result.y == doctest::Approx(9.0f));
    CHECK(result.z == doctest::Approx(12.0f));
}

TEST_CASE("Vector3 Scalar Division Operator")
{
    Vector3 v(6.0f, 9.0f, 12.0f);
    Vector3 result = v / 3.0f;
    CHECK(result.x == doctest::Approx(2.0f));
    CHECK(result.y == doctest::Approx(3.0f));
    CHECK(result.z == doctest::Approx(4.0f));
}

TEST_CASE("Vector3 Component-wise Multiplication Operator")
{
    Vector3 a(2.0f, 3.0f, 4.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 result = a * b;
    CHECK(result.x == doctest::Approx(8.0f));
    CHECK(result.y == doctest::Approx(15.0f));
    CHECK(result.z == doctest::Approx(24.0f));
}

TEST_CASE("Vector3 Negation Operator")
{
    Vector3 v(1.0f, -2.0f, 3.0f);
    Vector3 result = -v;
    CHECK(result.x == doctest::Approx(-1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(-3.0f));
}

TEST_CASE("Vector3 Addition Assignment Operator")
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    a += b;
    CHECK(a.x == doctest::Approx(5.0f));
    CHECK(a.y == doctest::Approx(7.0f));
    CHECK(a.z == doctest::Approx(9.0f));
}

TEST_CASE("Vector3 Subtraction Assignment Operator")
{
    Vector3 a(5.0f, 6.0f, 7.0f);
    Vector3 b(3.0f, 4.0f, 5.0f);
    a -= b;
    CHECK(a.x == doctest::Approx(2.0f));
    CHECK(a.y == doctest::Approx(2.0f));
    CHECK(a.z == doctest::Approx(2.0f));
}

TEST_CASE("Vector3 Scalar Multiplication Assignment Operator")
{
    Vector3 v(2.0f, 3.0f, 4.0f);
    v *= 3.0f;
    CHECK(v.x == doctest::Approx(6.0f));
    CHECK(v.y == doctest::Approx(9.0f));
    CHECK(v.z == doctest::Approx(12.0f));
}

TEST_CASE("Vector3 Scalar Division Assignment Operator")
{
    Vector3 v(6.0f, 9.0f, 12.0f);
    v /= 3.0f;
    CHECK(v.x == doctest::Approx(2.0f));
    CHECK(v.y == doctest::Approx(3.0f));
    CHECK(v.z == doctest::Approx(4.0f));
}

TEST_CASE("Vector3 Component-wise Multiplication Assignment Operator")
{
    Vector3 a(2.0f, 3.0f, 4.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    a *= b;
    CHECK(a.x == doctest::Approx(8.0f));
    CHECK(a.y == doctest::Approx(15.0f));
    CHECK(a.z == doctest::Approx(24.0f));
}

TEST_CASE("Vector3 Equality Operator")
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);
    Vector3 c(3.0f, 4.0f, 5.0f);
    CHECK(a == b);
    CHECK_FALSE(a == c);
}

TEST_CASE("Vector3 Inequality Operator")
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);
    Vector3 c(3.0f, 4.0f, 5.0f);
    CHECK_FALSE(a != b);
    CHECK(a != c);
}

TEST_CASE("Vector3 Subscript Operator")
{
    Vector3 v(1.0f, 2.0f, 3.0f);
    CHECK(v[0] == doctest::Approx(1.0f));
    CHECK(v[1] == doctest::Approx(2.0f));
    CHECK(v[2] == doctest::Approx(3.0f));
    v[0] = 5.0f;
    CHECK(v[0] == doctest::Approx(5.0f));
}

TEST_CASE("Vector3 Const Subscript Operator")
{
    const Vector3 v(1.0f, 2.0f, 3.0f);
    CHECK(v[0] == doctest::Approx(1.0f));
    CHECK(v[1] == doctest::Approx(2.0f));
    CHECK(v[2] == doctest::Approx(3.0f));
}

TEST_CASE("Vector3 Set Method")
{
    Vector3 v;
    v.Set(3.0f, 4.0f, 5.0f);
    CHECK(v.x == doctest::Approx(3.0f));
    CHECK(v.y == doctest::Approx(4.0f));
    CHECK(v.z == doctest::Approx(5.0f));
}

TEST_CASE("Vector3 Length")
{
    Vector3 v(2.0f, 3.0f, 6.0f);
    CHECK(v.Length() == doctest::Approx(7.0f));
}

TEST_CASE("Vector3 LengthSquared")
{
    Vector3 v(2.0f, 3.0f, 6.0f);
    CHECK(v.LengthSquared() == doctest::Approx(49.0f));
}

TEST_CASE("Vector3 Normalized")
{
    Vector3 v(2.0f, 3.0f, 6.0f);
    Vector3 result = v.Normalized();
    CHECK(result.x == doctest::Approx(2.0f / 7.0f));
    CHECK(result.y == doctest::Approx(3.0f / 7.0f));
    CHECK(result.z == doctest::Approx(6.0f / 7.0f));
}

TEST_CASE("Vector3 Normalized Zero Vector")
{
    Vector3 v(0.0f, 0.0f, 0.0f);
    Vector3 result = v.Normalized();
    CHECK(result.x == doctest::Approx(0.0f));
    CHECK(result.y == doctest::Approx(0.0f));
    CHECK(result.z == doctest::Approx(0.0f));
}

TEST_CASE("Vector3 Normalize In-Place")
{
    Vector3 v(2.0f, 3.0f, 6.0f);
    Vector3 &result = v.Normalize();
    CHECK(&result == &v);
    CHECK(v.x == doctest::Approx(2.0f / 7.0f));
    CHECK(v.y == doctest::Approx(3.0f / 7.0f));
    CHECK(v.z == doctest::Approx(6.0f / 7.0f));
}

TEST_CASE("Vector3 Normalize Zero Vector")
{
    Vector3 v(0.0f, 0.0f, 0.0f);
    v.Normalize();
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(0.0f));
    CHECK(v.z == doctest::Approx(0.0f));
}

TEST_CASE("Vector3 Dot Product Instance")
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    float result = a.Dot(b);
    CHECK(result == doctest::Approx(32.0f));
}

TEST_CASE("Vector3 Dot Product Static")
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    float result = Vector3::Dot(a, b);
    CHECK(result == doctest::Approx(32.0f));
}

TEST_CASE("Vector3 Cross Product Instance")
{
    Vector3 a(1.0f, 0.0f, 0.0f);
    Vector3 b(0.0f, 1.0f, 0.0f);
    Vector3 result = a.Cross(b);
    CHECK(result.x == doctest::Approx(0.0f));
    CHECK(result.y == doctest::Approx(0.0f));
    CHECK(result.z == doctest::Approx(1.0f));
}

TEST_CASE("Vector3 Cross Product Static")
{
    Vector3 a(1.0f, 0.0f, 0.0f);
    Vector3 b(0.0f, 1.0f, 0.0f);
    Vector3 result = Vector3::Cross(a, b);
    CHECK(result.x == doctest::Approx(0.0f));
    CHECK(result.y == doctest::Approx(0.0f));
    CHECK(result.z == doctest::Approx(1.0f));
}

TEST_CASE("Vector3 Cross Product Anti-Commutative")
{
    Vector3 a(1.0f, 0.0f, 0.0f);
    Vector3 b(0.0f, 1.0f, 0.0f);
    Vector3 result1 = a.Cross(b);
    Vector3 result2 = b.Cross(a);
    CHECK(result1.x == doctest::Approx(-result2.x));
    CHECK(result1.y == doctest::Approx(-result2.y));
    CHECK(result1.z == doctest::Approx(-result2.z));
}

TEST_CASE("Vector3 Distance Instance")
{
    Vector3 a(0.0f, 0.0f, 0.0f);
    Vector3 b(1.0f, 2.0f, 2.0f);
    float result = a.Distance(b);
    CHECK(result == doctest::Approx(3.0f));
}

TEST_CASE("Vector3 DistanceSquared Instance")
{
    Vector3 a(0.0f, 0.0f, 0.0f);
    Vector3 b(1.0f, 2.0f, 2.0f);
    float result = a.DistanceSquared(b);
    CHECK(result == doctest::Approx(9.0f));
}

TEST_CASE("Vector3 Distance Static")
{
    Vector3 a(0.0f, 0.0f, 0.0f);
    Vector3 b(1.0f, 2.0f, 2.0f);
    float result = Vector3::Distance(a, b);
    CHECK(result == doctest::Approx(3.0f));
}

TEST_CASE("Vector3 Lerp Instance")
{
    Vector3 a(0.0f, 0.0f, 0.0f);
    Vector3 b(10.0f, 20.0f, 30.0f);
    Vector3 result = a.Lerp(b, 0.5f);
    CHECK(result.x == doctest::Approx(5.0f));
    CHECK(result.y == doctest::Approx(10.0f));
    CHECK(result.z == doctest::Approx(15.0f));
}

TEST_CASE("Vector3 Lerp Static")
{
    Vector3 a(0.0f, 0.0f, 0.0f);
    Vector3 b(10.0f, 20.0f, 30.0f);
    Vector3 result = Vector3::Lerp(a, b, 0.5f);
    CHECK(result.x == doctest::Approx(5.0f));
    CHECK(result.y == doctest::Approx(10.0f));
    CHECK(result.z == doctest::Approx(15.0f));
}

TEST_CASE("Vector3 Lerp t=0")
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 result = Vector3::Lerp(a, b, 0.0f);
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(3.0f));
}

TEST_CASE("Vector3 Lerp t=1")
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 result = Vector3::Lerp(a, b, 1.0f);
    CHECK(result.x == doctest::Approx(4.0f));
    CHECK(result.y == doctest::Approx(5.0f));
    CHECK(result.z == doctest::Approx(6.0f));
}

TEST_CASE("Vector3 Reflect Instance")
{
    Vector3 v(1.0f, -1.0f, 0.0f);
    Vector3 normal(0.0f, 1.0f, 0.0f);
    Vector3 result = v.Reflect(normal);
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(1.0f));
    CHECK(result.z == doctest::Approx(0.0f));
}

TEST_CASE("Vector3 Reflect Static")
{
    Vector3 v(1.0f, -1.0f, 0.0f);
    Vector3 normal(0.0f, 1.0f, 0.0f);
    Vector3 result = Vector3::Reflect(v, normal);
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(1.0f));
    CHECK(result.z == doctest::Approx(0.0f));
}

TEST_CASE("Vector3 Min Instance")
{
    Vector3 a(1.0f, 5.0f, 3.0f);
    Vector3 b(3.0f, 2.0f, 4.0f);
    Vector3 result = a.Min(b);
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(3.0f));
}

TEST_CASE("Vector3 Max Instance")
{
    Vector3 a(1.0f, 5.0f, 3.0f);
    Vector3 b(3.0f, 2.0f, 4.0f);
    Vector3 result = a.Max(b);
    CHECK(result.x == doctest::Approx(3.0f));
    CHECK(result.y == doctest::Approx(5.0f));
    CHECK(result.z == doctest::Approx(4.0f));
}

TEST_CASE("Vector3 Min Static")
{
    Vector3 a(1.0f, 5.0f, 3.0f);
    Vector3 b(3.0f, 2.0f, 4.0f);
    Vector3 result = Vector3::Min(a, b);
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(3.0f));
}

TEST_CASE("Vector3 Max Static")
{
    Vector3 a(1.0f, 5.0f, 3.0f);
    Vector3 b(3.0f, 2.0f, 4.0f);
    Vector3 result = Vector3::Max(a, b);
    CHECK(result.x == doctest::Approx(3.0f));
    CHECK(result.y == doctest::Approx(5.0f));
    CHECK(result.z == doctest::Approx(4.0f));
}

TEST_CASE("Vector3 MaxComponent")
{
    Vector3 v(1.0f, 5.0f, 3.0f);
    CHECK(v.MaxComponent() == doctest::Approx(5.0f));
}

TEST_CASE("Vector3 MinComponent")
{
    Vector3 v(1.0f, 5.0f, 3.0f);
    CHECK(v.MinComponent() == doctest::Approx(1.0f));
}

TEST_CASE("Vector3 Zero Static")
{
    Vector3 v = Vector3::Zero();
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(0.0f));
    CHECK(v.z == doctest::Approx(0.0f));
}

TEST_CASE("Vector3 One Static")
{
    Vector3 v = Vector3::One();
    CHECK(v.x == doctest::Approx(1.0f));
    CHECK(v.y == doctest::Approx(1.0f));
    CHECK(v.z == doctest::Approx(1.0f));
}

TEST_CASE("Vector3 Up Static")
{
    Vector3 v = Vector3::Up();
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(1.0f));
    CHECK(v.z == doctest::Approx(0.0f));
}

TEST_CASE("Vector3 Down Static")
{
    Vector3 v = Vector3::Down();
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(-1.0f));
    CHECK(v.z == doctest::Approx(0.0f));
}

TEST_CASE("Vector3 Left Static")
{
    Vector3 v = Vector3::Left();
    CHECK(v.x == doctest::Approx(-1.0f));
    CHECK(v.y == doctest::Approx(0.0f));
    CHECK(v.z == doctest::Approx(0.0f));
}

TEST_CASE("Vector3 Right Static")
{
    Vector3 v = Vector3::Right();
    CHECK(v.x == doctest::Approx(1.0f));
    CHECK(v.y == doctest::Approx(0.0f));
    CHECK(v.z == doctest::Approx(0.0f));
}

TEST_CASE("Vector3 Forward Static")
{
    Vector3 v = Vector3::Forward();
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(0.0f));
    CHECK(v.z == doctest::Approx(1.0f));
}

TEST_CASE("Vector3 Back Static")
{
    Vector3 v = Vector3::Back();
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(0.0f));
    CHECK(v.z == doctest::Approx(-1.0f));
}

TEST_CASE("Vector4 Default Constructor")
{
    Vector4 v;
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(0.0f));
    CHECK(v.z == doctest::Approx(0.0f));
    CHECK(v.w == doctest::Approx(0.0f));
}

TEST_CASE("Vector4 Copy Constructor")
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(a);
    CHECK(b.x == doctest::Approx(1.0f));
    CHECK(b.y == doctest::Approx(2.0f));
    CHECK(b.z == doctest::Approx(3.0f));
    CHECK(b.w == doctest::Approx(4.0f));
}

TEST_CASE("Vector4 Parameterized Constructor")
{
    Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
    CHECK(v.x == doctest::Approx(1.0f));
    CHECK(v.y == doctest::Approx(2.0f));
    CHECK(v.z == doctest::Approx(3.0f));
    CHECK(v.w == doctest::Approx(4.0f));
}

TEST_CASE("Vector4 Constructor from Vector3 and w")
{
    Vector3 xyz(1.0f, 2.0f, 3.0f);
    Vector4 v(xyz, 5.0f);
    CHECK(v.x == doctest::Approx(1.0f));
    CHECK(v.y == doctest::Approx(2.0f));
    CHECK(v.z == doctest::Approx(3.0f));
    CHECK(v.w == doctest::Approx(5.0f));
}

TEST_CASE("Vector4 Constructor from Vector3 Only")
{
    Vector3 xyz(1.0f, 2.0f, 3.0f);
    Vector4 v(xyz);
    CHECK(v.x == doctest::Approx(1.0f));
    CHECK(v.y == doctest::Approx(2.0f));
    CHECK(v.z == doctest::Approx(3.0f));
    CHECK(v.w == doctest::Approx(1.0f));
}

TEST_CASE("Vector4 Addition Operator")
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);
    Vector4 result = a + b;
    CHECK(result.x == doctest::Approx(6.0f));
    CHECK(result.y == doctest::Approx(8.0f));
    CHECK(result.z == doctest::Approx(10.0f));
    CHECK(result.w == doctest::Approx(12.0f));
}

TEST_CASE("Vector4 Subtraction Operator")
{
    Vector4 a(5.0f, 6.0f, 7.0f, 8.0f);
    Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 result = a - b;
    CHECK(result.x == doctest::Approx(4.0f));
    CHECK(result.y == doctest::Approx(4.0f));
    CHECK(result.z == doctest::Approx(4.0f));
    CHECK(result.w == doctest::Approx(4.0f));
}

TEST_CASE("Vector4 Scalar Multiplication Operator")
{
    Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 result = v * 2.0f;
    CHECK(result.x == doctest::Approx(2.0f));
    CHECK(result.y == doctest::Approx(4.0f));
    CHECK(result.z == doctest::Approx(6.0f));
    CHECK(result.w == doctest::Approx(8.0f));
}

TEST_CASE("Vector4 Scalar Division Operator")
{
    Vector4 v(2.0f, 4.0f, 6.0f, 8.0f);
    Vector4 result = v / 2.0f;
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(3.0f));
    CHECK(result.w == doctest::Approx(4.0f));
}

TEST_CASE("Vector4 Component-wise Multiplication Operator")
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(2.0f, 3.0f, 4.0f, 5.0f);
    Vector4 result = a * b;
    CHECK(result.x == doctest::Approx(2.0f));
    CHECK(result.y == doctest::Approx(6.0f));
    CHECK(result.z == doctest::Approx(12.0f));
    CHECK(result.w == doctest::Approx(20.0f));
}

TEST_CASE("Vector4 Negation Operator")
{
    Vector4 v(1.0f, -2.0f, 3.0f, -4.0f);
    Vector4 result = -v;
    CHECK(result.x == doctest::Approx(-1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(-3.0f));
    CHECK(result.w == doctest::Approx(4.0f));
}

TEST_CASE("Vector4 Addition Assignment Operator")
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);
    a += b;
    CHECK(a.x == doctest::Approx(6.0f));
    CHECK(a.y == doctest::Approx(8.0f));
    CHECK(a.z == doctest::Approx(10.0f));
    CHECK(a.w == doctest::Approx(12.0f));
}

TEST_CASE("Vector4 Subtraction Assignment Operator")
{
    Vector4 a(5.0f, 6.0f, 7.0f, 8.0f);
    Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);
    a -= b;
    CHECK(a.x == doctest::Approx(4.0f));
    CHECK(a.y == doctest::Approx(4.0f));
    CHECK(a.z == doctest::Approx(4.0f));
    CHECK(a.w == doctest::Approx(4.0f));
}

TEST_CASE("Vector4 Scalar Multiplication Assignment Operator")
{
    Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
    v *= 2.0f;
    CHECK(v.x == doctest::Approx(2.0f));
    CHECK(v.y == doctest::Approx(4.0f));
    CHECK(v.z == doctest::Approx(6.0f));
    CHECK(v.w == doctest::Approx(8.0f));
}

TEST_CASE("Vector4 Scalar Division Assignment Operator")
{
    Vector4 v(2.0f, 4.0f, 6.0f, 8.0f);
    v /= 2.0f;
    CHECK(v.x == doctest::Approx(1.0f));
    CHECK(v.y == doctest::Approx(2.0f));
    CHECK(v.z == doctest::Approx(3.0f));
    CHECK(v.w == doctest::Approx(4.0f));
}

TEST_CASE("Vector4 Component-wise Multiplication Assignment Operator")
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(2.0f, 3.0f, 4.0f, 5.0f);
    a *= b;
    CHECK(a.x == doctest::Approx(2.0f));
    CHECK(a.y == doctest::Approx(6.0f));
    CHECK(a.z == doctest::Approx(12.0f));
    CHECK(a.w == doctest::Approx(20.0f));
}

TEST_CASE("Vector4 Equality Operator")
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 c(5.0f, 6.0f, 7.0f, 8.0f);
    CHECK(a == b);
    CHECK_FALSE(a == c);
}

TEST_CASE("Vector4 Inequality Operator")
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 c(5.0f, 6.0f, 7.0f, 8.0f);
    CHECK_FALSE(a != b);
    CHECK(a != c);
}

TEST_CASE("Vector4 Subscript Operator")
{
    Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
    CHECK(v[0] == doctest::Approx(1.0f));
    CHECK(v[1] == doctest::Approx(2.0f));
    CHECK(v[2] == doctest::Approx(3.0f));
    CHECK(v[3] == doctest::Approx(4.0f));
    v[0] = 5.0f;
    CHECK(v[0] == doctest::Approx(5.0f));
}

TEST_CASE("Vector4 Const Subscript Operator")
{
    const Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
    CHECK(v[0] == doctest::Approx(1.0f));
    CHECK(v[1] == doctest::Approx(2.0f));
    CHECK(v[2] == doctest::Approx(3.0f));
    CHECK(v[3] == doctest::Approx(4.0f));
}

TEST_CASE("Vector4 Set Method with Components")
{
    Vector4 v;
    v.Set(1.0f, 2.0f, 3.0f, 4.0f);
    CHECK(v.x == doctest::Approx(1.0f));
    CHECK(v.y == doctest::Approx(2.0f));
    CHECK(v.z == doctest::Approx(3.0f));
    CHECK(v.w == doctest::Approx(4.0f));
}

TEST_CASE("Vector4 Set Method with Vector3 and w")
{
    Vector4 v;
    Vector3 xyz(1.0f, 2.0f, 3.0f);
    v.Set(xyz, 5.0f);
    CHECK(v.x == doctest::Approx(1.0f));
    CHECK(v.y == doctest::Approx(2.0f));
    CHECK(v.z == doctest::Approx(3.0f));
    CHECK(v.w == doctest::Approx(5.0f));
}

TEST_CASE("Vector4 Length")
{
    Vector4 v(1.0f, 2.0f, 2.0f, 4.0f);
    CHECK(v.Length() == doctest::Approx(5.0f));
}

TEST_CASE("Vector4 LengthSquared")
{
    Vector4 v(1.0f, 2.0f, 2.0f, 4.0f);
    CHECK(v.LengthSquared() == doctest::Approx(25.0f));
}

TEST_CASE("Vector4 Normalized")
{
    Vector4 v(1.0f, 2.0f, 2.0f, 4.0f);
    Vector4 result = v.Normalized();
    CHECK(result.x == doctest::Approx(0.2f));
    CHECK(result.y == doctest::Approx(0.4f));
    CHECK(result.z == doctest::Approx(0.4f));
    CHECK(result.w == doctest::Approx(0.8f));
}

TEST_CASE("Vector4 Normalized Zero Vector")
{
    Vector4 v(0.0f, 0.0f, 0.0f, 0.0f);
    Vector4 result = v.Normalized();
    CHECK(result.x == doctest::Approx(0.0f));
    CHECK(result.y == doctest::Approx(0.0f));
    CHECK(result.z == doctest::Approx(0.0f));
    CHECK(result.w == doctest::Approx(0.0f));
}

TEST_CASE("Vector4 Normalize In-Place")
{
    Vector4 v(1.0f, 2.0f, 2.0f, 4.0f);
    Vector4 &result = v.Normalize();
    CHECK(&result == &v);
    CHECK(v.x == doctest::Approx(0.2f));
    CHECK(v.y == doctest::Approx(0.4f));
    CHECK(v.z == doctest::Approx(0.4f));
    CHECK(v.w == doctest::Approx(0.8f));
}

TEST_CASE("Vector4 Normalize Zero Vector")
{
    Vector4 v(0.0f, 0.0f, 0.0f, 0.0f);
    v.Normalize();
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(0.0f));
    CHECK(v.z == doctest::Approx(0.0f));
    CHECK(v.w == doctest::Approx(0.0f));
}

TEST_CASE("Vector4 Dot Product Instance")
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);
    float result = a.Dot(b);
    CHECK(result == doctest::Approx(70.0f));
}

TEST_CASE("Vector4 Dot Product Static")
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);
    float result = Vector4::Dot(a, b);
    CHECK(result == doctest::Approx(70.0f));
}

TEST_CASE("Vector4 Distance Instance")
{
    Vector4 a(0.0f, 0.0f, 0.0f, 0.0f);
    Vector4 b(1.0f, 2.0f, 2.0f, 4.0f);
    float result = a.Distance(b);
    CHECK(result == doctest::Approx(5.0f));
}

TEST_CASE("Vector4 DistanceSquared Instance")
{
    Vector4 a(0.0f, 0.0f, 0.0f, 0.0f);
    Vector4 b(1.0f, 2.0f, 2.0f, 4.0f);
    float result = a.DistanceSquared(b);
    CHECK(result == doctest::Approx(25.0f));
}

TEST_CASE("Vector4 Distance Static")
{
    Vector4 a(0.0f, 0.0f, 0.0f, 0.0f);
    Vector4 b(1.0f, 2.0f, 2.0f, 4.0f);
    float result = Vector4::Distance(a, b);
    CHECK(result == doctest::Approx(5.0f));
}

TEST_CASE("Vector4 Lerp Instance")
{
    Vector4 a(0.0f, 0.0f, 0.0f, 0.0f);
    Vector4 b(10.0f, 20.0f, 30.0f, 40.0f);
    Vector4 result = a.Lerp(b, 0.5f);
    CHECK(result.x == doctest::Approx(5.0f));
    CHECK(result.y == doctest::Approx(10.0f));
    CHECK(result.z == doctest::Approx(15.0f));
    CHECK(result.w == doctest::Approx(20.0f));
}

TEST_CASE("Vector4 Lerp Static")
{
    Vector4 a(0.0f, 0.0f, 0.0f, 0.0f);
    Vector4 b(10.0f, 20.0f, 30.0f, 40.0f);
    Vector4 result = Vector4::Lerp(a, b, 0.5f);
    CHECK(result.x == doctest::Approx(5.0f));
    CHECK(result.y == doctest::Approx(10.0f));
    CHECK(result.z == doctest::Approx(15.0f));
    CHECK(result.w == doctest::Approx(20.0f));
}

TEST_CASE("Vector4 Lerp t=0")
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);
    Vector4 result = Vector4::Lerp(a, b, 0.0f);
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(3.0f));
    CHECK(result.w == doctest::Approx(4.0f));
}

TEST_CASE("Vector4 Lerp t=1")
{
    Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);
    Vector4 result = Vector4::Lerp(a, b, 1.0f);
    CHECK(result.x == doctest::Approx(5.0f));
    CHECK(result.y == doctest::Approx(6.0f));
    CHECK(result.z == doctest::Approx(7.0f));
    CHECK(result.w == doctest::Approx(8.0f));
}

TEST_CASE("Vector4 ToVector3")
{
    Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
    Vector3 result = v.ToVector3();
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(3.0f));
}

TEST_CASE("Vector4 ToPosition with w != 0")
{
    Vector4 v(2.0f, 4.0f, 6.0f, 2.0f);
    Vector3 result = v.ToPosition();
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(3.0f));
}

TEST_CASE("Vector4 ToPosition with w == 0")
{
    Vector4 v(1.0f, 2.0f, 3.0f, 0.0f);
    Vector3 result = v.ToPosition();
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(3.0f));
}

TEST_CASE("Vector4 Min Instance")
{
    Vector4 a(1.0f, 5.0f, 3.0f, 8.0f);
    Vector4 b(3.0f, 2.0f, 4.0f, 6.0f);
    Vector4 result = a.Min(b);
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(3.0f));
    CHECK(result.w == doctest::Approx(6.0f));
}

TEST_CASE("Vector4 Max Instance")
{
    Vector4 a(1.0f, 5.0f, 3.0f, 8.0f);
    Vector4 b(3.0f, 2.0f, 4.0f, 6.0f);
    Vector4 result = a.Max(b);
    CHECK(result.x == doctest::Approx(3.0f));
    CHECK(result.y == doctest::Approx(5.0f));
    CHECK(result.z == doctest::Approx(4.0f));
    CHECK(result.w == doctest::Approx(8.0f));
}

TEST_CASE("Vector4 Min Static")
{
    Vector4 a(1.0f, 5.0f, 3.0f, 8.0f);
    Vector4 b(3.0f, 2.0f, 4.0f, 6.0f);
    Vector4 result = Vector4::Min(a, b);
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(3.0f));
    CHECK(result.w == doctest::Approx(6.0f));
}

TEST_CASE("Vector4 Max Static")
{
    Vector4 a(1.0f, 5.0f, 3.0f, 8.0f);
    Vector4 b(3.0f, 2.0f, 4.0f, 6.0f);
    Vector4 result = Vector4::Max(a, b);
    CHECK(result.x == doctest::Approx(3.0f));
    CHECK(result.y == doctest::Approx(5.0f));
    CHECK(result.z == doctest::Approx(4.0f));
    CHECK(result.w == doctest::Approx(8.0f));
}

TEST_CASE("Vector4 MaxComponent")
{
    Vector4 v(1.0f, 5.0f, 3.0f, 2.0f);
    CHECK(v.MaxComponent() == doctest::Approx(5.0f));
}

TEST_CASE("Vector4 MinComponent")
{
    Vector4 v(1.0f, 5.0f, 3.0f, 2.0f);
    CHECK(v.MinComponent() == doctest::Approx(1.0f));
}

TEST_CASE("Vector4 Zero Static")
{
    Vector4 v = Vector4::Zero();
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(0.0f));
    CHECK(v.z == doctest::Approx(0.0f));
    CHECK(v.w == doctest::Approx(0.0f));
}

TEST_CASE("Vector4 One Static")
{
    Vector4 v = Vector4::One();
    CHECK(v.x == doctest::Approx(1.0f));
    CHECK(v.y == doctest::Approx(1.0f));
    CHECK(v.z == doctest::Approx(1.0f));
    CHECK(v.w == doctest::Approx(1.0f));
}

TEST_CASE("Vector4 IdentityPoint Static")
{
    Vector4 v = Vector4::IdentityPoint();
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(0.0f));
    CHECK(v.z == doctest::Approx(0.0f));
    CHECK(v.w == doctest::Approx(1.0f));
}

TEST_CASE("Vector4 IdentityVector Static")
{
    Vector4 v = Vector4::IdentityVector();
    CHECK(v.x == doctest::Approx(0.0f));
    CHECK(v.y == doctest::Approx(0.0f));
    CHECK(v.z == doctest::Approx(0.0f));
    CHECK(v.w == doctest::Approx(0.0f));
}
