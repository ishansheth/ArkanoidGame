#include "MenuClass.hpp"


template class MenuManager<2>;

template<int N>
MenuManager<N>::MenuManager(sf::Font& fonts,int x,int y):selectedPosition(0),starShape(),starheight(10),starwidth(10),starXadjust(10),starYadjust(5)
  {
    libersans = fonts;
    boxPosition.x = x;
    boxPosition.y = y;
    boxPosition.offset = 30;
    
    for(int i = 0;i<N;i++)
      {
	optionsText.emplace_back(sf::Text());
	textPositions.emplace_back(sf::Vector2f());
      }
    
    for(int i=0;i<N;i++)
      {	
	optionsText[i].setFont(libersans);
	optionsText[i].setColor(sf::Color::White);
	optionsText[i].setCharacterSize(15.f);
	optionsText[i].setPosition(boxPosition.x,boxPosition.y + i*boxPosition.offset);
	
	textPositions[i] = {boxPosition.x-15,boxPosition.y+10+i*boxPosition.offset};
      }
    

    starShape.setParameters(textPositions[0].x-starXadjust,textPositions[0].y-starYadjust,starheight,starwidth);
  }



template<int N>
void MenuManager<N>::drawEntities(sf::RenderWindow& window){
  for(int i = 0;i<N;i++){
    window.draw(optionsText[i]);
  }
  window.draw(starShape);
}

template<int N>
void MenuManager<N>::changemode(sf::RenderWindow& window,sf::Event keyEvent){
  window.clear(sf::Color::Black);
  
  if(keyEvent.key.code == sf::Keyboard::Up){
    if(selectedPosition == 0){
	    selectedPosition = N-1;
    }
    else if(selectedPosition < N ){
      selectedPosition--;
    }  
  }else if(keyEvent.key.code == sf::Keyboard::Down){
    if(selectedPosition == N-1){
      selectedPosition = 0;
    }else if(selectedPosition < N ){
      selectedPosition++;
    }
  }
  
  starShape.setParameters(textPositions[selectedPosition].x-starXadjust,textPositions[selectedPosition].y-starYadjust,starheight,starwidth);
  
  drawEntities(window);
}

template<int N>
void MenuManager<N>::showModeSelectWindow(sf::RenderWindow& window){
  drawEntities(window);
}

template<int N>
int MenuManager<N>::getSelectedMode(){
  return selectedPosition;
}


