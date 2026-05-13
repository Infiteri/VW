
#include "MaterialSystem.h"
#include "Core/Logger.h"
#include "Texture/TextureSystem.h"
#include <random>

namespace VW
{
    static MaterialSystem::State s_State;

    void MaterialSystem::Init()
    {
        s_State.DefaultMat.SetAlbedoID(TextureSystem::GetDefaultTextureID());
        s_State.DefaultMat.SetNormalID(TextureSystem::GetDefaultTextureID());
        s_State.DefaultMat.SetORMID(TextureSystem::GetDefaultTextureID());
    }

    void MaterialSystem::Shutdown()
    {
    }

    bool MaterialSystem::Exists(const std::string &name)
    {
        return s_State.Mats.find(name) != s_State.Mats.end();
    }

    Material *MaterialSystem::GetMaterial(const std::string &name)
    {
        if (!Exists(name))
        {
            VW_WARN("vwrn", "Material system coudln't 'GetMaterial' for name '%s'", name.c_str());
            return &s_State.DefaultMat;
        }

        return &s_State.Mats[name];
    }

    Material *MaterialSystem::GetDefaultMaterial()
    {
        return &s_State.DefaultMat;
    }

    void MaterialSystem::AddMaterial(const std::string &name, const Material &mat)
    {
        if (Exists(name))
        {
            VW_ERROR("vwrn", "Material system cannot 'AddMaterial' as material '%s' exists",
                     name.c_str());
            return;
        }

        s_State.Mats[name] = mat;
    }
} // namespace VW
