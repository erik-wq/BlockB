#pragma once
#include "MenuState.h"

class Level;

const float CollumnSize = 200;
const float RowSize = 55;

class ControlMenu : public MenuState
{
public:
	ControlMenu(Level* level) : MenuState(level) {};

	virtual void UpdateUI(float widht, float height, float dt) override;
private:
	void RenderMappings();

	void SetCursorToCenterOfCell(const char* string, int collum, float tableOffset);
};