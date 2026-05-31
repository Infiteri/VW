#include "ActorSerializer.h"
#include "Base.h"
#include "Core/SerializerUtils.h"
#include "Scene/Serializer/ComponentSerializer.h"

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

        out << YAML::EndMap;
    }

    void ActorSerializer::Deserialize(YAML::Node &node)
    {
        VW_CHECK(m_Actor);
    }

} // namespace VW
