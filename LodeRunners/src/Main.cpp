#include "Core/Application.h"

int main(int argc, char** argv)
{
	Log::init();

	Application* app = new Application();
	app->run();
	delete app;

	return 0;
}