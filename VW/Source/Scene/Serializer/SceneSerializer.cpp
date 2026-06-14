#include "SceneSerializer.h"
#include "Core/SerializerUtils.h"
#include "Material/Material.h"
#include "Material/MaterialSystem.h"
#include "Scene/Actor.h"
#include "Scene/Serializer/ActorSerializer.h"
#include "Sky/Sky.h"
#include "yaml-cpp/emittermanip.h"

#include <string>
#include <unordered_map>
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

    static void _SerializeSky(YAML::Emitter &out, Sky *sky)
    {
        VW_SERIALIZE_FIELD("Sky", YAML::BeginMap);
        VW_SERIALIZE_FIELD("Mode", (int)sky->GetMode());

        switch (sky->GetMode())
        {
        case SkyMode::Color:
        {
            SerializerUtils::SerializeColor(out, "Color", sky->GetColor());
        }
        break;

        case SkyMode::Skybox:
            VW_SERIALIZE_FIELD("Left", sky->GetSkyboxConfig().Left);
            VW_SERIALIZE_FIELD("Right", sky->GetSkyboxConfig().Right);
            VW_SERIALIZE_FIELD("Top", sky->GetSkyboxConfig().Top);
            VW_SERIALIZE_FIELD("Bottom", sky->GetSkyboxConfig().Bottom);
            VW_SERIALIZE_FIELD("Front", sky->GetSkyboxConfig().Front);
            VW_SERIALIZE_FIELD("Back", sky->GetSkyboxConfig().Back);
            break;

        case SkyMode::Shader:
            VW_SERIALIZE_FIELD("Path", sky->GetShaderPath());
            SerializerUtils::SerializeShaderUniforms(out, sky->GetShaderUniforms());
            break;
        }

        out << YAML::EndMap;
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
        _SerializeSky(out, &m_Scene->GetSky());
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

        YAML::Node data = SerializerUtils::LoadFromPath(path);
        VW_CHECK(data);

        m_Scene->SetName(data["Name"].as<std::string>());

        // load materials
        auto materials = data["Materials"];
        std::unordered_map<std::string, Material> materialMap; // TODO: horrible
        VW_CHECK(materials);

        for (const auto &mat : materials)
        {
            // TODO: load these materials locally and keep track of them, only add the materials the
            // meshes themselves use. this is to fix bad paths related to models, which is an entire
            // issue on its own
            std::string name = mat["Name"].as<std::string>();

            Material material;
            material.SetAlbedoPath(mat["AlbedoPath"].as<std::string>());
            material.SetNormalPath(mat["NormalPath"].as<std::string>());
            material.SetORMPath(mat["ORMPath"].as<std::string>());

            materialMap[name] = material;
        }

        auto actors = data["Actors"];
        VW_CHECK(actors);

        for (auto actorNode : actors)
        {
            auto actor = std::make_unique<Actor>();
            ActorSerializer serializer(actor.get());
            serializer.Deserialize(actorNode, materialMap);
            m_Scene->AddActor(std::move(actor));
        }
    }
} // namespace VW
