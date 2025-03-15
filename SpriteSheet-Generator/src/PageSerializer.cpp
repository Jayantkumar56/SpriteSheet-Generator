


#include "PageSerializer.h"

#include <fstream>

namespace YAML {

	static Emitter& operator << (Emitter& out, const glm::vec2& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << EndSeq;
		return out;
	}

	static Emitter& operator << (Emitter& out, const glm::vec3& v) {
		out << Flow;
		out << BeginSeq << v.x << v.y << v.z << EndSeq;
		return out;
	}

}

PageSerializer::PageSerializer(Quirk::Ref<Page> page) :
		m_Page(std::move(page))
{
}

bool PageSerializer::SerializeSpriteSheet(const std::filesystem::path filePath) const {
	YAML::Emitter out;
	{
		out << YAML::BeginMap; // Root
		out << YAML::Key << "SpriteSheet" << YAML::Value;
		{
			out << YAML::BeginMap; // spritesheet

			out << YAML::Key << "Width"  << YAML::Value << m_Page->GetWidth();
			out << YAML::Key << "Height" << YAML::Value << m_Page->GetHeight();

			// serializing all the sprites
			{
				out << YAML::Key << "Sprites" << YAML::BeginSeq;

				auto  spritesView  = m_Page->GetSprites();
				auto* spritesScene = m_Page->GetScene();

				for (auto entity : spritesView) {
					Quirk::Entity spriteToSerialize = { entity, spritesScene };
					if (!spriteToSerialize)
						break;

					SerializeSprite(out, spriteToSerialize);
				}

				out << YAML::EndSeq;
			}

			out << YAML::EndMap;   // spritesheet
		}
		out << YAML::EndMap; // Root
	}

	std::ofstream fout(filePath);
	fout << out.c_str();

	return true;
}

void PageSerializer::SerializeSprite(YAML::Emitter& emitter, Quirk::Entity sprite) const {
	emitter << YAML::BeginMap;

	// name of the sprite
	{
		emitter << YAML::Key << "Name" << YAML::Value << sprite.GetComponent<Quirk::TagComponent>().Tag;
	}

	// position of the sprite
	{
		auto position = m_Page->GetSpritePos(sprite);

		float u = std::max(0.0f, position.x);
		float v = std::max(0.0f, position.y);

		emitter << YAML::Key << "U" << YAML::Value << u;
		emitter << YAML::Key << "V" << YAML::Value << v;
	}

	// size of the sprite
	{
		auto size = m_Page->GetSpriteSize(sprite);

		emitter << YAML::Key << "Width"  << YAML::Value << size.x;
		emitter << YAML::Key << "Height" << YAML::Value << size.y;
	}

	emitter << YAML::EndMap;
}


