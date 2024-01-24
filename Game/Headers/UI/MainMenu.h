#pragma once
#include "MenuState.h"

class Level;

class MainMenu : public MenuState
{
	public:
		MainMenu(Level* level) : MenuState(level) {};
		virtual void UpdateUI(float widht, float height, float dt) override;
};