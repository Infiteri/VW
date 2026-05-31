#include "Material.h"
#include "Texture/TextureSystem.h"
#include <ctype.h>

namespace VW
{
    Material::Material() : m_Color({255, 255, 255, 255}), m_AlbedoID(0), m_NormalID(0), m_ORMID(0)
    {
    }

    Material::~Material()
    {
    }

    void Material::SetColor(const Color &color)
    {
        m_Color = color;
    }

    void Material::SetAlbedo(const std::string &path)
    {
        if (path.empty())
        {
            m_AlbedoID = TextureSystem::GetDefaultTextureID();
            m_AlbedoPath = "";
            return;
        }

        m_AlbedoID = TextureSystem::GetTextureID(path);
        m_AlbedoPath = path;
    }

    void Material::SetNormal(const std::string &path)
    {
        if (path.empty())
        {
            m_NormalID = TextureSystem::GetDefaultTextureID();
            m_NormalPath = "";
            return;
        }

        m_NormalID = TextureSystem::GetTextureID(path);
        m_NormalPath = path;
    }

    void Material::SetORM(const std::string &path)
    {
        if (path.empty())
        {
            m_ORMID = TextureSystem::GetDefaultTextureID();
            m_ORMPath = "";
            return;
        }

        m_ORMID = TextureSystem::GetTextureID(path);
        m_ORMPath = path;
    }

} // namespace VW
