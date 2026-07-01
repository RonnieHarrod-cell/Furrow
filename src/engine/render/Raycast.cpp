#include "engine/render/Raycast.h"

#include <glm/gtc/matrix_inverse.hpp>

namespace engine
{
    bool ScreenToGroundPoint(
        glm::vec2 mousePos,
        int screenWidth,
        int screenHeight,
        const glm::vec3 &cameraPos,
        const glm::mat4 &view,
        const glm::mat4 &projection,
        glm::vec3 &outPoint)
    {
        float ndcX = (2.0f * mousePos.x) / static_cast<float>(screenWidth) - 1.0f;
        float ndcY = 1.0f - (2.0f * mousePos.y) / static_cast<float>(screenHeight);

        glm::vec4 rayClip(ndcX, ndcY, -1.0f, 1.0f);

        glm::vec4 rayEye = glm::inverse(projection) * rayClip;
        rayEye.z = -1.0f;
        rayEye.w = 0.0f;

        glm::vec3 rayWorldDir = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));

        if (std::abs(rayWorldDir.y) < 1e-6f)
        {
            return false;
        }

        float t = -cameraPos.y / rayWorldDir.y;
        if (t < 0.0f)
        {
            return false;
        }

        outPoint = cameraPos + rayWorldDir * t;
        return true;
    }
} // namespace engine