

#define IMGUI_DEFINE_MATH_OPERATORS

#include "ViewPortPanel.h"
#include "Frame.h"


static bool CustomDragInt2(int id, const char* labelX, int* valueX, const char* labelY, int* valueY,
	                       ImFont* labelFont, ImFont* vlueFont, int vMin, int vMax);


void ViewPortPanel::SetImguiProperties() {
	ImGuiWindowClass window_class;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
}

void ViewPortPanel::UnSetImguiProperties() {
	ImGui::PopStyleVar();
}

void ViewPortPanel::OnImguiUiUpdate() {
	m_IsInFocus = ImGui::IsWindowFocused();

	auto  frame       = (SpriteGeneratorFrame*)GetParentFrame();
	auto& currentPage = frame->GetCurrentPage();

	const auto fontRegular22 = Quirk::FontManager::GetFont(Quirk::FontWeight::Regular, 22);
	const auto fontMedium25  = Quirk::FontManager::GetFont(Quirk::FontWeight::Medium,  25);
	const auto fontMedium22  = Quirk::FontManager::GetFont(Quirk::FontWeight::Medium,  22);

	auto cursorPos = ImGui::GetCursorScreenPos();

	// handling the resizing of viewport
	{
		ImVec2 windowSize = ImGui::GetContentRegionAvail();

		if (windowSize.x > 0 && windowSize.y > 0) {
			if (m_PanelWidth != (int)windowSize.x || m_PanelHeight != (int)windowSize.y) {
				m_PanelWidth  = (int)windowSize.x;
				m_PanelHeight = (int)windowSize.y;

				if (currentPage != nullptr) {
					currentPage->OnViewportResize(m_PanelWidth, m_PanelHeight);
					m_Frame->Resize(m_PanelWidth, m_PanelHeight);
					Quirk::RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);
				}
			}
		}
	}

	// render the current page to framebuffer
	{
		m_Frame->Bind();
		m_Frame->ClearAttachments();

		if (currentPage != nullptr)
			currentPage->RenderPage();

		m_Frame->Unbind();
	}

	// showing the rendered image on the viewport
	{
		const auto buttonSize  = ImVec2((float)m_PanelWidth, (float)m_PanelHeight);
		const auto imagePos    = ImGui::GetCursorPos();
		const auto drawList    = ImGui::GetWindowDrawList();
		const auto frameBuffer = (ImTextureID)(intptr_t)m_Frame->GetColorAttachment(0);

		ImGui::SetNextItemAllowOverlap();
		const bool imageClicked = ImGui::InvisibleButton("viewportimageButton", buttonSize, ImGuiButtonFlags_PressedOnClick);
		drawList->AddImage(frameBuffer, cursorPos, cursorPos + buttonSize, { 0, 1 }, { 1, 0 });

		HandleSpriteSelectionAndMovement(imageClicked, imagePos);
	}

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

	// input for width and height of the page
	{
		if (currentPage != nullptr) {
			int width  = currentPage->GetWidth();
			int height = currentPage->GetHeight();

			const auto windowWidth = ImGui::GetWindowWidth();

			cursorPos.x += 0.5f * (windowWidth - 400.0f);
			cursorPos.y += 10.0f;

			ImGui::PushStyleColor(ImGuiCol_FrameBg,        { 0.102f, 0.102f, 0.102f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, { 0.102f, 0.102f, 0.102f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  { 0.102f, 0.102f, 0.102f, 1.0f });

			ImGui::SetCursorScreenPos(cursorPos);
			if (CustomDragInt2(0, "W", &width, "H", &height, fontMedium22, fontRegular22, 1, INT16_MAX)) {
				currentPage->SetPageSize((uint16_t)width, (uint16_t)height);
			}

			ImGui::PopStyleColor(3);
		}
	}

	// button to start exporting current page
	{
		if (currentPage != nullptr) {
			auto& styles = ImGui::GetStyle();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10.0f, styles.FramePadding.y });
			ImGui::PushStyleColor(ImGuiCol_Button,          { 0.102f, 0.102f, 0.102f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   { 0.132f, 0.132f, 0.132f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive,    { 0.102f, 0.102f, 0.102f, 1.0f });

			ImGui::SameLine(0.0f, 30.0f);
			if (ImGui::Button("Export")) {
				frame->AddPanel<ExportPanel>(currentPage);
				Quirk::RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleColor(3);
		}
	}

	ImGui::PopStyleVar();
}

int ViewPortPanel::GetEntityIdOnClick(const ImVec2& imagePos) {
	auto&  window    = ((SpriteGeneratorFrame*)GetParentFrame())->GetWindow();
	ImVec2 windowPos = ImGui::GetWindowPos();

	windowPos       = { windowPos.x - window.GetPosX(), windowPos.y - window.GetPosY() };
	ImVec2 mousePos = { Quirk::Input::MouseCurrentX() - windowPos.x, Quirk::Input::MouseCurrentY() - windowPos.y };

	// mouse position on the image button
	mousePos = { mousePos.x - imagePos.x, mousePos.y - imagePos.y };
	// inverting the y axis for mouse coords
	// 2 added because of slight visual error		TO DO: find this error
	mousePos.y = m_PanelHeight - mousePos.y - 2;

	int entityId = 0;
	m_Frame->GetColorPixelData(1, (int)mousePos.x, (int)mousePos.y, 1, 1, &entityId, sizeof(entityId));

	return entityId;
}

void ViewPortPanel::HandleSpriteSelectionAndMovement(const bool clickedOnViewPort, const ImVec2& viewportImageButtonPos) {
	auto  frame       = (SpriteGeneratorFrame*)GetParentFrame();
	auto& currentPage = frame->GetCurrentPage();

	if (currentPage == nullptr)
		return;

	if (clickedOnViewPort) {										// selecting sprite on click
		const auto entityId       = GetEntityIdOnClick(viewportImageButtonPos);
		auto       selectedSprite = (entityId == -1) ? Quirk::Entity() : Quirk::Entity((entt::entity)entityId, currentPage->GetScene());

		currentPage->SetSelectedEntity(selectedSprite);

		if (!selectedSprite.IsInvalidEntity()) {
			const auto  mousePos  = glm::vec2(Quirk::Input::MouseCurrentX(), Quirk::Input::MouseCurrentY());
			const auto& spritePos = selectedSprite.GetComponent<Quirk::TransformComponent>().Translation;

			auto spriteMoveSpeed  = CalculateSpriteMoveSpeed();
			m_RelativeSpritePos.x = spriteMoveSpeed.x * mousePos.x - spritePos.x;
			m_RelativeSpritePos.y = spriteMoveSpeed.y * mousePos.y - spritePos.y;
		}
	}
	else if (ImGui::IsItemActive()) {						// moving sprite on mouse draging
		auto mousePos       = glm::vec2(Quirk::Input::MouseCurrentX(), Quirk::Input::MouseCurrentY());
		auto selectedSprite = currentPage->GetSelectedEntity();

		if (!selectedSprite.IsInvalidEntity() && m_PreviousMousePos != mousePos) {
			m_PreviousMousePos = mousePos;

			auto spriteMoveSpeed = CalculateSpriteMoveSpeed();
			auto& spritePos = selectedSprite.GetComponent<Quirk::TransformComponent>().Translation;
			spritePos.x     = spriteMoveSpeed.x * mousePos.x - m_RelativeSpritePos.x;
			spritePos.y     = spriteMoveSpeed.y * mousePos.y - m_RelativeSpritePos.y;
		}
	}
}

glm::vec2 ViewPortPanel::CalculateSpriteMoveSpeed() {
	auto  frame       = (SpriteGeneratorFrame*)GetParentFrame();
	auto& currentPage = frame->GetCurrentPage();

	glm::vec2 viewPortSize       = { m_PanelWidth, m_PanelHeight };
	glm::vec2 cameraViewPortSize = currentPage->GetCamera().GetViewPortSize();

	glm::vec2 requiredSpeed = cameraViewPortSize / viewPortSize;
	return requiredSpeed;
}

static bool CustomDragInt2(int id, const char* labelX, int* valueX, const char* labelY, int* valueY,
	                       ImFont* labelFont, ImFont* vlueFont, int vMin, int vMax)
{
	bool valueChanged = false;
	ImGui::PushID(id);

	auto drawList     = ImGui::GetWindowDrawList();
	auto cursorPos    = ImGui::GetCursorScreenPos();
	auto framepadding = ImGui::GetStyle().FramePadding;

	// dragFloatSpacing :- space between first dragfloat and second dragfloat
	float  dragFloatSpacing = 32.0f;
	float  labelPaddingX    = 8.0f;
	float  dragFloatWidth   = 100.0f;

	ImVec2 labelSize    = { 25.0f, framepadding.y * 2 };
	ImVec2 dragFloatEnd = { cursorPos.x + labelSize.x + labelPaddingX + dragFloatWidth, cursorPos.y + ImGui::GetFrameHeight() };

	// first dragfloat
	{
		drawList->AddRectFilled(cursorPos, dragFloatEnd, 0xff030303, 5.0f);

		cursorPos.x += labelPaddingX;
		drawList->AddText(labelFont, labelFont->FontSize, cursorPos + framepadding, 0xFFFFFFFF, labelX);

		ImGui::SetCursorScreenPos({ cursorPos.x + labelSize.x, cursorPos.y });
		ImGui::SetNextItemWidth(dragFloatWidth);
		valueChanged |= ImGui::DragInt("##X", valueX, 1.0f, vMin, vMax);
	}

	cursorPos.x    += dragFloatSpacing + labelSize.x + dragFloatWidth - labelPaddingX;
	dragFloatEnd.x += dragFloatSpacing + labelSize.x + dragFloatWidth;

	// second dragfloat
	{
		drawList->AddRectFilled(cursorPos, dragFloatEnd, 0xff030303, 5.0f);

		cursorPos.x += labelPaddingX;
		drawList->AddText(labelFont, labelFont->FontSize, cursorPos + framepadding, 0xFFFFFFFF, labelY);

		ImGui::SetCursorScreenPos({ cursorPos.x + labelSize.x, cursorPos.y });
		ImGui::SetNextItemWidth(dragFloatWidth);
		valueChanged |= ImGui::DragInt("##Y", valueY, 1.0f, vMin, vMax);
	}

	ImGui::PopID();
	return valueChanged;
}


