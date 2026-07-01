#pragma once

#include <vector>

namespace engine
{
    class Mesh
    {
    public:
        Mesh(const std::vector<float> &vertices);
        ~Mesh();

        Mesh(const Mesh &) = delete;
        Mesh &operator=(const Mesh &) = delete;

        void Draw() const;

    private:
        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;
        int m_VertexCount = 0;
    };
} // namespace engine
