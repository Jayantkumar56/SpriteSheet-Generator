

#pragma once

#include "Quirk.h"

class ExportPanel : public Quirk::Panel {
public:
	ExportPanel(Quirk::Ref<Page> currentPage);

	virtual void SetImguiProperties()   override;
	virtual void UnSetImguiProperties() override;
	virtual void OnImguiUiUpdate()      override;

private:
	uint16_t m_PageWidth;
	uint16_t m_PageHeight;

	std::string m_SpriteSheetPath;
	Quirk::Ref<Quirk::Texture2D> m_FolderSelectionImage;

	std::string m_SpriteSheetName;
	Quirk::Ref<Quirk::FrameBuffer> m_SpriteSheet;
	Quirk::Ref<Quirk::Scene> m_SpriteSheetScene;
};
