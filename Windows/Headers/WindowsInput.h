#pragma once
#include "App/InputSystem.h"
#include <GLFW/glfw3.h>

class WindowsInput : public InputSystem
{
public:
	WindowsInput(GLFWwindow* window);
	~WindowsInput();

    virtual bool GetKey(char key) override;

    virtual bool GetButton(char key) override;

    GLFWwindow* GetWindow() const { return window; };
private:
    GLFWwindow* window;

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
};
