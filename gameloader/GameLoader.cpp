#include "GameLoader.hpp"
#include <iostream>

GameLoader::GameLoader():m_window(sf::VideoMode(WNDWIDTH,WNDHEIGHT),"GameLauncher"),m_tetrisObj(m_window)
{
  m_game_menu_font.loadFromFile(STRINGIZE_VALUE_OF(FONTFILEPATH));       
  m_window.setFramerateLimit(60);
  m_window.setKeyRepeatEnabled(false);
}
  
void GameLoader::showGameMenu()
{
  m_window.clear(sf::Color::Black);
  
  MenuManager<2> m_menu_class{m_game_menu_font,WNDWIDTH/2,WNDHEIGHT/2};
  m_menu_class.setOptionsString("Arknoid","Tetris");
  m_menu_class.showModeSelectWindow(m_window);

  while(m_window.isOpen())
  {
    while(m_window.pollEvent(windowEvent))    
	  { 
      if(windowEvent.type == sf::Event::KeyPressed)
      {      
        if(windowEvent.key.code == sf::Keyboard::Up || windowEvent.key.code == sf::Keyboard::Down)
        {
	        m_menu_class.changemode(m_window,windowEvent);
	      }

	      if(windowEvent.key.code == sf::Keyboard::Space)
        {
	        m_gamenumber = m_menu_class.getSelectedMode();
          std::cout<<"sected number:"<<m_gamenumber<<std::endl;
	        break;
        }
	    }
      else if(windowEvent.type == sf::Event::Closed || windowEvent.key.code == sf::Keyboard::Escape)
      {
	      m_window.close();
	      exit(0);
	    }
      m_window.display();

	  }

    if(m_gamenumber == 1)
    {
      std::cout<<"starting tetris"<<std::endl;
      m_tetrisObj.start();
    }
    else
    {
      std::cout<<"can not start any game"<<std::endl;
    }

  }
}
