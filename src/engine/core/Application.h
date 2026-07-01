#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "engine/core/Window.h"
#include "engine/render/Shader.h"
#include "engine/render/Camera.h"
#include "engine/render/Mesh.h"
#include "engine/ecs/Entity.h"
#include "engine/assets/ModelLoader.h"

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
        void RenderShopUI();

        Entity *FindEntityAtTile(int tileX, int tileZ);

        std::unique_ptr<Window> m_Window;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Camera> m_Camera;

        std::unique_ptr<Mesh> m_CubeMesh;
        std::unique_ptr<Mesh> m_GroundMesh;
        std::unique_ptr<Mesh> m_HighlightMesh;
        std::unique_ptr<Mesh> m_CropModel;

        std::vector<Entity> m_Entities;

        glm::vec3 m_ClearColor{0.10f, 0.42f, 0.20f}; // farm-field green
        float m_TimeAccum = 0.0f;
        int m_FrameCount = 0;
        float m_Fps = 0.0f;

        bool m_CameraControlEnabled = true;
        bool m_AltWasPressed = false;

        float m_TileSize = 1.0f;
        bool m_HasHoveredTile = false;
        int m_HoveredTileX = 0;
        int m_HoveredTileZ = 0;
        glm::vec3 m_HoveredWorldPoint{0.0f};

        bool m_LeftMouseWasPressed = false;
        bool m_HasClickedTile = false;
        int m_ClickedTileX = 0;
        int m_ClickedTileZ = 0;

        bool m_RightMouseWasPressed = false;

        glm::vec3 m_LightDir{-0.4f, -1.0f, -0.3f};
        float m_CropModelScale = 0.15f;
        float m_SecondsPerGrowthStage = 5.0f; // 5s for testing

        int m_CornInventory = 0;
        int m_Coins = 0;
        int m_CornSellPrice = 5;
    };
} // namespace engine
