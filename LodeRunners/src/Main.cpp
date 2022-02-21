#include "Core/Application.h"

int main(int argc, char** argv)
{
	Log::init();

	// Who knows how big the app will get ? It is safer to allocate it on the heap.
	Application* app = new Application();
	app->run();
	delete app;

	return 0;
}