#include "game.hpp"
#include "Logging.hpp"

int main()
{  
  LOG()<<DEBUG<<"main game execution started";
  Game game;
  game.restart();
  game.run();

  
  return 0;
}
