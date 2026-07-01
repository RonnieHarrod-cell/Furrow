#include "engine/core/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <cmath>
#include <algorithm>
#include "engine/render/Raycast.h"

namespace engine
{
    static void BuildCube(std::vector<Vertex> &outVertices, std::vector<unsigned int> &outIndices)
    {
        outVertices = {
            // Front (+Z)
            {{-0.5f, -0.5f, 0.5f}, {0, 0, 1}},
            {{0.5f, -0.5f, 0.5f}, {0, 0, 1}},
            {{0.5f, 0.5f, 0.5f}, {0, 0, 1}},
            {{-0.5f, 0.5f, 0.5f}, {0, 0, 1}},
            // Back (-Z)
            {{0.5f, -0.5f, -0.5f}, {0, 0, -1}},
            {{-0.5f, -0.5f, -0.5f}, {0, 0, -1}},
            {{-0.5f, 0.5f, -0.5f}, {0, 0, -1}},
            {{0.5f, 0.5f, -0.5f}, {0, 0, -1}},
            // Left (-X)
            {{-0.5f, -0.5f, -0.5f}, {-1, 0, 0}},
            {{-0.5f, -0.5f, 0.5f}, {-1, 0, 0}},
            {{-0.5f, 0.5f, 0.5f}, {-1, 0, 0}},
            {{-0.5f, 0.5f, -0.5f}, {-1, 0, 0}},
            // Right (+X)
            {{0.5f, -0.5f, 0.5f}, {1, 0, 0}},
            {{0.5f, -0.5f, -0.5f}, {1, 0, 0}},
            {{0.5f, 0.5f, -0.5f}, {1, 0, 0}},
            {{0.5f, 0.5f, 0.5f}, {1, 0, 0}},
            // Bottom (-Y)
            {{-0.5f, -0.5f, -0.5f}, {0, -1, 0}},
            {{0.5f, -0.5f, -0.5f}, {0, -1, 0}},
            {{0.5f, -0.5f, 0.5f}, {0, -1, 0}},
            {{-0.5f, -0.5f, 0.5f}, {0, -1, 0}},
            // Top (+Y)
            {{-0.5f, 0.5f, 0.5f}, {0, 1, 0}},
            {{0.5f, 0.5f, 0.5f}, {0, 1, 0}},
            {{0.5f, 0.5f, -0.5f}, {0, 1, 0}},
            {{-0.5f, 0.5f, -0.5f}, {0, 1, 0}},
        };

        outIndices.clear();
        for (unsigned int face = 0; face < 6; face++)
        {
            unsigned int base = face * 4;
            outIndices.push_back(base + 0);
            outIndices.push_back(base + 1);
            outIndices.push_back(base + 2);
            outIndices.push_back(base + 2);
            outIndices.push_back(base + 3);
            outIndices.push_back(base + 0);
        }
    }

    static void BuildGround(float size, std::vector<Vertex> &outVertices, std::vector<unsigned int> &outIndices)
    {
        float h = size * 0.5f;
        outVertices = {
            {{-h, 0.0f, -h}, {0, 1, 0}},
            {{h, 0.0f, -h}, {0, 1, 0}},
            {{h, 0.0f, h}, {0, 1, 0}},
            {{-h, 0.0f, h}, {0, 1, 0}},
        };
        outIndices = {0, 1, 2, 2, 3, 0};
    }

    static void BuildHighlight(float tileSize, std::vector<Vertex> &outVertices, std::vector<unsigned int> &outIndices)
    {
        float y = 0.01f;
        outVertices = {
            {{0.0f, y, 0.0f}, {0, 1, 0}},
            {{tileSize, y, 0.0f}, {0, 1, 0}},
            {{tileSize, y, tileSize}, {0, 1, 0}},
            {{0.0f, y, tileSize}, {0, 1, 0}},
        };
        outIndices = {0, 1, 2, 2, 3, 0};
    }

    Application::Application()
    {
        m_Window = std::make_unique<Window>(1280, 720, "Farm Engine");
        m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 2.0f, 5.0f));
        m_Window->SetCursorLocked(true);

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeHandle(), true);
        ImGui_ImplOpenGL3_Init("#version 410");

        m_Shader = std::make_unique<Shader>(
            "assets/shaders/basic.vert",
            "assets/shaders/basic.frag");

        glEnable(GL_DEPTH_TEST);

        std::vector<Vertex> verts;
        std::vector<unsigned int> indices;

        BuildCube(verts, indices);
        m_CubeMesh = std::make_unique<Mesh>(verts, indices);

        BuildGround(20.0f, verts, indices);
        m_GroundMesh = std::make_unique<Mesh>(verts, indices);

        BuildHighlight(m_TileSize, verts, indices);
        m_HighlightMesh = std::make_unique<Mesh>(verts, indices);

        m_CropModel = ModelLoader::Load("assets/models/corn-s1.obj");
    }

    Application::~Application()
    {
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

    Entity *Application::FindEntityAtTile(int tileX, int tileZ)
    {
        for (auto &entity : m_Entities)
        {
            if (entity.tileX == tileX && entity.tileZ == tileZ)
            {
                return &entity;
            }
        }
        return nullptr;
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

        bool altPressed = m_Window->GetKey(GLFW_KEY_LEFT_ALT);
        if (altPressed && !m_AltWasPressed)
        {
            m_CameraControlEnabled = !m_CameraControlEnabled;
            m_Window->SetCursorLocked(m_CameraControlEnabled);
        }
        m_AltWasPressed = altPressed;

        bool forward = m_Window->GetKey(GLFW_KEY_W);
        bool backward = m_Window->GetKey(GLFW_KEY_S);
        bool left = m_Window->GetKey(GLFW_KEY_A);
        bool right = m_Window->GetKey(GLFW_KEY_D);
        bool up = m_Window->GetKey(GLFW_KEY_E);
        bool down = m_Window->GetKey(GLFW_KEY_Q);
        m_Camera->ProcessKeyboard(forward, backward, left, right, up, down, deltaTime);

        glm::vec2 mouseDelta = m_Window->GetMouseDelta();
        if (m_CameraControlEnabled)
        {
            m_Camera->ProcessMouseMovement(mouseDelta.x, mouseDelta.y);
        }

        float aspect = static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight());
        glm::mat4 view = m_Camera->GetViewMatrix();
        glm::mat4 projection = m_Camera->GetProjectionMatrix(aspect);
        glm::vec2 mousePos = m_Window->GetMousePosition();

        glm::vec3 hitPoint;
        m_HasHoveredTile = ScreenToGroundPoint(
            mousePos,
            m_Window->GetWidth(), m_Window->GetHeight(),
            m_Camera->GetPosition(),
            view, projection,
            hitPoint);

        if (m_HasHoveredTile)
        {
            m_HoveredWorldPoint = hitPoint;
            m_HoveredTileX = static_cast<int>(std::floor(hitPoint.x / m_TileSize));
            m_HoveredTileZ = static_cast<int>(std::floor(hitPoint.z / m_TileSize));
        }

        bool leftPressed = m_Window->GetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
        if (leftPressed && !m_LeftMouseWasPressed && m_HasHoveredTile)
        {
            m_HasClickedTile = true;
            m_ClickedTileX = m_HoveredTileX;
            m_ClickedTileZ = m_HoveredTileZ;

            if (FindEntityAtTile(m_HoveredTileX, m_HoveredTileZ) == nullptr)
            {
                m_Entities.push_back(Entity{EntityType::Crop, m_HoveredTileX, m_HoveredTileZ});
            }
        }
        m_LeftMouseWasPressed = leftPressed;

        bool rightPressed = m_Window->GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT);
        if (rightPressed && !m_RightMouseWasPressed && m_HasHoveredTile)
        {
            Entity *target = FindEntityAtTile(m_HoveredTileX, m_HoveredTileZ);
            if (target != nullptr && target->growthStage == GrowthStage::Grown)
            {
                m_CornInventory++;
                m_Entities.erase(
                    std::remove_if(m_Entities.begin(), m_Entities.end(),
                                   [&](const Entity &e)
                                   { return e.tileX == target->tileX && e.tileZ == target->tileZ; }),
                    m_Entities.end());
            }
        }
        m_RightMouseWasPressed = rightPressed;

        for (auto &entity : m_Entities)
        {
            if (entity.growthStage == GrowthStage::Grown)
            {
                continue; // fully grown, nothing left to do
            }

            entity.growthTimer += deltaTime;
            if (entity.growthTimer >= m_SecondsPerGrowthStage)
            {
                entity.growthTimer = 0.0f;
                if (entity.growthStage == GrowthStage::Seed)
                {
                    entity.growthStage = GrowthStage::Sprout;
                }
                else if (entity.growthStage == GrowthStage::Sprout)
                {
                    entity.growthStage = GrowthStage::Grown;
                }
            }
        }
    }

    void Application::Render()
    {
        glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float aspect = static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight());
        glm::mat4 view = m_Camera->GetViewMatrix();
        glm::mat4 projection = m_Camera->GetProjectionMatrix(aspect);

        m_Shader->Bind();
        m_Shader->SetMat4("uView", view);
        m_Shader->SetMat4("uProjection", projection);
        m_Shader->SetVec3("uLightDir", m_LightDir);

        // Ground
        m_Shader->SetMat4("uModel", glm::mat4(1.0f));
        m_Shader->SetVec3("uObjectColor", glm::vec3(0.25f, 0.55f, 0.25f));
        m_GroundMesh->Draw();

        // Hovered tile highlight
        if (m_HasHoveredTile)
        {
            glm::vec3 tileOrigin(m_HoveredTileX * m_TileSize, 0.0f, m_HoveredTileZ * m_TileSize);
            glm::mat4 highlightModel = glm::translate(glm::mat4(1.0f), tileOrigin);
            m_Shader->SetMat4("uModel", highlightModel);
            m_Shader->SetVec3("uObjectColor", glm::vec3(1.0f, 1.0f, 0.3f));
            m_HighlightMesh->Draw();
        }

        // Placed entities
        for (const auto &entity : m_Entities)
        {
            glm::vec3 pos(
                entity.tileX * m_TileSize + m_TileSize * 0.5f,
                0.0f,
                entity.tileZ * m_TileSize + m_TileSize * 0.5f);

            float stageScale = m_CropModelScale;
            switch (entity.growthStage)
            {
            case GrowthStage::Seed:
                stageScale *= 0.01f;
                break;
            case GrowthStage::Sprout:
                stageScale *= 0.03f;
                break;
            case GrowthStage::Grown:
                stageScale *= 0.05f;
                break;
            }

            glm::mat4 entityModel = glm::translate(glm::mat4(1.0f), pos);
            entityModel = glm::rotate(entityModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            entityModel = glm::scale(entityModel, glm::vec3(stageScale));
            m_Shader->SetMat4("uModel", entityModel);
            m_Shader->SetVec3("uObjectColor", glm::vec3(0.4f, 0.7f, 0.3f));
            m_CropModel->Draw();
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RenderDebugUI();
        RenderShopUI();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Application::RenderDebugUI()
    {
        ImGui::Begin("Engine debug");
        ImGui::Text("FPS: %.1f", m_Fps);
        ImGui::ColorEdit3("Clear color", &m_ClearColor.x);
        ImGui::Text("Camera control: %s (Left Alt to toggle)", m_CameraControlEnabled ? "ON" : "OFF");
        ImGui::Separator();
        if (m_HasHoveredTile)
        {
            ImGui::Text("Hovered tile: %d, %d", m_HoveredTileX, m_HoveredTileZ);
        }
        else
        {
            ImGui::Text("Hovered tile: none (looking away from ground)");
        }
        ImGui::Text("Entities placed: %zu", m_Entities.size());
        ImGui::Separator();
        ImGui::Text("Corn inventory: %d", m_CornInventory);
        ImGui::Text("Left click: plant   Right click: harvest (once grown)");
        ImGui::End();
    }

    void Application::RenderShopUI()
    {
        ImGui::Begin("Farm Shop");

        ImGui::Text("Corn: %d", m_CornInventory);
        ImGui::Text("Coins: %d", m_Coins);
        ImGui::Separator();

        ImGui::Text("Sell price: %d coins each", m_CornSellPrice);

        bool hasCornToSell = m_CornInventory > 0;
        if (!hasCornToSell)
        {
            ImGui::BeginDisabled();
        }
        if (ImGui::Button("Sell All Corn"))
        {
            m_Coins += m_CornInventory * m_CornSellPrice;
            m_CornInventory = 0;
        }
        if (!hasCornToSell)
        {
            ImGui::EndDisabled();
        }

        ImGui::End();
    }

} // namespace engine
