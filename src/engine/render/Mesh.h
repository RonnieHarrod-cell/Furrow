#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace engine
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
    };

    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
        ~Mesh();

        Mesh(const Mesh &) = delete;
        Mesh &operator=(const Mesh &) = delete;

        void Draw() const;

    private:
        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;
        unsigned int m_EBO = 0;
        int m_IndexCount = 0;
    };
} // namespace engine
