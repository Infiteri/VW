#pragma once

#include "Scene/Actor.h"

namespace YAML
{
    class Emitter;
    class Node;
}; // namespace YAML

namespace VW
{
    class ComponentSerializer
    {
    public:
        ComponentSerializer(Actor *actor);
        ~ComponentSerializer() = default;

        void Serialize(YAML::Emitter &out);
        void Deserialize(YAML::Node &out);

    private:
        Actor *m_Actor;

        struct
        {
            u32 Mesh;
            u32 Model;
        } m_ComponentCounts;

    private:
        void _GetAndSerializeComponentCounts(YAML::Emitter& out);
    };
} // namespace VW
