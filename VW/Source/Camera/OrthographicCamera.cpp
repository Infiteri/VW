#include "OrthographicCamera.h"
#include "Camera/PerspectiveCamera.h"

namespace VW
{
    OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom,
                                           float near, float far)
        : m_Left(left), m_Right(right), m_Top(top), m_Bottom(bottom), m_Near(near), m_Far(far)
    {
        GetProjection();
        GetView();
    }

    void OrthographicCamera::Resize(float width, float height)
    {
        m_Right = width;
        m_Bottom = height;
        GetProjection();
    }

    Matrix4 OrthographicCamera::GetProjection()
    {
        m_View = Matrix4::Ortho(m_Left, m_Right, m_Top, m_Bottom, m_Near, m_Far);
        return m_View;
    }

} // namespace VW
