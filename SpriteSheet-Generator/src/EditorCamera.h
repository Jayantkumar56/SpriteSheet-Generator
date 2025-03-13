

#pragma once

#include "Core/Camera/Camera.h" 
#include "Core/Input/Events.h"
#include "Core/Input/MouseEvents.h"

#include <glm/glm.hpp>


class EditorCamera {
public:
	EditorCamera(uint32_t width, uint32_t height);

	void OnUpdate() { }
	bool OnEvent(Quirk::Event& e) { return false; }

	void OnViewportResize(uint32_t width, uint32_t height);
	void SetOrthographicSize(float width, float height);

	glm::vec2 GetViewPortSize() const;
	inline glm::mat4 GetProjectionView() const { return m_ProjectionMatrix * m_ViewMatrix; }

private:
	float m_AspectRatio;
	float m_OrthographicSize;
	glm::vec3 m_Position;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
};
