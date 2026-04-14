#pragma once

#include "Camera.h"

namespace VW
{
    class PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera(float fov = 90, float aspect = 16.f / 9, float near = 0.001,
                          float far = 1000.f);
        ~PerspectiveCamera() = default;

        Matrix4 GetProjection();

    private:
        float m_FOV, m_Aspect, m_Near, m_Far;
    };
} // namespace VW
