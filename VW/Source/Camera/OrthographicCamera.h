#pragma once

#include "Base.h"
#include "Camera/Camera.h"

namespace VW
{
    class OrthographicCamera : public Camera
    {
    public:
        OrthographicCamera(float left = 0, float right = 1920, float top = 0, float bottom = 1080,
                           float near = 0.001f, float far = 1000.f);
        ~OrthographicCamera() = default;

        Matrix4 GetProjection();
        void Resize(float width, float height);

    private:
        float m_Left, m_Right, m_Top, m_Bottom, m_Near, m_Far;
    };
} // namespace VW
