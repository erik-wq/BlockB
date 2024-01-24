#pragma once
#include "App/WindowSystem.h"
#include <GLFW/glfw3.h>
#include <iostream>


class WindowsWindow : public WindowSystem
{
public:
	WindowsWindow(int width, int height, const char* title);
	~WindowsWindow();

	GLFWwindow* GetWindow() const;

	virtual void Clear() override;

	virtual void SwapBuffers() override;
private:
	GLFWwindow* window;
};
