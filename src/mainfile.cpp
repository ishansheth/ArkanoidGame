#include "game.hpp"
#include <X11/Xlib.h>
#include "Logging.hpp"
#include "clock.hpp"

int main()
{
  	XInitThreads();
	LOG()<<DEBUG<<"main game execution started";

       	MyTimer m(10,10);
	
	Game game;
	game.restart();
	game.run();

	return 0;
}
