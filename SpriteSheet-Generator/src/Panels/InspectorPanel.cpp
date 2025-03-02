


#include "InspectorPanel.h"


void InspectorPanel::SetImguiProperties() {
	ImGuiWindowClass window_class;
	window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_class);
}

void InspectorPanel::UnSetImguiProperties() {

}

void InspectorPanel::OnImguiUiUpdate() {

}
