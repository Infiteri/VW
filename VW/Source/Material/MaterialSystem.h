#pragma once

#include "Base.h"
#include "Material/Material.h"
#include <string>
#include <unordered_map>
#include <vector>
namespace VW
{
    // is this trully asystem
    class VW_API MaterialSystem
    {
    public:
        struct State
        {
            std::unordered_map<std::string, Material> Mats;
            Material DefaultMat;
        };

    public:
        MaterialSystem() = delete;
        ~MaterialSystem() = delete;

        static void Init();
        static void Shutdown();

        static bool Exists(const std::string &name);
        static Material *GetMaterial(const std::string &name);
        static Material *GetDefaultMaterial();

        /// Returns the name of the material, or an empty string if the material is null.
        static std::string GetMaterialName(const Material *mat);

        static const std::unordered_map<std::string, Material>& GetMaterials();

        // TODO: add file based materials (maybe comes with multiple materials per file or some .mtl
        // to vw material format)
        static void AddMaterial(const std::string &name, const Material &mat);
    };
} // namespace VW
