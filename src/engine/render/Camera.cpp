#include "engine/render/Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

namespace engine
{

    Camera::Camera(glm::vec3 position)
        : m_Position(position)
    {
        UpdateVectors();
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    }

    glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const
    {
        return glm::perspective(glm::radians(m_Fov), aspectRatio, 0.1f, 100.0f);
    }

    void Camera::ProcessKeyboard(bool forward, bool backward, bool left, bool right, float deltaTime)
    {
        float velocity = m_MoveSpeed * deltaTime;

        if (forward)
        {
            m_Position += m_Front * velocity;
        }
        if (backward)
        {
            m_Position -= m_Front * velocity;
        }
        if (left)
        {
            m_Position -= m_Right * velocity;
        }
        if (right)
        {
            m_Position += m_Right * velocity;
        }
    }

    void Camera::ProcessMouseMovement(float deltaX, float deltaY)
    {
        m_Yaw += deltaX * m_MouseSensitivity;
        m_Pitch += deltaY * m_MouseSensitivity;

        m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);

        UpdateVectors();
    }

    void Camera::UpdateVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);

        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    }

} // namespace engine
