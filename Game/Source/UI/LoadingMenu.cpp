#include "UI/LoadingMenu.h"
#include "Application/GameApp.h"
#include "MathUtils.h"

LoadingMenu::LoadingMenu(Level* level) : MenuState(level)
{
}

void LoadingMenu::UpdateUI(float widht, float height, float dt)
{
	ImGui::SetNextWindowPos(ImVec2(widht * 0.25f, height * 0.25f), ImGuiCond_Always);
	CreateCustomWindow(widht * 0.5f, height * 0.5f, "Loading");

	ImVec2 windowSize = ImGui::GetWindowSize();

	float fontSize = 4;

	ImVec2 textPosition((windowSize.x - ImGui::CalcTextSize("Loading").x * fontSize) * 0.5f, 40);

	ImGui::SetWindowFontScale(fontSize);

	ImGui::SetCursorPos(textPosition);
	ImGui::Text("Loading");

	ImGui::SetWindowFontScale(1);

	target = level->GetGameInstance()->GetProgress();
	UpdateProgress(dt);

	if (target == 1)
	{
		progres = 1;
	}

	ImGui::SetCursorPos(ImVec2((windowSize.x - 400) * 0.5f, 180));
	ImGui::ProgressBar(progres, ImVec2(400, 40), "");

	// render loading percentage using ImGui::Text
	ImGui::SetWindowFontScale(2);	

	textPosition = { (windowSize.x - ImGui::CalcTextSize("Loading: %.0f%%").x) * 0.5f, 240 };
	ImGui::SetCursorPos(textPosition);
	ImGui::Text("Loading: %.0f%%", MathUtils::Lerp(0, 100, progres));

	ImGui::SetWindowFontScale(1);
	FinishWindow();
}

void LoadingMenu::UpdateProgress(float dt)
{
	if (progres >= target)
		return;

	progres += 0.3f * dt;
	if(progres > 1)
		progres = 1;
}
