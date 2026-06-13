#pragma once

#include "Color.h"
#include "Math/Transform.h"
#include "Math/Vector.h"
#include <string>
#define VW_SERIALIZE_FIELD(name, value) out << YAML::Key << name << YAML::Value << value
#define VW_SERIALIZE_FIELD_TO(name, value, to) to << YAML::Key << name << YAML::Value << value

namespace YAML
{
    class Emitter;
    class Node;
} // namespace YAML

namespace VW
{
    namespace SerializerUtils
    {
        YAML::Node LoadFromPath(const std::string &path);

        void SaveEmitter(YAML::Emitter &em, const std::string &path);
        void SerializeVector3(YAML::Emitter &out, const char *field, const Vector3 &vec);
        void SerializeColor(YAML::Emitter &out, const char *field, const Color &color);
        void SerializeTransform(YAML::Emitter &out, const char *field, const Transform &transform);

        Color DeserializeColor(const YAML::Node &node);
        Vector3 DeserializeVector3(const YAML::Node &node);
    } // namespace SerializerUtils
} // namespace VW
