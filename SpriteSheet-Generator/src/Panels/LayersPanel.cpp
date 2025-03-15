


#define IMGUI_DEFINE_MATH_OPERATORS

#include "LayersPanel.h"
#include "Frame.h"

#include <algorithm>

static bool CustomTextButton(const char* label, int id, ImFont* font, const ImVec2 pos, const ImVec2 padding, const ImVec2 size, ImU32 color);

void LayersPanel::SetImguiProperties(){
	ImGuiWindowClass window_class;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);
}

void LayersPanel::OnImguiUiUpdate(){
	m_IsInFocus        = ImGui::IsWindowFocused();
	auto  frame        = (SpriteGeneratorFrame*)GetParentFrame();
	auto& currentPage  = frame->GetCurrentPage();

	const auto windowWidth = ImGui::GetWindowWidth();
	const auto windowPos   = ImGui::GetWindowPos();
	const auto regionSize  = ImGui::GetContentRegionAvail();
	auto       cursor      = ImGui::GetCursorScreenPos();
	const auto drawList    = ImGui::GetWindowDrawList();
	int        buttonId    = 0;

	// Title of Pages section
	{
		const char* titleLabel    = "Pages";
		ImVec2      titleTextSize = ImGui::CalcTextSize(titleLabel);

		// title lable
		auto titleFont = Quirk::FontManager::GetFont(Quirk::FontWeight::Medium, 25);
		drawList->AddText(titleFont, titleFont->FontSize, cursor, 0xFFF8FF00, titleLabel);

		// new page creation button
		{
			const char* label = "+";

			ImGui::PushFont(titleFont);
			const ImVec2 buttonPadding = { 17.0f, 5.0f };
			const ImVec2 buttonSize    = ImGui::CalcTextSize(label) + buttonPadding;
			const ImVec2 buttonPos     = { cursor.x + regionSize.x - buttonSize.x, cursor.y };
			ImGui::PopFont();

			// if button clicked adding new page to the list
			if (CustomTextButton(label, buttonId++, titleFont, buttonPos, buttonPadding, buttonSize, 0xff303020)) {
				frame->AddNewPage();
			}

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
				ImGui::SetTooltip("Add new page", ImGui::GetStyle().HoverDelayNormal);
		}

		cursor.y += (titleFont->FontSize) + 8.0f;
		// separator line
		drawList->AddLine({ windowPos.x, cursor.y }, { windowPos.x + windowWidth, cursor.y }, 0xFF303030);
		cursor.y += 10.0f;
	}

	// displaying all the pages name to select one
	{
		const auto& pages = frame->GetPages();
		const auto  font  = Quirk::FontManager::GetFont(Quirk::FontWeight::Regular, 22);

		const ImVec2 buttonPadding    = { 20.0f, 10.0f };
		const ImVec2 pagesSectionSize = { regionSize.x, 3 * (font->FontSize + buttonPadding.y + 5.0f) };

		const ImVec2 regionSize = ImGui::GetContentRegionAvail();
		const ImVec2 buttonSize = { regionSize.x, font->FontSize + buttonPadding.y };

		const char* crossButtonLabel = "x";
		ImGui::PushFont(font);
		ImVec2 crossButtonSize = ImGui::CalcTextSize(crossButtonLabel) + buttonPadding;
		ImGui::PopFont();

		ImVec2 buttonPos      = cursor;
		ImVec2 crossButtonPos = buttonPos + buttonSize - crossButtonSize;

		for (size_t i = 0; i < pages.size(); ++i) {
			const char* label = pages[i]->GetName().c_str();
			ImU32 buttonColor = currentPage.get() == pages[i].get() ? 0xff303020 : 0x0000;

			// button to set current page
			ImGui::SetNextItemAllowOverlap();
			if (CustomTextButton(label, buttonId++, font, buttonPos, buttonPadding, buttonSize, buttonColor)) {
				frame->SetCurrentPage(pages[i]);
			}

			// button to delete this page
			if (CustomTextButton(crossButtonLabel, buttonId++, font, crossButtonPos, buttonPadding, crossButtonSize, buttonColor)) {
				frame->DeletePage(pages[i--]);
			}

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
				ImGui::SetTooltip("Delete page", ImGui::GetStyle().HoverDelayNormal);

			buttonPos.y      += buttonSize.y + 5.0f;
			crossButtonPos.y += buttonSize.y + 5.0f;
		}

		cursor.y = std::min(buttonPos.y, pagesSectionSize.y + cursor.y);

		if (pages.size())
			drawList->AddLine({ windowPos.x, cursor.y }, { windowPos.x + windowWidth, cursor.y }, 0xFF303030);

		// display list of all the sprites in the current page 
		if (currentPage != nullptr) {
			cursor.y += 20.0f;

			const auto spriteView      = currentPage->GetSprites();
			const auto& selectedEntity = currentPage->GetSelectedEntity();

			ImGui::PushFont(font);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.0f, 0.0f, 0.0f, 0.0f }      );
			ImGui::PushStyleColor(ImGuiCol_Text,    { 0.729f, 0.729f, 0.729f, 1.0f });
			ImGui::SetCursorScreenPos(cursor);

			// input for name of the current page
			ImGui::InputText("##PageNameInput", &currentPage->GetName());

			ImGui::PopStyleColor(2);
			ImGui::PopFont();

			// button to add new sprite
			crossButtonPos = cursor + buttonSize - crossButtonSize;
			if (CustomTextButton("+", buttonId++, font, crossButtonPos, buttonPadding, crossButtonSize, 0xff303020)) {
				currentPage->CreateSprite("New Sprite");
			}

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
				ImGui::SetTooltip("Add new sprite", ImGui::GetStyle().HoverDelayNormal);

			cursor.y      += buttonSize.y + 24.0f;
			buttonPos      = cursor;
			crossButtonPos = buttonPos + buttonSize - crossButtonSize;

			for (auto e : spriteView) {
				Quirk::Entity entity(e, currentPage->GetScene());

				ImU32 buttonColor = (selectedEntity == entity) ? 0xff303020 : 0x0000;
				const std::string& label = entity.GetComponent<Quirk::TagComponent>().Tag;

				// button to select sprite
				ImGui::SetNextItemAllowOverlap();
				if (CustomTextButton(label.c_str(), buttonId++, font, buttonPos, buttonPadding, buttonSize, buttonColor)) {
					currentPage->SetSelectedEntity(entity);
				}

				// button to delete this sprite
				if (CustomTextButton(crossButtonLabel, buttonId++, font, crossButtonPos, buttonPadding, crossButtonSize, buttonColor)) {
					currentPage->DestroySprite(entity);
				}

				if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
					ImGui::SetTooltip("Delete Sprite", ImGui::GetStyle().HoverDelayNormal);

				buttonPos.y      += buttonSize.y + 5.0f;
				crossButtonPos.y += buttonSize.y + 5.0f;
			}
		}
	}
}

static bool CustomTextButton(const char* label, int id, ImFont* font, const ImVec2 pos, const ImVec2 padding, const ImVec2 size, ImU32 color) {
	ImGui::PushID(id);
	ImGui::SetCursorScreenPos(pos);

	const ImVec2 buttonEnd = pos + size;

	bool isClicked = ImGui::InvisibleButton(label, size);
	bool isHovered = ImGui::IsItemHovered();
	bool isActive  = isHovered && ImGui::IsMouseDown(0);

	auto drawList = ImGui::GetWindowDrawList();

	ImU32 buttonColor = isHovered ? isActive ? 0xff303020 : 0xff272720 : color;
	drawList->AddRectFilled(pos, buttonEnd, buttonColor, 8.0f);

	ImVec2 textPos = pos + (padding * 0.5f);
	drawList->AddText(font, font->FontSize, textPos, 0xFFBABABA, label);

	ImGui::PopID();
	return isClicked;
}
