#include "PreMenuState.h"

#include "MainMenuState.h"

PreMenuState::PreMenuState()
{
	m_HUD = MakeRef<Widget>();
	m_HUD->setViewport({ 0.f, 0.f, 1.f, 1.f });
	m_HUD->fillParent();

	m_TitleText = MakeRef<TextWidget>();
	Widget::bindWidgets(m_TitleText.get(), m_HUD.get());
	m_TitleText->setGlobalPosition({ .3f, .05f });
	m_TitleText->setGlobalSize({ .4f, .05f });
	m_TitleText->setText("Lode Runners");
	m_TitleText->setColor(sf::Color::White);
	m_TitleText->setBold();

	m_BodyText = MakeRef<TextWidget>();
	Widget::bindWidgets(m_BodyText.get(), m_HUD.get());
	m_BodyText->setColor(sf::Color::White);
	m_BodyText->setText
	(
		"Ce programme est un build d'un projet universitaire, UE \"Systèmes d'Exploitation\", UFR Sciences & Techniques, Dijon.\n"
		"Il s'agit d'un remake du jeu \"Lone Runner\", en réseau, avec un ou plusieurs joueurs alliés, et un nombre fixe d'ennemis,\n"
		"dont tous peuvent être contrôlés, ou non, par des joueurs.\n\n"
		"Professeurs encadrants : Annabelle Gillet, Eric Leclercq\n"
		"Programmation : Alexandre Beaujon\n"
		"Textures du skin \"default\" : Guillaume Côte\n"
		"Police par défaut : fontsquirrel.com"
	);
	m_BodyText->setGlobalPosition({ .05f, .2f });
	m_BodyText->setGlobalSize({ .9f, .5f });

	m_LaunchButton = MakeRef<TextButtonWidget>("Lancer le jeu.");
	Widget::bindWidgets(m_LaunchButton.get(), m_HUD.get());
	m_LaunchButton->setGlobalPosition({ .4f, .7f });
	m_LaunchButton->setGlobalSize({ .2f, .1f });
	m_LaunchButton->bindCallback([this]()
		{
			Application::get()->killState();
			Application::get()->pushState(MakeRef<MainMenuState>());
		});

	m_AnimShape.setFillColor(sf::Color::Black);
	m_AnimShape.setPosition(0.f, 0.f);
	m_AnimShape.setSize({1.f, 1.f});
	m_View.reset({ 0.f, 0.f, 1.f, 1.f });
}

void PreMenuState::update(const float& dt)
{
	m_HUD->update(dt);

	m_AnimEllapsedTime += dt * 20.f;
	if (m_AnimEllapsedTime > 255.f) m_AnimEllapsedTime = 255.f;

	m_AnimShape.setFillColor(sf::Color::Color(0, 0, 0, (sf::Uint8)(255.f - m_AnimEllapsedTime)));
}

void PreMenuState::render(Ref<sf::RenderWindow> window)
{
	m_HUD->render(window);

	window->setView(m_View);
	window->draw(m_AnimShape);
	window->setView(window->getDefaultView());
}

void PreMenuState::onResize()
{
	m_HUD->onResize();
}
