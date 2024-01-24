#include "WindowsInput.h"


WindowsInput::WindowsInput(GLFWwindow* window) : InputSystem(), window(window)
{
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
}

WindowsInput::~WindowsInput()
{
}

bool WindowsInput::GetKey(char key)
{
    if (key < 97 || key > 122) return false;
    return glfwGetKey(window, key - 32);
}

bool WindowsInput::GetButton(char)
{
    return false;
}


void WindowsInput::KeyCallback(GLFWwindow*, int , int, int , int)
{
}

void WindowsInput::MouseButtonCallback(GLFWwindow*, int, int, int)
{
}

void WindowsInput::CursorPosCallback(GLFWwindow*, double, double)
{
}
