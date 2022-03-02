#include "MainMenuState.h"

#include "EditorState.h"

#include "../HUD/ButtonWidget.h"
#include "../HUD/TextWidget.h"

MainMenuState::MainMenuState()
{
	m_HUD = MakeRef<Widget>();
	m_HUD->setViewport({ 0.f, 0.f, 1.f, 1.f });
	m_HUD->fillParent();
	
	auto playButton = MakeRef<ButtonWidget>();
	Widget::addChild(playButton, m_HUD);
	playButton->setGlobalPosition({ .35f, .3f });
	playButton->setGlobalSize({ .3f, .1f });
	
	auto playText = MakeRef<TextWidget>();
	Widget::addChild(playText, playButton);
	playText->fillParent();
	playText->setBold();
	playText->setText("Play");

	auto editorButton = MakeRef<ButtonWidget>();
	Widget::addChild(editorButton, m_HUD);
	editorButton->setGlobalPosition({ .35f, .45f });
	editorButton->setGlobalSize({ .3f, .1f });
	editorButton->bindCallback([this]()
		{
			Application::get()->pushState(MakeRef<EditorState>());
		});

	auto editorText = MakeRef<TextWidget>();
	Widget::addChild(editorText, editorButton);
	editorText->fillParent();
	editorText->setBold();
	editorText->setText("Level Editor");

	auto settingsButton = MakeRef<ButtonWidget>();
	Widget::addChild(settingsButton, m_HUD);
	settingsButton->setGlobalPosition({ .35f, .6f });
	settingsButton->setGlobalSize({ .3f, .1f });

	auto settingsText = MakeRef<TextWidget>();
	Widget::addChild(settingsText, settingsButton);
	settingsText->fillParent();
	settingsText->setBold();
	settingsText->setText("General Settings");
}

void MainMenuState::init()
{
}

void MainMenuState::update(const float& dt)
{
	m_HUD->update(dt);

	if (Application::get()->getEvent(EventType::Escape))
		Application::get()->killState();
}

void MainMenuState::render(Ref<sf::RenderWindow> window)
{
	m_HUD->render(window);
}

void MainMenuState::onResize()
{
	m_HUD->onResize();
}
