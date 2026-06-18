#include "Transform.h"

#include "Math/Math.h"
#include "Quaternion.h"

namespace VW
{

    Transform::Transform()
    {
        Position = {0, 0, 0};
        Rotation = {0, 0, 0};
        Scale = {1, 1, 1};
    }

    Transform::Transform(const Vector3 &position, const Vector3 &rotation, const Vector3 &scale)
    {
        Position = position;
        Rotation = rotation;
        Scale = scale;
    }

    Transform::Transform(const Transform &other)
    {
        Position = other.Position;
        Rotation = other.Rotation;
        Scale = other.Scale;
    }

    Matrix4 Transform::GetMatrix() const
    {
        Quaternion quatX{{1, 0, 0}, Rotation.x * VW_DEG_TO_RAD};
        Quaternion quatY{{0, 1, 0}, Rotation.y * VW_DEG_TO_RAD};
        Quaternion quatZ{{0, 0, 1}, Rotation.z * VW_DEG_TO_RAD};
        Quaternion quat = quatX * quatY * quatZ;

        Matrix4 translationMatrix = Matrix4::Translate(Position);
        Matrix4 scaleMatrix = Matrix4::Scale(Scale);
        Matrix4 rotationMatrix = quat.GetMatrix();

        return (translationMatrix * rotationMatrix) * scaleMatrix;
    }
} // namespace VW
