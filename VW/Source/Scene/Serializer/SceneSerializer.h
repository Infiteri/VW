#pragma once

#include "Base.h"
#include "Scene/Scene.h"
#include <string>
namespace VW
{
    class VW_API SceneSerializer
    {
    public:
        // TODO: think about input type, copying a scene isn't that expensive i think as i just copy
        // the pointers
        SceneSerializer(Scene *scene);
        ~SceneSerializer() = default;

        void Serialize(const std::string &path);
        void Deserialize(const std::string &path);

    private:
        Scene *m_Scene;
    };
} // namespace VW
