#include "Application.h"
#include "../Assets/Assets.h"
#include "../Assets/AssetLoader.h"
#include "../States/PreMenuState.h"

Application* Application::m_Singleton = nullptr;

Application::Application()
	:
	m_Window(MakeRef<sf::RenderWindow>()),
	m_SFMLEvent(sf::Event())
{
	ASSERT(m_Singleton == nullptr, "Application already exists.");
	m_Singleton = this;

	AssetLoader::init();
	Assets::load();
}

void Application::run()
{
	const unsigned int& elementSize = Assets::getElementSize();
	m_Window->setVerticalSyncEnabled(true);
	m_Window->create(sf::VideoMode::getDesktopMode(), "Lode Runners");

	// For testing purpose
	m_States.push(MakeRef<PreMenuState>());

	while (m_Window->isOpen())
	{
		if (Application::get()->getWindow()->hasFocus())
		{
			updateEvents();
			update();
		}

		render();
		checkState();
		updateDt();
	}
}

const Event& Application::getEvent(const EventType& type) const
{
	return m_Events.at(type);
}

void Application::updateEvents()
{
	// Action events. These actions trigger with the OS' delay between key presses. So we need to disable them this way, not only when the key is released.
	m_Events[EventType::TextEntered].activated = false;
	m_Events[EventType::TextErased].activated = false;
	m_Events[EventType::Escape].activated = false;

	m_Events[EventType::TextEntered].text = "";

	while (m_Window->pollEvent(m_SFMLEvent))
	{
		switch (m_SFMLEvent.type)
		{
		case sf::Event::Closed:
			close();
			break;
		case sf::Event::Resized:
			m_States.top()->onResize();
			break;
		case sf::Event::TextEntered:
		{
			char c = m_SFMLEvent.text.unicode;
			if (c >= ' ' && c <= '~')
			{
				m_Events[EventType::TextEntered].activated = true;
				m_Events[EventType::TextEntered].text += c;
			}
			break;
		}
		case sf::Event::KeyPressed:
		{
			switch (m_SFMLEvent.key.code)
			{
			case sf::Keyboard::Z:
				m_Events[EventType::MoveUp].activated = true;
				break;
			case sf::Keyboard::Q:
				m_Events[EventType::MoveLeft].activated = true;
				break;
			case sf::Keyboard::S:
				m_Events[EventType::MoveDown].activated = true;
				break;
			case sf::Keyboard::D:
				m_Events[EventType::MoveRight].activated = true;
				break;
			case sf::Keyboard::A:
				m_Events[EventType::DigRight].activated = true;
				break;
			case sf::Keyboard::E:
				m_Events[EventType::DigLeft].activated = true;
				break;
			case sf::Keyboard::BackSpace:
				m_Events[EventType::TextErased].activated = true;
				break;
			case sf::Keyboard::Escape:
				m_Events[EventType::Escape].activated = true;
				break;
			}
			break;
		}
		case sf::Event::KeyReleased:
		{
			switch (m_SFMLEvent.key.code)
			{
			case sf::Keyboard::Z:
				m_Events[EventType::MoveUp].activated = false;
				break;
			case sf::Keyboard::Q:
				m_Events[EventType::MoveLeft].activated = false;
				break;
			case sf::Keyboard::S:
				m_Events[EventType::MoveDown].activated = false;
				break;
			case sf::Keyboard::D:
				m_Events[EventType::MoveRight].activated = false;
				break;
			case sf::Keyboard::A:
				m_Events[EventType::DigRight].activated = false;
				break;
			case sf::Keyboard::E:
				m_Events[EventType::DigLeft].activated = false;
				break;
			}
			break;
		}
		}
	}
}

void Application::update()
{
	// States updating
	m_States.top()->update(m_DeltaTime);
}

void Application::render()
{
	m_Window->clear(m_States.top()->getClearColor());
	
	// States rendering
	m_States.top()->render(m_Window);

	m_Window->display();
}

void Application::checkState()
{
	if( m_PopStateRequest || m_NextState)
	{
		if (m_PopStateRequest)
		{
			m_States.pop();
			m_PopStateRequest = false;
			LOG_INFO("State killed.");
		}

		if (m_NextState) // If there is another state to push
		{
			m_States.push(m_NextState);
			m_NextState->init();
			m_NextState = nullptr;
			LOG_INFO("New State pushed.");
		}

		if (m_States.empty())
			close();
		else
			m_States.top()->init();	
	}
}

void Application::updateDt()
{
	#if defined(_DEBUG) && defined(SHOW_DELTA_TIME)

	static float time = 0.f;
	time += m_DeltaTime;

	static size_t frameNb = 0;
	frameNb++;

	if (time > DELTA_TIME_LOG_STEP)
	{
		LOG_TRACE("Delta time : " + std::to_string(time / (float)frameNb) + " s");
		time -= DELTA_TIME_LOG_STEP;
		frameNb = 0;
	}

	#endif

	m_DeltaTime = m_DtClock.restart().asSeconds();
}

void Application::pushState(const Ref<State>& newState)
{
	LOG_INFO("Requested new state.");
	m_NextState = newState;
}

void Application::killState()
{
	LOG_INFO("Requested kill state.");

	m_PopStateRequest = true;
}

[[noreturn]] void Application::emergencyStop(const std::string& errMsg)
{
	LOG_ERROR(std::string("Emergency stop has been called. Details:\n") + errMsg);
	exit(1);
}

void Application::close()
{
	m_Window->close();
}
