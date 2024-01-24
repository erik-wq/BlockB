#pragma once
#include "MenuState.h"

class Level;

class LoadingMenu : public MenuState
{
public:
		LoadingMenu(Level* level);

		virtual void UpdateUI(float widht, float height, float dt) override;

		void UpdateProgress(float dt);

		float progres = 0;

		float target = 0;
};