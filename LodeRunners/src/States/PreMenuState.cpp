#include "PreMenuState.h"

#include "MainMenuState.h"

#include "../HUD/TextWidget.h"
#include "../HUD/ButtonWidget.h"

PreMenuState::PreMenuState()
{
	m_HUD = MakeRef<Widget>();
	m_HUD->setViewport({ 0.f, 0.f, 1.f, 1.f });
	m_HUD->fillParent();

	auto titleText = MakeRef<TextWidget>();
	Widget::addChild(titleText, m_HUD);
	titleText->setGlobalPosition({ .3f, .05f });
	titleText->setGlobalSize({ .4f, .05f });
	titleText->setText("Lode Runners");
	titleText->setColor(sf::Color::White);
	titleText->setBold();

	auto bodyText = MakeRef<TextWidget>();
	Widget::addChild(bodyText, m_HUD);
	bodyText->setColor(sf::Color::White);
	bodyText->setText
	(
		"Ce programme est un build d'un projet universitaire, UE \"Systèmes d'Exploitation\", UFR Sciences & Techniques, Dijon.\n"
		"Il s'agit d'un remake du jeu \"Lone Runner\", en réseau, avec un ou plusieurs joueurs alliés, et un nombre fixe d'ennemis,\n"
		"dont tous peuvent être contrôlés, ou non, par des joueurs.\n\n"
		"Professeurs encadrants : Annabelle Gillet, Eric Leclercq\n"
		"Programmation : Alexandre Beaujon\n"
		"Textures du skin \"default\" : Guillaume Côte\n"
		"Police par défaut : fontsquirrel.com"
	);
	bodyText->setGlobalPosition({ .05f, .2f });
	bodyText->setGlobalSize({ .9f, .5f });

	auto launchButton = MakeRef<ButtonWidget>();
	Widget::addChild(launchButton, m_HUD);
	launchButton->setGlobalPosition({ .4f, .7f });
	launchButton->setGlobalSize({ .2f, .1f });
	launchButton->bindCallback([this]()
		{
			Application::get()->killState();
			Application::get()->pushState(MakeRef<MainMenuState>());
		});

	auto launchText = MakeRef<TextWidget>();
	Widget::addChild(launchText, launchButton);
	launchText->fillParent();
	launchText->setText("Lancer le jeu.");
	launchText->setBold();

	m_AnimShape.setFillColor(sf::Color::Black);
	m_AnimShape.setPosition(0.f, 0.f);
	m_AnimShape.setSize({1.f, 1.f});
	m_View.reset({ 0.f, 0.f, 1.f, 1.f });
}

void PreMenuState::init()
{

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
