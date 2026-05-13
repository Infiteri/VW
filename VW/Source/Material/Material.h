#pragma once

#include "Base.h"
#include "Color.h"
namespace VW
{
    class VW_API Material
    {
    public:
        Material();
        ~Material();

        void SetColor(const Color &color);
        void SetAlbedoID(u32 id);
        void SetNormalID(u32 id);
        void SetORMID(u32 id);

        inline Color GetColor() const
        {
            return m_Color;
        };

        inline u32 GetAlbedoID() const
        {
            return m_AlbedoID;
        };

        inline u32 GetNormalID() const
        {
            return m_NormalID;
        };

        inline u32 GetORMID() const
        {
            return m_ORMID;
        };

    private:
        Color m_Color;
        u32 m_AlbedoID, m_NormalID, m_ORMID;
    };
} // namespace VW
