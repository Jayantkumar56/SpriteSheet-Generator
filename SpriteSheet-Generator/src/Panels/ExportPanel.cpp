

#define IMGUI_DEFINE_MATH_OPERATORS

#include "Frame.h"
#include "ExportPanel.h"


static bool CustomTextButton(const char* label, ImVec2 position, ImVec2 size, ImVec2 padding);
static void FlipImageVertically(uint8_t* image, int width, int height, int channels);


ExportPanel::ExportPanel(Quirk::Ref<Page> currentPage) : 
		Panel("Export Panel"),
		m_SpriteSheetName(currentPage->GetName() + ".png")
{
	SetWindowFlags(ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	m_PageWidth   = currentPage->GetWidth();
	m_PageHeight  = currentPage->GetHeight();
	m_SpriteSheet = Quirk::FrameBuffer::Create({ m_PageWidth, m_PageHeight });

	m_SpriteSheet->SetAttachments({
		{ Quirk::FrameBufferTextureType::RGBA_8,			 {.RGBA = {0.094f, 0.227f, 0.216f, 1.0f} } },
		{ Quirk::FrameBufferTextureType::DEPTH_24_STENCIL_8, {.DepthValue = 1.0f }					   }
	});

	m_SpriteSheet->Bind();
	currentPage->RenderSpriteSheet();
	m_SpriteSheet->Unbind();
}

void ExportPanel::SetImguiProperties() {
	ImGuiWindowClass window_class;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);

	ImGui::SetNextWindowSize({ 800.0f, 650.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
}

void ExportPanel::UnSetImguiProperties() {
	ImGui::PopStyleVar();
}

void ExportPanel::OnImguiUiUpdate() {
	const auto  frame           = (SpriteGeneratorFrame*)GetParentFrame();

	const auto availableRegion = ImGui::GetContentRegionAvail();
	auto         cursorPos     = ImGui::GetCursorScreenPos();
	const ImVec2 windowPadding = { 20.0f, 10.f };

	// export button properties
	const char*  buttonLabel   = "Export";
	const auto   labelSize     = ImGui::CalcTextSize(buttonLabel);
	const ImVec2 buttonPadding = { 20.0f, 10.0f };
	const ImVec2 buttonSize    = labelSize + (buttonPadding * 2.0f);
	const ImVec2 buttonPos     = cursorPos + availableRegion - buttonSize;
	// export button properties

	// properties for final image to export
	const float  imageButtonSpacing = 30.0f;
	const ImVec2 imageAvailRegion   = {
		availableRegion.x - (2.0f * windowPadding.x),
		availableRegion.y - buttonSize.y - (2 * windowPadding.y) - imageButtonSpacing
	};

	const auto pageAspectRatio        = (float)m_PageWidth / (float)m_PageHeight;
	const auto imageRegionAspectRatio = imageAvailRegion.x / imageAvailRegion.y;

	ImVec2 imageSize = { imageAvailRegion.y * pageAspectRatio, imageAvailRegion.y };
	if (pageAspectRatio > imageRegionAspectRatio) {
		imageSize = { imageAvailRegion.x, imageAvailRegion.x / pageAspectRatio };
	}

	const ImVec2 imageMargin = imageAvailRegion - imageSize;
	const ImVec2 imagePos    = cursorPos + windowPadding + imageMargin * 0.5;
	// properties for final image to export

	// display the final spriteSheet
	{
		const auto spriteSheet = m_SpriteSheet->GetColorAttachment(0);

		ImGui::PushStyleVar  (ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

		ImGui::SetCursorScreenPos(imagePos);
		ImGui::ImageButton("spriteSheet", (ImTextureID)(intptr_t)spriteSheet, imageSize, { 0, 1 }, { 1, 0 });

		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar();
	}

	// export button
	{
		if (CustomTextButton(buttonLabel, buttonPos, buttonSize, buttonPadding)) {
			int imageDataSize = (int)m_PageWidth * (int)m_PageHeight * 4;
			std::vector<uint8_t> imageData(imageDataSize);

			m_SpriteSheet->GetColorPixelData(0, 0, 0, m_PageWidth, m_PageHeight, imageData.data(), imageDataSize * sizeof(uint8_t));

			FlipImageVertically(imageData.data(), m_PageWidth, m_PageHeight, 4);
			stbi_write_png(m_SpriteSheetName.c_str(), m_PageWidth, m_PageHeight, 4, imageData.data(), m_PageWidth * 4);
		}
	}
}


static bool CustomTextButton(const char* label, ImVec2 position, ImVec2 size, ImVec2 padding) {
	ImGui::SetCursorScreenPos(position);

	const auto drawList = ImGui::GetWindowDrawList();
	const bool clicked  = ImGui::InvisibleButton(label, size, ImGuiButtonFlags_PressedOnClickRelease);
	const bool hovered  = ImGui::IsItemHovered();
	const auto font     = Quirk::FontManager::GetFont(Quirk::FontWeight::Medium, 25);

	const auto buttonColor = hovered ? (clicked ? 0xFF8f9d2a : 0xFF758519) : 0xFF8f9d2a;
	
	drawList->AddRectFilled(position, position + size, buttonColor, 3.0f);
	drawList->AddText(font, font->FontSize, position + padding, 0xFF000000, label);

	return clicked;
}

static void FlipImageVertically(uint8_t* image, int width, int height, int channels) {
	int rowSize = width * channels;
	std::vector<uint8_t> temp(rowSize);

	for (int i = 0; i < height / 2; i++) {
		uint8_t* topRow    = image + (i * rowSize);
		uint8_t* bottomRow = image + ((height - 1 - i) * rowSize);

		// Swap rows
		memcpy(temp.data(), topRow,      rowSize);
		memcpy(topRow,      bottomRow,   rowSize);
		memcpy(bottomRow,   temp.data(), rowSize);
	}
}
