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

        Color DeserializeColor(const YAML::Node &node)
        {
            // TODO: safety checks
            return Color(node[0].as<float>(), node[1].as<float>(), node[2].as<float>(),
                         node[3].as<float>());
        }

        Vector3 DeserializeVector3(const YAML::Node &node)
        {
            // TODO: safety checks
            return Vector3(node[0].as<float>(), node[1].as<float>(), node[2].as<float>());
        }
    } // namespace SerializerUtils
} // namespace VW
