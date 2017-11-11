/*
 * ModeSelect.hpp
 *
 *  Created on: Oct 23, 2017
 *      Author: ishan
 */

#ifndef SRC_MODESELECT_HPP_
#define SRC_MODESELECT_HPP_

#include "circle.hpp"

class ModeSelect : public Circle
{
	sf::Text textAuto,textManual;
	sf::Font libersans;
	sf::Vector2f autoTextPosition,manualTextPosition;

public:
	ModeSelect(ModeSelect& ) = delete;

	ModeSelect(sf::Font& fonts)
	{
		libersans = fonts;

		textAuto.setFont(libersans);
		textAuto.setPosition(100,100);
		autoTextPosition = {85,110};
		textAuto.setCharacterSize(15.f);
		textAuto.setColor(sf::Color::White);
		textAuto.setString("AI Mode");

		textManual.setFont(libersans);
		textManual.setPosition(100,130);
		manualTextPosition = {85,140};
		textManual.setCharacterSize(15.f);
		textManual.setColor(sf::Color::White);
		textManual.setString("Manual Mode");

		shape.setPosition(autoTextPosition);
		shape.setRadius(5.f);
		shape.setFillColor(sf::Color::Red);
		shape.setOrigin(5.f,5.f);

	}

	void showModeSelectWindow(sf::RenderWindow& window)
	{
		window.draw(textAuto);
		window.draw(textManual);
		window.draw(shape);
	}

	void changemode(sf::RenderWindow& window)
	{
		window.clear(sf::Color::Black);
		if(shape.getPosition() == autoTextPosition){shape.setPosition(manualTextPosition);}
		else
		{shape.setPosition(autoTextPosition);}
		shape.setRadius(5.f);
		shape.setFillColor(sf::Color::Red);
		shape.setOrigin(5.f,5.f);

		window.draw(textAuto);
		window.draw(textManual);
		window.draw(shape);
	}

	int getSelectedMode()
	{
		if(shape.getPosition() == autoTextPosition){return 0;}
		else
		{return 1;}
	}
};
#endif /* SRC_MODESELECT_HPP_ */
