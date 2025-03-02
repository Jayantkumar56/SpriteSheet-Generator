


#pragma once

#include "Quirk.h"


class InspectorPanel : public Quirk::Panel {
public:
	InspectorPanel() : Panel("Inspector") { }

	virtual void SetImguiProperties()   override;
	virtual void UnSetImguiProperties() override;
	virtual void OnImguiUiUpdate()      override;

private:
	bool m_IsInFocus = false;
};

