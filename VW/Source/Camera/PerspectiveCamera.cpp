#include "PerspectiveCamera.h"
#include "Math/Math.h"

namespace VW
{

    PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
        : m_FOV(fov), m_Aspect(aspect), m_Near(near), m_Far(far)
    {
        GetProjection();
    }

    void PerspectiveCamera::Resize(float width, float height)
    {
        m_Aspect = width / height;
        GetProjection();
    }

    Matrix4 PerspectiveCamera::GetProjection()
    {
        m_Proj = Matrix4::Perspective(m_FOV * VW_DEG_TO_RAD, m_Aspect, m_Near, m_Far);
        return m_Proj;
    }
} // namespace VW
