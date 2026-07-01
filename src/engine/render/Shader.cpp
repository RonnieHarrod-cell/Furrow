#include "engine/render/Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

namespace engine
{

    std::string Shader::ReadFile(const std::string &path)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            throw std::runtime_error("Could not open shader file: " + path);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    unsigned int Shader::CompileStage(unsigned int stage, const std::string &source)
    {
        unsigned int id = glCreateShader(stage);
        const char *src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int success = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char log[1024];
            glGetShaderInfoLog(id, sizeof(log), nullptr, log);
            std::cerr << "Shader compile error:\n"
                      << log << std::endl;
            throw std::runtime_error("Shader compilation failed");
        }
        return id;
    }

    Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
    {
        unsigned int vs = CompileStage(GL_VERTEX_SHADER, ReadFile(vertexPath));
        unsigned int fs = CompileStage(GL_FRAGMENT_SHADER, ReadFile(fragmentPath));

        m_ProgramId = glCreateProgram();
        glAttachShader(m_ProgramId, vs);
        glAttachShader(m_ProgramId, fs);
        glLinkProgram(m_ProgramId);

        int success = 0;
        glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &success);
        if (!success)
        {
            char log[1024];
            glGetProgramInfoLog(m_ProgramId, sizeof(log), nullptr, log);
            std::cerr << "Shader link error:\n"
                      << log << std::endl;
            throw std::runtime_error("Shader linking failed");
        }

        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_ProgramId);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_ProgramId);
    }

    void Shader::SetMat4(const std::string &name, const glm::mat4 &value) const
    {
        int loc = glGetUniformLocation(m_ProgramId, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
    }

    void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
    {
        int loc = glGetUniformLocation(m_ProgramId, name.c_str());
        glUniform3fv(loc, 1, &value[0]);
    }

} // namespace engine
