


#include "ViewPortPanel.h"
#include "Frame.h"


void ViewPortPanel::OnUpdate() {
	if (m_IsInFocus) {

	}
}

bool ViewPortPanel::OnEvent(Quirk::Event& event) {
	if (m_IsInFocus) {

	}

    return false;
}

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
	CheckAndHandleResize();
	RenderViewport();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	ImVec2 imagePos = ImGui::GetCursorPos();
	ImTextureID frameBuffer = (ImTextureID)(intptr_t)m_Frame->GetColorAttachment(0);
	bool clickedOnImage = ImGui::ImageButton(
		"viewportimage",
		frameBuffer,
		ImVec2((float)m_PanelWidth, (float)m_PanelHeight),
		{ 0, 1 },
		{ 1, 0 }
	);

	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar();
}

void ViewPortPanel::CheckAndHandleResize() {
	ImVec2 windowSize = ImGui::GetContentRegionAvail();

	if (windowSize.x < 0 || windowSize.y < 0) {
		return;
	}

	if (m_PanelWidth != (int)windowSize.x || m_PanelHeight != (int)windowSize.y) {
		m_PanelWidth  = (int)windowSize.x;
		m_PanelHeight = (int)windowSize.y;

		auto frame = (SpriteGeneratorFrame*)GetParentFrame();

		if (const auto& currentPage = frame->GetCurrentPage(); currentPage != nullptr) {
			currentPage->OnViewportResize(m_PanelWidth, m_PanelHeight);
		}

		m_Frame->Resize(m_PanelWidth, m_PanelHeight);
		Quirk::RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);
	}
}

void ViewPortPanel::RenderViewport() {
	m_Frame->Bind();
	m_Frame->ClearAttachments();

	m_Frame->Unbind();
}
