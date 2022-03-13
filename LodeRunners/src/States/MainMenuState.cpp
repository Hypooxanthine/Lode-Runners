#include "MainMenuState.h"

#include "EditorState.h"
#include "PreLobbyState.h"

MainMenuState::MainMenuState()
{
	m_HUD = MakeRef<Widget>();
	m_HUD->setViewport({ 0.f, 0.f, 1.f, 1.f });
	m_HUD->fillParent();
	
	m_PlayButton = MakeRef<TextButtonWidget>(m_HUD.get(), "Play");
	m_PlayButton->setGlobalPosition({ .35f, .3f });
	m_PlayButton->setGlobalSize({ .3f, .1f });
	m_PlayButton->bindCallback([this]()
		{
			this->pushState(MakeRef<PreLobbyState>());
		});

	m_EditorButton = MakeRef<TextButtonWidget>(m_HUD.get(), "Level Editor");
	m_EditorButton->setGlobalPosition({ .35f, .45f });
	m_EditorButton->setGlobalSize({ .3f, .1f });
	m_EditorButton->bindCallback([this]()
		{
			this->pushState(MakeRef<EditorState>());
		});

	m_SettingsButton = MakeRef<TextButtonWidget>(m_HUD.get(), "General Settings");
	m_SettingsButton->setGlobalPosition({ .35f, .6f });
	m_SettingsButton->setGlobalSize({ .3f, .1f });
}

void MainMenuState::update(const float& dt)
{
	m_HUD->update(dt);

	if (Application::get()->getEvent(EventType::Escape))
		kill();
}

void MainMenuState::render(Ref<sf::RenderWindow> window)
{
	m_HUD->render(window);
}

void MainMenuState::onResize()
{
	m_HUD->onResize();
}
