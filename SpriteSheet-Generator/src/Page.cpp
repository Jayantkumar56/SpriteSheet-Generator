

#include "Page.h"


Page::Page() :
		m_Width     ( 512 ),
		m_Height    ( 512 ),
		m_PageColor ( 1.0f, 1.0f, 1.0f, 1.0f ),
		m_Camera    ( m_Width, m_Height      ),
		m_SelectedEntity ( ),
		m_Scene          ( "New Page", m_Width, m_Height                     )
{
	m_Transforms = glm::translate(glm::mat4(1.0f), glm::vec3{ m_Width * 0.5f, m_Height * 0.5f, 1.0f })
		           * glm::scale(glm::mat4(1.0f), glm::vec3{ m_Width, m_Height, 1.0f });
}

void Page::SetPageSize(uint16_t width, uint16_t height) {
	m_Width  = width;
	m_Height = height;

	m_Transforms = glm::translate(glm::mat4(1.0f), glm::vec3{ m_Width * 0.5f, m_Height * 0.5f, 1.0f })
			       * glm::scale(glm::mat4(1.0f), glm::vec3{ m_Width, m_Height, 1.0f });

	m_Camera.SetOrthographicSize((float)m_Width, (float)m_Height);
}

void Page::CreateSprite(std::string spriteName) {
	auto entity = m_Scene.CreateEntity(spriteName);
	entity.AddComponent<Quirk::SpriteRendererComponent>();
	m_SelectedEntity = entity;
}

void Page::DestroySprite(Quirk::Entity entity) {
	if (m_SelectedEntity == entity)
		m_SelectedEntity = Quirk::Entity();

	m_Scene.DestroyEntity(entity);
}

void Page::RenderPage() {
	const auto& registry = m_Scene.GetRegistry();

	Quirk::Renderer2D::BeginScene(m_Camera.GetProjectionView());

	Quirk::Renderer2D::SubmitQuad(m_Transforms, m_PageColor);

	const auto renderables = registry.view<Quirk::TransformComponent, Quirk::SpriteRendererComponent>();
	for (auto entity : renderables) {
		Quirk::Renderer2D::SubmitQuadEntity({ entity, &m_Scene });
	}

	Quirk::Renderer2D::EndScene();
}

void Page::RenderSpriteSheet() {
	const auto&     registry         = m_Scene.GetRegistry();
	const glm::mat4 projectionMatrix = glm::ortho(-m_Width * 0.5f, m_Width * 0.5f, m_Height * 0.5f, -m_Height * 0.5f, -1.0f, 1.0f);
	const glm::vec3 cameraPos        = glm::vec3(m_Width * 0.5f, m_Height * 0.5f, 1.0f);
	const glm::mat4 viewMatrix       = glm::inverse(glm::translate(glm::mat4(1.0f), cameraPos));

	Quirk::RenderCommands::UpdateViewPort(m_Width, m_Height);

	Quirk::Renderer2D::BeginScene(projectionMatrix * viewMatrix);

	Quirk::Renderer2D::SubmitQuad(m_Transforms, m_PageColor);

	const auto renderables = registry.view<Quirk::TransformComponent, Quirk::SpriteRendererComponent>();
	for (auto entity : renderables) {
		Quirk::Renderer2D::SubmitQuadEntity({ entity, &m_Scene });
	}

	Quirk::Renderer2D::EndScene();
}
