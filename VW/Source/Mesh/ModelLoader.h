#pragma once

#include "Base.h"
#include "Mesh.h"
#include <memory>
#include <string>

namespace VW
{
    class VW_API ModelLoader
    {
    public:
        static std::shared_ptr<Mesh> LoadObj(const std::string &path);
    };

} // namespace VW
