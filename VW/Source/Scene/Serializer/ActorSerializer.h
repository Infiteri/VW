#pragma once

#include "Material/Material.h"
#include "Scene/Actor.h"
#include <string>
#include <unordered_map>

namespace YAML
{
    class Emitter;
    class Node;
} // namespace YAML

namespace VW
{
    class ActorSerializer
    {
    public:
        ActorSerializer(Actor *actor);
        ~ActorSerializer() = default;

        void Serialize(YAML::Emitter &out);
        void Deserialize(YAML::Node &node, std::unordered_map<std::string, Material> &materialMap);

    private:
        Actor *m_Actor;
    };
} // namespace VW
