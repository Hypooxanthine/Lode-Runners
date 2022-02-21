#pragma once

#include <stack>
#include "Base.h"

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

	// Called by the active state as he must stop.
	void killState(const Ref<State>& newState);

	// Big problem time (not used yet).
	void emergencyStop(const char* errMsg);

private: // Attributes
	// Singleton instance
	static Application* m_Singleton;

	// Windowing and event handling
	Ref<sf::RenderWindow> m_Window;
	sf::Event m_Event;

	// Delta time handling
	sf::Clock m_DtClock;
	float m_DeltaTime = 0.f;

	// State stack
	std::stack<Ref<State>> m_States;
	Ref<State> m_NextState;

private: // Methods
	void close();
};
