#pragma once

#include "Base.h"
#include "Color.h"
#include <string>
namespace VW
{
    class VW_API Material
    {
    public:
        Material();
        ~Material();

        void SetColor(const Color &color);

        void SetAlbedo(const std::string& path);
        void SetNormal(const std::string& path);
        void SetORM(const std::string& path);

        void SetAlbedoID(u32 id) { m_AlbedoID = id; }
        void SetNormalID(u32 id) { m_NormalID = id; }
        void SetORMID(u32 id) { m_ORMID = id; }

        void SetAlbedoPath(const std::string &p) { m_AlbedoPath = p; }
        void SetNormalPath(const std::string &p) { m_NormalPath = p; }
        void SetORMPath(const std::string &p) { m_ORMPath = p; }

        inline Color GetColor() const
        {
            return m_Color;
        };

        inline std::string GetAlbedo() const
        {
            return m_AlbedoPath;
        };

        inline std::string GetNormal() const
        {
            return m_NormalPath;
        };

        inline std::string GetORM() const
        {
            return m_ORMPath;
        };

        inline u32 GetAlbedoID() const { return m_AlbedoID; }
        inline u32 GetNormalID() const { return m_NormalID; }
        inline u32 GetORMID() const { return m_ORMID; }

    private:
        Color m_Color;
        u32 m_AlbedoID, m_NormalID, m_ORMID;
        std::string m_AlbedoPath, m_NormalPath, m_ORMPath;
    };
} // namespace VW
