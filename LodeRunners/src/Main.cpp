#include "Core/Application.h"
#include "Network/Networker.h"

//#define SHOW_DELTA_TIME

int main(int argc, char** argv)
{
	Log::init();

	auto networker = std::make_unique<Network::Networker>();

	// Who knows how big the app will get ? It is safer to allocate it on the heap.
	auto app = std::make_unique<Application>();
	app->run();

	networker.release();

	#ifdef _DEBUG
	LOG_TRACE("\n\nPress Enter key to close.");
	std::cin.get();
	#endif

	return 0;
}