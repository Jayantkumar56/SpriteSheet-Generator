


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

PageSerializer::PageSerializer(uint16_t width, uint16_t height, std::string name, Quirk::Ref<Quirk::Scene> scene) :
		m_Width            ( width            ),
		m_Height           ( height           ),
		m_SpriteSheetName  ( std::move(name)  ),
		m_SpriteSheetScene ( std::move(scene) )
{
}

bool PageSerializer::SerializeSpriteSheet(const std::filesystem::path filePath) const {
	YAML::Emitter out;
	{
		out << YAML::BeginMap; // Root
		out << YAML::Key << "SpriteSheet" << YAML::Value;
		{
			out << YAML::BeginMap; // spritesheet

			out << YAML::Key << "Width"  << YAML::Value << m_Width;
			out << YAML::Key << "Height" << YAML::Value << m_Height;

			// serializing all the sprites
			{
				out << YAML::Key << "Sprites" << YAML::BeginSeq;

				auto spritesView = m_SpriteSheetScene->GetRegistry().view<entt::entity>();

				for (auto entity : spritesView) {
					Quirk::Entity spriteToSerialize = { entity, m_SpriteSheetScene.get() };
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

void PageSerializer::SerializeSprite(YAML::Emitter& emitter, Quirk::Entity entity) const {
	emitter << YAML::BeginMap;

	if (entity.HasComponent<Quirk::TagComponent>()) {
		emitter << YAML::Key << "Name" << YAML::Value << entity.GetComponent<Quirk::TagComponent>().Tag;
	}

	if (entity.HasComponent<Quirk::TransformComponent>()) {
		auto& component = entity.GetComponent<Quirk::TransformComponent>();

		float u = std::max(0.0f, component.Translation.x - (component.Scale.x * 0.5f));
		float v = std::max(0.0f, component.Translation.y - (component.Scale.y * 0.5f));

		emitter << YAML::Key << "U" << YAML::Value << u;
		emitter << YAML::Key << "V" << YAML::Value << v;

		emitter << YAML::Key << "Width"  << YAML::Value << component.Scale.x;
		emitter << YAML::Key << "Height" << YAML::Value << component.Scale.y;
	}

	emitter << YAML::EndMap;
}


