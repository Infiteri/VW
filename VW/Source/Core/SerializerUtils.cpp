#include "SerializerUtils.h"
#include <fstream>

#include <yaml-cpp/yaml.h>

namespace VW
{
    namespace SerializerUtils
    {
        void SaveEmitter(YAML::Emitter &em, const std::string &path)
        {
            std::ofstream fout(path);
            fout << em.c_str();
            fout.close();
        }

        void SerializeVector3(YAML::Emitter &out, const char *field, const Vector3 &vec)
        {
            VW_SERIALIZE_FIELD(field, YAML::Flow);
            out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
        }
    } // namespace SerializerUtils
} // namespace VW
