#include "engine/render/Mesh.h"

#include <glad/glad.h>

namespace engine
{

    Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
    {
        m_IndexCount = static_cast<int>(indices.size());

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(Vertex),
            vertices.data(),
            GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(unsigned int),
            indices.data(),
            GL_STATIC_DRAW);

        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    Mesh::~Mesh()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void Mesh::Draw() const
    {
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

} // namespace engine
