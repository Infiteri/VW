#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
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

TEST_CASE("Matrix4 Default Constructor is Identity")
{
    Matrix4 m;
    CHECK(m.data[0] == doctest::Approx(1.0f));
    CHECK(m.data[1] == doctest::Approx(0.0f));
    CHECK(m.data[2] == doctest::Approx(0.0f));
    CHECK(m.data[3] == doctest::Approx(0.0f));
    CHECK(m.data[4] == doctest::Approx(0.0f));
    CHECK(m.data[5] == doctest::Approx(1.0f));
    CHECK(m.data[6] == doctest::Approx(0.0f));
    CHECK(m.data[7] == doctest::Approx(0.0f));
    CHECK(m.data[8] == doctest::Approx(0.0f));
    CHECK(m.data[9] == doctest::Approx(0.0f));
    CHECK(m.data[10] == doctest::Approx(1.0f));
    CHECK(m.data[11] == doctest::Approx(0.0f));
    CHECK(m.data[12] == doctest::Approx(0.0f));
    CHECK(m.data[13] == doctest::Approx(0.0f));
    CHECK(m.data[14] == doctest::Approx(0.0f));
    CHECK(m.data[15] == doctest::Approx(1.0f));
}

TEST_CASE("Matrix4 Copy Constructor")
{
    float rawData[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    Matrix4 a(rawData);
    Matrix4 b(a);
    for (int i = 0; i < 16; i++)
        CHECK(b.data[i] == doctest::Approx(rawData[i]));
}

TEST_CASE("Matrix4 Float Pointer Constructor")
{
    float rawData[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    Matrix4 m(rawData);
    for (int i = 0; i < 16; i++)
        CHECK(m.data[i] == doctest::Approx(rawData[i]));
}

TEST_CASE("Matrix4 Perspective")
{
    Matrix4 m = Matrix4::Perspective(90.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    float f = 1.0f / tanf(90.0f * 0.5f);
    float aspect = 16.0f / 9.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    CHECK(m.data[0] == doctest::Approx(f / aspect));
    CHECK(m.data[5] == doctest::Approx(f));
    CHECK(m.data[10] == doctest::Approx((farPlane + nearPlane) / (nearPlane - farPlane)));
    CHECK(m.data[11] == doctest::Approx(-1.0f));
    CHECK(m.data[14] == doctest::Approx((2.0f * farPlane * nearPlane) / (nearPlane - farPlane)));
    CHECK(m.data[15] == doctest::Approx(0.0f));
    CHECK(m.data[2] == doctest::Approx(0.0f));
    CHECK(m.data[3] == doctest::Approx(0.0f));
    CHECK(m.data[8] == doctest::Approx(0.0f));
    CHECK(m.data[9] == doctest::Approx(0.0f));
    CHECK(m.data[12] == doctest::Approx(0.0f));
    CHECK(m.data[13] == doctest::Approx(0.0f));
}

TEST_CASE("Matrix4 Ortho")
{
    Matrix4 m = Matrix4::Ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
    float lr = 1.0f / (800.0f - 0.0f);
    float bt = 1.0f / (600.0f - 0.0f);
    float nf = 1.0f / (-1.0f - 1.0f);

    CHECK(m.data[0] == doctest::Approx(2.0f * lr));
    CHECK(m.data[5] == doctest::Approx(2.0f * bt));
    CHECK(m.data[10] == doctest::Approx(2.0f * nf));
    CHECK(m.data[12] == doctest::Approx(-(800.0f + 0.0f) * lr));
    CHECK(m.data[13] == doctest::Approx(-(600.0f + 0.0f) * bt));
    CHECK(m.data[14] == doctest::Approx((1.0f + (-1.0f)) * nf));
    // Off-diagonal should be identity
    CHECK(m.data[1] == doctest::Approx(0.0f));
    CHECK(m.data[2] == doctest::Approx(0.0f));
    CHECK(m.data[3] == doctest::Approx(0.0f));
    CHECK(m.data[4] == doctest::Approx(0.0f));
    CHECK(m.data[6] == doctest::Approx(0.0f));
    CHECK(m.data[7] == doctest::Approx(0.0f));
    CHECK(m.data[8] == doctest::Approx(0.0f));
    CHECK(m.data[9] == doctest::Approx(0.0f));
    CHECK(m.data[11] == doctest::Approx(0.0f));
    CHECK(m.data[15] == doctest::Approx(1.0f));
}

TEST_CASE("Matrix4 Multiply Identity")
{
    Matrix4 a;
    Matrix4 b;
    Matrix4 result = Matrix4::Multiply(a, b);
    for (int i = 0; i < 16; i++)
    {
        if (i == 0 || i == 5 || i == 10 || i == 15)
            CHECK(result.data[i] == doctest::Approx(1.0f));
        else
            CHECK(result.data[i] == doctest::Approx(0.0f));
    }
}

TEST_CASE("Matrix4 Multiply")
{
    // Column-major layout: data[row + col*4]
    float adata[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    float bdata[16] = {2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 1, 2, 3, 1};
    Matrix4 a(adata);
    Matrix4 b(bdata);

    Matrix4 result = a * b;

    // Standard column-major multiply: m[row,col] = sum_k(a[row,k] * b[k,col])
    CHECK(result.data[0] == doctest::Approx(1 * 2 + 5 * 0 + 9 * 0 + 13 * 0));   // 2
    CHECK(result.data[1] == doctest::Approx(2 * 2 + 6 * 0 + 10 * 0 + 14 * 0));  // 4
    CHECK(result.data[2] == doctest::Approx(3 * 2 + 7 * 0 + 11 * 0 + 15 * 0));  // 6
    CHECK(result.data[3] == doctest::Approx(4 * 2 + 8 * 0 + 12 * 0 + 16 * 0));  // 8
    CHECK(result.data[4] == doctest::Approx(1 * 0 + 5 * 2 + 9 * 0 + 13 * 0));   // 10
    CHECK(result.data[5] == doctest::Approx(2 * 0 + 6 * 2 + 10 * 0 + 14 * 0));  // 12
    CHECK(result.data[6] == doctest::Approx(3 * 0 + 7 * 2 + 11 * 0 + 15 * 0));  // 14
    CHECK(result.data[7] == doctest::Approx(4 * 0 + 8 * 2 + 12 * 0 + 16 * 0));  // 16
    CHECK(result.data[8] == doctest::Approx(1 * 0 + 5 * 0 + 9 * 2 + 13 * 0));   // 18
    CHECK(result.data[9] == doctest::Approx(2 * 0 + 6 * 0 + 10 * 2 + 14 * 0));  // 20
    CHECK(result.data[10] == doctest::Approx(3 * 0 + 7 * 0 + 11 * 2 + 15 * 0)); // 22
    CHECK(result.data[11] == doctest::Approx(4 * 0 + 8 * 0 + 12 * 2 + 16 * 0)); // 24
    CHECK(result.data[12] == doctest::Approx(1 * 1 + 5 * 2 + 9 * 3 + 13 * 1));  // 51
    CHECK(result.data[13] == doctest::Approx(2 * 1 + 6 * 2 + 10 * 3 + 14 * 1)); // 58
    CHECK(result.data[14] == doctest::Approx(3 * 1 + 7 * 2 + 11 * 3 + 15 * 1)); // 65
    CHECK(result.data[15] == doctest::Approx(4 * 1 + 8 * 2 + 12 * 3 + 16 * 1)); // 72
}

TEST_CASE("Matrix4 Multiply Associativity")
{
    float d1[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 2, 3, 1};
    float d2[16] = {2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1};
    float d3[16] = {1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    Matrix4 a(d1), b(d2), c(d3);

    Matrix4 left = (a * b) * c;
    Matrix4 right = a * (b * c);

    for (int i = 0; i < 16; i++)
        CHECK(left.data[i] == doctest::Approx(right.data[i]));
}

TEST_CASE("Matrix4 Invert Identity")
{
    Matrix4 identity;
    Matrix4 result = Matrix4::Invert(identity);
    for (int i = 0; i < 16; i++)
    {
        if (i == 0 || i == 5 || i == 10 || i == 15)
            CHECK(result.data[i] == doctest::Approx(1.0f));
        else
            CHECK(result.data[i] == doctest::Approx(0.0f));
    }
}

TEST_CASE("Matrix4 Invert Translation")
{
    Matrix4 t = Matrix4::Translate(Vector3(1.0f, 2.0f, 3.0f));
    Matrix4 inv = Matrix4::Invert(t);

    CHECK(inv.data[12] == doctest::Approx(-1.0f));
    CHECK(inv.data[13] == doctest::Approx(-2.0f));
    CHECK(inv.data[14] == doctest::Approx(-3.0f));
    CHECK(inv.data[15] == doctest::Approx(1.0f));
}

TEST_CASE("Matrix4 Invert Scale")
{
    Matrix4 s = Matrix4::Scale(Vector3(2.0f, 3.0f, 4.0f));
    Matrix4 inv = Matrix4::Invert(s);

    CHECK(inv.data[0] == doctest::Approx(0.5f));
    CHECK(inv.data[5] == doctest::Approx(1.0f / 3.0f));
    CHECK(inv.data[10] == doctest::Approx(0.25f));
}

TEST_CASE("Matrix4 Invert Then Multiply is Identity")
{
    float rawData[16] = {1, 0, 0, 2, 0, 3, 0, 4, 0, 0, 5, 6, 0, 0, 0, 1};
    Matrix4 m(rawData);
    Matrix4 inv = Matrix4::Invert(m);
    Matrix4 result = Matrix4::Multiply(m, inv);

    for (int i = 0; i < 16; i++)
    {
        if (i == 0 || i == 5 || i == 10 || i == 15)
            CHECK(result.data[i] == doctest::Approx(1.0f).epsilon(0.001));
        else
            CHECK(result.data[i] == doctest::Approx(0.0f).epsilon(0.001));
    }
}

TEST_CASE("Matrix4 Translate")
{
    Matrix4 m = Matrix4::Translate(Vector3(1.0f, 2.0f, 3.0f));

    CHECK(m.data[0] == doctest::Approx(1.0f));
    CHECK(m.data[5] == doctest::Approx(1.0f));
    CHECK(m.data[10] == doctest::Approx(1.0f));
    CHECK(m.data[15] == doctest::Approx(1.0f));
    CHECK(m.data[12] == doctest::Approx(1.0f));
    CHECK(m.data[13] == doctest::Approx(2.0f));
    CHECK(m.data[14] == doctest::Approx(3.0f));
    // All others zero
    CHECK(m.data[1] == doctest::Approx(0.0f));
    CHECK(m.data[2] == doctest::Approx(0.0f));
    CHECK(m.data[3] == doctest::Approx(0.0f));
    CHECK(m.data[4] == doctest::Approx(0.0f));
    CHECK(m.data[6] == doctest::Approx(0.0f));
    CHECK(m.data[7] == doctest::Approx(0.0f));
    CHECK(m.data[8] == doctest::Approx(0.0f));
    CHECK(m.data[9] == doctest::Approx(0.0f));
    CHECK(m.data[11] == doctest::Approx(0.0f));
}

TEST_CASE("Matrix4 RotateX")
{
    float angle = 3.14159265f / 2.0f; // 90 degrees
    Matrix4 m = Matrix4::RotateX(angle);

    CHECK(m.data[0] == doctest::Approx(1.0f));
    CHECK(m.data[5] == doctest::Approx(0.0f));
    CHECK(m.data[6] == doctest::Approx(1.0f));
    CHECK(m.data[9] == doctest::Approx(-1.0f));
    CHECK(m.data[10] == doctest::Approx(0.0f));
    CHECK(m.data[15] == doctest::Approx(1.0f));
    CHECK(m.data[12] == doctest::Approx(0.0f));
    CHECK(m.data[13] == doctest::Approx(0.0f));
    CHECK(m.data[14] == doctest::Approx(0.0f));
}

TEST_CASE("Matrix4 RotateY")
{
    float angle = 3.14159265f / 2.0f; // 90 degrees
    Matrix4 m = Matrix4::RotateY(angle);

    CHECK(m.data[0] == doctest::Approx(0.0f));
    CHECK(m.data[2] == doctest::Approx(1.0f));
    CHECK(m.data[8] == doctest::Approx(-1.0f));
    CHECK(m.data[10] == doctest::Approx(0.0f));
    CHECK(m.data[5] == doctest::Approx(1.0f));
    CHECK(m.data[15] == doctest::Approx(1.0f));
    CHECK(m.data[12] == doctest::Approx(0.0f));
    CHECK(m.data[13] == doctest::Approx(0.0f));
    CHECK(m.data[14] == doctest::Approx(0.0f));
}

TEST_CASE("Matrix4 RotateZ")
{
    float angle = 3.14159265f / 2.0f; // 90 degrees
    Matrix4 m = Matrix4::RotateZ(angle);

    CHECK(m.data[0] == doctest::Approx(0.0f));
    CHECK(m.data[1] == doctest::Approx(1.0f));
    CHECK(m.data[4] == doctest::Approx(-1.0f));
    CHECK(m.data[5] == doctest::Approx(0.0f));
    CHECK(m.data[10] == doctest::Approx(1.0f));
    CHECK(m.data[15] == doctest::Approx(1.0f));
    CHECK(m.data[12] == doctest::Approx(0.0f));
    CHECK(m.data[13] == doctest::Approx(0.0f));
    CHECK(m.data[14] == doctest::Approx(0.0f));
}

TEST_CASE("Matrix4 RotateX Zero Angle is Identity")
{
    Matrix4 m = Matrix4::RotateX(0.0f);
    CHECK(m.data[0] == doctest::Approx(1.0f));
    CHECK(m.data[5] == doctest::Approx(1.0f));
    CHECK(m.data[10] == doctest::Approx(1.0f));
    CHECK(m.data[15] == doctest::Approx(1.0f));
}

TEST_CASE("Matrix4 RotateY Zero Angle is Identity")
{
    Matrix4 m = Matrix4::RotateY(0.0f);
    CHECK(m.data[0] == doctest::Approx(1.0f));
    CHECK(m.data[10] == doctest::Approx(1.0f));
    CHECK(m.data[5] == doctest::Approx(1.0f));
    CHECK(m.data[15] == doctest::Approx(1.0f));
}

TEST_CASE("Matrix4 RotateZ Zero Angle is Identity")
{
    Matrix4 m = Matrix4::RotateZ(0.0f);
    CHECK(m.data[0] == doctest::Approx(1.0f));
    CHECK(m.data[5] == doctest::Approx(1.0f));
    CHECK(m.data[10] == doctest::Approx(1.0f));
    CHECK(m.data[15] == doctest::Approx(1.0f));
}

TEST_CASE("Matrix4 RotateZYX Zero Vector is Identity")
{
    Matrix4 m = Matrix4::RotateZYX(Vector3(0.0f, 0.0f, 0.0f));
    CHECK(m.data[0] == doctest::Approx(1.0f));
    CHECK(m.data[5] == doctest::Approx(1.0f));
    CHECK(m.data[10] == doctest::Approx(1.0f));
    CHECK(m.data[15] == doctest::Approx(1.0f));
}

TEST_CASE("Matrix4 RotateXYZ Zero Vector is Identity")
{
    Matrix4 m = Matrix4::RotateXYZ(Vector3(0.0f, 0.0f, 0.0f));
    CHECK(m.data[0] == doctest::Approx(1.0f));
    CHECK(m.data[5] == doctest::Approx(1.0f));
    CHECK(m.data[10] == doctest::Approx(1.0f));
    CHECK(m.data[15] == doctest::Approx(1.0f));
}

TEST_CASE("Matrix4 Scale")
{
    Matrix4 m = Matrix4::Scale(Vector3(2.0f, 3.0f, 4.0f));

    CHECK(m.data[0] == doctest::Approx(2.0f));
    CHECK(m.data[5] == doctest::Approx(3.0f));
    CHECK(m.data[10] == doctest::Approx(4.0f));
    CHECK(m.data[15] == doctest::Approx(1.0f));
    // All off-diagonal zero
    CHECK(m.data[1] == doctest::Approx(0.0f));
    CHECK(m.data[2] == doctest::Approx(0.0f));
    CHECK(m.data[3] == doctest::Approx(0.0f));
    CHECK(m.data[4] == doctest::Approx(0.0f));
    CHECK(m.data[6] == doctest::Approx(0.0f));
    CHECK(m.data[7] == doctest::Approx(0.0f));
    CHECK(m.data[8] == doctest::Approx(0.0f));
    CHECK(m.data[9] == doctest::Approx(0.0f));
    CHECK(m.data[11] == doctest::Approx(0.0f));
    CHECK(m.data[12] == doctest::Approx(0.0f));
    CHECK(m.data[13] == doctest::Approx(0.0f));
    CHECK(m.data[14] == doctest::Approx(0.0f));
}

TEST_CASE("Matrix4 Scale Uniform")
{
    Matrix4 m = Matrix4::Scale(Vector3(5.0f, 5.0f, 5.0f));
    CHECK(m.data[0] == doctest::Approx(5.0f));
    CHECK(m.data[5] == doctest::Approx(5.0f));
    CHECK(m.data[10] == doctest::Approx(5.0f));
}

TEST_CASE("Matrix4 Forward")
{
    Matrix4 m = Matrix4::RotateY(3.14159265f / 4.0f); // 45 degrees
    Vector3 fwd = Matrix4::Forward(m);

    CHECK(fwd.x == doctest::Approx(-m.data[8]));
    CHECK(fwd.y == doctest::Approx(-m.data[9]));
    CHECK(fwd.z == doctest::Approx(-m.data[10]));
}

TEST_CASE("Matrix4 Forward Identity Matrix")
{
    Matrix4 m;
    Vector3 fwd = Matrix4::Forward(m);
    CHECK(fwd.x == doctest::Approx(0.0f));
    CHECK(fwd.y == doctest::Approx(0.0f));
    CHECK(fwd.z == doctest::Approx(-1.0f));
}

TEST_CASE("Matrix4 Right")
{
    Matrix4 m = Matrix4::RotateY(3.14159265f / 4.0f);
    Vector3 right = Matrix4::Right(m);

    CHECK(right.x == doctest::Approx(m.data[0]));
    CHECK(right.y == doctest::Approx(m.data[1]));
    CHECK(right.z == doctest::Approx(m.data[2]));
}

TEST_CASE("Matrix4 Right Identity Matrix")
{
    Matrix4 m;
    Vector3 right = Matrix4::Right(m);
    CHECK(right.x == doctest::Approx(1.0f));
    CHECK(right.y == doctest::Approx(0.0f));
    CHECK(right.z == doctest::Approx(0.0f));
}

TEST_CASE("Matrix4 Transpose Identity")
{
    Matrix4 m;
    Matrix4 result = Matrix4::Transpose(m);
    for (int i = 0; i < 16; i++)
    {
        if (i == 0 || i == 5 || i == 10 || i == 15)
            CHECK(result.data[i] == doctest::Approx(1.0f));
        else
            CHECK(result.data[i] == doctest::Approx(0.0f));
    }
}

TEST_CASE("Matrix4 Transpose")
{
    float rawData[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    Matrix4 m(rawData);
    Matrix4 result = Matrix4::Transpose(m);

    CHECK(result.data[0] == doctest::Approx(1.0f));
    CHECK(result.data[1] == doctest::Approx(5.0f));
    CHECK(result.data[2] == doctest::Approx(9.0f));
    CHECK(result.data[3] == doctest::Approx(13.0f));
    CHECK(result.data[4] == doctest::Approx(2.0f));
    CHECK(result.data[5] == doctest::Approx(6.0f));
    CHECK(result.data[6] == doctest::Approx(10.0f));
    CHECK(result.data[7] == doctest::Approx(14.0f));
    CHECK(result.data[8] == doctest::Approx(3.0f));
    CHECK(result.data[9] == doctest::Approx(7.0f));
    CHECK(result.data[10] == doctest::Approx(11.0f));
    CHECK(result.data[11] == doctest::Approx(15.0f));
    CHECK(result.data[12] == doctest::Approx(4.0f));
    CHECK(result.data[13] == doctest::Approx(8.0f));
    CHECK(result.data[14] == doctest::Approx(12.0f));
    CHECK(result.data[15] == doctest::Approx(16.0f));
}

TEST_CASE("Matrix4 Transpose Twice Returns Original")
{
    float rawData[16] = {1, 0, 0, 5, 0, 2, 0, 6, 0, 0, 3, 7, 0, 0, 0, 1};
    Matrix4 m(rawData);
    Matrix4 result = Matrix4::Transpose(Matrix4::Transpose(m));

    for (int i = 0; i < 16; i++)
        CHECK(result.data[i] == doctest::Approx(rawData[i]));
}

TEST_CASE("Matrix4 Composition Translation Then Rotation")
{
    Matrix4 t = Matrix4::Translate(Vector3(1.0f, 0.0f, 0.0f));
    Matrix4 r = Matrix4::RotateY(3.14159265f); // 180 degrees
    Matrix4 result = r * t;

    // After translation then rotation, position should be flipped
    CHECK(result.data[12] == doctest::Approx(-1.0f));
}

TEST_CASE("Matrix4 Operator Multiply Equivalence")
{
    float d1[16] = {1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 1, 2, 3, 1};
    float d2[16] = {2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    Matrix4 a(d1), b(d2);

    Matrix4 viaOp = a * b;
    Matrix4 viaFn = Matrix4::Multiply(a, b);

    for (int i = 0; i < 16; i++)
        CHECK(viaOp.data[i] == doctest::Approx(viaFn.data[i]));
}

// ============================================================================
// Quaternion Tests
// ============================================================================

TEST_CASE("Quaternion Default Constructor is Identity")
{
    Quaternion q;
    CHECK(q.x == doctest::Approx(0.0f));
    CHECK(q.y == doctest::Approx(0.0f));
    CHECK(q.z == doctest::Approx(0.0f));
    CHECK(q.w == doctest::Approx(1.0f));
}

TEST_CASE("Quaternion Parameterized Constructor")
{
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    CHECK(q.x == doctest::Approx(1.0f));
    CHECK(q.y == doctest::Approx(2.0f));
    CHECK(q.z == doctest::Approx(3.0f));
    CHECK(q.w == doctest::Approx(4.0f));
}

TEST_CASE("Quaternion Copy Constructor")
{
    Quaternion a(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion b(a);
    CHECK(b.x == doctest::Approx(1.0f));
    CHECK(b.y == doctest::Approx(2.0f));
    CHECK(b.z == doctest::Approx(3.0f));
    CHECK(b.w == doctest::Approx(4.0f));
}

TEST_CASE("Quaternion Axis-Angle Constructor")
{
    // 180 degrees around X axis
    Quaternion q(Vector3(1.0f, 0.0f, 0.0f), 3.14159265f);
    float half = 3.14159265f / 2.0f;
    CHECK(q.x == doctest::Approx(std::sin(half)));
    CHECK(q.y == doctest::Approx(0.0f));
    CHECK(q.z == doctest::Approx(0.0f));
    CHECK(q.w == doctest::Approx(std::cos(half)));
}

TEST_CASE("Quaternion Axis-Angle 90 degrees around Y")
{
    float angle = 3.14159265f / 2.0f;
    Quaternion q(Vector3(0.0f, 1.0f, 0.0f), angle);
    float half = angle / 2.0f;
    float s = std::sin(half);
    float c = std::cos(half);
    CHECK(q.x == doctest::Approx(0.0f));
    CHECK(q.y == doctest::Approx(s));
    CHECK(q.z == doctest::Approx(0.0f));
    CHECK(q.w == doctest::Approx(c));
}

TEST_CASE("Quaternion Set Components")
{
    Quaternion q;
    q.Set(1.0f, 2.0f, 3.0f, 4.0f);
    CHECK(q.x == doctest::Approx(1.0f));
    CHECK(q.y == doctest::Approx(2.0f));
    CHECK(q.z == doctest::Approx(3.0f));
    CHECK(q.w == doctest::Approx(4.0f));
}

TEST_CASE("Quaternion Set From Other")
{
    Quaternion a(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion b;
    b.Set(a);
    CHECK(b.x == doctest::Approx(1.0f));
    CHECK(b.y == doctest::Approx(2.0f));
    CHECK(b.z == doctest::Approx(3.0f));
    CHECK(b.w == doctest::Approx(4.0f));
}

TEST_CASE("Quaternion Multiplication Operator")
{
    Quaternion a(0.0f, 0.0f, 0.0f, 1.0f); // identity
    Quaternion b(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion result = a * b;
    // identity * q = q
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(3.0f));
    CHECK(result.w == doctest::Approx(4.0f));
}

TEST_CASE("Quaternion Multiplication Non-Identity")
{
    // q1 = rotation 90° around X, q2 = rotation 90° around Y
    float a = 3.14159265f / 2.0f;
    float s = std::sin(a / 2.0f); // ~0.7071
    float c = std::cos(a / 2.0f); // ~0.7071
    Quaternion qx(s, 0.0f, 0.0f, c);
    Quaternion qy(0.0f, s, 0.0f, c);

    Quaternion result = qx * qy;

    // w = c*c - s*0 - 0*s - 0*0 = c^2
    CHECK(result.w == doctest::Approx(c * c));
    // x = c*0 + s*c + 0*0 - 0*s = s*c
    CHECK(result.x == doctest::Approx(s * c));
    // y = c*s - s*0 + 0*c + 0*0 = s*c
    CHECK(result.y == doctest::Approx(s * c));
    // z = c*0 + s*s - 0*0 + 0*c = s^2
    CHECK(result.z == doctest::Approx(s * s));
}

TEST_CASE("Quaternion Multiplication Associativity")
{
    Quaternion a(0.1f, 0.2f, 0.3f, 0.9f);
    Quaternion b(0.4f, 0.5f, 0.6f, 0.7f);
    Quaternion c(0.8f, 0.9f, 0.1f, 0.2f);

    Quaternion left = (a * b) * c;
    Quaternion right = a * (b * c);

    CHECK(left.x == doctest::Approx(right.x));
    CHECK(left.y == doctest::Approx(right.y));
    CHECK(left.z == doctest::Approx(right.z));
    CHECK(left.w == doctest::Approx(right.w));
}

TEST_CASE("Quaternion Multiplication Assignment Operator")
{
    Quaternion a(0.0f, 0.0f, 0.0f, 1.0f);
    Quaternion b(1.0f, 2.0f, 3.0f, 4.0f);
    a *= b;
    CHECK(a.x == doctest::Approx(1.0f));
    CHECK(a.y == doctest::Approx(2.0f));
    CHECK(a.z == doctest::Approx(3.0f));
    CHECK(a.w == doctest::Approx(4.0f));
}

TEST_CASE("Quaternion Scalar Multiplication Assignment")
{
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    q *= 2.0f;
    CHECK(q.x == doctest::Approx(2.0f));
    CHECK(q.y == doctest::Approx(4.0f));
    CHECK(q.z == doctest::Approx(6.0f));
    CHECK(q.w == doctest::Approx(8.0f));
}

TEST_CASE("Quaternion Scalar Division Assignment")
{
    Quaternion q(2.0f, 4.0f, 6.0f, 8.0f);
    q /= 2.0f;
    CHECK(q.x == doctest::Approx(1.0f));
    CHECK(q.y == doctest::Approx(2.0f));
    CHECK(q.z == doctest::Approx(3.0f));
    CHECK(q.w == doctest::Approx(4.0f));
}

TEST_CASE("Quaternion Addition Assignment")
{
    Quaternion a(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion b(5.0f, 6.0f, 7.0f, 8.0f);
    a += b;
    CHECK(a.x == doctest::Approx(6.0f));
    CHECK(a.y == doctest::Approx(8.0f));
    CHECK(a.z == doctest::Approx(10.0f));
    CHECK(a.w == doctest::Approx(12.0f));
}

TEST_CASE("Quaternion Subtraction Assignment")
{
    Quaternion a(5.0f, 6.0f, 7.0f, 8.0f);
    Quaternion b(1.0f, 2.0f, 3.0f, 4.0f);
    a -= b;
    CHECK(a.x == doctest::Approx(4.0f));
    CHECK(a.y == doctest::Approx(4.0f));
    CHECK(a.z == doctest::Approx(4.0f));
    CHECK(a.w == doctest::Approx(4.0f));
}

TEST_CASE("Quaternion Equality Operator")
{
    Quaternion a(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion b(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion c(5.0f, 6.0f, 7.0f, 8.0f);
    CHECK(a == b);
    CHECK_FALSE(a == c);
}

TEST_CASE("Quaternion Inequality Operator")
{
    Quaternion a(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion b(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion c(5.0f, 6.0f, 7.0f, 8.0f);
    CHECK_FALSE(a != b);
    CHECK(a != c);
}

TEST_CASE("Quaternion Length")
{
    Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
    CHECK(q.Length() == doctest::Approx(1.0f));
}

TEST_CASE("Quaternion Length Non-Unit")
{
    Quaternion q(3.0f, 0.0f, 0.0f, 4.0f);
    CHECK(q.Length() == doctest::Approx(5.0f));
}

TEST_CASE("Quaternion Length Squared")
{
    Quaternion q(3.0f, 0.0f, 0.0f, 4.0f);
    CHECK(q.LengthSquared() == doctest::Approx(25.0f));
}

TEST_CASE("Quaternion Normalize")
{
    Quaternion q(3.0f, 0.0f, 0.0f, 4.0f);
    Quaternion result = q.Normalized();
    CHECK(result.x == doctest::Approx(0.6f));
    CHECK(result.y == doctest::Approx(0.0f));
    CHECK(result.z == doctest::Approx(0.0f));
    CHECK(result.w == doctest::Approx(0.8f));
}

TEST_CASE("Quaternion Normalize In-Place")
{
    Quaternion q(3.0f, 0.0f, 0.0f, 4.0f);
    q.Normalize();
    CHECK(q.x == doctest::Approx(0.6f));
    CHECK(q.w == doctest::Approx(0.8f));
}

TEST_CASE("Quaternion Normalize Zero Quaternion")
{
    Quaternion q(0.0f, 0.0f, 0.0f, 0.0f);
    q.Normalize();
    CHECK(q.x == doctest::Approx(1.0f));
    CHECK(q.y == doctest::Approx(0.0f));
    CHECK(q.z == doctest::Approx(0.0f));
    CHECK(q.w == doctest::Approx(0.0f));
}

TEST_CASE("Quaternion Conjugate")
{
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion result = q.Conjugated();
    CHECK(result.x == doctest::Approx(-1.0f));
    CHECK(result.y == doctest::Approx(-2.0f));
    CHECK(result.z == doctest::Approx(-3.0f));
    CHECK(result.w == doctest::Approx(4.0f));
}

TEST_CASE("Quaternion Conjugate In-Place")
{
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    q.Conjugate();
    CHECK(q.x == doctest::Approx(-1.0f));
    CHECK(q.y == doctest::Approx(-2.0f));
    CHECK(q.z == doctest::Approx(-3.0f));
    CHECK(q.w == doctest::Approx(4.0f));
}

TEST_CASE("Quaternion Dot Product")
{
    Quaternion a(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion b(0.0f, 1.0f, 0.0f, 0.0f);
    CHECK(a.Dot(b) == doctest::Approx(0.0f));
}

TEST_CASE("Quaternion Dot Product Non-Zero")
{
    Quaternion a(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion b(5.0f, 6.0f, 7.0f, 8.0f);
    CHECK(a.Dot(b) == doctest::Approx(70.0f));
}

TEST_CASE("Quaternion RotateVector Identity")
{
    Quaternion q = Quaternion::Identity();
    Vector3 v(1.0f, 2.0f, 3.0f);
    Vector3 result = q.RotateVector(v);
    CHECK(result.x == doctest::Approx(1.0f));
    CHECK(result.y == doctest::Approx(2.0f));
    CHECK(result.z == doctest::Approx(3.0f));
}

TEST_CASE("Quaternion RotateVector 180 around X")
{
    // 180 degrees around X should flip Y and Z
    Quaternion q(Vector3(1.0f, 0.0f, 0.0f), 3.14159265f);
    Vector3 result = q.RotateVector(Vector3(0.0f, 1.0f, 0.0f));
    CHECK(result.x == doctest::Approx(0.0f).epsilon(0.001));
    CHECK(result.y == doctest::Approx(-1.0f).epsilon(0.001));
    CHECK(result.z == doctest::Approx(0.0f).epsilon(0.001));
}

TEST_CASE("Quaternion RotateVector 180 around Z")
{
    Quaternion q(Vector3(0.0f, 0.0f, 1.0f), 3.14159265f);
    Vector3 result = q.RotateVector(Vector3(1.0f, 0.0f, 0.0f));
    CHECK(result.x == doctest::Approx(-1.0f).epsilon(0.001));
    CHECK(result.y == doctest::Approx(0.0f).epsilon(0.001));
    CHECK(result.z == doctest::Approx(0.0f).epsilon(0.001));
}

TEST_CASE("Quaternion GetMatrix Identity")
{
    Quaternion q = Quaternion::Identity();
    Matrix4 m = q.GetMatrix();

    CHECK(m.data[0] == doctest::Approx(1.0f));
    CHECK(m.data[5] == doctest::Approx(1.0f));
    CHECK(m.data[10] == doctest::Approx(1.0f));
    CHECK(m.data[15] == doctest::Approx(1.0f));
    // All off-diagonal should be 0
    for (int i = 0; i < 16; i++)
    {
        if (i != 0 && i != 5 && i != 10 && i != 15)
            CHECK(m.data[i] == doctest::Approx(0.0f));
    }
}

TEST_CASE("Quaternion GetMatrix 90 around Y")
{
    float angle = 3.14159265f / 2.0f;
    Quaternion q(Vector3(0.0f, 1.0f, 0.0f), angle);
    Matrix4 m = q.GetMatrix();

    // Verify by rotating a vector: (1,0,0) rotated 90° around Y → (0,0,-1)
    // The matrix rotation should match the quaternion rotation
    Vector3 vResult = q.RotateVector(Vector3(1.0f, 0.0f, 0.0f));
    // Extract column 0 of matrix (the X basis vector)
    Vector3 mResult(m.data[0], m.data[1], m.data[2]);

    CHECK(mResult.x == doctest::Approx(vResult.x).epsilon(0.001));
    CHECK(mResult.y == doctest::Approx(vResult.y).epsilon(0.001));
    CHECK(mResult.z == doctest::Approx(vResult.z).epsilon(0.001));
}

TEST_CASE("Quaternion GetMatrix 90 around X")
{
    float angle = 3.14159265f / 2.0f;
    Quaternion q(Vector3(1.0f, 0.0f, 0.0f), angle);
    Matrix4 m = q.GetMatrix();

    // Extract column 1 of matrix (the Y basis vector)
    Vector3 mResult(m.data[4], m.data[5], m.data[6]);
    Vector3 vResult = q.RotateVector(Vector3(0.0f, 1.0f, 0.0f));

    CHECK(mResult.x == doctest::Approx(vResult.x).epsilon(0.001));
    CHECK(mResult.y == doctest::Approx(vResult.y).epsilon(0.001));
    CHECK(mResult.z == doctest::Approx(vResult.z).epsilon(0.001));
}

TEST_CASE("Quaternion Static Identity")
{
    Quaternion q = Quaternion::Identity();
    CHECK(q.x == doctest::Approx(0.0f));
    CHECK(q.y == doctest::Approx(0.0f));
    CHECK(q.z == doctest::Approx(0.0f));
    CHECK(q.w == doctest::Approx(1.0f));
}

TEST_CASE("Quaternion FromEulerAngles Zero")
{
    Quaternion q = Quaternion::FromEulerAngles(0.0f, 0.0f, 0.0f);
    CHECK(q.x == doctest::Approx(0.0f));
    CHECK(q.y == doctest::Approx(0.0f));
    CHECK(q.z == doctest::Approx(0.0f));
    CHECK(q.w == doctest::Approx(1.0f));
}

TEST_CASE("Quaternion FromEulerAngles 180 Yaw")
{
    Quaternion q = Quaternion::FromEulerAngles(0.0f, 3.14159265f, 0.0f);
    // 180 yaw = half angle ~90, so cos(90)=0, sin(90)=1
    CHECK(q.w == doctest::Approx(0.0f).epsilon(0.001));
    CHECK(q.y == doctest::Approx(1.0f).epsilon(0.001));
    CHECK(q.x == doctest::Approx(0.0f).epsilon(0.001));
    CHECK(q.z == doctest::Approx(0.0f).epsilon(0.001));
}

TEST_CASE("Quaternion RotateVector 90 Y rotates X to Z")
{
    float angle = 3.14159265f / 2.0f;
    Quaternion q(Vector3(0.0f, 1.0f, 0.0f), angle);
    // Rotating (1,0,0) by 90° around Y should give (0,0,-1)
    Vector3 result = q.RotateVector(Vector3(1.0f, 0.0f, 0.0f));
    CHECK(result.x == doctest::Approx(0.0f).epsilon(0.001));
    CHECK(result.y == doctest::Approx(0.0f).epsilon(0.001));
    CHECK(result.z == doctest::Approx(-1.0f).epsilon(0.001));
}

TEST_CASE("Quaternion Compose Two Rotations")
{
    // Rotate 90° around X, then 90° around Y
    float a = 3.14159265f / 2.0f;
    Quaternion rx = Quaternion(Vector3(1.0f, 0.0f, 0.0f), a);
    Quaternion ry = Quaternion(Vector3(0.0f, 1.0f, 0.0f), a);
    Quaternion combined = ry * rx;

    // Apply to vector (1, 0, 0):
    // First 90° X rotation: (1,0,0) -> (1,0,0)
    // Then 90° Y rotation: (1,0,0) -> (0,0,-1)
    Vector3 result = combined.RotateVector(Vector3(1.0f, 0.0f, 0.0f));
    CHECK(result.x == doctest::Approx(0.0f).epsilon(0.001));
    CHECK(result.y == doctest::Approx(0.0f).epsilon(0.001));
    CHECK(result.z == doctest::Approx(-1.0f).epsilon(0.001));
}

TEST_CASE("Quaternion Normalize Preserves Direction")
{
    Quaternion q(2.0f, 2.0f, 2.0f, 2.0f);
    Quaternion n = q.Normalized();
    float len = n.Length();
    CHECK(len == doctest::Approx(1.0f));
}

TEST_CASE("Quaternion Identity GetMatrix Equals Identity Matrix")
{
    Quaternion q = Quaternion::Identity();
    Matrix4 qm = q.GetMatrix();
    Matrix4 im; // identity matrix

    for (int i = 0; i < 16; i++)
        CHECK(qm.data[i] == doctest::Approx(im.data[i]));
}
