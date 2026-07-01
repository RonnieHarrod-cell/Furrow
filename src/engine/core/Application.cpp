#include "engine/core/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace engine
{

    static const float kCubeVertices[] = {
        // positions          // colors
        // Front face (red-ish)
        -0.5f,
        -0.5f,
        0.5f,
        0.8f,
        0.3f,
        0.3f,
        0.5f,
        -0.5f,
        0.5f,
        0.8f,
        0.3f,
        0.3f,
        0.5f,
        0.5f,
        0.5f,
        0.8f,
        0.3f,
        0.3f,
        0.5f,
        0.5f,
        0.5f,
        0.8f,
        0.3f,
        0.3f,
        -0.5f,
        0.5f,
        0.5f,
        0.8f,
        0.3f,
        0.3f,
        -0.5f,
        -0.5f,
        0.5f,
        0.8f,
        0.3f,
        0.3f,

        // Back face (blue-ish)
        -0.5f,
        -0.5f,
        -0.5f,
        0.3f,
        0.4f,
        0.8f,
        -0.5f,
        0.5f,
        -0.5f,
        0.3f,
        0.4f,
        0.8f,
        0.5f,
        0.5f,
        -0.5f,
        0.3f,
        0.4f,
        0.8f,
        0.5f,
        0.5f,
        -0.5f,
        0.3f,
        0.4f,
        0.8f,
        0.5f,
        -0.5f,
        -0.5f,
        0.3f,
        0.4f,
        0.8f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.3f,
        0.4f,
        0.8f,

        // Left face (green-ish)
        -0.5f,
        0.5f,
        0.5f,
        0.3f,
        0.7f,
        0.4f,
        -0.5f,
        0.5f,
        -0.5f,
        0.3f,
        0.7f,
        0.4f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.3f,
        0.7f,
        0.4f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.3f,
        0.7f,
        0.4f,
        -0.5f,
        -0.5f,
        0.5f,
        0.3f,
        0.7f,
        0.4f,
        -0.5f,
        0.5f,
        0.5f,
        0.3f,
        0.7f,
        0.4f,

        // Right face (yellow-ish)
        0.5f,
        0.5f,
        0.5f,
        0.8f,
        0.7f,
        0.3f,
        0.5f,
        -0.5f,
        -0.5f,
        0.8f,
        0.7f,
        0.3f,
        0.5f,
        0.5f,
        -0.5f,
        0.8f,
        0.7f,
        0.3f,
        0.5f,
        -0.5f,
        -0.5f,
        0.8f,
        0.7f,
        0.3f,
        0.5f,
        0.5f,
        0.5f,
        0.8f,
        0.7f,
        0.3f,
        0.5f,
        -0.5f,
        0.5f,
        0.8f,
        0.7f,
        0.3f,

        // Bottom face (purple-ish)
        -0.5f,
        -0.5f,
        -0.5f,
        0.6f,
        0.3f,
        0.7f,
        0.5f,
        -0.5f,
        -0.5f,
        0.6f,
        0.3f,
        0.7f,
        0.5f,
        -0.5f,
        0.5f,
        0.6f,
        0.3f,
        0.7f,
        0.5f,
        -0.5f,
        0.5f,
        0.6f,
        0.3f,
        0.7f,
        -0.5f,
        -0.5f,
        0.5f,
        0.6f,
        0.3f,
        0.7f,
        -0.5f,
        -0.5f,
        -0.5f,
        0.6f,
        0.3f,
        0.7f,

        // Top face (orange-ish)
        -0.5f,
        0.5f,
        -0.5f,
        0.9f,
        0.5f,
        0.2f,
        0.5f,
        0.5f,
        0.5f,
        0.9f,
        0.5f,
        0.2f,
        0.5f,
        0.5f,
        -0.5f,
        0.9f,
        0.5f,
        0.2f,
        0.5f,
        0.5f,
        0.5f,
        0.9f,
        0.5f,
        0.2f,
        -0.5f,
        0.5f,
        -0.5f,
        0.9f,
        0.5f,
        0.2f,
        -0.5f,
        0.5f,
        0.5f,
        0.9f,
        0.5f,
        0.2f,
    };

    Application::Application()
    {
        m_Window = std::make_unique<Window>(1280, 720, "Furrow");
        m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        m_Window->SetCursorLocked(true);

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeHandle(), true);
        ImGui_ImplOpenGL3_Init("#version 410");

        m_Shader = std::make_unique<Shader>(
            "assets/shaders/basic.vert",
            "assets/shaders/basic.frag");

        glEnable(GL_DEPTH_TEST);

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(kCubeVertices), kCubeVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    Application::~Application()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Application::Run()
    {
        double lastTime = glfwGetTime();

        while (!m_Window->ShouldClose())
        {
            double now = glfwGetTime();
            float deltaTime = static_cast<float>(now - lastTime);
            lastTime = now;

            m_Window->PollEvents();
            Update(deltaTime);
            Render();
            m_Window->SwapBuffers();
        }
    }

    void Application::Update(float deltaTime)
    {
        m_TimeAccum += deltaTime;
        m_FrameCount++;
        if (m_TimeAccum >= 0.5f)
        {
            m_Fps = m_FrameCount / m_TimeAccum;
            m_TimeAccum = 0.0f;
            m_FrameCount = 0;
        }

        bool forward = m_Window->GetKey(GLFW_KEY_W);
        bool backward = m_Window->GetKey(GLFW_KEY_S);
        bool left = m_Window->GetKey(GLFW_KEY_A);
        bool right = m_Window->GetKey(GLFW_KEY_D);
        m_Camera->ProcessKeyboard(forward, backward, left, right, deltaTime);

        glm::vec2 mouseDelta = m_Window->GetMouseDelta();
        if (m_CameraControlEnabled)
        {
            m_Camera->ProcessMouseMovement(mouseDelta.x, mouseDelta.y);
        }

        bool altPressed = m_Window->GetKey(GLFW_KEY_LEFT_ALT);
        if (altPressed && !m_AltWasPressed)
        {
            m_CameraControlEnabled = !m_CameraControlEnabled;
            m_Window->SetCursorLocked(m_CameraControlEnabled);
        }
        m_AltWasPressed = altPressed;
    }

    void Application::Render()
    {
        glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float aspect = static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight());
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = m_Camera->GetViewMatrix();
        glm::mat4 procjection = m_Camera->GetProjectionMatrix(aspect);
        glm::mat4 mvp = procjection * view * model;

        m_Shader->Bind();
        m_Shader->SetMat4("uMVP", mvp);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        RenderDebugUI();
    }

    void Application::RenderDebugUI()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Engine debug");
        ImGui::Text("FPS: %.1f", m_Fps);
        ImGui::ColorEdit3("Clear color", &m_ClearColor.x);
        ImGui::Text("This panel is your engine's dev console.");
        ImGui::Text("Add crop/tile/inventory debug widgets here as you build them.");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

} // namespace engine
