#include "EndGameResultsUI.h"

#include "../TextWidget.h"
#include "../TextButtonWidget.h"

#include "../../Network/Network.h"

EndGameResultsUI::EndGameResultsUI()
	: EndGameResultsUI(nullptr)
{}

EndGameResultsUI::EndGameResultsUI(Widget* parent)
	: Widget(parent)
{
	m_Text = MakeRef<TextWidget>();
	m_Text->setBold();
	m_Text->setRelativePosition({ .2f, .2f });
	m_Text->setRelativeSize({ .6f, .3f });

	m_BackToLobbyButton = MakeRef<TextButtonWidget>("Back to Lobby");
	m_BackToLobbyButton->setRelativePosition({ .4f, .6f });
	m_BackToLobbyButton->setRelativeSize({ .2f, .1f });
}

void EndGameResultsUI::show(const PawnType& winners)
{
	m_Text->setParent(this);

	if (winners == PawnType::Runner)
		m_Text->setText("Runners won !");
	else
		m_Text->setText("Ennemies won !");

	if(IS_SERVER)
		m_BackToLobbyButton->setParent(this);
}

void EndGameResultsUI::bindEvent(const Callback& callback)
{
	m_BackToLobbyButton->bindCallback(callback);
}
