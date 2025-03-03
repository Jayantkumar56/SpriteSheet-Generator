


#pragma once

#include "Quirk.h"


class InspectorPanel : public Quirk::Panel {
public:
	InspectorPanel() : 
			Panel         ( "Inspector" ),
			m_UploadImage ( Quirk::Texture2D::Create("assets/Images/Upload.png"))
	{
	}

	virtual void SetImguiProperties() override;
	virtual void OnImguiUiUpdate()    override;

private:
	bool m_IsInFocus = false;
	Quirk::Ref<Quirk::Texture2D> m_UploadImage;
};

