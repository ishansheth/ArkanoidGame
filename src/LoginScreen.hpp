/*
 * LoginScreen.hpp
 *
 *  Created on: Nov 12, 2017
 *      Author: ishan
 */

#ifndef SRC_LOGINSCREEN_HPP_
#define SRC_LOGINSCREEN_HPP_

#include "circle.hpp"

class LoginScreen:public Circle
{
	sf::Text textLogin,textPassword;
	sf::Font libersans;

public:
	LoginScreen(sf::Font& fontObj):libersans(fontObj)
	{
		textLogin.setFont(libersans);
		textLogin.setPosition(100,100);
		textLogin.setColor(sf::Color::White);
		textLogin.setString("Username:");
		textLogin.setCharacterSize(15.f);

		textPassword.setFont(libersans);
		textPassword.setPosition(100,100);
		textPassword.setColor(sf::Color::White);
		textPassword.setString("Password:");
		textPassword.setCharacterSize(15.f);

	}

	void showLoginScreen(sf::RenderWindow& window)
	{
		window.draw(textLogin);
		window.draw(textPassword);
	}

	void acceptUsername()
	{

	}



};



#endif /* SRC_LOGINSCREEN_HPP_ */
