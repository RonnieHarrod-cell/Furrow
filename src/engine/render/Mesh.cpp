#include "engine/render/Mesh.h"

#include <glad/glad.h>

namespace engine
{

    Mesh::Mesh(const std::vector<float> &vertices)
    {
        // 6 floats per vertex: 3 position + 3 color.
        m_VertexCount = static_cast<int>(vertices.size() / 6);

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(float),
            vertices.data(),
            GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    Mesh::~Mesh()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    }

    void Mesh::Draw() const
    {
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);
        glBindVertexArray(0);
    }

} // namespace engine
