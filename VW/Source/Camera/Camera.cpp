#include "Camera.h"
#include "Math/Quaternion.h"
#include <cmath>

namespace VW
{

    Matrix4 Camera::GetView()
    {
        if (m_Dirty)
        {
            m_Dirty = false;
            auto rot = m_Orientation.GetMatrix();
            auto pos = Matrix4::Translate(m_Pos);
            m_View = Matrix4::Invert(pos * rot);
        }

        return m_View;
    }

    void Camera::SetPosition(const Vector3 &pos)
    {
        m_Pos = pos;
        m_Dirty = true;
        GetView();
    }

    void Camera::SetOrientation(const Quaternion &quat)
    {
        m_Orientation = quat;
        m_Dirty = true;
        GetView();
    }
} // namespace VW
