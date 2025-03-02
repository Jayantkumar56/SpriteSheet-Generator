




#include "Quirk.h"
#include "TitleBar.h"
#include "Theme.h"

void SpriteGeneratorTitleBar::SetImguiProperties() {
	ImGuiStyle& style = ImGui::GetStyle();
	style.FramePadding.y = 8.0f;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
}

void SpriteGeneratorTitleBar::UnSetImguiProperties() {
	ImGuiStyle& style = ImGui::GetStyle();
	style.FramePadding.y = 5.0f;

	ImGui::PopStyleVar();
}

void SpriteGeneratorTitleBar::OnImguiUiUpdate() {
	ImGui::PushStyleColor(ImGuiCol_Border, Theme::GetColor(ColorName::PopupBorder));

	if (ImGui::BeginMenu("File")) {
		if (ImGui::MenuItem("Exit")) {
			Quirk::WindowCloseEvent event;
			Quirk::Application::Get().OnWindowClose(event);
		}

		ImGui::EndMenu();
	}

	float buttonSize = 23.0f;
	float buttonPadding = 9.0f;
	ImVec2 windowSize = ImGui::GetWindowSize();

	auto framePadding = ImGui::GetStyle().FramePadding;
	float buttonY = (windowSize.y - buttonSize - (2 * framePadding.y)) * 0.5f;
	float buttonX = windowSize.x - (3 * buttonSize) - 2 * (buttonPadding + framePadding.x);

	ImGui::SetCursorPos({ buttonX, buttonY });
	ImTextureID minimiseIconId = (ImTextureID)(intptr_t)m_MinimiseIcon->GetRendererId();
	ImGui::ImageButton(minimiseIconId, { buttonSize, buttonSize }, { 0, 1 }, { 1, 0 });
	SetCursorOverMinimiseButton(ImGui::IsItemHovered());

	ImGui::SetCursorPos({ buttonX + buttonSize + buttonPadding, buttonY });
	ImTextureID maximiseIconId = (ImTextureID)(intptr_t)m_MaximiseIcon->GetRendererId();
	ImGui::ImageButton(maximiseIconId, { buttonSize, buttonSize }, { 0, 1 }, { 1, 0 });
	SetCursorOverMaximiseButton(ImGui::IsItemHovered());

	ImGui::SetCursorPos({ buttonX + 2 * (buttonSize + buttonPadding), buttonY });
	ImTextureID closeIconId = (ImTextureID)(intptr_t)m_CloseIcon->GetRendererId();
	ImGui::ImageButton(closeIconId, { buttonSize, buttonSize }, { 0, 1 }, { 1, 0 });
	SetCursorOverCloseButton(ImGui::IsItemHovered());

	ImGui::PopStyleColor();
}

