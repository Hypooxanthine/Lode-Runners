#pragma once

#include "../Widget.h"

class TextWidget;
class TextButtonWidget;

enum class PawnType { Runner, Ennemy };

class EndGameResultsUI : public Widget
{
public:
	EndGameResultsUI();
	EndGameResultsUI(Widget* parent);

	void show(const PawnType& winners);

	void bindEvent(const Callback& callback);

private:
	Ref<TextWidget> m_Text;
	Ref<TextButtonWidget> m_BackToLobbyButton;
};

