#pragma once

#include <glm/glm.hpp>

namespace engine
{
    bool ScreenToGroundPoint(
        glm::vec2 mousePos,
        int screenWidth,
        int screenHeight,
        const glm::vec3 &cameraPos,
        const glm::mat4 &view,
        const glm::mat4 &projection,
        glm::vec3 &outPoint);
} // namespace engine
