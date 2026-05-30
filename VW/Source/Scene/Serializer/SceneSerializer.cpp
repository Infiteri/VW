#include "SceneSerializer.h"
#include "Core/SerializerUtils.h"
#include "Scene/Serializer/ActorSerializer.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

namespace VW
{
    SceneSerializer::SceneSerializer(Scene *scene) : m_Scene(scene)
    {
    }

    void SceneSerializer::Serialize(const std::string &path)
    {
        VW_CHECK(m_Scene);

        YAML::Emitter out;
        out << YAML::BeginMap;
        VW_SERIALIZE_FIELD("Name", m_Scene->GetName().c_str());
        VW_SERIALIZE_FIELD("Actors", YAML::BeginSeq);

        for (const auto &actor : m_Scene->m_Actors)
        {
            ActorSerializer serializer(actor.get());
            serializer.Serialize(out);
        }

        out << YAML::EndSeq;
        out << YAML::EndMap;
        SerializerUtils::SaveEmitter(out, path);
    }

    void SceneSerializer::Deserialize(const std::string &path)
    {
        VW_CHECK(m_Scene);
    }
} // namespace VW
