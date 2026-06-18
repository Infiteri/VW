#include "ComponentSerializer.h"

#include "Base.h"
#include "Core/Logger.h"
#include "Core/SerializerUtils.h"
#include "Material/MaterialSystem.h"
#include "Mesh/Mesh.h"
#include "Mesh/MeshSystem.h"
#include "Mesh/Model.h"
#include "Mesh/ModelSystem.h"
#include "Scene/Components.h"
#include "Shader/ShaderSystem.h"

#include <immintrin.h>
#include <string>
#include <unordered_map>
#include <windows.h>
#include <winscard.h>
#include <yaml-cpp/yaml.h>

namespace VW
{
    static std::unordered_map<std::string, Material>
        *materialMap; // TODO: no 's_' cus this is the worst code ill ever write

    static int _GetNodeCount(const std::string &name, YAML::Node node)
    {
        std::string fmt = name + "Count";
        if (node[fmt])
            return node[fmt].as<int>();
        else
            return 0;
    }

    static void _SerializeMeshComponent(YAML::Emitter &out, MeshComponent *mesh, u32 index)
    {
        VW_CHECK(mesh)

        out << YAML::Key << "MeshComponent " + std::to_string(index);
        out << YAML::BeginMap;
        VW_SERIALIZE_FIELD("Type", (int)mesh->GetMesh()->GetType());
        VW_SERIALIZE_FIELD("MaterialName", MaterialSystem::GetMaterialName(mesh->GetMaterial()));
        SerializerUtils::SerializeTransform(out, "DeltaTransform", mesh->GetDeltaTransform());
        out << YAML::EndMap;
    }

    // TODO: ADD THE ABILITY FOR CUSTOM SHADERS ON MESH AND MODELS
    static void _DeserializeMeshComponent(YAML::Node node, Actor *actor)
    {
        VW_CHECK(actor)

        auto mesh = actor->AddComponent<MeshComponent>(
            MeshSystem::GetMesh((MeshType)node["Type"].as<int>()).get());

        mesh->SetMaterial(MaterialSystem::GetDefaultMaterial());

        if (node["MaterialName"].as<std::string>() != "")
        {
            auto name = node["MaterialName"].as<std::string>();

            // FIX: if setting the material's path without setting it's ID, the texture ID will be 0
            // and the material's texture's will not be loaded This is why below there are the
            // `material.Set...(material.Get...())` calls, the ID's get filled
            auto &material = materialMap->at(name);
            material.SetAlbedo(material.GetAlbedo());
            material.SetNormal(material.GetNormal());
            material.SetORM(material.GetORM());
            MaterialSystem::AddMaterial(name, material);
            mesh->SetMaterial(MaterialSystem::GetMaterial(name));
        }

        mesh->SetDeltaTransform(SerializerUtils::DeserializeTransform(node["DeltaTransform"]));

        mesh->SetShader(ShaderSystem::GetEngineShader("Object.glsl"));
    }

    static void _SerializeModelComponent(YAML::Emitter &out, ModelComponent *model, u32 index)
    {
        VW_CHECK(model)
        auto m = model->GetModel();

        out << YAML::Key << "ModelComponent " + std::to_string(index);
        out << YAML::BeginMap;
        VW_SERIALIZE_FIELD("Path", m->GetPath());
        SerializerUtils::SerializeTransform(out, "DeltaTransform", model->GetDeltaTransform());
        out << YAML::EndMap;
    }

    static void _DeserializeModelComponent(YAML::Node node, Actor *actor)
    {
        VW_CHECK(actor)
        ModelSystem::LoadModel(node["Path"].as<std::string>(), node["Path"].as<std::string>());

        auto m = actor->AddComponent<ModelComponent>(
            ModelSystem::GetModel(node["Path"].as<std::string>()).get());

        m->SetDeltaTransform(SerializerUtils::DeserializeTransform(node["DeltaTransform"]));
    }

    static void _SerializeAmbientLightComponent(YAML::Emitter &out, AmbientLightComponent *ambient,
                                                u32 index)
    {
        VW_CHECK(ambient);
        out << YAML::Key << "AmbientLightComponent " + std::to_string(index);
        out << YAML::BeginMap;

        // TODO: this should be common in all lights
        // TODO: to fix this repetition, there should be a common `LightComponent` class
        {
            SerializerUtils::SerializeColor(out, "Color", ambient->GetColor());
            VW_SERIALIZE_FIELD("Intensity", ambient->GetIntensity());
        }
        out << YAML::EndMap;
    }

    static void _DeserializeAmbientLightComponent(YAML::Node node, Actor *actor)
    {
        VW_CHECK(actor);
        auto a = actor->AddComponent<AmbientLightComponent>();

        // TODO: this is also common deserialization for all `LightComponent`s
        {
            a->SetIntensity(node["Intensity"].as<float>());
            a->SetColor(SerializerUtils::DeserializeColor(node["Color"]));
        }
    }

    static void _SerializeDirectionalLightComponent(YAML::Emitter &out,
                                                    DirectionalLightComponent *directional,
                                                    u32 index)
    {
        VW_CHECK(directional);
        out << YAML::Key << "DirectionalLightComponent " + std::to_string(index);
        out << YAML::BeginMap;

        SerializerUtils::SerializeVector3(out, "Direction", directional->GetDirection());

        {
            SerializerUtils::SerializeColor(out, "Color", directional->GetColor());
            VW_SERIALIZE_FIELD("Intensity", directional->GetIntensity());
        }
        out << YAML::EndMap;
    }

    static void _DeserializeDirectionalLightComponent(YAML::Node node, Actor *actor)
    {
        VW_CHECK(actor);
        auto a = actor->AddComponent<DirectionalLightComponent>();

        {
            a->SetIntensity(node["Intensity"].as<float>());
            a->SetColor(SerializerUtils::DeserializeColor(node["Color"]));
        }

        a->SetDirection(SerializerUtils::DeserializeVector3(node["Direction"]));
    }

    static void _SerializePointLightComponent(YAML::Emitter &out, PointLightComponent *point,
                                              u32 index)
    {
        VW_CHECK(point);
        out << YAML::Key << "PointLightComponent " + std::to_string(index);
        out << YAML::BeginMap;
        SerializerUtils::SerializeVector3(out, "Position", point->GetPosition());
        VW_SERIALIZE_FIELD("Range", point->GetRange());

        {
            SerializerUtils::SerializeColor(out, "Color", point->GetColor());
            VW_SERIALIZE_FIELD("Intensity", point->GetIntensity());
        }

        out << YAML::EndMap;
    }

    static void _DeserializePointLightComponent(YAML::Node node, Actor *actor)
    {
        VW_CHECK(actor);
        auto a = actor->AddComponent<PointLightComponent>();

        {
            a->SetIntensity(node["Intensity"].as<float>());
            a->SetColor(SerializerUtils::DeserializeColor(node["Color"]));
        }

        a->SetPosition(SerializerUtils::DeserializeVector3(node["Position"]));
        a->SetRange(node["Range"].as<float>());
    }

    static void _SerializeSpotLightComponent(YAML::Emitter &out, SpotLightComponent *spot,
                                             u32 index)
    {
        VW_CHECK(spot);
        out << YAML::Key << "SpotLightComponent " + std::to_string(index);
        out << YAML::BeginMap;
        SerializerUtils::SerializeVector3(out, "Position", spot->GetPosition());
        SerializerUtils::SerializeVector3(out, "Direction", spot->GetDirection());
        VW_SERIALIZE_FIELD("Range", spot->GetRange());
        VW_SERIALIZE_FIELD("InnerConeAngle", spot->GetInnerConeAngle());
        VW_SERIALIZE_FIELD("OuterConeAngle", spot->GetOuterConeAngle());

        {
            SerializerUtils::SerializeColor(out, "Color", spot->GetColor());
            VW_SERIALIZE_FIELD("Intensity", spot->GetIntensity());
        }

        out << YAML::EndMap;
    }

    static void _DeserializeSpotLightComponent(YAML::Node node, Actor *actor)
    {
        VW_CHECK(actor);
        auto a = actor->AddComponent<SpotLightComponent>();

        {
            a->SetIntensity(node["Intensity"].as<float>());
            a->SetColor(SerializerUtils::DeserializeColor(node["Color"]));
        }

        a->SetPosition(SerializerUtils::DeserializeVector3(node["Position"]));
        a->SetDirection(SerializerUtils::DeserializeVector3(node["Direction"]));
        a->SetRange(node["Range"].as<float>());
        a->SetInnerConeAngle(node["InnerConeAngle"].as<float>());
        a->SetOuterConeAngle(node["OuterConeAngle"].as<float>());
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
        SERIALIZE_COMPONENT(Model);
        SERIALIZE_COMPONENT(AmbientLight);
        SERIALIZE_COMPONENT(DirectionalLight);
        SERIALIZE_COMPONENT(PointLight);
        SERIALIZE_COMPONENT(SpotLight);
    }

    void ComponentSerializer::Deserialize(YAML::Node &node,
                                          std::unordered_map<std::string, Material> &materialMap2)
    {
        VW_CHECK(m_Actor);

#define DESERIALIZE_COMPONENT(name, fun) \
    for (int i = 0; i < _GetNodeCount(name, node); i++) \
    fun(node[std::string(name) + std::string(" ") + std::to_string(i)], m_Actor)

        materialMap = &materialMap2;
        DESERIALIZE_COMPONENT("MeshComponent", _DeserializeMeshComponent);
        DESERIALIZE_COMPONENT("ModelComponent", _DeserializeModelComponent);
        DESERIALIZE_COMPONENT("AmbientLightComponent", _DeserializeAmbientLightComponent);
        DESERIALIZE_COMPONENT("DirectionalLightComponent", _DeserializeDirectionalLightComponent);
        DESERIALIZE_COMPONENT("PointLightComponent", _DeserializePointLightComponent);
        DESERIALIZE_COMPONENT("SpotLightComponent", _DeserializeSpotLightComponent);
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
        GET_SER_COUNT(AmbientLight);
        GET_SER_COUNT(DirectionalLight);
        GET_SER_COUNT(PointLight);
        GET_SER_COUNT(SpotLight);
    }
} // namespace VW
