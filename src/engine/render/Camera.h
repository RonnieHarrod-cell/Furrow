#pragma once

#include <glm/glm.hpp>

namespace engine
{
    class Camera
    {
    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f));

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix(float aspectRatio) const;

        void ProcessKeyboard(bool forward, bool backward, bool left, bool right, bool up, bool down, float deltaTime);
        void ProcessMouseMovement(float deltaX, float deltaY);

        glm::vec3 GetPosition() const { return m_Position; }

    private:
        void UpdateVectors();

        glm::vec3 m_Position;
        glm::vec3 m_Front;
        glm::vec3 m_Up;
        glm::vec3 m_Right;
        glm::vec3 m_WorldUp{0.0f, 1.0f, 0.0f};

        float m_Yaw = -90.0f; // facing -Z initially
        float m_Pitch = 0.0f;

        float m_MoveSpeed = 3.0f;
        float m_MouseSensitivity = 0.1f;
        float m_Fov = 45.0f;
    };
} // namespace engine
