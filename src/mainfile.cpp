#include "game.hpp"
#include <X11/Xlib.h>
#include "Logging.hpp"
/**
 * main function
 */
int main()
{
	XInitThreads();
	LOG(DEBUG)<<"main game execution started"<<std::endl;

	Game game;
	game.restart();
	game.run();

	return 0;
}
