#pragma once

#include <string>
#include <glm/glm.hpp>

namespace engine
{
    class Shader
    {
    public:
        Shader(const std::string &vertexPath, const std::string &fragmentPath);
        ~Shader();

        void Bind() const;
        void SetMat4(const std::string &name, const glm::mat4 &value) const;
        void SetVec3(const std::string &name, const glm::vec3 &value) const;

    private:
        unsigned int m_ProgramId = 0;

        static std::string ReadFile(const std::string &path);
        static unsigned int CompileStage(unsigned int stage, const std::string &source);
    };
} // namespace engine
