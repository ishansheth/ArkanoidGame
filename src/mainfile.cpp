#include "game.hpp"
#include <X11/Xlib.h>
#include "Logging.hpp"

int main()
{
  	XInitThreads();
	LOG()<<DEBUG<<"main game execution started";

	Game game;
	game.restart();
	game.run();
	
	return 0;
}
