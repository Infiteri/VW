#include "SerializerUtils.h"
#include <fstream>

#include <yaml-cpp/yaml.h>

namespace VW
{
    namespace SerializerUtils
    {
        YAML::Node LoadFromPath(const std::string &path)
        {
            std::ifstream stream(path);
            std::stringstream strStream;
            strStream << stream.rdbuf();
            YAML::Node data = YAML::Load(strStream.str());
            return data;
        }

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

        void SerializeColor(YAML::Emitter &out, const char *field, const Color &color)
        {
            VW_SERIALIZE_FIELD(field, YAML::Flow);
            out << YAML::BeginSeq << color.r << color.g << color.b << color.a << YAML::EndSeq;
        }

        void SerializeTransform(YAML::Emitter &out, const char *field, const Transform &transform)
        {
            VW_SERIALIZE_FIELD(field, YAML::BeginMap);
            SerializerUtils::SerializeVector3(out, "Position", transform.Position);
            SerializerUtils::SerializeVector3(out, "Rotation", transform.Rotation);
            SerializerUtils::SerializeVector3(out, "Scale", transform.Scale);
            out << YAML::EndMap;
        }
    } // namespace SerializerUtils
} // namespace VW
