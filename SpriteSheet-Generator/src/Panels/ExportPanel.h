

#pragma once

#include "Quirk.h"

class ExportPanel : public Quirk::Panel {
public:
	ExportPanel(Quirk::Ref<Page> currentPage);

	virtual void SetImguiProperties()   override;
	virtual void UnSetImguiProperties() override;
	virtual void OnImguiUiUpdate()      override;

private:
	std::string m_SpriteSheetPath;
	Quirk::Ref<Quirk::Texture2D> m_FolderSelectionImage;

	Quirk::Ref<Page> m_Page;
	Quirk::Ref<Quirk::FrameBuffer> m_SpriteSheet;
};
