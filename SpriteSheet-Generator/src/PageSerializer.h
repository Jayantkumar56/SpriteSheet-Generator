

#pragma once


#include "Quirk.h"
#include "Page.h"

class PageSerializer {
public:
	PageSerializer(Quirk::Ref<Page> page);

	bool SerializeSpriteSheet(const std::filesystem::path path) const;

private:
	void SerializeSprite(YAML::Emitter& emitter, Quirk::Entity entity) const;

private:
	Quirk::Ref<Page> m_Page;
};
