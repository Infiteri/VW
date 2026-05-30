#pragma once

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
        void SaveEmitter(YAML::Emitter &em, const std::string& path);
        void SerializeVector3(YAML::Emitter &out, const char* field, const Vector3& vec);
    }
} // namespace VW
