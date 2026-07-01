#include "engine/core/Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <stdexcept>

namespace engine
{
    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    Window::Window(int width, int height, const std::string &title)
        : m_Width(width), m_Height(height)
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

        m_Handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!m_Handle)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_Handle);
        glfwSetFramebufferSizeCallback(m_Handle, FramebufferSizeCallback);
        glfwSetWindowUserPointer(m_Handle, this);
        glfwSetCursorPosCallback(m_Handle, MouseCallback);
        glfwSwapInterval(1); // vsync

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        std::printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    }

    Window::~Window()
    {
        if (m_Handle)
        {
            glfwDestroyWindow(m_Handle);
        }
        glfwTerminate();
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Handle);
    }

    void Window::SwapBuffers()
    {
        glfwSwapBuffers(m_Handle);
    }

    void Window::PollEvents()
    {
        glfwPollEvents();
    }

    void Window::SetCursorLocked(bool locked)
    {
        glfwSetInputMode(m_Handle, GLFW_CURSOR, locked ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
    }

    bool Window::GetKey(int key) const
    {
        return glfwGetKey(m_Handle, key) == GLFW_PRESS;
    }

    glm::vec2 Window::GetMouseDelta()
    {
        glm::vec2 delta(static_cast<float>(m_MouseDeltaX), static_cast<float>(m_MouseDeltaY));
        m_MouseDeltaX = 0.0;
        m_MouseDeltaY = 0.0;
        return delta;
    }

    glm::vec2 Window::GetMousePosition() const
    {
        return glm::vec2(static_cast<float>(m_LastMouseX), static_cast<float>(m_LastMouseY));
    }

    void Window::MouseCallback(GLFWwindow *window, double xpos, double ypos)
    {
        Window *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
        if (!self)
            return;

        if (self->m_FirstMouse)
        {
            self->m_LastMouseX = xpos;
            self->m_LastMouseY = ypos;
            self->m_FirstMouse = false;
            return;
        }

        self->m_MouseDeltaX += xpos - self->m_LastMouseX;
        self->m_MouseDeltaY += self->m_LastMouseY - ypos;

        self->m_LastMouseX = xpos;
        self->m_LastMouseY = ypos;
    }

    bool Window::GetMouseButton(int button) const
    {
        return glfwGetMouseButton(m_Handle, button) == GLFW_PRESS;
    }
} // namespace engine
