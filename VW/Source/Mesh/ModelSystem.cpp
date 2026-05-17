#include "ModelSystem.h"
#include "Mesh/ModelLoader.h"

namespace VW
{
    static ModelSystem::State s_State;

    void ModelSystem::Init()
    {
    }

    void ModelSystem::Shutdown()
    {
    }

    std::shared_ptr<Model> ModelSystem::GetModel(const std::string &name)
    {
        if (s_State.Models.find(name) == s_State.Models.end())
            return nullptr;

        return s_State.Models[name];
    }

    std::shared_ptr<Model> ModelSystem::LoadModel(const std::string &name, const std::string &path)
    {
        if (s_State.Models.find(name) != s_State.Models.end())
        {
            // TODO: warn
            return s_State.Models[name];
        }

        std::shared_ptr<Model> model = ModelLoader::Load(path);
        s_State.Models[name] = model;

        return s_State.Models[name];
    }
} // namespace VW
