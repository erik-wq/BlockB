#include "App/InputSystem.h"

InputSystem::InputSystem()
{
   
}

MouseData InputSystem::GetMouseData()
{
    MouseData data = MouseData{ mouseDeltaX, mouseDeltaY, mouseX, mouseY };
    mouseDeltaX = 0;
    mouseDeltaY = 0;
    return data;
}

