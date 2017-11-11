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
	std::unordered_map<FontType,sf::Text,FontsMapHash> textMap;

public:
	explicit FontEntity(std::string fontFilePath)
	{
		libersans.loadFromFile(fontFilePath);
	}

	int setFontProperties(std::tuple<FontType,float,float> textProperties)
	{
		sf::Text text;
		text.setFont(libersans);
		text.setPosition(std::get<1>(textProperties),std::get<2>(textProperties));
		text.setCharacterSize(15.f);
		text.setColor(sf::Color::White);
		textMap.insert(std::make_pair(std::get<0>(textProperties),text));
	}

	std::size_t getNumberOfFonts()
	{
		return this->textMap.size();
	}

	template<FontType T>
	void setFontsString(std::string textString)
	{
		std::lock_guard<std::mutex> lock(textContainerMutex);
		textMap[T].setString(textString);
	}

	template<FontType T>
	auto getTypedFont()
	{
		return textMap[T];
	}
};



#endif /* SRC_FONTENTITY_HPP_ */
