#include "ActorSerializer.h"
#include "Base.h"
#include "Core/SerializerUtils.h"
#include "Core/UUID.h"
#include "Scene/Serializer/ComponentSerializer.h"

#include <string>
#include <yaml-cpp/yaml.h>

namespace VW
{
    ActorSerializer::ActorSerializer(Actor *actor) : m_Actor(actor)
    {
    }

    // TODO: turn `VW_CHECK` into `VW_ASSERT` and add some error logs for when conditions are not
    // met (in general)
    void ActorSerializer::Serialize(YAML::Emitter &out)
    {
        VW_CHECK(m_Actor);

        out << YAML::BeginMap;
        out << YAML::Key << "Actor";
        out << YAML::Value << "?";

        VW_SERIALIZE_FIELD("Name", m_Actor->GetName().c_str());
        VW_SERIALIZE_FIELD("UUID", m_Actor->GetID());
        SerializerUtils::SerializeTransform(out, "Transform", m_Actor->GetTransform());

        ComponentSerializer componentSerializer(m_Actor);
        componentSerializer.Serialize(out);

        VW_SERIALIZE_FIELD("Children", YAML::BeginSeq);

        for (auto &child : m_Actor->m_Children)
        {
            ActorSerializer(child.get()).Serialize(out);
        }

        out << YAML::EndSeq;
        out << YAML::EndMap;
    }

    void ActorSerializer::Deserialize(YAML::Node &node,
                                      std::unordered_map<std::string, Material> &materialMap)
    {
        VW_CHECK(m_Actor);

        std::string name = node["Name"].as<std::string>();
        m_Actor->SetName(name);

        UUID uuid = node["UUID"].as<u64>();
        m_Actor->m_ID = uuid;

        YAML::Node transformNode = node["Transform"];
        if (transformNode.IsMap())
            m_Actor->SetTransform(SerializerUtils::DeserializeTransform(transformNode));

        ComponentSerializer componentSerializer(m_Actor);
        componentSerializer.Deserialize(node, materialMap);

        YAML::Node childrenNode = node["Children"];
        if (childrenNode.IsSequence())
        {
            for (auto childNode : childrenNode)
            {
                auto actor = std::make_unique<Actor>();
                ActorSerializer(actor.get()).Deserialize(childNode, materialMap);
                m_Actor->AddChild(std::move(actor));
            }
        }
    }

} // namespace VW
