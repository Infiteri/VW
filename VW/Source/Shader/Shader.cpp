#include "Shader.h"
#include "Core/Logger.h"
#include <algorithm>
#include <fstream>
#include <glad/glad.h>
#include <string>

namespace VW
{
    Shader::Shader() : m_ID(0)
    {
    }

    Shader::Shader(const std::string &filepath) : m_ID(0)
    {
        Load(filepath);
    }

    Shader::~Shader()
    {
        Unload();
    }

    int Shader::_ComposeSources(const std::string &path, std::string &vs, std::string &fs)
    {
        if (path.empty())
            return -1;

        std::ifstream file(path, std::ios::in | std::ios::binary);

        if (!file.is_open())
            return -4;

        std::string fileContent;
        file.seekg(0, std::ios::end);
        fileContent.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&fileContent[0], fileContent.size());
        file.close();

        if (fileContent.empty())
            return -2;

        auto vertexCommentPosition = fileContent.find("// VERTEX");
        auto fragmentCommentPosition = fileContent.find("// FRAGMENT");
        if (vertexCommentPosition == std::string::npos ||
            fragmentCommentPosition == std::string::npos)
        {
            return -3;
        }
        vs = fileContent.substr(vertexCommentPosition,
                                fragmentCommentPosition - vertexCommentPosition);
        fs = fileContent.substr(fragmentCommentPosition);
        return 0;
    }

    u32 Shader::_LoadShader(const std::string &str, u32 type)
    {
        const char *src = str.c_str();
        u32 shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, 0);
        glCompileShader(shader);

        int compileStatus = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

        if (compileStatus == GL_FALSE)
        {
            int maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            char *buffer = new char[maxLength];
            glGetShaderInfoLog(shader, maxLength, &maxLength, &buffer[0]);

            VW_LOG(Error, "vwrn", "Unable to compile shader");
            VW_LOG(Error, "vwrn", buffer);

            delete[] buffer;
            return 0;
        }

        return shader;
        return 0;
    }

    void Shader::Load(const std::string &filepath)
    {
        m_ID = glCreateProgram();

        std::string vertexSource, fragmentSource;
        _ComposeSources(filepath, vertexSource, fragmentSource);

        if (vertexSource.empty() || fragmentSource.empty())
        {
            VW_LOG(Error, "vwrn", "Unable to load shader '%s'", filepath.c_str());

            Unload();
            return;
        }

        u32 vertexShader = _LoadShader(vertexSource.c_str(), GL_VERTEX_SHADER);
        u32 fragmentShader = _LoadShader(fragmentSource.c_str(), GL_FRAGMENT_SHADER);
        glAttachShader(m_ID, vertexShader);
        glAttachShader(m_ID, fragmentShader);
        glLinkProgram(m_ID);

        GLint isLinked = 0;
        glGetProgramiv(m_ID, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &maxLength);
            char *infoLog = new char[maxLength];
            glGetProgramInfoLog(m_ID, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(m_ID);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            VW_LOG(Error, "vwrn", "Cannot create shader '%s', output log:", filepath.c_str());
            VW_LOG(Error, "vwrn", "%s", infoLog);

            delete[] infoLog;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void Shader::Unload()
    {
        glDeleteProgram(m_ID);
        m_ID = 0;
    }

    bool Shader::IsLoaded() const
    {
        return m_ID != 0;
    }

    void Shader::Use()
    {
        glUseProgram(m_ID);
    }
} // namespace VW
