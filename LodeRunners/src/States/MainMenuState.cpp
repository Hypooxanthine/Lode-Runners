#include "MainMenuState.h"

#include "EditorState.h"
#include "PreLobbyState.h"

MainMenuState::MainMenuState()
{
	m_HUD = MakeRef<Widget>();
	m_HUD->setViewport({ 0.f, 0.f, 1.f, 1.f });
	m_HUD->fillParent();
	
	m_PlayButton = MakeRef<ButtonWidget>();
	Widget::bindWidgets(m_PlayButton.get(), m_HUD.get());
	m_PlayButton->setGlobalPosition({ .35f, .3f });
	m_PlayButton->setGlobalSize({ .3f, .1f });
	m_PlayButton->bindCallback([this]()
		{
			this->pushState(MakeRef<PreLobbyState>());
		});
	
	m_PlayText = MakeRef<TextWidget>();
	Widget::bindWidgets(m_PlayText.get(), m_PlayButton.get());
	m_PlayText->fillParent();
	m_PlayText->setBold();
	m_PlayText->setText("Play");

	m_EditorButton = MakeRef<ButtonWidget>();
	Widget::bindWidgets(m_EditorButton.get(), m_HUD.get());
	m_EditorButton->setGlobalPosition({ .35f, .45f });
	m_EditorButton->setGlobalSize({ .3f, .1f });
	m_EditorButton->bindCallback([this]()
		{
			this->pushState(MakeRef<EditorState>());
		});

	m_EditorText = MakeRef<TextWidget>();
	Widget::bindWidgets(m_EditorText.get(), m_EditorButton.get());
	m_EditorText->fillParent();
	m_EditorText->setBold();
	m_EditorText->setText("Level Editor");

	m_SettingsButton = MakeRef<ButtonWidget>();
	Widget::bindWidgets(m_SettingsButton.get(), m_HUD.get());
	m_SettingsButton->setGlobalPosition({ .35f, .6f });
	m_SettingsButton->setGlobalSize({ .3f, .1f });

	m_SettingsText = MakeRef<TextWidget>();
	Widget::bindWidgets(m_SettingsText.get(), m_SettingsButton.get());
	m_SettingsText->fillParent();
	m_SettingsText->setBold();
	m_SettingsText->setText("General Settings");
}

void MainMenuState::init()
{
	onResize();
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
