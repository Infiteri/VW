
#include "MaterialSystem.h"
#include "Core/Logger.h"
#include "Texture/TextureSystem.h"
#include <random>

namespace VW
{
    static MaterialSystem::State s_State;

    void MaterialSystem::Init()
    {
        s_State.DefaultMat.SetAlbedo("");
        s_State.DefaultMat.SetNormal("");
        s_State.DefaultMat.SetORM("");
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

    std::string MaterialSystem::GetMaterialName(const Material *mat)
    {
        if (!mat)
            return "";

        for (const auto &pair : s_State.Mats)
        {
            if (&pair.second == mat)
                return pair.first;
        }
        return "";
    }

    const std::unordered_map<std::string, Material> &MaterialSystem::GetMaterials()
    {
        return s_State.Mats;
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
