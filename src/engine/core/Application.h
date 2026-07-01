#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "engine/core/Window.h"
#include "engine/render/Shader.h"
#include "engine/render/Camera.h"

namespace engine
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();

    private:
        void Update(float deltaTime);
        void Render();
        void RenderDebugUI();

        std::unique_ptr<Window> m_Window;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Camera> m_Camera;

        unsigned int m_VAO = 0;
        unsigned int m_VBO = 0;

        glm::vec3 m_ClearColor{0.10f, 0.42f, 0.20f}; // farm-field green
        float m_TimeAccum = 0.0f;
        int m_FrameCount = 0;
        float m_Fps = 0.0f;

        bool m_CameraControlEnabled = true;
        bool m_AltWasPressed = false;
    };
} // namespace engine
