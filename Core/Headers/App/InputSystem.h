#pragma once
#include "Event.h"

#define Pressed 1
#define Released 0

struct MouseData
{
	int deltax;
	int deltay;
	int posx;
	int posy;
};

class InputSystem
{
public:
	InputSystem();
	virtual ~InputSystem() {};
	MouseData GetMouseData();
	void ProcessInput() { inputEvent.triggerEvent(this); }

	virtual bool GetKey(char key) = 0;

	virtual bool GetButton(char key) = 0;

	EventManager<InputSystem*> inputEvent;

protected:

	int mouseX = 0;
	int mouseY = 0;
	int mouseDeltaX = 0;
	int mouseDeltaY = 0;
};