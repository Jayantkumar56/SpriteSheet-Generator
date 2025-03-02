


#pragma once

#include "Quirk.h"


class LayersPanel : public Quirk::Panel {
public:
	LayersPanel() : Panel("Layers") { }

	virtual void SetImguiProperties() override;
	virtual void OnImguiUiUpdate()    override;

private:
	bool m_IsInFocus = false;
};

