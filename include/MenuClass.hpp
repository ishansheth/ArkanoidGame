/*
 * MenuClass.hpp
 *
 *  Created on: Feb 3, 2018
 *      Author: ishan
 */

#ifndef SRC_SOUNDS_MENUCLASS_HPP_
#define SRC_SOUNDS_MENUCLASS_HPP_

#include <vector>
#include "circle.hpp"
#include "starEntity.hpp"


template<int N>
class MenuManager 
{
  std::vector<sf::Text> options;
  std::vector<sf::Vector2f> textPositions;
  sf::Font libersans;

  Star starShape;

  unsigned int starheight;
  unsigned int starwidth;

  unsigned int starXadjust;
  unsigned int starYadjust;
  
  struct menuCoordinates
  {
    int offset;
    int x;
    int y;
  };

  menuCoordinates boxPosition;
  int selectedPosition;
  
  
  void drawEntities(sf::RenderWindow& window);
  
public:

  MenuManager(sf::Font& fonts,int x,int y);
  
  
  template <typename>
  struct forNArgsImpl;
  
  template<std::size_t...TNcalls>
  struct forNArgsImpl< std::index_sequence<TNcalls...> >
  {
    
    template<typename TF,typename... Ts>
    static void exec(TF&& mFn,const std::tuple<Ts...>& mXs)
    {
      using swallow = bool[];
      
      (void)swallow{
	(execN<TNcalls>(mFn,mXs),true)...
	  };
    }
    
    template<std::size_t TNBase,typename TF,typename...Ts>
    static void execN(TF&& mFn,const std::tuple<Ts...>& mXs)
    {
      mFn(TNBase,std::get<TNBase>(mXs));
    }
  };
  
  template<typename...T>
  void setOptionsString(T&& ... args)
  {
    static_assert((sizeof...(args))==N,"No of string should be same as no of fonts to be displayed");
    
    constexpr auto noOfStrings(sizeof...(args));
    
    forNArgsImpl< std::make_index_sequence<noOfStrings> >::exec(
								[this](auto x,auto y)
								{
								  options[x].setString(y);
								},
								std::forward_as_tuple(std::forward<T>(args)...)
								);
  }
  
  void changemode(sf::RenderWindow& window,sf::Event keyEvent);
  
  void showModeSelectWindow(sf::RenderWindow& window);

  int getSelectedMode();
  
};



#endif /* SRC_SOUNDS_MENUCLASS_HPP_ */
