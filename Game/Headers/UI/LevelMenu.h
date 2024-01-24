#pragma once
#include "MenuState.h"

class Level;

class LevelMenu : public MenuState
{
public:
	LevelMenu(Level* level) : MenuState(level) {};
	
	virtual void UpdateUI(float widht, float height, float dt) override;
};