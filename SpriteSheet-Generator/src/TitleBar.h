

#pragma once

#include "Quirk.h"

class SpriteGeneratorTitleBar : public Quirk::TitleBar {
public:
	SpriteGeneratorTitleBar() {
		m_CloseIcon    = Quirk::Texture2D::Create("assets/Images/close.png");
		m_MinimiseIcon = Quirk::Texture2D::Create("assets/Images/minus.png");
		m_MaximiseIcon = Quirk::Texture2D::Create("assets/Images/maximize.png");
	}

	~SpriteGeneratorTitleBar() = default;

	virtual void SetImguiProperties()   override;
	virtual void UnSetImguiProperties() override;
	virtual void OnImguiUiUpdate()      override;

private:
	Quirk::Ref<Quirk::Texture2D> m_CloseIcon;
	Quirk::Ref<Quirk::Texture2D> m_MinimiseIcon;
	Quirk::Ref<Quirk::Texture2D> m_MaximiseIcon;
};
