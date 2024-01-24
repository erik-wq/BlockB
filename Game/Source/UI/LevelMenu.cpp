#include "UI/LevelMenu.h"
#include "UI/MainMenu.h"
#include "UI/LoadingMenu.h"
#include "Application/GameApp.h"

void LevelMenu::UpdateUI(float widht, float height, float)
{
	ImGui::SetNextWindowPos(ImVec2(widht * 0.25f, height * 0.25f), ImGuiCond_Always);
	CreateCustomWindow(widht * 0.5f, height * 0.5f, "Levels");

	ImVec2 windowSize = ImGui::GetWindowSize();

	float fontSize = 4;

	ImVec2 textPosition((windowSize.x - ImGui::CalcTextSize("Levels").x * fontSize) * 0.5f, 40);

	ImGui::SetWindowFontScale(fontSize);

	ImGui::SetCursorPos(textPosition);
	ImGui::Text("Levels");

	ImGui::SetWindowFontScale(1);

	ImVec2 buttonSize(200, 65);

	ImGui::SetWindowFontScale(2);
	ImVec2 buttonPosition((windowSize.x - buttonSize.x) * 0.5f, 141.75f);

	ImGui::SetCursorPos(buttonPosition);

	SetButtonStyle();

	if (ImGui::Button("EASY", buttonSize)) {
		level->GetGameInstance()->UpdateLevel(Level_Easy);
		level->ChangeUIState(new LoadingMenu(level));
	}

	buttonPosition.y += 41.75f + buttonSize.y;
	ImGui::SetCursorPos(buttonPosition);
	if (ImGui::Button("NORMAL", buttonSize)) {
		level->GetGameInstance()->UpdateLevel(Level_Normal);
		level->ChangeUIState(new LoadingMenu(level));
	}


	buttonPosition.y += 41.75f + buttonSize.y;
	ImGui::SetCursorPos(buttonPosition);
	if (ImGui::Button("HARD", buttonSize)) {
		level->GetGameInstance()->UpdateLevel(Level_Hard);
		level->ChangeUIState(new LoadingMenu(level));
	}

	buttonPosition.y = 437;
	buttonPosition.x = 475;
	buttonSize = { 125, 45 };
	ImGui::SetCursorPos(buttonPosition);
	if (ImGui::Button("BACK", buttonSize)) {
		level->ChangeUIState(new MainMenu(level));
	}

	ClearButtonStyle();

	ImGui::SetWindowFontScale(1);
	FinishWindow();
}
