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
} // namespace engine
