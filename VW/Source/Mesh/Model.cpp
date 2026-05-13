#include "Model.h"

namespace VW
{
    Model::Model()
    {
    }

    Model::~Model()
    {
    }

    ModelSubmesh *Model::GetSubmesh(const std::string &name)
    {
        for (auto &s : m_Submeshes)
            if (s.Name == name)
                return &s;
        return nullptr;
    }
} // namespace VW
