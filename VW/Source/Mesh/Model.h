#pragma once

#include "Base.h"
#include "Material/Material.h"
#include "Math/Matrix.h"
#include "Mesh/Mesh.h"
#include <memory>
#include <string>
#include <vector>

namespace VW
{
    struct ModelSubmesh
    {
        std::shared_ptr<Mesh> Mesh;
        Matrix4 LocalTransform;
        std::string Name;
        std::string MaterialName;
    };

    class VW_API Model
    {
    public:
        Model();
        ~Model();

        ModelSubmesh *GetSubmesh(const std::string &name);

        const std::vector<ModelSubmesh> &GetSubmeshes() const
        {
            return m_Submeshes;
        };

        inline const std::string &GetPath() const
        {
            return m_Path;
        };

    private:
        friend class ModelLoader; // TODO: do i really want this?
        std::vector<ModelSubmesh> m_Submeshes;
        std::string m_Path;
    };
} // namespace VW
