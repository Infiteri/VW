#include "SceneSerializer.h"
#include "Core/SerializerUtils.h"
#include "yaml-cpp/emitter.h"

namespace VW
{
    SceneSerializer::SceneSerializer(Scene *scene) : m_Scene(scene)
    {
    }

    void SceneSerializer::Serialize(const std::string &path)
    {
        VW_CHECK(m_Scene);

        YAML::Emitter out;
        out << YAML::BeginDoc;
        out << YAML::BeginMap;
        out << YAML::Key << "a" << YAML::Value << "b";

        out << YAML::EndMap;
        out << YAML::EndDoc;
        SerializerUtils::SaveEmitter(out, path);
    }

    void SceneSerializer::Deserialize(const std::string &path)
    {
        VW_CHECK(m_Scene);
    }
} // namespace VW
