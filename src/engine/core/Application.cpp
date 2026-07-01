#include "engine/core/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace engine
{

    // A single triangle in NDC space, colored per-vertex — this is the
    // "hello world" of the renderer. Swap this out for real mesh loading
    // (via Assimp) once the pipeline is proven.
    static const float kTriangleVertices[] = {
        // positions          // colors
        0.0f,
        0.5f,
        0.0f,
        0.9f,
        0.7f,
        0.2f,
        -0.5f,
        -0.5f,
        0.0f,
        0.3f,
        0.6f,
        0.3f,
        0.5f,
        -0.5f,
        0.0f,
        0.5f,
        0.35f,
        0.2f,
    };

    Application::Application()
    {
        m_Window = std::make_unique<Window>(1280, 720, "Farm Engine");

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeHandle(), true);
        ImGui_ImplOpenGL3_Init("#version 410");

        m_Shader = std::make_unique<Shader>(
            "assets/shaders/basic.vert",
            "assets/shaders/basic.frag");

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(kTriangleVertices), kTriangleVertices, GL_STATIC_DRAW);

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
    }

    void Application::Render()
    {
        glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_Shader->Bind();
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
