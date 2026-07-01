#pragma once

#include <string>

#include <glm/glm.hpp>

struct GLFWwindow;

namespace engine
{
    class Window
    {
    public:
        Window(int width, int height, const std::string &title);
        ~Window();

        bool ShouldClose() const;
        void SwapBuffers();
        void PollEvents();

        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }
        GLFWwindow *GetNativeHandle() const { return m_Handle; }

        bool GetKey(int key) const;

        glm::vec2 GetMouseDelta();

        void SetCursorLocked(bool locked);

    private:
        GLFWwindow *m_Handle = nullptr;
        int m_Width;
        int m_Height;

        static void MouseCallback(GLFWwindow *window, double xpos, double ypos);

        double m_LastMouseX = 0.0;
        double m_LastMouseY = 0.0;
        double m_MouseDeltaX = 0.0;
        double m_MouseDeltaY = 0.0;
        bool m_FirstMouse = true;
    };
} // namespace engine
