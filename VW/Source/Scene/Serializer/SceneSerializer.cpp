#include "SceneSerializer.h"
#include "Core/SerializerUtils.h"
#include "Material/MaterialSystem.h"
#include "Scene/Serializer/ActorSerializer.h"

#include <yaml-cpp/yaml.h>

namespace VW
{
    static void _SerializeMaterials(YAML::Emitter &out)
    {
        auto materials = MaterialSystem::GetMaterials();

        // out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;
        VW_SERIALIZE_FIELD("Materials", YAML::BeginSeq);

        for (const auto &[name, mat] : materials)
        {
            out << YAML::BeginMap;
            VW_SERIALIZE_FIELD("Name", name);
            SerializerUtils::SerializeColor(out, "Color", mat.GetColor());
            VW_SERIALIZE_FIELD("AlbedoPath", mat.GetAlbedo());
            VW_SERIALIZE_FIELD("NormalPath", mat.GetNormal());
            VW_SERIALIZE_FIELD("ORMPath", mat.GetORM());
            out << YAML::EndMap;
        }

        out << YAML::EndSeq;
    }

    SceneSerializer::SceneSerializer(Scene *scene) : m_Scene(scene)
    {
    }

    void SceneSerializer::Serialize(const std::string &path)
    {
        VW_CHECK(m_Scene);

        YAML::Emitter out;
        out << YAML::BeginMap;
        VW_SERIALIZE_FIELD("Name", m_Scene->GetName().c_str());
        _SerializeMaterials(out);
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
