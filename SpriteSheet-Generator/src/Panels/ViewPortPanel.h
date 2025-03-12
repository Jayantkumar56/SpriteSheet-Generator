

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

	virtual void SetImguiProperties()   override;
	virtual void UnSetImguiProperties() override;
	virtual void OnImguiUiUpdate()      override;

	auto GetWidth()  const noexcept { return m_PanelWidth;  }
	auto GetHeight() const noexcept { return m_PanelHeight; }

private:
	int  GetEntityIdOnClick               (const ImVec2& imagePos);
	void HandleSpriteSelectionAndMovement (const bool clickedOnViewPort, const ImVec2& viewportImageButtonPos);

private:
	uint16_t m_PanelWidth, m_PanelHeight;
	bool	 m_IsInFocus;
	Quirk::Ref<Quirk::FrameBuffer> m_Frame;

	glm::vec2 m_PreviousMousePos{ 0.0f, 0.0f };
	// position of selected sprite relative to the mouse position
	glm::vec2 m_RelativeSpritePos{ 0.0f, 0.0f };
};
