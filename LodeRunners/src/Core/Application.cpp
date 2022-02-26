#include "Application.h"
#include "../States/State.h"
#include "../Assets/Assets.h"
#include "../Assets/AssetLoader.h"

// For testing purpose
#include "../States/EditorState.h"

Application* Application::m_Singleton = nullptr;

Application::Application()
	:
	m_Window(MakeRef<sf::RenderWindow>()),
	m_Event(sf::Event())
{
	AssetLoader::init();
	Assets::load();

	if (m_Singleton)
		emergencyStop("Application already exists.");

	m_Singleton = this;
}

void Application::run()
{
	const unsigned int& elementSize = Assets::getElementSize();
	m_Window->create(sf::VideoMode::getDesktopMode(), "Lode Runners");

	// For testing purpose
	m_States.push(MakeRef<EditorState>());

	while (m_Window->isOpen())
	{
		updateEvents();
		update();
		render();
		checkState();
		updateDt();
	}
}

void Application::updateEvents()
{
	while (m_Window->pollEvent(m_Event))
	{
		if (m_Event.type == sf::Event::Closed)
			close();
		if (m_Event.type == sf::Event::Resized)
			m_States.top()->onResize();
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
	if (m_PopStateRequest)
	{
		m_States.pop();
		m_PopStateRequest = false;
	}

	if (m_NextState) // If there is another state to push
	{
		m_States.push(m_NextState);
		m_NextState->init();
		m_NextState = nullptr;
	}
	else if (!m_States.empty()) // I there isn't any state to push but another state remains on the stack
		m_States.top()->init();
	else // If there isn't any state to push, plus the states stack is empty, the application has to stop
		close();
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
	LOG_INFO("New state pushed.");
	m_NextState = newState;
}

void Application::killState()
{
	LOG_INFO("State killed.");

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
