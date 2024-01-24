#include "Levels/Menu.h"
#include <iostream>
#include "imgui_impl_opengl3.h"
#include "App/WindowSystem.h"
#include "Application/GameApp.h"
#include "UI/MenuState.h"
#include "UI/MainMenu.h"

MenuLevel::MenuLevel(GameApp* game, Factory* factory) :	Level(game, factory)
{
    menuState = new MainMenu(this);
}

MenuLevel::~MenuLevel()
{
    delete menuState;
}