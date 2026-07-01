#pragma once

#include <string>

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

    private:
        GLFWwindow *m_Handle = nullptr;
        int m_Width;
        int m_Height;
    };
} // namespace engine
