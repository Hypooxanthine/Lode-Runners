#include "Core/Application.h"

//#define SHOW_DELTA_TIME

int main(int argc, char** argv)
{
	Log::init();

	// Who knows how big the app will get ? It is safer to allocate it on the heap.
	Application* app = new Application();
	app->run();
	delete app;

	#ifdef _DEBUG
	LOG_TRACE("\n\nPress any key to close.");
	std::cin.get();
	#endif

	return 0;
}