#include "WindowsWindow.h"

WindowsWindow::WindowsWindow(int width, int height, const char* title)
{

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    screenSize.widht = width;
    screenSize.height = height;

    glfwMakeContextCurrent(window);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

WindowsWindow::~WindowsWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* WindowsWindow::GetWindow() const
{
	return window;
}

void WindowsWindow::Clear()
{
    glViewport(0, 0, screenSize.widht, screenSize.height);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void WindowsWindow::SwapBuffers()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}
