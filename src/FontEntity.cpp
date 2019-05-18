#include "FontEntity.hpp"

FontEntity::FontEntity(std::string fontFilePath){
  libersans.loadFromFile(fontFilePath);
}

int
FontEntity::setFontProperties(std::tuple<FontType,float,float> textProperties)
{
  sf::Text text;
  text.setFont(libersans);
  text.setPosition(std::get<1>(textProperties),std::get<2>(textProperties));
  text.setCharacterSize(15.f);
  text.setColor(sf::Color::White);
  textMap.insert(std::make_pair(std::get<0>(textProperties),text));
}

std::size_t
FontEntity::getNumberOfFonts()
{
  return this->textMap.size();
}

void
FontEntity::clear()
{
  textMap.clear();
}


