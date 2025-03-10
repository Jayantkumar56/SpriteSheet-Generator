

#pragma once

#include "Quirk.h"
#include "Theme.h"
#include "Page.h"
#include "TitleBar.h"
#include "Panels/ExportPanel.h"
#include "Panels/ViewPortPanel.h"
#include "Panels/LayersPanel.h"
#include "Panels/InspectorPanel.h"

class SpriteGeneratorFrame : public Quirk::Frame {
public:
	SpriteGeneratorFrame(Quirk::WindowSpecification& spec) :
			Frame         ( spec    ),
			m_CurrentPage ( nullptr )
	{
		Quirk::Renderer2D::InitRenderer();
		Theme::SetTheme(ThemeName::DarkTheme);

		SetTitleBar<SpriteGeneratorTitleBar>();

		AddPanel< ViewPortPanel  >();
		AddPanel< LayersPanel    >();
		AddPanel< InspectorPanel >();
		//AddPanel< ExportPanel    >();
	}

	virtual inline bool OnEvent(Quirk::Event& event) override { return m_CurrentPage->OnEvent(event); }
	virtual inline void OnUpdate()                   override { m_CurrentPage->OnUpdate();            }

	virtual void OnImguiUiUpdate() override {
		// Disabling alt key for imgui to prevent navigation with alt key (problems when using editor cotrols)
		ImGui::SetKeyOwner(ImGuiKey_LeftAlt, ImGuiKeyOwner_Any, ImGuiInputFlags_LockThisFrame);
	}

	inline const auto& GetCurrentPage() const noexcept { return m_CurrentPage; }
	inline const auto& GetPages()       const noexcept { return m_Pages;       }

	inline void AddNewPage() {
		m_CurrentPage = Quirk::CreateRef<Page>();
		m_Pages.emplace_back(m_CurrentPage);
	}

	inline void SetCurrentPage(const Quirk::Ref<Page>& page) {
		if (page.get() != m_CurrentPage.get()) 
			m_CurrentPage = page;
	}

	inline void DeletePage(const Quirk::Ref<Page>& page) {
		for (size_t i = 0; i < m_Pages.size(); ++i) {
			if (m_Pages[i].get() == page.get()) {
				// if page to delete is set as current page, clearing the m_CurrentPage
				if (m_CurrentPage.get() == page.get()) {
					m_CurrentPage = nullptr;
				}

				m_Pages.erase(m_Pages.begin() + i);
			}
		}
	}

private:
	Quirk::Ref<Page> m_CurrentPage;
	std::vector<Quirk::Ref<Page>> m_Pages;
};
