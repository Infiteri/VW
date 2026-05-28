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
    } // namespace SerializerUtils
} // namespace VW
