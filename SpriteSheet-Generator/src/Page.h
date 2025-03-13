

#pragma once

#include "Quirk.h"
#include "EditorCamera.h"


class Page {
public:
	Page();

	inline bool OnEvent(Quirk::Event& event) { return m_Camera.OnEvent(event); }
	inline void OnUpdate()                   { m_Camera.OnUpdate();            }

	inline uint16_t GetWidth()  const noexcept { return m_Width;  }
	inline uint16_t GetHeight() const noexcept { return m_Height; }

	void SetPageSize(uint16_t width, uint16_t height);

	inline auto&         GetName()    noexcept { return m_Scene.GetName(); }
	inline Quirk::Scene* GetScene()   noexcept { return &m_Scene;          }
	inline const auto    GetSprites()          { return m_Scene.GetRegistry().view<Quirk::SpriteRendererComponent>(); }

	inline const auto& GetCamera()                      const noexcept { return m_Camera; }
	inline auto GetSelectedEntity()                     const noexcept { return m_SelectedEntity;                  }
	inline void SetSelectedEntity(Quirk::Entity entity)       noexcept { m_SelectedEntity = entity;                }
	inline void OnViewportResize(uint32_t width, uint32_t height)      { m_Camera.OnViewportResize(width, height); }

	void CreateSprite(std::string spriteName);
	void DestroySprite(Quirk::Entity entity);

	void RenderPage();
	void RenderSpriteSheet();

private:
	uint16_t m_Width, m_Height;

	// transforms for the rendering of the blank page
	glm::mat4 m_Transforms;
	glm::vec4 m_PageColor;

	EditorCamera  m_Camera;
	Quirk::Entity m_SelectedEntity;
	Quirk::Scene  m_Scene;
};

