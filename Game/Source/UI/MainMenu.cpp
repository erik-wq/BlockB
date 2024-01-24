#include "UI/MainMenu.h"
#include "UI/LevelMenu.h"
#include "UI/ControlMenu.h"

void MainMenu::UpdateUI(float widht, float height, float)
{
	ImGui::SetNextWindowPos(ImVec2(widht * 0.25f, height * 0.25f), ImGuiCond_Always);
	CreateCustomWindow(widht * 0.5f, height * 0.5f, "Menu");

	ImVec2 windowSize = ImGui::GetWindowSize();

	float fontSize = 4;

	ImVec2 textPosition((windowSize.x - ImGui::CalcTextSize("Pitfall3D").x * fontSize) * 0.5f, 40);

	ImGui::SetWindowFontScale(fontSize);

	ImGui::SetCursorPos(textPosition);
	ImGui::Text("Pitfall3D");

	ImGui::SetWindowFontScale(1);

	ImVec2 buttonSize(200, 65);

	ImGui::SetWindowFontScale(2);
	ImVec2 buttonPosition((windowSize.x - buttonSize.x) * 0.5f, 141.75f);

	ImGui::SetCursorPos(buttonPosition);

	SetButtonStyle();
    
    if (ImGui::Button("Levels", buttonSize)) {
        level->ChangeUIState(new LevelMenu(level));
    }

	buttonPosition.y += 41.75f + buttonSize.y;
	ImGui::SetCursorPos(buttonPosition);
	if (ImGui::Button("Controls", buttonSize)) {
		level->ChangeUIState(new ControlMenu(level));
	}


	buttonPosition.y += 41.75f + buttonSize.y;
	ImGui::SetCursorPos(buttonPosition);
	if (ImGui::Button("Exit", buttonSize)) {
		level->CloseApplication();
	}

	ClearButtonStyle();

	ImGui::SetWindowFontScale(1);
	FinishWindow();
}