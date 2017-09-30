/*
 * game.hpp
 *
 *  Created on: Sep 10, 2017
 *      Author: ishan
 */

#ifndef SRC_GAME_HPP_
#define SRC_GAME_HPP_


#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <tuple>
#include <cmath>
#include <typeinfo>
#include <chrono>
#include "ball.hpp"
#include "brick.hpp"
#include "paddle.hpp"
#include "bullet.hpp"
#include "entity.hpp"
#include "lives.hpp"
#include "manager.hpp"
#include "utility.hpp"
#include <thread>

class Game
{
	enum class GameState
	{
		inprocess,
		paused,
		victory,
		lost,
		newlife,
		transit,
		gameover
	};

	sf::Font liberationSans;
	sf::Text textState,textLives,textScore,textStage;
	Manager manager;
	GameState state{GameState::inprocess};
	bool ifGamePaused{false};
	int currentStage{0};

	std::thread spawedThread;
	static constexpr int brickCountX{11};
	static constexpr int brickCountY{4};
	static constexpr int brickStartCol{1};
	static constexpr int brickStartRow{2};
	static constexpr int brickSpacing{6};
	static constexpr float brickOffsetX{22.f};
	sf::RenderWindow window{{wndWidth,wndHeight},"Arkanoid - 2"};
	int gamescore;

	int setFontPropertiesCall(sf::Text& fontType,float&& xlocation, float&& ylocation)
	{
		fontType.setFont(liberationSans);
		fontType.setPosition(xlocation,ylocation);
		fontType.setCharacterSize(25.f);
		fontType.setColor(sf::Color::White);
		// Return value is required because initializer_list is of type "int" in setFontProperties
		return 1;
	}

	template<typename Only>
	void swallowArgs(Only o){}

	template<typename...Ts>
	void setFontProperties(Ts&&...args)
	{
		std::initializer_list<int>{(swallowArgs(args),0)...};
	}

public:
	Game()
	{
		liberationSans.loadFromFile("/home/ishan/LiberationMono-Regular.ttf");
		gamescore = 0;
		currentStage = 0;
		window.setFramerateLimit(60);

		// TODO : std::make_tuple(textLives,600.f,2.f) compiles but it does not change the property of the sf::Text object. This can be investigated/researched
		setFontProperties(setFontPropertiesCall(textLives,600.f,2.f),setFontPropertiesCall(textScore,2.f,2.f),
				setFontPropertiesCall(textStage,wndHeight/2.f,wndWidth/2.f),
				setFontPropertiesCall(textState,wndWidth/2.f - 100.f,wndHeight/2.f));
	}

	void restart()
	{
		manager.clear();
		for(int i = 0;i < brickCountX;i++)
		{
			for(int j = 0;j < brickCountY;j++)
			{
				float x{(i + brickStartCol*(0.7f))*(Brick::defWidth + brickSpacing)};
				float y{(j + brickStartRow)*(Brick::defHeight + brickSpacing)};
				if(i%2==0)
					manager.create<Brick>(brickOffsetX +x ,y,sf::Color::Cyan,1,false);
				else
					manager.create<Brick>(brickOffsetX +x ,y,sf::Color::Magenta,3,false);
			}
		}

		manager.create<Ball>(wndWidth/2.f,wndHeight/2.f,false);
		manager.create<Paddle>(wndWidth/2.f,wndHeight-50,false);
		int offset = 0;
		for(int i = 0; i < manager.totalLives; i++)
		{
			manager.create<lives>(700.f + offset,20.f,false);
			offset += 2*lives::defRadius + 2.f;
		}
	}

	void changeState(const GameState& s)
	{
		// This Delay is required because when spacebar is pressed, the bullet can not be shot immediately
		std::this_thread::sleep_for (std::chrono::milliseconds(100));
		state = s;
	}

	void run()
	{
		while(true)
		{
			window.clear(sf::Color::Black);

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) break;

			if(manager.checkBallDropped())
			{
				manager.handleBallDrop();
				--manager.totalLives;
				if(0 >= manager.totalLives)
				{
					window.clear(sf::Color::Black);
					state = GameState::lost;
					textState.setString("You Lost!!");
			    	manager.draw(window);
					window.draw(textState);
			    	window.display();
				}else{
					state = GameState::newlife;
					Paddle* paddleentity = manager.getSingleEntity<Paddle>();
			     	manager.create<Ball>(paddleentity->x(),paddleentity->y()-2*Ball::defRadius,true);
				}
			}

			// If game is in progress and space bar is hit, then shoot bullets
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && state == GameState::inprocess)
			{
				Paddle* paddleentity = manager.getSingleEntity<Paddle>();
				manager.create<Bullet>(paddleentity->x(),paddleentity->y(),false);
			}

			if(state == GameState::newlife)
			{
				Ball* mball = manager.getSingleEntity<Ball>();
				mball->solveBallPaddleRelativeMotion();
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && state == GameState::newlife)
			{
					spawedThread = std::thread([this](){changeState(GameState::inprocess);});
					Ball* ballentity = manager.getSingleEntity<Ball>();
					ballentity->updateRequired = false;
			}


			if(manager.getAll<Brick>().empty())
			{
				//TODO: Put a delay and then break the loop
				state = GameState::victory;
				textState.setString("You Won!!");
		    	manager.draw(window);
				window.draw(textState);
		    	window.display();
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P) && !ifGamePaused)
			{
				state = GameState::paused;
				textState.setString("Paused");
				ifGamePaused = true;
		    	manager.draw(window);
				window.draw(textState);
		    	window.display();
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) && ifGamePaused)
			{
				state = GameState::inprocess;
				ifGamePaused = false;
			}

		    if(state == GameState::inprocess)
		    {
		    	manager.update();

		    	// resolve ball and brick collision for each brick
                manager.forEach<Ball>([this](Ball& mBall)
                {
                	manager.forEach<Brick>([&mBall](Brick& mBrick)
                    {
                		solveBallBrickCollision(mBrick, mBall);
                    });
                });

                // resolve ball and paddle collision
                manager.forEach<Ball>([this](Ball& mball)
                		{
                			manager.forEach<Paddle>([&mball](Paddle& mPaddle)
                			{
                				solvePaddleBallCollision(mPaddle, mball);
                			});
                		});


                if(manager.getAll<Bullet>().size()>0)
                {
					manager.forEach<Brick>([this](Brick& mbrick)
							{
						manager.forEach<Bullet>([&mbrick](Bullet& mbullet)
								{
									solveBrickBulletCollision(mbrick,mbullet);
								});
							});
                }

		    	manager.forEach<Brick>([this](Brick& brick)
		    			{
		    				if(brick.destroyed)
		    				{
		    					gamescore += brick.strength;
		    				}
		    			});

                textScore.setString("Score:"+std::to_string(gamescore));
                textLives.setString("Balls:");

                window.draw(textScore);
                window.draw(textLives);
		    	manager.refresh();
		    	manager.draw(window);
		    	window.display();

		    }else if(state == GameState::newlife)
		    {
		    	manager.update();
                textScore.setString("Score:"+std::to_string(gamescore));
                textLives.setString("Balls:");
                window.draw(textScore);
                window.draw(textLives);
		    	manager.refresh();
		    	manager.draw(window);
		    	window.display();

		    }
			if(spawedThread.joinable())
			{
				spawedThread.join();
			}

		}

	}
};

#endif /* SRC_GAME_HPP_ */
