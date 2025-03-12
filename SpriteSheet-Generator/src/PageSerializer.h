

#pragma once


#include "Quirk.h"

class PageSerializer {
public:
	PageSerializer(uint16_t width, uint16_t height, std::string name, Quirk::Ref<Quirk::Scene> scene);

	bool SerializeSpriteSheet(const std::filesystem::path path) const;

private:
	void SerializeSprite(YAML::Emitter& emitter, Quirk::Entity entity) const;

private:
	uint16_t m_Width, m_Height;

	std::string m_SpriteSheetName;
	Quirk::Ref<Quirk::Scene> m_SpriteSheetScene;
};
