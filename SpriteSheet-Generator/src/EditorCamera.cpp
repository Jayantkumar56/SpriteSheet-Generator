
#include "EditorCamera.h"

#include "Core/Input/Input.h"
#include "Core/Input/KeyCodes.h"


EditorCamera::EditorCamera(uint32_t width, uint32_t height) :
	m_AspectRatio((float)width / (float)height),
	m_OrthographicSize((float)height),
	m_Position(0.5f * width, -0.5f * height, 0.0f)
{
	m_ProjectionMatrix = glm::ortho(
		-(m_OrthographicSize * m_AspectRatio), m_OrthographicSize * m_AspectRatio,
		-m_OrthographicSize, m_OrthographicSize,
		-1.0f, 1.0f
	);

	m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position);
	m_ViewMatrix = glm::inverse(m_ViewMatrix);
}

void EditorCamera::OnViewportResize(uint32_t width, uint32_t height) {
	m_AspectRatio = (float)width / (float)height;

	m_ProjectionMatrix = glm::ortho(
		-(m_OrthographicSize * m_AspectRatio), m_OrthographicSize * m_AspectRatio,
		-m_OrthographicSize, m_OrthographicSize,
		-1.0f, 1.0f
	);
}

void EditorCamera::SetOrthographicSize(float width, float height) {
	m_OrthographicSize = height;

	m_ProjectionMatrix = glm::ortho(
		-(m_OrthographicSize * m_AspectRatio), m_OrthographicSize * m_AspectRatio,
		-m_OrthographicSize, m_OrthographicSize,
		-1.0f, 1.0f
	);

	m_Position   = glm::vec3(0.5f * width, -0.5f * height, 0.0f);
	m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position);
	m_ViewMatrix = glm::inverse(m_ViewMatrix);
}

glm::vec2 EditorCamera::GetViewPortSize() const {
	glm::vec2 size = {
		(m_OrthographicSize * m_AspectRatio) + (m_OrthographicSize * m_AspectRatio),
		m_OrthographicSize + m_OrthographicSize,
	};

	return size;
}
