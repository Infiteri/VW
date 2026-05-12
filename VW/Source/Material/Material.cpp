#include "Material.h"

namespace VW
{
    Material::Material() : m_Color({255, 255, 255, 255}), m_AlbedoID(0), m_NormalID(0)
    {
    }

    Material::~Material()
    {
    }

    void Material::SetColor(const Color &color)
    {
        m_Color = color;
    }

    void Material::SetAlbedoID(u32 id)
    {
        m_AlbedoID = id;
    }

    void Material::SetNormalID(u32 id)
    {
        m_NormalID = id;
    }

} // namespace VW
