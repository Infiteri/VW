#pragma once

#include "Base.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"

namespace VW
{
    struct VW_API Transform
    {
        Vector3 Position{0, 0, 0};
        Vector3 Rotation{0, 0, 0};
        Vector3 Scale{1, 1, 1};

        Transform();
        Transform(const Vector3 &position, const Vector3 &rotation, const Vector3 &scale);
        Transform(const Transform &other);
        ~Transform() = default;

        Matrix4 GetMatrix() const;
    };
} // namespace Core
