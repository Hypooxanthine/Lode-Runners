#pragma once
#define APPLICATION_CLASS_FILE

//#define SHOW_DELTA_TIME

#include <stack>

#include "Base.h"

#define GET_EVENT(eventType) Application::get()->getEvent(eventType)

enum class EventType
{
	Escape,
	
	TextEntered,
	TextErased,

	MoveLeft,
	MoveRight,
	MoveUp,
	MoveDown,
	DigLeft,
	DigRight
};

struct Event
{
	friend class Application;
	Event() = default;

	operator bool() const { return activated; }

	std::string text = "";

	bool activated = false;
};

class State;

class Application
{
public:
	// Singleton copy and move constructors aren't needed
	Application();
	Application(const Application&) = delete;
	Application(const Application&&) = delete;

	// Singleton getter
	static inline Application* get() { return m_Singleton; }

	// Handling the main loop
	void run();

	// Getting an Application event.
	const Event& getEvent(const EventType& type) const;

	// Called by the active state to push another one. The caller doesn't die, he will wake up when he'll be on the top of the stack (when all his descendants will be killed).
	void pushState(const Ref<State>& newState);

	// Called by the active state as he must stop.
	void killState();

	// Get the window ref (const).
	inline Ref<const sf::RenderWindow> getWindow() const { return m_Window; }

	// Big problem time.
	[[noreturn]] void emergencyStop(const std::string& errMsg);

private: // Private methods
	void close();

	// Handling global events : sf::Event::Close
	void updateEvents();

	// Updating active state
	void update();

	// Rendering active state
	void render();

	// Checking the state... State. When there is another state to push, it has to be at the end of a frame to prevent rendering issues. This method also initializes the active state, if there is one.
	void checkState();

	// Simply updates delta time : time between two frames.
	void updateDt();

private: // Private attributes
	// Singleton instance
	static Application* m_Singleton;

	// Windowing and event handling
	Ref<sf::RenderWindow> m_Window;
	bool m_Fullscreen = false;
	sf::Event m_SFMLEvent;
	std::map<EventType, Event> m_Events;

	// Delta time handling
	sf::Clock m_DtClock;
	float m_DeltaTime = 0.f;

	// State stack
	std::stack<Ref<State>> m_States;
	Ref<State> m_NextState;
	bool m_PopStateRequest = false;
};

