#pragma once

#include "Base.h"
#include "Mesh/Model.h"
#include <memory>
#include <string>

namespace VW
{
    class VW_API ModelLoader
    {
    public:
        static std::shared_ptr<Model> Load(const std::string &path);
        static std::shared_ptr<Model> LoadWithMaterial(const std::string &path, bool createMaterials);
    };

} // namespace VW
