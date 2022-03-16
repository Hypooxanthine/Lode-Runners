#include "TeamDispatcherUI.h"

const float PLAYER_ENTRY_RELATIVE_HEIGHT = .1f;

TeamDispatcherUI::TeamDispatcherUI(const size_t& maxEnnemies, const std::string& playerName)
	: TeamDispatcherUI(nullptr, maxEnnemies, playerName)
{}

TeamDispatcherUI::TeamDispatcherUI(Widget* parent, const size_t& maxEnnemies, const std::string& playerName)
	: Widget(parent), m_MaxEnnemies(maxEnnemies), m_Name(playerName)
{
	// Titles ("Runners" and "Ennemies")
	m_RunnersTitleText = MakeRef<TextWidget>(this);
	m_RunnersTitleText->setRelativePosition({ .1f, .0f });
	m_RunnersTitleText->setRelativeSize({ .3f, .1f });
	m_RunnersTitleText->setText("Runners");

	m_EnnemiesTitleText = MakeRef<TextWidget>(this);
	m_EnnemiesTitleText->setRelativePosition({ .6f, .0f });
	m_EnnemiesTitleText->setRelativeSize({ .3f, .1f });
	m_EnnemiesTitleText->setText("Ennemies");

	// Number of players ("4", "2/4" for example)
	m_RunnersNumberText = MakeRef<TextWidget>(this);
	m_RunnersNumberText->setRelativePosition({ .1f, .1f });
	m_RunnersNumberText->setRelativeSize({ .3f, .1f });
	m_RunnersNumberText->setText("0");

	m_EnnemiesNumberText = MakeRef<TextWidget>(this);
	m_EnnemiesNumberText->setRelativePosition({ .6f, .1f });
	m_EnnemiesNumberText->setRelativeSize({ .3f, .1f });
	m_EnnemiesNumberText->setText("0 / " + std::to_string(m_MaxEnnemies));

	// Joining buttons
	m_JoinRunnersButton = MakeRef<TextButtonWidget>(this, "Join");
	m_JoinRunnersButton->setRelativePosition({ .1f, .2f });
	m_JoinRunnersButton->setRelativeSize({ .3f, .1f });
	m_JoinRunnersButton->bindCallback
	(
		[this]()
		{
			requestJoinRunners_Server(PLAYER_ID, m_Name);
		}
	);

	m_JoinEnnemiesButton = MakeRef<TextButtonWidget>(this, "Join");
	m_JoinEnnemiesButton->setRelativePosition({ .6f, .2f });
	m_JoinEnnemiesButton->setRelativeSize({ .3f, .1f });
	m_JoinEnnemiesButton->bindCallback
	(
		[this]()
		{
			requestJoinEnnemies_Server(PLAYER_ID, m_Name);
		}
	);

	// Lists fields, that contains player entries.
	m_RunnersField = MakeRef<Widget>(this);
	m_RunnersField->setRelativePosition({ 0.f, .3f });
	m_RunnersField->setRelativeSize({ .5f, .7f });

	m_EnnemiesField = MakeRef<Widget>(this);
	m_EnnemiesField->setRelativePosition({ .5f, .3f });
	m_EnnemiesField->setRelativeSize({ .5f, .7f });

	getInitDataFromAsker_OnServer(PLAYER_ID);
	requestJoinRunners_Server(PLAYER_ID, m_Name);
}

void TeamDispatcherUI::setMaxEnnemies(const size_t& val)
{
	LOG_TRACE("Triggered");
	m_MaxEnnemies = val;

	for (size_t i = m_Ennemies.size(); i > 0; i--)
	{
		auto& element = m_Ennemies[i - 1];

		setPlayerToRunner(std::get<0>(element), std::string(std::get<1>(element))); // Copy assignment not to loose data
	}

	updateNumbersTexts();
}

void TeamDispatcherUI::setPlayerToRunner(const size_t& playerID, const std::string& playerName)
{
	auto runnersIt = std::find_if
	(
		m_Runners.begin(), m_Runners.end(),
		[&](const std::tuple<size_t, std::string, Ref<TextWidget>>& element) -> bool
		{
			return std::get<0>(element) == playerID;
		}
	);

	// If the player is already in runners list, there is nothing to do.
	if (runnersIt != m_Runners.end()) return;

	auto ennemiesIt = std::find_if
	(
		m_Ennemies.begin(), m_Ennemies.end(),
		[&](const std::tuple<size_t, std::string, Ref<TextWidget>>& element) -> bool
		{
			return std::get<0>(element) == playerID;
		}
	);

	// If the player is in ennemies teams, we remove him from it.
	if (ennemiesIt != m_Ennemies.end())
	{
		std::for_each
		(
			ennemiesIt + 1, m_Ennemies.end(),
			[this](const std::tuple<size_t, std::string, Ref<TextWidget>>& element)
			{
				auto& widget = std::get<2>(element);
				widget->setRelativePosition({ widget->getRelativePosition().x, widget->getRelativePosition().y - PLAYER_ENTRY_RELATIVE_HEIGHT });
			}
		);

		m_Ennemies.erase(ennemiesIt);
	}

	// Then we add the player in runners list.
	m_Runners.push_back(makeRunnerEntry(playerID, playerName));
	updateNumbersTexts();
}

void TeamDispatcherUI::setPlayerToEnnemy(const size_t& playerID, const std::string& playerName)
{
	// Trying to find player in ennemies list.
	auto ennemiesIt = std::find_if
	(
		m_Ennemies.begin(), m_Ennemies.end(),
		[&](const std::tuple<size_t, std::string, Ref<TextWidget>>& element) -> bool
		{
			return std::get<0>(element) == playerID;
		}
	);

	// If the player is already in ennemies list, there is nothing to do.
	if (ennemiesIt != m_Ennemies.end()) return;

	auto runnersIt = std::find_if
	(
		m_Runners.begin(), m_Runners.end(),
		[&](const std::tuple<size_t, std::string, Ref<TextWidget>>& element) -> bool
		{
			return std::get<0>(element) == playerID;
		}
	);

	// If the player is in runners teams, we remove him from it.
	// The server has to check if a player can leave runners list or not. Here, we just do the job.
	if (runnersIt != m_Runners.end())
	{
		std::for_each
		(
			runnersIt + 1, m_Runners.end(),
			[this](const std::tuple<size_t, std::string, Ref<TextWidget>>& element)
			{
				auto& widget = std::get<2>(element);
				widget->setRelativePosition({ widget->getRelativePosition().x, widget->getRelativePosition().y - PLAYER_ENTRY_RELATIVE_HEIGHT });
			}
		);

		m_Runners.erase(runnersIt);
	}

	// Then we add the player in runners list.
	m_Ennemies.push_back(makeEnnemyEntry(playerID, playerName));
	updateNumbersTexts();
}

void TeamDispatcherUI::updateNumbersTexts()
{
	m_EnnemiesNumberText->setText(std::to_string(m_Ennemies.size()) + " / " + std::to_string(m_MaxEnnemies));
	m_RunnersNumberText->setText(std::to_string(m_Runners.size()));
}

std::tuple<size_t, std::string, Ref<TextWidget>> TeamDispatcherUI::makeRunnerEntry(const size_t& playerID, const std::string& playerName)
{
	std::tuple<size_t, std::string, Ref<TextWidget>> out = { playerID, playerName, MakeRef<TextWidget>(m_RunnersField.get()) };

	std::get<2>(out)->setRelativePosition({ 0.f, PLAYER_ENTRY_RELATIVE_HEIGHT * m_Runners.size() });
	std::get<2>(out)->setRelativeSize({ 1.f, PLAYER_ENTRY_RELATIVE_HEIGHT });
	std::get<2>(out)->setText(playerName + " (" + std::to_string(playerID) + ")");
	if (PLAYER_ID == playerID)
	{
		std::get<2>(out)->setBold();
		std::get<2>(out)->setColor(sf::Color::Red);
	}

	return out;
}

std::tuple<size_t, std::string, Ref<TextWidget>> TeamDispatcherUI::makeEnnemyEntry(const size_t& playerID, const std::string& playerName)
{
	std::tuple<size_t, std::string, Ref<TextWidget>> out = { playerID, playerName, MakeRef<TextWidget>(m_EnnemiesField.get()) };

	std::get<2>(out)->setRelativePosition({ 0.f, PLAYER_ENTRY_RELATIVE_HEIGHT * m_Ennemies.size() });
	std::get<2>(out)->setRelativeSize({ 1.f, PLAYER_ENTRY_RELATIVE_HEIGHT });
	std::get<2>(out)->setText(playerName + " (" + std::to_string(playerID) + ")");
	if (PLAYER_ID == playerID)
	{
		std::get<2>(out)->setBold();
		std::get<2>(out)->setColor(sf::Color::Red);
	}

	return out;
}

bool TeamDispatcherUI::playerBelongsToRunners(const size_t& playerID) const
{
	auto runnersIt = std::find_if
	(
		m_Runners.begin(), m_Runners.end(),
		[&](const std::tuple<size_t, std::string, Ref<TextWidget>>& element) -> bool
		{
			return std::get<0>(element) == playerID;
		}
	);

	return runnersIt != m_Runners.end();
}

bool TeamDispatcherUI::playerBelongsToEnnemies(const size_t& playerID) const
{
	auto ennemyIt = std::find_if
	(
		m_Ennemies.begin(), m_Ennemies.end(),
		[&](const std::tuple<size_t, std::string, Ref<TextWidget>>& element) -> bool
		{
			return std::get<0>(element) == playerID;
		}
	);

	return ennemyIt != m_Ennemies.end();
}
