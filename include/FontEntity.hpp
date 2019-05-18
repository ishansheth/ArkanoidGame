/*
 * FontEntity.hpp
 *
 *  Created on: Nov 11, 2017
 *      Author: ishan
 */

#ifndef SRC_FONTENTITY_HPP_
#define SRC_FONTENTITY_HPP_

#include <unordered_map>
#include <string>
#include <mutex>
#include <SFML/Graphics.hpp>

enum class FontType
{
	STAGEFONT,
	CLOCKFONT,
	LIVESFONT,
	SCOREFONT,
	GAMEMODEFONT
};

struct FontsMapHash
{
	template<typename T>
	std::size_t operator()(T t) const
	{
		return static_cast<std::size_t>(t);
	}
};

class FontEntity
{
	sf::Font libersans;
	std::mutex textContainerMutex;
	std::unordered_map<FontType,sf::Text> textMap;

public:
  explicit FontEntity(std::string fontFilePath);

  int setFontProperties(std::tuple<FontType,float,float> textProperties);

  std::size_t getNumberOfFonts();

  template<FontType T>
  void setFontsString(std::string textString){
    std::lock_guard<std::mutex> lock(textContainerMutex);
    textMap[T].setString(textString);
  }

  template<FontType T>
  sf::Text getTypedFont(){
    return textMap[T];
  }
    
  template<FontType T>
  void draw(sf::RenderWindow& window){
    window.draw(textMap[T]);
  }

  void clear();
};
#endif /* SRC_FONTENTITY_HPP_ */
