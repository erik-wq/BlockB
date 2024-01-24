#pragma once
#include "imgui_impl_opengl3.h"
#include "Levels/BaseLevel.h"

class MenuState
{
public:
	MenuState(Level* level);

	virtual ~MenuState() {};

	virtual void UpdateUI(float widht, float height, float dt) = 0;

protected:
	void SetButtonStyle();

	void ClearButtonStyle();

	void CreateCustomWindow(float widht, float height, const char* name);

	void FinishWindow();

	Level* level;
};