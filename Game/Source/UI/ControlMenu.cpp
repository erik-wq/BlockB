#include "UI/ControlMenu.h"
#include "UI/MainMenu.h"

void ControlMenu::UpdateUI(float widht, float height, float)
{
	ImGui::SetNextWindowPos(ImVec2(widht * 0.25f, height * 0.25f), ImGuiCond_Always);
	CreateCustomWindow(widht * 0.5f, height * 0.5f, "Levels");

	ImVec2 windowSize = ImGui::GetWindowSize();

	float fontSize = 4;

	ImVec2 textPosition((windowSize.x - ImGui::CalcTextSize("Controls").x * fontSize) * 0.5f, 40);

	ImGui::SetWindowFontScale(fontSize);

	ImGui::SetCursorPos(textPosition);
	ImGui::Text("Controls");

	ImGui::SetWindowFontScale(1);

	RenderMappings();

	SetButtonStyle();

	ImVec2 buttonPosition = { 475, 437 };
	ImVec2 buttonSize = { 125, 45 };
	ImGui::SetCursorPos(buttonPosition);
	if (ImGui::Button("BACK", buttonSize)) {
		level->ChangeUIState(new MainMenu(level));
	}

	ClearButtonStyle();

	ImGui::SetWindowFontScale(1);
	FinishWindow();
}

void ControlMenu::RenderMappings()
{
	ImGui::SetWindowFontScale(2);

	ImGui::SetCursorPos(ImVec2(120, 160));

	if (ImGui::BeginTable("Mappings", 2))
	{
		ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_WidthFixed, CollumnSize);
		ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, CollumnSize);

		ImGui::TableNextRow(ImGuiTableRowFlags_None, RowSize);
		ImGui::TableSetColumnIndex(0);
		SetCursorToCenterOfCell("Action", 0, 120);
		ImGui::Text("Action");
		ImGui::TableSetColumnIndex(1);
		SetCursorToCenterOfCell("Key", 1, 120);
		ImGui::Text("Key");

		for (int row = 0; row < 3; row++) // 3 is the number of rows
		{
			ImGui::TableNextRow(ImGuiTableRowFlags_None, RowSize);
			for (int column = 0; column < 2; column++)
			{
				ImGui::TableSetColumnIndex(column);
				SetCursorToCenterOfCell("WOW", column, 120);
				ImGui::Text("WOW", column, row);
			}
		}
		ImGui::EndTable();
	}

	ImGui::SetWindowFontScale(1);
}

void ControlMenu::SetCursorToCenterOfCell(const char* string, int collum, float tableOffset)
{
	float textWidth = ImGui::CalcTextSize(string).x;
	float cellWidth = ImGui::GetContentRegionAvail().x;
	ImGui::SetCursorPosX((cellWidth - textWidth) * 0.5f + (collum * CollumnSize) + tableOffset);
}
