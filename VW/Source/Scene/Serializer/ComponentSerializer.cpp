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

#include <string>
#include <unordered_map>
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

    static void _DeserializeMeshComponent(YAML::Node node, Actor *actor)
    {
        VW_CHECK(actor)

        auto mesh = actor->AddComponent<MeshComponent>(
            MeshSystem::GetMesh((MeshType)node["Type"].as<int>()).get());

        mesh->SetMaterial(MaterialSystem::GetDefaultMaterial());

        if (node["MaterialName"].as<std::string>() != "")
        {
            auto name = node["MaterialName"].as<std::string>();

            // FIX: if setting the material's path without setting it's ID, the texture ID will be 0 and the material's texture's will not be loaded
            // This is why below there are the `material.Set...(material.Get...())` calls, the ID's get filled
            auto &material = materialMap->at(name);
            material.SetAlbedo(material.GetAlbedo());
            material.SetNormal(material.GetNormal());
            material.SetORM(material.GetORM());
            MaterialSystem::AddMaterial(name, material);
            mesh->SetMaterial(MaterialSystem::GetMaterial(name));
        }

        mesh->SetShader(ShaderSystem::GetEngineShader("Object.glsl"));
    }

    static void _SerializeModelComponent(YAML::Emitter &out, ModelComponent *model, u32 index)
    {
        VW_CHECK(model)
        auto m = model->GetModel();

        out << YAML::Key << "ModelComponent " + std::to_string(index);
        out << YAML::BeginMap;
        VW_SERIALIZE_FIELD("Path", m->GetPath());
        SerializerUtils::SerializeTransform(out, "DeltaTransform",
                                            model->GetDeltaTransform()); // TODO: RENAME TO DELTA
        out << YAML::EndMap;
    }

    static void _DeserializeModelComponent(YAML::Node node, Actor *actor)
    {
        VW_CHECK(actor)
        ModelSystem::LoadModel(node["Path"].as<std::string>(), node["Path"].as<std::string>());

        auto m = actor->AddComponent<ModelComponent>(
            ModelSystem::GetModel(node["Path"].as<std::string>()).get());
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
