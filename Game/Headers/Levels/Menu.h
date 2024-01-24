#pragma once
#include "BaseLevel.h"

class GameApp;
class Factory;

class MenuLevel : public Level
{
public:
	MenuLevel(GameApp* game, Factory* factory);
	~MenuLevel();
};