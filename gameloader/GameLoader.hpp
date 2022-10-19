#ifndef GAMELOADER_HPP
#define GAMELOADER_HPP

#include <SFML/Graphics.hpp>
#include "MenuClass.hpp"
#include "MenuClass.cpp"
#include "TetrisLoader.h"
#include "macros.hpp"

class GameLoader
{
  sf::RenderWindow m_window;
  sf::Font m_game_menu_font;
  sf::Event windowEvent;
  int m_gamenumber;

  TetrisLoader m_tetrisObj;

public:

  GameLoader();
  ~GameLoader(){}

  void showGameMenu();
};


#endif;
