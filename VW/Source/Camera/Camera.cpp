#include "Camera.h"
#include "Core/Logger.h"
#include "Math/Quaternion.h"
#include <cmath>

namespace VW
{

    bool Frustum::IsSphereInside(const Vector3 &center, float radius) const
    {
        for (int i = 0; i < 6; ++i)
        {
            if (Planes[i].Distance(center) < -radius)
                return false;
        }
        return true;
    }

    void Frustum::Extract(const Matrix4 &m)
    {
        Planes[0].Normal.x = m.m[0][3] + m.m[0][0];
        Planes[0].Normal.y = m.m[1][3] + m.m[1][0];
        Planes[0].Normal.z = m.m[2][3] + m.m[2][0];
        Planes[0].D = m.m[3][3] + m.m[3][0];

        Planes[1].Normal.x = m.m[0][3] - m.m[0][0];
        Planes[1].Normal.y = m.m[1][3] - m.m[1][0];
        Planes[1].Normal.z = m.m[2][3] - m.m[2][0];
        Planes[1].D = m.m[3][3] - m.m[3][0];

        Planes[2].Normal.x = m.m[0][3] + m.m[0][1];
        Planes[2].Normal.y = m.m[1][3] + m.m[1][1];
        Planes[2].Normal.z = m.m[2][3] + m.m[2][1];
        Planes[2].D = m.m[3][3] + m.m[3][1];

        Planes[3].Normal.x = m.m[0][3] - m.m[0][1];
        Planes[3].Normal.y = m.m[1][3] - m.m[1][1];
        Planes[3].Normal.z = m.m[2][3] - m.m[2][1];
        Planes[3].D = m.m[3][3] - m.m[3][1];

        Planes[4].Normal.x = m.m[0][2];
        Planes[4].Normal.y = m.m[1][2];
        Planes[4].Normal.z = m.m[2][2];
        Planes[4].D = m.m[3][2];

        Planes[5].Normal.x = m.m[0][3] - m.m[0][2];
        Planes[5].Normal.y = m.m[1][3] - m.m[1][2];
        Planes[5].Normal.z = m.m[2][3] - m.m[2][2];
        Planes[5].D = m.m[3][3] - m.m[3][2];

        for (int i = 0; i < 6; ++i)
        {
            float len = Planes[i].Normal.Length();
            if (len == 0.0f || std::isnan(len))
            {
                // todo: real category here
                VW_LOG(Error, "vwrn", "Frustum Plane %d: invalid normal before normalize!", i);
                continue;
            }
            Planes[i].Normal.x /= len;
            Planes[i].Normal.y /= len;
            Planes[i].Normal.z /= len;
            Planes[i].D /= len;
        }
    }

    Matrix4 Camera::GetView()
    {
        if (m_Dirty)
        {
            m_Dirty = false;
            auto rot = Matrix4::Transpose(m_Orientation.GetMatrix());
            auto negPos = Matrix4::Translate(-m_Pos);
            m_View = rot * negPos;
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
