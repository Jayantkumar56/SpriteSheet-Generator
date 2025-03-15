

#include "Page.h"


Page::Page() :
		m_Width     ( 512 ),
		m_Height    ( 512 ),
		m_PageColor ( 1.0f, 1.0f, 1.0f, 1.0f ),
		m_Camera    ( m_Width, m_Height      ),
		m_SelectedSprite ( ),
		m_Scene          ( "New Page", m_Width, m_Height                     )
{
	m_Transforms = glm::translate(glm::mat4(1.0f), glm::vec3{ m_Width * 0.5f, -m_Height * 0.5f, 1.0f })
		           * glm::scale(glm::mat4(1.0f), glm::vec3{ m_Width, m_Height, 1.0f });
}

void Page::SetPageSize(uint16_t width, uint16_t height) {
	m_Width  = width;
	m_Height = height;

	m_Transforms = glm::translate(glm::mat4(1.0f), glm::vec3{ m_Width * 0.5f, -m_Height * 0.5f, 1.0f })
			       * glm::scale(glm::mat4(1.0f), glm::vec3{ m_Width, m_Height, 1.0f });

	m_Camera.SetOrthographicSize((float)m_Width, (float)m_Height);
}

void Page::CreateSprite(std::string spriteName) {
	auto entity      = m_Scene.CreateEntity(spriteName);
	m_SelectedSprite = entity;

	entity.AddComponent<Quirk::SpriteRendererComponent>();

	auto& transform = entity.GetComponent<Quirk::TransformComponent>();
	transform.Translation.x = 0.5f;
	transform.Translation.y = -0.5f;
}

void Page::DestroySprite(Quirk::Entity entity) {
	if (m_SelectedSprite == entity)
		m_SelectedSprite = Quirk::Entity();

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
	const glm::mat4 projectionMatrix = glm::ortho(-m_Width * 0.5f, m_Width * 0.5f, -m_Height * 0.5f, m_Height * 0.5f, -1.0f, 1.0f);
	const glm::vec3 cameraPos        = glm::vec3(m_Width * 0.5f, -m_Height * 0.5f, 1.0f);
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

glm::vec2 Page::GetSpritePos(Quirk::Entity entity) {
	glm::vec2 pos(0.0f);

	if (!entity.IsInvalidEntity()) {
		auto& transform = entity.GetComponent<Quirk::TransformComponent>();
		pos.x = transform.Translation.x - transform.Scale.x * 0.5f;
		// inverting the y - axis
		pos.y = -(transform.Translation.y + transform.Scale.y * 0.5f);

		// -0.0f appears in the input field if it is removed
		if (pos.y == -0.0f) pos.y = 0.0f;

		return pos;
	}

	QK_WARN("No Selected Sprite!");
	return pos;
}

void Page::SetSpritePos(Quirk::Entity entity, const glm::vec2 pos) {
	if (!entity.IsInvalidEntity()) {
		auto& transform = entity.GetComponent<Quirk::TransformComponent>();
		transform.Translation.x = pos.x + transform.Scale.x * 0.5f;
		// inverting the y - axis
		transform.Translation.y = -(pos.y + transform.Scale.y * 0.5f);

		return;
	}

	QK_WARN("No Selected Sprite!");
}

glm::vec2 Page::GetSpriteSize(Quirk::Entity entity) {
	glm::vec2 size(0.0f);

	if (!entity.IsInvalidEntity()) {
		auto& scale = entity.GetComponent<Quirk::TransformComponent>().Scale;
		size.x = scale.x;
		size.y = scale.y;

		return size;
	}

	QK_WARN("No Selected Sprite!");
	return size;
}

void Page::SetSpriteSize(Quirk::Entity entity, const glm::vec2 size) {
	if (!entity.IsInvalidEntity()) {
		auto& transform = entity.GetComponent<Quirk::TransformComponent>();

		transform.Translation.x += (size.x - transform.Scale.x) * 0.5f;
		transform.Translation.y -= (size.y - transform.Scale.y) * 0.5f;

		transform.Scale.x = size.x;
		transform.Scale.y = size.y;

		return;
	}

	QK_WARN("No Selected Sprite!");
}

