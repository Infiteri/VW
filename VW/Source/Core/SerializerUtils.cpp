#include "SerializerUtils.h"
#include "Shader/ShaderUniforms.h"
#include <fstream>
#include <type_traits>

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

        void SerializeVector2(YAML::Emitter &out, const char *field, const Vector2 &vec)
        {
            VW_SERIALIZE_FIELD(field, YAML::Flow);
            out << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
        }

        void SerializeVector3(YAML::Emitter &out, const char *field, const Vector3 &vec)
        {
            VW_SERIALIZE_FIELD(field, YAML::Flow);
            out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
        }

        void SerializeVector4(YAML::Emitter &out, const char *field, const Vector4 &vec)
        {
            VW_SERIALIZE_FIELD(field, YAML::Flow);
            out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
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

        static void _SerializeUniform(YAML::Emitter &out, const char *name,
                                      const UniformValue &value)
        {
            out << YAML::BeginMap;
            VW_SERIALIZE_FIELD("Name", name);
            VW_SERIALIZE_FIELD("Type", (int)value.index());

            std::visit([&](auto &&arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float>) {
                    VW_SERIALIZE_FIELD("Value", arg);
                } else if constexpr (std::is_same_v<T, Vector2>) {
                    SerializeVector2(out, "Value", arg);
                } else if constexpr (std::is_same_v<T, Vector3>) {
                    SerializeVector3(out, "Value", arg);
                } else if constexpr (std::is_same_v<T, Vector4>) {
                    SerializeVector4(out, "Value", arg);
                } else if constexpr (std::is_same_v<T, Color>) {
                    SerializeColor(out, "Value", arg);
                }
            }, value);

            out << YAML::EndMap;
        }

        void SerializeShaderUniforms(YAML::Emitter &out, const ShaderUniforms &uniforms)
        {
            out << YAML::Key << "Uniforms" << YAML::Value << YAML::BeginSeq;
            for (const auto &[name, value] : uniforms.GetUniforms())
                _SerializeUniform(out, name.c_str(), value);
            out << YAML::EndSeq;
        }
    } // namespace SerializerUtils
} // namespace VW
