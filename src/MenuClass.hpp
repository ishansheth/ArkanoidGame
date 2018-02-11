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

template<int N>
class MenuManager : public Circle
{
	std::vector<sf::Text> options;
	std::vector<sf::Vector2f> textPositions;
	sf::Font libersans;

	struct menuCoordinates
	{
		int offset;
		int x;
		int y;
	};

	menuCoordinates boxPosition;
	int selectedPosition;


	void drawEntities(sf::RenderWindow& window)
	{
		for(int i = 0;i<N;i++)
		{
			window.draw(options[i]);
		}
		window.draw(shape);

	}

public:

	MenuManager(sf::Font& fonts,int x,int y):selectedPosition(0)
	{
		libersans = fonts;
		boxPosition.x = x;
		boxPosition.y = y;
		boxPosition.offset = 30;

		for(int i = 0;i<N;i++)
		{
			options.emplace_back(sf::Text());
			textPositions.emplace_back(sf::Vector2f());
		}

		for(int i=0;i<N;i++)
		{

			options[i].setFont(libersans);
			options[i].setColor(sf::Color::White);
			options[i].setCharacterSize(15.f);
			options[i].setPosition(boxPosition.x,boxPosition.y + i*boxPosition.offset);

			textPositions[i] = {boxPosition.x-15,boxPosition.y+10+i*boxPosition.offset};
		}

		shape.setRadius(5.f);
		shape.setFillColor(sf::Color::Red);
		shape.setOrigin(5.f,5.f);
		shape.setPosition(textPositions[0]);
	}


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

	void changemode(sf::RenderWindow& window,sf::Event keyEvent)
	{
		window.clear(sf::Color::Black);

		if(keyEvent.key.code == sf::Keyboard::Up)
		{
			if(selectedPosition == 0)
			{
				selectedPosition = N-1;
			}
			else if(selectedPosition < N )
			{
				selectedPosition--;
			}

		}
		else if(keyEvent.key.code == sf::Keyboard::Down)
		{
			if(selectedPosition == N-1)
			{
				selectedPosition = 0;
			}
			else if(selectedPosition < N )
			{
				selectedPosition++;
			}
		}

		shape.setPosition(textPositions[selectedPosition]);
		shape.setRadius(5.f);
		shape.setFillColor(sf::Color::Red);
		shape.setOrigin(5.f,5.f);
		drawEntities(window);
	}


	void showModeSelectWindow(sf::RenderWindow& window)
	{
		drawEntities(window);
	}
	int getSelectedMode()
	{
		return selectedPosition;
	}


};



#endif /* SRC_SOUNDS_MENUCLASS_HPP_ */
