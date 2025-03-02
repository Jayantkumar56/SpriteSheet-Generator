

#pragma once

#include "Quirk.h"
#include "Theme.h"
#include "TitleBar.h"
#include "Panels/ViewPortPanel.h"
#include "Panels/LayersPanel.h"
#include "Panels/InspectorPanel.h"

class SpriteGeneratorFrame : public Quirk::Frame {
public:
	SpriteGeneratorFrame(Quirk::WindowSpecification& spec) :
			Frame(spec),
			m_SelectedEntity(),
			m_CurrentPage(nullptr)
	{
		Quirk::Renderer2D::InitRenderer();
		Theme::SetTheme(ThemeName::DarkTheme);

		SetTitleBar<SpriteGeneratorTitleBar>();

		AddPanel<ViewPortPanel>();
		AddPanel<LayersPanel>();
		AddPanel<InspectorPanel>();
	}

	virtual void OnImguiUiUpdate() override {
		// Disabling alt key for imgui to prevent navigation with alt key (problems when using editor cotrols)
		ImGui::SetKeyOwner(ImGuiKey_LeftAlt, ImGuiKeyOwner_Any, ImGuiInputFlags_LockThisFrame);
	}

	inline auto& GetCurrentPage()    noexcept { return m_CurrentPage;    }
	inline auto& GetPages()          noexcept { return m_Pages;          }
	inline auto& GetSelectedEntity() noexcept { return m_SelectedEntity; }

	inline void AddNewPage() {
		m_CurrentPage = Quirk::CreateRef<Quirk::Scene>("New Page", 512, 512);
		m_Pages.emplace_back(m_CurrentPage);
	}

	inline void SetCurrentPage(std::string& pageName) {
		for (auto& page : m_Pages) {
			if (page->GetName() == pageName) {
				m_CurrentPage = page;
				return;
			}
		}

		QK_WARN("Page with provided page name do not exists!");
	}

	inline void SetCurrentPage(const Quirk::Ref<Quirk::Scene>& page) {
		if (page.get() != m_CurrentPage.get()) 
			m_CurrentPage = page;
	}

	inline void DeletePage(const Quirk::Ref<Quirk::Scene>& page) {
		for (size_t i = 0; i < m_Pages.size(); ++i) {
			if (m_Pages[i].get() == page.get()) {
				// if page to delete is set as current page, clearing the m_CurrentPage
				if (m_CurrentPage.get() == page.get())
					m_CurrentPage = nullptr;

				m_Pages.erase(m_Pages.begin() + i);
			}
		}
	}

private:
	Quirk::Entity m_SelectedEntity;

	Quirk::Ref<Quirk::Scene> m_CurrentPage;
	std::vector<Quirk::Ref<Quirk::Scene>> m_Pages;
};
