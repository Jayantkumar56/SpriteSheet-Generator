

#pragma once

#include "Quirk.h"

class ViewPortPanel : public Quirk::Panel {
public:
	ViewPortPanel() :
		Panel         ( "ViewPort" ),
		m_PanelWidth  ( 800   ),
		m_PanelHeight ( 800   ),
		m_IsInFocus   ( false ),
		m_Frame       ( Quirk::FrameBuffer::Create({ m_PanelWidth, m_PanelHeight }) )
	{
		Quirk::RenderCommands::UpdateViewPort(m_PanelWidth, m_PanelHeight);
		m_Frame->SetAttachments({
			{ Quirk::FrameBufferTextureType::RGBA_8,			 {.RGBA = {0.094f, 0.227f, 0.216f, 1.0f} } },
			{ Quirk::FrameBufferTextureType::RED_INTEGER,        {.RedInteger = -1   }					   },
			{ Quirk::FrameBufferTextureType::DEPTH_24_STENCIL_8, {.DepthValue = 1.0f }					   }
		});
	}

	virtual void OnUpdate()                   override;
	virtual bool OnEvent(Quirk::Event& event) override;
	virtual void SetImguiProperties()         override;
	virtual void UnSetImguiProperties()       override;
	virtual void OnImguiUiUpdate()            override;

private:
	void CheckAndHandleResize();
	void RenderViewport();

private:
	uint16_t m_PanelWidth, m_PanelHeight;
	bool	 m_IsInFocus;
	Quirk::Ref<Quirk::FrameBuffer> m_Frame;
};
