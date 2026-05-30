#pragma once

#include "Base.h"
#include "Scene/Actor.h"

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
        void Deserialize(YAML::Node &node);

    private:
        Actor *m_Actor;
    };
} // namespace VW
