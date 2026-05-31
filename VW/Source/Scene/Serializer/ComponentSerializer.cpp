#include "ComponentSerializer.h"
#include "Base.h"
#include "Core/SerializerUtils.h"
#include "Material/MaterialSystem.h"
#include "yaml-cpp/emitter.h"

#include <yaml-cpp/yaml.h>

namespace VW
{
    static void _SerializeMeshComponent(YAML::Emitter &out, MeshComponent *mesh, u32 index)
    {
        VW_CHECK(mesh)

        out << YAML::Key << "MeshComponent " + std::to_string(index);
        out << YAML::BeginMap;
        VW_SERIALIZE_FIELD("Type", (int)mesh->GetMesh()->GetType());
        VW_SERIALIZE_FIELD("MaterialName", MaterialSystem::GetMaterialName(mesh->GetMaterial()));
        SerializerUtils::SerializeTransform(out, "Delta", mesh->GetDeltaTransform());
        out << YAML::EndMap;
    }

    ComponentSerializer::ComponentSerializer(Actor *actor) : m_Actor(actor)
    {
    }

    void ComponentSerializer::Serialize(YAML::Emitter &out)
    {
        VW_CHECK(m_Actor);

        _GetAndSerializeComponentCounts(out);

// serialize specific components
#define SERIALIZE_COMPONENT(type) \
    for (u32 i = 0; i < m_ComponentCounts.type; ++i) \
    { \
        _Serialize##type##Component(out, m_Actor->GetComponents<type##Component>()[i], i); \
    }

        SERIALIZE_COMPONENT(Mesh);
    }

    void ComponentSerializer::Deserialize(YAML::Node &node)
    {
        VW_CHECK(m_Actor);
    }

    void ComponentSerializer::_GetAndSerializeComponentCounts(YAML::Emitter &out)
    {
#define GET_SER_COUNT(type) \
    { \
        m_ComponentCounts.type = m_Actor->GetComponents<type##Component>().size(); \
        VW_SERIALIZE_FIELD(#type "ComponentCount", m_ComponentCounts.type); \
    }

        GET_SER_COUNT(Mesh);
        GET_SER_COUNT(Model);
    }
} // namespace VW
