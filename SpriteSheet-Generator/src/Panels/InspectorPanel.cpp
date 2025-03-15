
#define IMGUI_DEFINE_MATH_OPERATORS

#include "InspectorPanel.h"
#include "Frame.h"


static bool CustomDragFloat2(int id, const char* labelX, float* valueX, const char* labelY, float* valueY, 
							 ImFont* labelFont, ImFont* vlueFont, float vMin = 0.0f, float vMax = 0.0f);


void InspectorPanel::SetImguiProperties() {
	ImGuiWindowClass window_class;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);
}

void InspectorPanel::OnImguiUiUpdate() {
	const auto frame = (SpriteGeneratorFrame*)GetParentFrame();

	auto& currentPage = frame->GetCurrentPage();
	if (currentPage == nullptr)
		return;

	auto selectedEntity = currentPage->GetSelectedEntity();
	if (selectedEntity.IsInvalidEntity())
		return;

	const auto  framePadding  = ImGui::GetStyle().FramePadding;
	const auto  windowWidth   = ImGui::GetWindowWidth();
	const auto  windowPos     = ImGui::GetWindowPos();
	const auto  fontRegular22 = Quirk::FontManager::GetFont(Quirk::FontWeight::Regular, 22);
	const auto  fontMedium25  = Quirk::FontManager::GetFont(Quirk::FontWeight::Medium,  25);
	const auto  fontMedium22  = Quirk::FontManager::GetFont(Quirk::FontWeight::Medium,  22);
	const auto  drawList      = ImGui::GetWindowDrawList();
	const auto& style         = ImGui::GetStyle();
	auto        cursorPos     = ImGui::GetCursorScreenPos();

	auto& spriteTransforms        = selectedEntity.GetComponent<Quirk::TransformComponent>();
	auto& spriteRendererComponent = selectedEntity.GetComponent<Quirk::SpriteRendererComponent>();

	// Sprite Name
	{
		std::string& entityName = selectedEntity.GetComponent<Quirk::TagComponent>().Tag;

		ImGui::PushFont(fontRegular22);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_Text,    { 0.651f, 0.651f, 0.651f, 1.0f });

		ImGui::InputText("##PageNameInput", &entityName);

		ImGui::PopStyleColor(2);
		ImGui::PopFont();

		cursorPos    = ImGui::GetCursorScreenPos();
		cursorPos.y += 5.0f;
		drawList->AddLine({ windowPos.x, cursorPos.y }, { windowPos.x + windowWidth, cursorPos.y }, 0xFF303030);
	}

	{
		ImGui::PushStyleColor(ImGuiCol_FrameBg,        { 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, { 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  { 0.0f, 0.0f, 0.0f, 0.0f });

		// position of Sprite
		{
			// positions title
			cursorPos.y += 16.0f;
			drawList->AddText(fontMedium25, fontMedium25->FontSize, cursorPos, 0xFFF8FF00, "Position");
		
			cursorPos.y += ImGui::GetTextLineHeightWithSpacing() + 12.0f;
			ImGui::SetCursorScreenPos(cursorPos);

			auto position = currentPage->GetSelectedSpritePos();
			if (CustomDragFloat2(0, "U", &position.x, "V", &position.y, fontMedium22, fontRegular22)) {
				currentPage->SetSelectedSpritePos(position);
			}

			cursorPos.y += ImGui::GetTextLineHeightWithSpacing() + 20.0f;
			drawList->AddLine({ windowPos.x, cursorPos.y }, { windowPos.x + windowWidth, cursorPos.y }, 0xFF303030);
		}

		// dimension of Sprite
		{
			cursorPos.y += 16.0f;
			drawList->AddText(fontMedium25, fontMedium25->FontSize, cursorPos, 0xFFF8FF00, "Dimension");

			cursorPos.y += ImGui::GetTextLineHeightWithSpacing() + 12.0f;
			ImGui::SetCursorScreenPos(cursorPos);

			auto size = currentPage->GetSelectedSpriteSize();
			if (CustomDragFloat2(1, "W", &size.x, "H", &size.y, fontMedium22, fontRegular22, 0.0f, FLT_MAX)) {
				currentPage->SetSelectedSpriteSize(size);
			}

			cursorPos.y += ImGui::GetTextLineHeightWithSpacing() + 20.0f;
			drawList->AddLine({ windowPos.x, cursorPos.y }, { windowPos.x + windowWidth, cursorPos.y }, 0xFF303030);
		}

		ImGui::PopStyleColor(3);
	}

	{
		ImGui::PushStyleColor(ImGuiCol_FrameBg,            { 0.125f, 0.188f, 0.188f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered,     { 0.125f, 0.188f, 0.188f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive,      { 0.125f, 0.188f, 0.188f, 1.0f });
		ImGui::PushStyleVar  (ImGuiStyleVar_FrameRounding, 5.0f                            );

		// image detail of the sprite
		{
			cursorPos.y += 16.0f;
			drawList->AddText(fontMedium25, fontMedium25->FontSize, cursorPos, 0xFFF8FF00, "Image Properties");

			const char* labelTintColor   = "Tint Color";
			const char* labelImageName   = "Image";
			const auto  labelColumnWidth = std::max(ImGui::CalcTextSize(labelTintColor).x, ImGui::CalcTextSize(labelImageName).x);

			// tint color
			{
				cursorPos.y += ImGui::GetTextLineHeightWithSpacing() + 12.0f + framePadding.y;

				drawList->AddText(fontMedium22, fontMedium22->FontSize, cursorPos, 0xFFA6A6A6, labelTintColor);

				cursorPos.x += labelColumnWidth + 5.0f;
				cursorPos.y -= framePadding.y;

				ImGui::SetCursorScreenPos(cursorPos);
				ImGui::PushFont(fontRegular22);

				ImGui::ColorEdit4("##color", glm::value_ptr(spriteRendererComponent.Color));

				ImGui::PopFont();

				cursorPos.x -= labelColumnWidth + 5.0f;
				cursorPos.y += 8.0f;
			}

			// image selection
			{
				std::string imageName = "No Texture";
				if (spriteRendererComponent.Texture != nullptr) {
					imageName = spriteRendererComponent.Texture->GetPath().filename().string();
				}

				// label
				{
					cursorPos.y += ImGui::GetTextLineHeightWithSpacing() + 8.0f + framePadding.y;

					drawList->AddText(fontMedium22, fontMedium22->FontSize, cursorPos, 0xFFA6A6A6, labelImageName);

					cursorPos.x += labelColumnWidth + 5.0f;
					cursorPos.y -= framePadding.y;
				}

				// Image Name display
				{
					ImGui::SetCursorScreenPos(cursorPos);
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {8.0f, framePadding.y});
					ImGui::PushFont(fontRegular22);

					const float availWidth     = ImGui::GetContentRegionAvail().x;
					const float ImageNameWidth = availWidth * 0.8f;

					ImGui::SetNextItemWidth(ImageNameWidth);
					ImGui::InputText("##ImageName", imageName.data(), imageName.size(), ImGuiInputTextFlags_ReadOnly);

					ImGui::PopFont();
					ImGui::PopStyleVar();
				}

				// Image upload button
				{
					const float buttonSize = 22.0f;

					ImGui::SameLine(0.0f, 3.0f);
					const ImTextureID uploadImageIconId = (ImTextureID)(intptr_t)m_UploadImage->GetRendererId();

					if (ImGui::ImageButton("uploadImageButton", uploadImageIconId, { buttonSize, buttonSize }, { 0, 1 }, { 1, 0 })) {
						Quirk::FileFilter filters[] = {
							{L"image",		L"*.png;*.JPG;*.JPEG*.jpg;*.jpeg"}
						};

						Quirk::FileDialogSpecification fileDialogSpec;
						fileDialogSpec.Title         = L"Select Texture";
						fileDialogSpec.FileNameLabel = L"Texture Name";
						fileDialogSpec.Filters       = filters;
						fileDialogSpec.NoOfFilters   = sizeof(filters) / sizeof(Quirk::FileFilter);
						fileDialogSpec.ParentWindow  = &GetWindow();

						std::filesystem::path filePath;
						if (Quirk::FileDialog::OpenFile(fileDialogSpec, filePath)) {
							spriteRendererComponent.Texture = Quirk::Texture2D::Create(filePath);
							glm::vec2 imageSize = { spriteRendererComponent.Texture->GetWidth(), spriteRendererComponent.Texture->GetHeight() };
							currentPage->SetSelectedSpriteSize(imageSize);
						}
					}
					if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay)) {
						ImGui::SetTooltip("Select Image");
					}
				}
			}

			cursorPos    = ImGui::GetCursorScreenPos();
			cursorPos.y += 10.0f;
			drawList->AddLine({ windowPos.x, cursorPos.y }, { windowPos.x + windowWidth, cursorPos.y }, 0xFF303030);
		}

		ImGui::PopStyleVar();
		ImGui::PopStyleColor(3);
	}
}

static bool CustomDragFloat2(int id, const char* labelX, float* valueX, const char* labelY, float* valueY,
							 ImFont* labelFont, ImFont* vlueFont, float vMin, float vMax) 
{
	bool valueChanged = false;
	ImGui::PushID(id);

	const auto drawList     = ImGui::GetWindowDrawList();
	const auto framepadding = ImGui::GetStyle().FramePadding;
	auto cursorPos          = ImGui::GetCursorScreenPos();

	// dragFloatSpacing :- space between first dragfloat and second dragfloat
	const float  dragFloatSpacing = 32.0f;
	const float  labelPaddingX    = 8.0f;
	const float  dragFloatWidth   = 100.0f;

	ImVec2 labelSize    = { 25.0f, framepadding.y * 2 };
	ImVec2 dragFloatEnd = { cursorPos.x + labelSize.x + labelPaddingX + dragFloatWidth, cursorPos.y + ImGui::GetFrameHeight() };

	// first dragfloat
	{
		drawList->AddRectFilled(cursorPos, dragFloatEnd, 0xff303020, 5.0f);

		cursorPos.x += labelPaddingX;
		drawList->AddText(labelFont, labelFont->FontSize, cursorPos + framepadding, 0xFFFFFFFF, labelX);

		ImGui::SetCursorScreenPos({ cursorPos.x + labelSize.x, cursorPos.y });
		ImGui::SetNextItemWidth(dragFloatWidth);
		valueChanged |= ImGui::DragFloat("##X", valueX, 1.0f, vMin, vMax, "%0.f");
	}

	cursorPos.x    += dragFloatSpacing + labelSize.x + dragFloatWidth - labelPaddingX;
	dragFloatEnd.x += dragFloatSpacing + labelSize.x + dragFloatWidth;

	// second dragfloat
	{
		drawList->AddRectFilled(cursorPos, dragFloatEnd, 0xff303020, 5.0f);

		cursorPos.x += labelPaddingX;
		drawList->AddText(labelFont, labelFont->FontSize, cursorPos + framepadding, 0xFFFFFFFF, labelY);

		ImGui::SetCursorScreenPos({ cursorPos.x + labelSize.x, cursorPos.y });
		ImGui::SetNextItemWidth(dragFloatWidth);
		valueChanged |= ImGui::DragFloat("##Y", valueY, 1.0f, vMin, vMax, "%0.f");
	}

	ImGui::PopID();
	return valueChanged;
}
