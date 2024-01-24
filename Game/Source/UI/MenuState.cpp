#include "UI/MenuState.h"
#include <iostream>

MenuState::MenuState(Level* level) : level(level)
{
	if (!level)
	{
		std::cout << "MenuState: Level is null potentional problems\n";
	}
}

void MenuState::SetButtonStyle()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(20, 35, 145)));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(ImColor(25, 49, 227)));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(ImColor(13, 24, 107)));
	ImGui::SetWindowFontScale(2);
}

void MenuState::ClearButtonStyle()
{
	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();
	ImGui::SetWindowFontScale(1);
}

void MenuState::CreateCustomWindow(float widht, float height, const char* name)
{
	ImGui::SetNextWindowSize(ImVec2(widht, height), ImGuiCond_Always);
	ImGui::Begin(name, nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
}

void MenuState::FinishWindow()
{
	ImGui::End();
}