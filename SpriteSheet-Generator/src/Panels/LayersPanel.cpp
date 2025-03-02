


#define IMGUI_DEFINE_MATH_OPERATORS

#include "LayersPanel.h"
#include "Frame.h"


static bool CustomTextButton(const char* label, int id, ImFont* font, const ImVec2 pos, const ImVec2 padding, const ImVec2 size);

void LayersPanel::SetImguiProperties(){
	ImGuiWindowClass window_class;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);
}

void LayersPanel::OnImguiUiUpdate(){
	m_IsInFocus        = ImGui::IsWindowFocused();
	auto  frame        = (SpriteGeneratorFrame*)GetParentFrame();
	auto& currentPage  = frame->GetCurrentPage();

	auto regionSize = ImGui::GetContentRegionAvail();
	auto cursor     = ImGui::GetCursorScreenPos();
	auto drawList   = ImGui::GetWindowDrawList();

	// Title of Pages section
	{
		const char* titleLabel    = "Pages";
		ImVec2      titleTextSize = ImGui::CalcTextSize(titleLabel);

		// title lable
		auto titleFont = Quirk::FontManager::GetFont(Quirk::FontWeight::Medium, 25);
		drawList->AddText(titleFont, titleFont->FontSize, cursor, 0xFFFFFFFF, titleLabel);

		// new page creation button
		{
			const char* label = "+";

			ImGui::PushFont(titleFont);
			const ImVec2 buttonPadding = { 17.0f, 5.0f };
			const ImVec2 buttonSize    = ImGui::CalcTextSize(label) + buttonPadding;
			const ImVec2 buttonPos     = { cursor.x + regionSize.x - buttonSize.x, cursor.y };
			ImGui::PopFont();

			// if button clicked adding new page to the list
			if (CustomTextButton(label, 1, titleFont, buttonPos, buttonPadding, buttonSize)) {
				frame->AddNewPage();
			}
		}

		cursor.y += (titleFont->FontSize) + 8.0f;

		// separator line
		const float windowPosX  = ImGui::GetWindowPos().x;
		const float windowWidth = ImGui::GetWindowWidth();
		drawList->AddLine({ windowPosX, cursor.y }, { windowPosX + windowWidth, cursor.y }, 0xFF303030);

		cursor.y += 10.0f;
	}

	// displaying all the pages name to select one
	{
		const auto& pages = frame->GetPages();
		const auto  font  = Quirk::FontManager::GetFont(Quirk::FontWeight::Regular, 22);

		const ImVec2 buttonPadding   = { 20.0f, 10.0f };
		const ImVec2 buttonSize      = { regionSize.x, font->FontSize + buttonPadding.y };

		const char* crossButtonLabel = "x";
		ImGui::PushFont(font);
		ImVec2 crossButtonSize = ImGui::CalcTextSize(crossButtonLabel) + buttonPadding;
		ImGui::PopFont();

		ImVec2 buttonPos      = cursor;
		ImVec2 crossButtonPos = buttonPos + buttonSize - crossButtonSize;

		for (size_t i = 0; i < pages.size(); ++i) {
			// button to set current page
			const char* label      = pages[i]->GetNamePtr();

			ImGui::SetNextItemAllowOverlap();
			if (CustomTextButton(label, (int)i, font, buttonPos, buttonPadding, buttonSize)) {
				frame->SetCurrentPage(pages[i]);
			}

			// button to delete this page
			if (CustomTextButton(crossButtonLabel, (int)i, font, crossButtonPos, buttonPadding, crossButtonSize)) {
				frame->DeletePage(pages[i--]);
			}

			buttonPos.y      += buttonSize.y + 5.0f;
			crossButtonPos.y += buttonSize.y + 5.0f;
		}
	}
}

static bool CustomTextButton(const char* label, int id, ImFont* font, const ImVec2 pos, const ImVec2 padding, const ImVec2 size) {
	ImGui::PushID(id);
	ImGui::SetCursorScreenPos(pos);

	const ImVec2 buttonEnd = pos + size;

	bool isClicked = ImGui::InvisibleButton(label, size);
	bool isHovered = ImGui::IsItemHovered();
	bool isActive  = isHovered && ImGui::IsMouseDown(0);

	auto drawList = ImGui::GetWindowDrawList();

	ImU32 buttonColor = isHovered ? isActive ? 0xff303020 : 0xff272720 : 0xff303020;
	drawList->AddRectFilled(pos, buttonEnd, buttonColor, 8.0f);

	ImVec2 textPos = pos + (padding * 0.5f);
	drawList->AddText(font, font->FontSize, textPos, 0xFFFFFFFF, label);

	ImGui::PopID();
	return isClicked;
}
