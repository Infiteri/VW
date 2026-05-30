#include "ActorSerializer.h"
#include "Base.h"
#include "Core/SerializerUtils.h"

#include <yaml-cpp/yaml.h>

namespace VW
{
    static void _SerializeTransform(const Transform &t, YAML::Emitter &out)
    {
        out << YAML::Key << "Transform";
        out << YAML::Value << YAML::BeginMap;
        SerializerUtils::SerializeVector3(out, "Position", t.Position);
        SerializerUtils::SerializeVector3(out, "Rotation", t.Rotation);
        SerializerUtils::SerializeVector3(out, "Scale", t.Scale);
        out << YAML::EndMap;
    }

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
        _SerializeTransform(m_Actor->GetTransform(), out);

        out << YAML::EndMap;
    }

    void ActorSerializer::Deserialize(YAML::Node &node)
    {
        VW_CHECK(m_Actor);
    }

} // namespace VW
